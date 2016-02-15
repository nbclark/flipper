#include "StdAfx.h"
#include <regext.h>
#include <aygshell.h>
#include <shellapi.h>
#include <windows.h>
#include <windowsx.h>
#include <connmgr.h>
#include <commctrl.h>
#include <regext.h>
#include "resource.h"
#include "POutlook.h"
#include "Weather.h"
#include "ComDate.h"
#include "..\FlipperConfig\Anchor.h"

HMODULE CWeather::g_hInstance = NULL;
WCHAR* CWeather::g_wzUpdating = L"Updating...";
WCHAR* CWeather::g_wzError = L"Error Updating...";

CWeather::CWeather(void* pPlugin)
{
	g_wzMessage = g_wzUpdating;
	g_pPlugin = pPlugin;
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	StringCchCopy(g_zipCode, ARRAYSIZE(g_zipCode), L"98072");
	SHGetSpecialFolderPath(NULL, g_wzSettingsPath, CSIDL_APPDATA, TRUE);

	g_hSemUpdate = CreateEvent(NULL, FALSE, FALSE, NULL);
	ZeroMemory(&g_lastUpdated, sizeof(FILETIME));
	ZeroMemory(&g_updatingNode, sizeof(node));
	ZeroMemory(g_wzLocation, sizeof(g_wzLocation));

	g_iNodeCount = 1;
	g_bCelsius = false;

	for (int i = 0; i < ARRAYSIZE(g_forecastNodes); ++i)
	{
		ZeroMemory(&g_forecastNodes[i], sizeof(node));
	}
	g_bUpdating = true;

	StringCchCopy(g_updatingNode.wzText, ARRAYSIZE(g_updatingNode.wzText), L"Updating...");
    g_hThumbThread = CreateThread(NULL, 0, &CWeather::PollWeather, this, 0, 0);
	SetThreadPriority(g_hThumbThread, THREAD_PRIORITY_BELOW_NORMAL);
}

CWeather::~CWeather(void)
{
    if (g_hThumbThread)
    {
        TerminateThread(g_hThumbThread, 0);
        CloseHandle(g_hThumbThread);
        g_hThumbThread = NULL;
    }
	if (g_hSemUpdate)
	{
		CloseHandle(g_hSemUpdate);
		g_hSemUpdate = NULL;
	}

	DestroyNodeList(&g_forecastNodes[0]);

	hash_map<WCHAR*, HBITMAP>::iterator iter = g_cachedImages.begin();
	while (iter != g_cachedImages.end())
	{
		DeleteObject(iter->second);
		iter++;
	}
	g_cachedImages.clear();
}

void CWeather::ShowSettings(HWND hWndParent, HKEY hKey)
{
	DWORD x = DialogBoxParamW(g_hInstance, MAKEINTRESOURCE(IDD_DIALOG_WEATHER), hWndParent, CWeather::WeatherProc, (LPARAM)hKey);
	DWORD error = GetLastError();
}

void CWeather::LoadSettings(IFlipperApp* pFlipper, HKEY hKey)
{
	g_pFlipper = pFlipper;
	RegistryGetString(hKey, NULL, REG_ZipCode, g_zipCode, ARRAYSIZE(g_zipCode));

	StringCchPrintf(g_wzWeatherPath, ARRAYSIZE(g_wzWeatherPath), L"%s\\%s\\%s.xml", g_wzSettingsPath, WEATHER_SAVE_PATH, g_zipCode);
	StringCchPrintf(g_wzImagesPath, ARRAYSIZE(g_wzImagesPath), L"%s\\%s\\%s", g_wzSettingsPath, WEATHER_SAVE_PATH, WEATHER_IMAGES_PATH);
	StringCchPrintf(g_wzWeatherUrl, ARRAYSIZE(g_wzWeatherUrl), WEATHER_SERVICE_URL, g_zipCode);

	CreateDirectoryRecursively(g_wzImagesPath);

	LoadWeatherItems(&g_forecastDays);
}

void CWeather::DeserializeXml(list<ForecastDay*>* pForecastList)
{
	DestroyWeatherItems(pForecastList);

	// Load from XML
	CComPtr<IXMLDOMDocument> spXMLDoc;
	XMLElementPtr spRootElem;
	CComVariant varFile;
	varFile = g_wzWeatherPath;
	VARIANT_BOOL vbLoadResult = VARIANT_FALSE;
	spXMLDoc.CoCreateInstance(CLSID_DOMDocument);
	spXMLDoc->load(varFile, &vbLoadResult);

	spXMLDoc->get_documentElement(&spRootElem);
	
	if (spRootElem)
	{
		CComBSTR bstrLocation(L"//forecast_information");
		CComBSTR bstrCurrent(L"//current_conditions");
		CComBSTR bstrForecast(L"//forecast_conditions");
		CComBSTR bstrData("data"), bstrDay("day_of_week"), bstrImage("icon"), bstrMaxFCurr("temp_f"), bstrMinFCurr("temp_f"), bstrMaxFFore("high"), bstrMinFFore("low"), bstrCondition("condition");

		CComPtr<IXMLDOMNode> spLocationElement;
		spRootElem->selectSingleNode(bstrLocation, &spLocationElement);

		if (spLocationElement)
		{
			CComPtr<IXMLDOMNode> spCityElement;
			IXMLDOMNamedNodeMap* pNodeMap;
			IXMLDOMNode* pNodeData;
			CComBSTR bstrCity(L"city");

			if (SUCCEEDED(spLocationElement->selectSingleNode(bstrCity, &spCityElement)) && spCityElement)
			{
				CComBSTR bstrText;
				if (SUCCEEDED(spCityElement->get_attributes(&pNodeMap)))
				{
					if (SUCCEEDED(pNodeMap->getNamedItem(bstrData, &pNodeData)))
					{
						if (SUCCEEDED(pNodeData->get_text(&bstrText)))
						{
							StringCchCopy(g_wzLocation, ARRAYSIZE(g_wzLocation), bstrText);
						}
						pNodeData->Release();
					}
					pNodeMap->Release();
				}
			}
		}

		VARIANT_BOOL bHasChildNodes;
		for (int j = 0; j < 2; ++j)
		{
			BSTR bstrNode = (j == 0) ? bstrCurrent : bstrForecast;
			XMLNodeListPtr spNodeList;
			spRootElem->selectNodes(bstrNode, &spNodeList);

			long count;
			spNodeList->get_length(&count);

			BSTR bstrMaxF = (j == 0) ? bstrMaxFCurr : bstrMaxFFore;
			BSTR bstrMinF = (j == 0) ? bstrMinFCurr : bstrMinFFore;

			for (long i = 0; i < count; ++i)
			{
				CComPtr<IXMLDOMNode> spWeatherElement;
				if (SUCCEEDED(spNodeList->get_item(i, &spWeatherElement)))
				{
					if (SUCCEEDED(spWeatherElement->hasChildNodes(&bHasChildNodes)) && bHasChildNodes)
					{
						ForecastDay* fDay = new ForecastDay();
						CComPtr<IXMLDOMNode> spDay, spImage, spMaxF, spMinF, spCondition;
						IXMLDOMNamedNodeMap* pNodeMap;
						IXMLDOMNode* pNodeData;

						if (SUCCEEDED(spWeatherElement->selectSingleNode(bstrDay, &spDay)) && spDay)
						{
							CComBSTR bstrText;
							if (SUCCEEDED(spDay->get_attributes(&pNodeMap)))
							{
								if (SUCCEEDED(pNodeMap->getNamedItem(bstrData, &pNodeData)))
								{
									if (SUCCEEDED(pNodeData->get_text(&bstrText)))
									{
										if (j == 1 && i == 0)
										{
											StringCchCopy(fDay->wzDay, ARRAYSIZE(fDay->wzDay), L"Tonight");
										}
										else
										{
											StringCchCopy(fDay->wzDay, ARRAYSIZE(fDay->wzDay), bstrText);
										}
									}
									pNodeData->Release();
								}
								pNodeMap->Release();
							}
						}
						else
						{
							StringCchCopy(fDay->wzDay, ARRAYSIZE(fDay->wzDay), L"Today");
						}
						if (SUCCEEDED(spWeatherElement->selectSingleNode(bstrImage, &spImage)) && spImage)
						{
							CComBSTR bstrText;
							if (SUCCEEDED(spImage->get_attributes(&pNodeMap)))
							{
								if (SUCCEEDED(pNodeMap->getNamedItem(bstrData, &pNodeData)))
								{
									if (SUCCEEDED(pNodeData->get_text(&bstrText)))
									{
										StringCchCopy(fDay->wzImage, ARRAYSIZE(fDay->wzImage), bstrText);
									}
									pNodeData->Release();
								}
								pNodeMap->Release();
							}
						}
						if (SUCCEEDED(spWeatherElement->selectSingleNode(bstrCondition, &spCondition)) && spCondition)
						{
							CComBSTR bstrText;
							if (SUCCEEDED(spCondition->get_attributes(&pNodeMap)))
							{
								if (SUCCEEDED(pNodeMap->getNamedItem(bstrData, &pNodeData)))
								{
									if (SUCCEEDED(pNodeData->get_text(&bstrText)))
									{
										StringCchCopy(fDay->wzCondition, ARRAYSIZE(fDay->wzCondition), bstrText);
									}
									pNodeData->Release();
								}
								pNodeMap->Release();
							}
						}
						if (SUCCEEDED(spWeatherElement->selectSingleNode(bstrMaxF, &spMaxF)) && spMaxF)
						{
							CComBSTR bstrText;
							if (SUCCEEDED(spMaxF->get_attributes(&pNodeMap)))
							{
								if (SUCCEEDED(pNodeMap->getNamedItem(bstrData, &pNodeData)))
								{
									if (SUCCEEDED(pNodeData->get_text(&bstrText)))
									{
										fDay->shMaxTempF = (short)_wtoi(bstrText);
									}
									pNodeData->Release();
								}
								pNodeMap->Release();
							}
						}
						if (SUCCEEDED(spWeatherElement->selectSingleNode(bstrMinF, &spMinF)) && spMinF)
						{
							CComBSTR bstrText;
							if (SUCCEEDED(spMinF->get_attributes(&pNodeMap)))
							{
								if (SUCCEEDED(pNodeMap->getNamedItem(bstrData, &pNodeData)))
								{
									if (SUCCEEDED(pNodeData->get_text(&bstrText)))
									{
										fDay->shMinTempF = (short)_wtoi(bstrText);
									}
									pNodeData->Release();
								}
								pNodeMap->Release();
							}
						}
						pForecastList->push_back(fDay);
					}
				}
			}
		}
	}
}

void CWeather::CreateNodes(list<ForecastDay*>* pForecastList)
{
	list<ForecastDay*>::iterator iter = pForecastList->begin();

	int index = 0;
	while (iter != pForecastList->end())
	{
		ForecastDay* fDay = *iter;

		WCHAR* wzImage = new WCHAR[MAX_PATH];

		if (!wcslen(fDay->wzImage))
		{
			StringCchCopy(fDay->wzImage, ARRAYSIZE(fDay->wzImage), L"/dunno.png");
		}
		WCHAR* wzName = wcsrchr(fDay->wzImage, L'/')+1;

		StringCchPrintf(wzImage, MAX_PATH, L"%s\\%s", g_wzImagesPath, wzName);

		WCHAR* wzExt = wcsrchr(wzImage, L'.');

		if (wzExt)
		{
			wzExt[0] = 0;
		}

		StringCchCat(wzImage, MAX_PATH, L".png");

		HBITMAP hbmpImage = NULL;

		if (g_cachedImages.find(wzImage) != g_cachedImages.end() && g_cachedImages[wzImage])
		{
			hbmpImage = g_cachedImages[wzImage];
			delete[] wzImage;
		}
		else
		{
			g_cachedImages.erase(wzImage);
			hbmpImage = SHLoadImageFile(wzImage);

			if (!hbmpImage)
			{
				StringCchPrintf(wzImage, MAX_PATH, L"%s\\dunno.png", g_wzImagesPath);
				hbmpImage = SHLoadImageFile(wzImage);
			}
			g_cachedImages.insert(pair<WCHAR*, HBITMAP>(wzImage, hbmpImage));
		}

		ZeroMemory(&g_forecastNodes[index], sizeof(node));

		g_forecastNodes[index].pData = fDay;
		g_forecastNodes[index].iIndex = index;
		g_forecastNodes[index].hBmp = hbmpImage;
		g_forecastNodes[index].bCenter = true;
		g_forecastNodes[index].hIcon = (HICON)INVALID_HANDLE_VALUE;
		StringCchPrintf(g_forecastNodes[index].wzText, ARRAYSIZE(g_forecastNodes[index].wzText), L"%s - %s", fDay->wzDay, g_wzLocation);

		short shMax = (g_bCelsius) ? fDay->shMaxTempC : fDay->shMaxTempF;
		short shMin = (g_bCelsius) ? fDay->shMinTempC : fDay->shMinTempF;

		StringCchPrintf(g_forecastNodes[index].wzDetails, ARRAYSIZE(g_forecastNodes[index].wzDetails), L"%s (%d°-%d°)", fDay->wzCondition, shMin, shMax);
		if (index > 0)
		{
			g_forecastNodes[index].prev = &g_forecastNodes[index-1];
			g_forecastNodes[index-1].next = &g_forecastNodes[index];
		}
		else
		{
			g_forecastNodes[index].prev = NULL;
		}

		index++;
		iter++;
	}
	g_iNodeCount = index;
}

void CWeather::LoadWeatherItems(list<ForecastDay*>* pForecastList)
{
	bool bFileExists = FileExists(g_wzWeatherPath, &g_lastUpdated);

	SYSTEMTIME st;
	GetLocalTime(&st);

	CComDATE fileDate(g_lastUpdated);
	CComDATE currentDate(st);

	CComSpan dateDiff = (currentDate - fileDate);
	double totalSeconds = dateDiff.TotalSeconds();

	// TODO: put a critical section around this
	if (!bFileExists || (totalSeconds > WEATHER_POLL_INTERVAL_SECONDS))
	{
		// We are updating...
		pForecastList->clear();
		SetEvent(g_hSemUpdate);
	}
	else
	{
		DeserializeXml(pForecastList);
		CreateNodes(pForecastList);
		g_bUpdating = false;
	}
}

bool CWeather::DownloadItem(WCHAR* wzUrl, WCHAR* wzPath, bool bOverwriteExisting, bool requireXml)
{
	bool bExist = false;

	if (FileExists(wzPath, NULL) && !bOverwriteExisting)
	{
		return true;
	}

	GUID destGuid = {0};
	ConnMgrMapURL(wzUrl, &destGuid, 0);

	CONNMGR_CONNECTIONINFO connInfo = {0};
	connInfo.cbSize = sizeof(CONNMGR_CONNECTIONINFO);
	connInfo.dwPriority		  = CONNMGR_PRIORITY_HIPRIBKGND;
    connInfo.dwParams         = CONNMGR_PARAM_GUIDDESTNET;
    connInfo.dwFlags          = CONNMGR_FLAG_PROXY_HTTP;
    connInfo.ulMaxConnLatency = 4000;
	connInfo.bExclusive		  = FALSE;
    connInfo.bDisabled        = FALSE;
	connInfo.guidDestNet	  = destGuid;

	HANDLE hConnection = NULL;
	DWORD dwStatus = 0;
	if (SUCCEEDED(ConnMgrEstablishConnectionSync(&connInfo, &hConnection, 10*1000, &dwStatus)))
	{
		HRESULT hr = URLDownloadToFileW(NULL, wzUrl, wzPath, 0, NULL);
		bool bSuccess = SUCCEEDED(hr);
		ConnMgrReleaseConnection(hConnection, TRUE);
		return bSuccess;
	}
	return false;
}

bool CWeather::RefreshWeatherItems()
{
	// TODO: just force the refresh here
	DownloadItem(g_wzWeatherUrl, g_wzWeatherPath, true, true);

	return false;
}

void CWeather::DestroyWeatherItems(list<ForecastDay*>* pLaunchItems)
{
	list<ForecastDay*>::iterator iter = pLaunchItems->begin();

	while (iter != pLaunchItems->end())
	{
		delete *iter;
		iter++;
	}

	pLaunchItems->clear();
}

node* CWeather::BuildNodeList(int* piNodeLen, int* piThumbWidth, int* piThumbHeight, int iMaxWidth, int iMaxHeight)
{
	if (g_bUpdating)
	{
		*piThumbWidth = iMaxWidth;
		*piNodeLen = 1;
		return &g_updatingNode;
	}
	*piNodeLen = g_iNodeCount;
	return &g_forecastNodes[0];
}

bool CWeather::DrawThumbnail(void* pPlugin, node* pNode, HDC hdc, HDC hdcThumb, RECT* pRect, bool bSelected, bool* pbDrawLabels)
{
	if (g_bUpdating)
	{
		//pRect->right -= (pRect->left*2);
		//FillRect(hdc, pRect, (HBRUSH)GetStockObject(BLACK_BRUSH));

		*pbDrawLabels = false;

		RECT rect;
		memcpy(&rect, pRect, sizeof(RECT));

		int _height = (rect.bottom - rect.top);
		int _width = (rect.right - rect.left);

		LOGFONT lf;
		ZeroMemory(&lf, sizeof(lf));
		lf.lfHeight = -(_height / 4);
		lf.lfWeight = FW_BOLD;
		lf.lfCharSet = DEFAULT_CHARSET;
		HFONT hFont = CreateFontIndirect(&lf);

		SetBkMode(hdc, TRANSPARENT);
		SetBkColor(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(0xff,0xff,0xff));
		HGDIOBJ hOldFont = SelectObject(hdc, hFont);

		WCHAR* wzUpdating = g_wzMessage;

		DrawText(hdc, wzUpdating, wcslen(wzUpdating), &rect, DT_VCENTER | DT_CENTER | DT_CALCRECT);

		HDC hdcBlack = CreateCompatibleDC(hdc);
		HBITMAP hbmpBlack = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
		HGDIOBJ hObjOld = SelectObject(hdcBlack, hbmpBlack);

		RECT rectFill = {0};
		rectFill.left = rectFill.top = 0;
		rectFill.right = rect.right - rect.left;
		rectFill.bottom = rect.bottom - rect.top;

		FillRect(hdcBlack, &rectFill, (HBRUSH)GetStockObject(BLACK_BRUSH));

		rectFill.left += (_width - rectFill.right) / 2 - 5;
		rectFill.right += rectFill.left + 10;

		rectFill.top += (_height - rectFill.bottom) / 2 - 10;
		rectFill.bottom += rectFill.top + 20;

		RECT rectBg = {0};
		rectBg.left = 0;
		rectBg.top = (pRect->top + pRect->bottom - (rectFill.bottom - rectFill.top)) / 2;
		rectBg.right = _width;
		rectBg.bottom = rectBg.top + (rectFill.bottom - rectFill.top);
		g_pFlipper->DrawBackground(pPlugin, hdc, &rectBg);

		BLENDFUNCTION blendFunc;
		blendFunc.AlphaFormat = 0;
		blendFunc.BlendFlags = 0;
		blendFunc.BlendOp = AC_SRC_OVER;
		blendFunc.SourceConstantAlpha = 100;
		AlphaBlend(hdc, rect.left, (pRect->top + pRect->bottom - (rectFill.bottom - rectFill.top)) / 2, _width, rectFill.bottom - rectFill.top, hdcBlack, 0, 0, rect.right - rect.left, rect.bottom - rect.top, blendFunc);
		
		SelectObject(hdcBlack, hObjOld);
		DeleteObject(hbmpBlack);
		DeleteDC(hdcBlack);

		rectFill.top = pRect->top;
		rectFill.bottom = pRect->bottom;
		DrawText(hdc, wzUpdating, wcslen(wzUpdating), &rectFill, DT_VCENTER | DT_CENTER);

		SelectObject(hdc, hOldFont);
		DeleteObject(hFont);

		return true;
	}
	return false;
}

void CWeather::UpdateWeather()
{
	g_forecastDays.clear();
	SetEvent(g_hSemUpdate);
}

DWORD WINAPI CWeather::PollWeather(LPVOID lpParameter)
{
    CWeather* p_this = (CWeather*)lpParameter;

	HANDLE hEvents[2];
	hEvents[0] = OpenEvent(EVENT_ALL_ACCESS, FALSE, EVENT_FLIPPEREXIT);
	hEvents[1] = p_this->g_hSemUpdate;

    DWORD dwEvent = 0;

    while (WAIT_ABANDONED_0 != (dwEvent = WaitForMultipleObjects(ARRAYSIZE(hEvents), hEvents, FALSE, WEATHER_POLL_INTERVAL)))
    {
        // Update weather here
		if (dwEvent == WAIT_OBJECT_0)
		{
			Sleep(0);
		}
		else if (dwEvent == WAIT_OBJECT_0+1)
		{
			p_this->g_bUpdating = true;
			p_this->g_wzMessage = p_this->g_wzUpdating;
			p_this->g_pFlipper->Refresh(p_this->g_pPlugin);
			if (DownloadItem(p_this->g_wzWeatherUrl, p_this->g_wzWeatherPath, true, true))
			{
				p_this->DeserializeXml(&p_this->g_forecastDays);
				// Loop through them all and download images
				list<ForecastDay*>::iterator iter = p_this->g_forecastDays.begin();

				while (iter != p_this->g_forecastDays.end())
				{
					ForecastDay* pRecord = *iter;

					WCHAR wzImage[MAX_PATH];
					WCHAR* wzName = wcsrchr(pRecord->wzImage, L'/')+1;
					StringCchPrintf(wzImage, ARRAYSIZE(wzImage), L"%s\\%s", p_this->g_wzImagesPath, wzName);

					//bool bpass = p_this->DownloadItem(pRecord->wzImage, wzImage, false, false);

					iter++;
				}
				p_this->CreateNodes(&p_this->g_forecastDays);
				p_this->g_bUpdating = false;
			}
			else
			{
				DWORD dwError = GetLastError();
				p_this->g_wzMessage = p_this->g_wzError;
			}
			p_this->g_pFlipper->Refresh(p_this->g_pPlugin);
			ResetEvent(hEvents[1]);
		}
    }
    CloseHandle(hEvents[0]);

    return 0;
}

DrawThumbnailApi(WeatherDrawThumbnail)
{
	CWeather* pWeather = (CWeather*)((FlipperPlugin*)pPlugin)->pData;
	return pWeather->DrawThumbnail(pPlugin, pNode, hdc, hdcThumb, pRect, bSelected, pbDrawLabels);
}

NodeSelectedApi(WeatherNodeSelected)
{
	CWeather* pWeather = (CWeather*)((FlipperPlugin*)pPlugin)->pData;
	*pbRefresh = false;

	// TODO - show webpage?
	pWeather->g_pFlipper->Hide(pPlugin);
}

CloseOnSelectionApi(WeatherCloseOnSelection)
{
	CWeather* pWeather = (CWeather*)((FlipperPlugin*)pPlugin)->pData;
	return true;
}

BuildNodeListApi(WeatherBuildNodeList)
{
	CWeather* pWeather = (CWeather*)((FlipperPlugin*)pPlugin)->pData;
	return pWeather->BuildNodeList(piNodeLen, piThumbWidth, piThumbHeight, iMaxWidth, iMaxHeight);
}

SetSoftKeysApi(WeatherSetSoftKeys)
{
	StringCchCopy(pLeftSoftKey->wzText, ARRAYSIZE(pLeftSoftKey->wzText), L"Detailed");
	StringCchCopy(pRightSoftKey->wzText, ARRAYSIZE(pRightSoftKey->wzText), L"Update Now");

	pLeftSoftKey->bEnabled = true;
	pRightSoftKey->bEnabled = true;
}

SoftKeySelectedApi(WeatherSoftKeySelected)
{
	CWeather* pWeather = (CWeather*)((FlipperPlugin*)pPlugin)->pData;
	if (bLeftSoftKey)
	{
		Sleep(0);
	}
	else
	{
		pWeather->UpdateWeather();
		// We are updating...
		Sleep(0);
	}
}

DestroyNodeListApi(WeatherDestroyNodeList)
{
	// we don't clean up here since we have a static # of items
	return;
	node* pHead = pHeadNode;
	// Cleanup here
	if (pHead)
	{
		// keep us from looping

		if (pHead->prev)
		{
			pHead->prev->next = NULL;
		}

		node* pCurr = pHead;
		do
		{
			node* pTemp = pCurr;
			pCurr = pTemp->next;

			if (pTemp->hIcon)
			{
				pTemp->hIcon = NULL;
			}

			delete pTemp;
		} while (pCurr);
	}
}

DWORD WINAPI CWeather::SearchLocation(LPVOID lpParameter)
{
    HWND hWndDlg = (HWND)lpParameter;
	WCHAR wzUrl[MAX_PATH];
	WCHAR wzZip[MAX_PATH];
	WCHAR wzPath[MAX_PATH];

	bool bSuccess = false;

	GetTempPath(ARRAYSIZE(wzPath), wzPath);
	StringCchCat(wzPath, ARRAYSIZE(wzPath), L"Flipper\\");
	CreateDirectory(wzPath, NULL);

	StringCchCat(wzPath, ARRAYSIZE(wzPath), L"weather.xml");

	GetDlgItemText(hWndDlg, IDC_EDIT_ZIP, wzZip, ARRAYSIZE(wzZip));
	StringCchPrintf(wzUrl, ARRAYSIZE(wzUrl), WEATHER_SERVICE_URL, wzZip);

	if (CWeather::DownloadItem(wzUrl, wzPath, true, true))
	{
		CComPtr<IXMLDOMDocument> spXMLDoc;
		XMLElementPtr spRootElem;
		CComVariant varFile;
		varFile = wzPath;
		VARIANT_BOOL vbLoadResult = VARIANT_FALSE;
		spXMLDoc.CoCreateInstance(CLSID_DOMDocument);
		spXMLDoc->load(varFile, &vbLoadResult);

		spXMLDoc->get_documentElement(&spRootElem);
		
		if (spRootElem)
		{
			CComBSTR bstrLocation(L"//forecast_information");

			CComPtr<IXMLDOMNode> spLocationElement;
			spRootElem->selectSingleNode(bstrLocation, &spLocationElement);

			if (spLocationElement)
			{
				bSuccess = true;
				SetDlgItemText(hWndDlg, IDC_STATIC_RESOLVE, L"Successfully found location...");
			}
		}
	}

	if (!bSuccess)
	{
		SetDlgItemText(hWndDlg, IDC_STATIC_RESOLVE, L"Error finding location...");
	}
	EnableWindow(GetDlgItem(hWndDlg, IDC_BUTTON_ADD), TRUE);

	return 0;
}

BOOL CALLBACK CWeather::WeatherProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{
	static HWND hwndZip, hWndMenu = NULL;
	static HKEY g_hKey = NULL;
	static CDlgAnchor g_dlgAnchor;
	static HANDLE g_hThread = NULL;

    switch (message) 
    { 
		case WM_SIZE :
		{
			g_dlgAnchor.OnSize();

			RECT rc;
			GetWindowRect(GetDlgItem(hwndDlg, IDC_BUTTON_ADD), &rc);
		}
		break;
        case WM_INITDIALOG:
        {
			g_dlgAnchor.Init(hwndDlg);
			g_dlgAnchor.Add(IDC_EDIT_ZIP, ANCHOR_TOP | ANCHOR_LEFT | ANCHOR_RIGHT);
			g_dlgAnchor.Add(IDC_BUTTON_ADD, ANCHOR_TOP | ANCHOR_RIGHT);
			g_dlgAnchor.Add(IDC_STATIC_RESOLVE, ANCHOR_TOP | ANCHOR_LEFT | ANCHOR_RIGHT);
			g_dlgAnchor.Add(IDC_STATIC_WEATHER, ANCHOR_TOP | ANCHOR_LEFT | ANCHOR_RIGHT | ANCHOR_BOTTOM);

			HBITMAP hBMP;
			int dim = DRA::SCALEX(40);
			hBMP = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_ADD));
			//DRA::ImageList_StretchBitmap(hBMP, &hBMP, DRA::SCALEX(40), DRA::SCALEY(40), 1, 1);
			SendDlgItemMessage(hwndDlg, IDC_BUTTON_ADD, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBMP);

			hwndZip = GetDlgItem(hwndDlg, IDC_EDIT_ZIP);

			SHINITDLGINFO shidi = {0};
            // Create a Done button and size it.
            shidi.dwMask = SHIDIM_FLAGS;
            shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN;
            shidi.hDlg = hwndDlg;
            SHInitDialog(&shidi);

            SHMENUBARINFO mbi = {0};
            memset(&mbi, 0, sizeof(SHMENUBARINFO));
            mbi.cbSize = sizeof(SHMENUBARINFO);
            mbi.hwndParent = hwndDlg;
            mbi.nToolBarId = IDR_Flipper_SET_MENUBAR;
			mbi.hInstRes = CWeather::g_hInstance;
            mbi.nBmpId = 0;
            mbi.cBmpImages = 0;    

            SHCreateMenuBar(&mbi);
            hWndMenu = mbi.hwndMB;

			g_hKey = (HKEY)lParam;
			WCHAR wzZip[20];

			RegistryGetString(g_hKey, NULL, REG_ZipCode, wzZip, ARRAYSIZE(wzZip));
			SetWindowText(hwndZip, wzZip);

			SetFocus(hwndZip);

            return 0;
        }
        break;
        case WM_COMMAND: 
			{
				switch (LOWORD(wParam))
				{
					case IDC_BUTTON_ADD :
					{
						if (HIWORD(wParam) == 0)
						{
							if (g_hThread)
							{
								TerminateThread(g_hThread, 0);
							}
							SetDlgItemText(hwndDlg, IDC_STATIC_RESOLVE, L"Searching...");
							EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON_ADD), FALSE);
							g_hThread = CreateThread(NULL, 0, &CWeather::SearchLocation, hwndDlg, 0, 0);
						}
					}
					break;
					case IDM_Flipper_SET_ACCEPT :
					case IDOK:
						{
							int index = 0;

							WCHAR wzName[MAX_PATH];
							DWORD dwName = ARRAYSIZE(wzName);

							GetWindowText(hwndZip, wzName, dwName);
							
							RegistrySetString(g_hKey, NULL, REG_ZipCode, wzName);

							EndDialog(hwndDlg, IDOK);
						}
						break;
					case IDM_Flipper_SET_CANCEL:
						{
							EndDialog(hwndDlg, IDOK);
						}
						break;
				}
            }
            break;
        case WM_DESTROY:
            {
				if (hWndMenu)
				{
                    DestroyWindow(hWndMenu);
					hWndMenu = NULL;
				}
                Sleep(0);
            }
            break;
    } 
    return FALSE; 
}
