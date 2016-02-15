#include "stdafx.h"
#include <regext.h>
#include <phone.h>
#include <aygshell.h>
#include <shellapi.h>
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>   // includes the common control header
#include "Mapi.h"
#include "SpeedDial.h"
#include "resource.h"
#include "..\FlipperConfig\Anchor.h"

HMODULE CSpeedDial::g_hInstance = NULL;

CSpeedDial::CSpeedDial()
{
	//PhoneRecord* pRecord = new PhoneRecord();
	//pRecord->oid = 0;
	//StringCchCopy(pRecord->wzName, ARRAYSIZE(pRecord->wzName), L"Add Contact");

	g_iThumbWidth = g_iThumbHeight = 0;
	g_pHeadNode = NULL;

	//g_phoneList.push_back(pRecord);
}
CSpeedDial::~CSpeedDial()
{
	DestroyPhoneItems(&g_phoneList);
	DestroyNodeList(g_pHeadNode);
	g_pHeadNode = NULL;
}

void CSpeedDial::LoadSettings(IFlipperApp* pFlipper, HKEY hKey)
{
	g_pFlipper = pFlipper;
	LoadPhoneItems(hKey, &g_phoneList, &g_outlookApp);
}

void CSpeedDial::ShowSettings(HWND hWndParent, HKEY hKey)
{
	DWORD x = DialogBoxParamW(g_hInstance, MAKEINTRESOURCE(IDD_DIALOG_LISTEDITOR), hWndParent, CSpeedDial::SpeedDialProc, (LPARAM)hKey);
	DWORD error = GetLastError();
}

void CSpeedDial::LoadPhoneItems(HKEY hKey, list<PhoneRecord*>* pLaunchItems, CPOutlook* pOutlook)
{
	DWORD dwCount = 0;
	RegistryGetDWORD(hKey, NULL, L"Count", &dwCount);

	for (DWORD i = 0; i < dwCount; ++i)
	{
		WCHAR wzOid[10], wzName[10], wzPhone[10], wzCount[5];
		DWORD dwSize = 0;
		_itow(i, wzCount, 10);

		StringCchCopy(wzOid, ARRAYSIZE(wzOid), wzCount);
		StringCchCopy(wzPhone, ARRAYSIZE(wzPhone), wzCount);
		StringCchCopy(wzName, ARRAYSIZE(wzName), wzCount);

		StringCchCat(wzOid, ARRAYSIZE(wzOid), L"_OID");
		StringCchCat(wzPhone, ARRAYSIZE(wzPhone), L"_Phone");
		StringCchCat(wzName, ARRAYSIZE(wzName), L"_Name");

		PhoneRecord* pRecord = new PhoneRecord();
		
		bool bDelete = true;

		if (ERROR_SUCCESS == RegistryGetDWORD(hKey, NULL, wzOid, &(pRecord->oid)))
		{
			if (ERROR_SUCCESS == RegistryGetString(hKey, NULL, wzName, pRecord->wzName, ARRAYSIZE(pRecord->wzName)))
			{
				if (ERROR_SUCCESS == RegistryGetString(hKey, NULL, wzPhone, pRecord->wzPhone, ARRAYSIZE(pRecord->wzPhone)))
				{
					pRecord->uiWidth = pRecord->uiHeight = 1000;
					if (ERROR_SUCCESS == pOutlook->UpdateContact(pRecord->oid, pRecord->wzName, ARRAYSIZE(pRecord->wzName), &pRecord->hBmp, &pRecord->uiWidth, &pRecord->uiHeight))
					{
						bDelete = false;
					}
				}
			}
		}
		if (bDelete)
		{
			delete pRecord;
		}
		else
		{
			pLaunchItems->push_back(pRecord);
		}
	}
}

void CSpeedDial::DestroyPhoneItems(list<PhoneRecord*>* pLaunchItems)
{
	list<PhoneRecord*>::iterator iter = pLaunchItems->begin();

	int iPhoneNodeListCount = 0;

	while (iter != pLaunchItems->end())
	{
		delete *iter;
		iter++;
	}

	pLaunchItems->clear();
}

void CSpeedDial::DestroyPhoneNodes(node* pHead)
{
	while (g_pHeadNode)
	{
		node* pTemp = g_pHeadNode;
		g_pHeadNode = g_pHeadNode->next;

		if (pTemp->hBmp)
		{
			DeleteObject(pTemp->hBmp);
			pTemp->hBmp = NULL;
		}
		if (pTemp->hIcon)
		{
			DestroyIcon(pTemp->hIcon);
			pTemp->hIcon = NULL;
		}

		delete pTemp;
	}
	g_pHeadNode = NULL;
}

node* CSpeedDial::BuildPhoneNodes(int* piNodeLen, int thumbWidth, int thumbHeight)
{
	if (g_iThumbHeight != thumbHeight || g_iThumbWidth != thumbWidth)
	{
		DestroyPhoneNodes(NULL);
	}
	if (!g_pHeadNode)
	{
		node* pHead = NULL;
		node* pPrev = NULL;

		list<PhoneRecord*> pPhoneList = g_phoneList;
		list<PhoneRecord*>::iterator iter = pPhoneList.begin();

		int iPhoneNodeListCount = 0;

		while (iter != pPhoneList.end())
		{
			PhoneRecord* pRecord = *iter;
			node* pCurr = new node();
			if (!pHead)
			{
				pHead = pCurr;
				pCurr = pHead;
			}
			else
			{
				pPrev->next = pCurr;
			}
			pCurr->prev = pPrev;
			pCurr->hIcon = (HICON)INVALID_HANDLE_VALUE;
			pCurr->iIndex = iPhoneNodeListCount;
			pCurr->bCenter = true;

			StringCchCopy(pCurr->wzText, ARRAYSIZE(pCurr->wzText), pRecord->wzName);
			StringCchPrintf(pCurr->wzDetails, ARRAYSIZE(pCurr->wzDetails), L"Call %s", pRecord->wzPhone);
			pCurr->pData = pRecord;

			pCurr->hBmp = pRecord->hBmp;

			pPrev = pCurr;
			iter++;
			iPhoneNodeListCount++;
		}
		g_iNodeLen = iPhoneNodeListCount;
		g_pHeadNode = pHead;
	}
	g_iThumbHeight = thumbHeight;
	g_iThumbWidth = thumbWidth;

	*piNodeLen = g_iNodeLen;
	return g_pHeadNode;
}

bool CSpeedDial::AddContact()
{
	PhoneRecord* newRecord = new PhoneRecord();

	if (S_OK == g_outlookApp.SelectContact(GetForegroundWindow(), newRecord->wzName, ARRAYSIZE(newRecord->wzName), newRecord->wzPhone, ARRAYSIZE(newRecord->wzPhone), &newRecord->oid))
	{
		g_phoneList.push_front(newRecord);
		DestroyPhoneNodes(NULL);
		
		return true;
	}
	else
	{
		delete newRecord;
	}
	return false;
}

DrawThumbnailApi(SpeedDialerDrawThumbnail)
{
	return false;
}

NodeSelectedApi(SpeedDialerNodeSelected)
{
	CSpeedDial* pSD = (CSpeedDial*)((FlipperPlugin*)pPlugin)->pData;
	PhoneRecord* pRecord = (PhoneRecord*)pNode->pData;
	*pbRefresh = false;

	pSD->g_pFlipper->Hide(pPlugin);

	PHONEMAKECALLINFO info = {0};
	info.cbSize = sizeof(PHONEMAKECALLINFO);
	info.dwFlags = PMCF_DEFAULT;// PMCF_PROMPTBEFORECALLING;
	info.pszDestAddress = pRecord->wzPhone;
	info.pszCalledParty = pRecord->wzName;
	PhoneMakeCall(&info);
}

CloseOnSelectionApi(SpeedDialerCloseOnSelection)
{
	return true;
}

BuildNodeListApi(SpeedDialerBuildNodeList)
{
	CSpeedDial* pSD = (CSpeedDial*)((FlipperPlugin*)pPlugin)->pData;

	return pSD->BuildPhoneNodes(piNodeLen, *piThumbWidth, *piThumbHeight);
}

SetSoftKeysApi(SpeedDialerSetSoftKeys)
{
	StringCchCopy(pLeftSoftKey->wzText, ARRAYSIZE(pLeftSoftKey->wzText), L"Send Text");
	StringCchCopy(pRightSoftKey->wzText, ARRAYSIZE(pRightSoftKey->wzText), L"View Details");

	pLeftSoftKey->bEnabled = true;
	pRightSoftKey->bEnabled = true;
}

SoftKeySelectedApi(SpeedDialerSoftKeySelected)
{
	CSpeedDial* pSD = (CSpeedDial*)((FlipperPlugin*)pPlugin)->pData;
	PhoneRecord* pRecord = (PhoneRecord*)pNode->pData;
	if (bLeftSoftKey)
	{
		pSD->g_pFlipper->Hide(pPlugin);
		CMapi::SendSMS(pRecord->wzName, pRecord->wzPhone);
		Sleep(0);
	}
	else
	{
		pSD->g_pFlipper->Hide(pPlugin);
		pSD->g_outlookApp.ShowContact(pRecord->oid, GetForegroundWindow());
		Sleep(0);
	}
}

DestroyNodeListApi(SpeedDialerDestroyNodeList)
{
	CSpeedDial* pSD = (CSpeedDial*)((FlipperPlugin*)pPlugin)->pData;

	if (bProcessExiting)
	{
		pSD->DestroyPhoneNodes(pHeadNode);
		pSD->DestroyPhoneItems(&(pSD->g_phoneList));
	}
}

BOOL CALLBACK CSpeedDial::SpeedDialProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{
	static HWND hwndList;
	static CPOutlook* g_pOutlook = NULL;
	static HKEY g_hKey = NULL;
	static list<PhoneRecord*> launchItems;
	TEXTMETRIC tm;

	static CDlgAnchor g_dlgAnchor;

    switch (message) 
    {
		case WM_SIZE :
		{
			g_dlgAnchor.OnSize();
		}
		break;
		case WM_MEASUREITEM :
			{
				LPMEASUREITEMSTRUCT lpmis;
				lpmis = (MEASUREITEMSTRUCT FAR*) lParam;
				if (GetTextMetricsW(GetDC(hwndDlg), &tm))
				{
					lpmis->itemHeight = (UINT)(tm.tmHeight * 2.5);
				}
				else
				{
					lpmis->itemHeight = 50;
				}
			}
			break;
		case WM_DRAWITEM :
			{
				WCHAR tchBuffer[256] = {0};
				int y = 0;
				LPDRAWITEMSTRUCT lpdis = (DRAWITEMSTRUCT FAR*) lParam;

				PhoneRecord* pRecord = (PhoneRecord*)lpdis->itemData;
				GetTextMetrics(lpdis->hDC, &tm);

				HBRUSH hBrush = NULL;
				SetBkMode(lpdis->hDC, TRANSPARENT);
				if ((lpdis->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
				{
					if ((lpdis->itemState & ODS_SELECTED))
					{
						hBrush = GetSysColorBrush(COLOR_HIGHLIGHT);
						SetTextColor(lpdis->hDC, GetSysColor(COLOR_HIGHLIGHTTEXT));
					}
					else
					{
						hBrush = GetSysColorBrush(COLOR_WINDOW);
						SetTextColor(lpdis->hDC, GetSysColor(COLOR_WINDOWTEXT));
					}
				}
				if (hBrush)
				{
					int desiredHeight = (lpdis->rcItem.bottom - lpdis->rcItem.top) - 2;

					RECT rect;
					memcpy(&rect, &(lpdis->rcItem), sizeof(RECT));
					rect.top += 1;
					rect.bottom -= 1;
					rect.left += 1;
					rect.right -= 1;
					rect.left += desiredHeight;

					FillRect(lpdis->hDC, &rect, hBrush);

					rect.left += 5;
					rect.right -= 5;
					rect.bottom -= 5;
					rect.top += 5;

					DrawText(lpdis->hDC, pRecord->wzName, wcslen(pRecord->wzName), &rect, DT_TOP);
					DrawText(lpdis->hDC, pRecord->wzPhone, wcslen(pRecord->wzPhone), &rect, DT_BOTTOM);

					//HBITMAP hBmp;
					//UINT uiWidth, uiHeight;
					//uiWidth = uiHeight = desiredHeight;
					//g_pOutlook->FindBitmap(pRecord->oid, &hBmp, &uiWidth, &uiHeight);

					HDC hDC = CreateCompatibleDC(lpdis->hDC);
					HGDIOBJ hOld = SelectObject(hDC, pRecord->hBmp);
					StretchBlt(lpdis->hDC, lpdis->rcItem.left + 1, lpdis->rcItem.top + 1, desiredHeight, desiredHeight, hDC, 0, 0, pRecord->uiWidth, pRecord->uiHeight, SRCCOPY);
					SelectObject(hDC, hOld);
					//DeleteObject(hBmp);
				}
			}
			break;
        case WM_INITDIALOG:
        {
			g_dlgAnchor.Init(hwndDlg);
			g_dlgAnchor.Add(IDC_LIST_SPEEDDIAL, ANCHOR_ALL);
			g_dlgAnchor.Add(IDC_BUTTON_ADD, ANCHOR_RIGHT | ANCHOR_BOTTOM);
			g_dlgAnchor.Add(IDC_BUTTON_REMOVE, ANCHOR_RIGHT | ANCHOR_TOP);
			g_dlgAnchor.Add(IDC_BUTTON_MOVEDOWN, ANCHOR_RIGHT | ANCHOR_TOP);
			g_dlgAnchor.Add(IDC_BUTTON_MOVEUP, ANCHOR_RIGHT | ANCHOR_TOP);

			g_pOutlook = new CPOutlook();
			hwndList = GetDlgItem(hwndDlg, IDC_LIST_SPEEDDIAL);

            SHINITDLGINFO shidi;
            // Create a Done button and size it.
            shidi.dwMask = SHIDIM_FLAGS;
            shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN;
            shidi.hDlg = hwndDlg;
            SHInitDialog(&shidi);

            SHMENUBARINFO mbi;
            memset(&mbi, 0, sizeof(SHMENUBARINFO));
            mbi.cbSize = sizeof(SHMENUBARINFO);
            mbi.hwndParent = hwndDlg;
            mbi.nToolBarId = IDR_Flipper_SET_MENUBAR;
			mbi.hInstRes = CSpeedDial::g_hInstance;
            mbi.nBmpId = 0;
            mbi.cBmpImages = 0;    

            SHCreateMenuBar(&mbi);

			RECT rect;
			GetWindowRect(hwndList, &rect);
			
			LV_COLUMN lvC;
			lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
			lvC.fmt = LVCFMT_LEFT;  // left-align column
			lvC.cx = rect.right - rect.left - 2;
			lvC.pszText = L"Name";

			HKEY hKey = (HKEY)lParam;
			g_hKey = hKey;

			CSpeedDial::LoadPhoneItems(hKey, &launchItems, g_pOutlook);

			list<PhoneRecord*>::iterator iter = launchItems.begin();
			while (iter != launchItems.end())
			{
				PhoneRecord* pRecord = *iter;
				int index = ListBox_InsertString(hwndList, -1, pRecord->wzName);
				ListBox_SetItemData(hwndList, index, pRecord);
				iter++;
			}
			SetFocus(hwndList);

            return 0;
        }
        break;
        case WM_COMMAND: 
			{
				if (HIWORD(wParam) == LBN_SELCHANGE)
				{
					int index = ListBox_GetCurSel(hwndList);
					int count = ListBox_GetCount(hwndList);

					bool bMoveUp = (index > 0);
					bool bMoveDown = (index < (count - 1));

					if (index < 0)
					{
						bMoveUp = bMoveDown = false;
					}

					EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON_REMOVE), index >= 0);
					EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON_MOVEUP), bMoveUp);
					EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON_MOVEDOWN), bMoveDown);
				}
				else if (HIWORD(wParam) == BN_CLICKED)
				{
					switch (LOWORD(wParam)) 
					{
						case IDC_BUTTON_ADD :
						{
							PhoneRecord* pRecord = new PhoneRecord();
							SetCursor(LoadCursor(NULL, IDC_WAIT));
							if (ERROR_SUCCESS == g_pOutlook->SelectContact(hwndDlg, pRecord->wzName, ARRAYSIZE(pRecord->wzName), pRecord->wzPhone, ARRAYSIZE(pRecord->wzPhone), &(pRecord->oid)))
							{
								pRecord->uiWidth = pRecord->uiHeight = 1000;
								if (ERROR_SUCCESS == g_pOutlook->UpdateContact(pRecord->oid, pRecord->wzName, ARRAYSIZE(pRecord->wzName), &pRecord->hBmp, &pRecord->uiWidth, &pRecord->uiHeight))
								{
									int index = ListBox_InsertString(hwndList, -1, pRecord->wzName);
									ListBox_SetItemData(hwndList, index, pRecord);
									ListBox_SetCurSel(hwndList, index);
									PostMessage(hwndDlg, WM_COMMAND, MAKELONG(0, LBN_SELCHANGE), 0);
								}
							}
							else
							{
								delete pRecord;
							}
							SetCursor(NULL);
						}
						break;
						case IDC_BUTTON_REMOVE :
						{
							int index = ListBox_GetCurSel(hwndList);

							if (index >= 0)
							{
								PhoneRecord* pRecord = (PhoneRecord*)ListBox_GetItemData(hwndList, index);
								ListBox_DeleteString(hwndList, index);
								delete pRecord;
							}
							int count = ListBox_GetCount(hwndList);
							ListBox_SetCurSel(hwndList, min(count-1, index));
							PostMessage(hwndDlg, WM_COMMAND, MAKELONG(0, LBN_SELCHANGE), 0);
						}
						break;
						case IDC_BUTTON_MOVEUP :
						{
							int index = ListBox_GetCurSel(hwndList);
							if (index > 0)
							{
								PhoneRecord* pRecord = (PhoneRecord*)ListBox_GetItemData(hwndList, index);
								ListBox_DeleteString(hwndList, index);
								ListBox_InsertString(hwndList, index-1, pRecord->wzName);
								ListBox_SetItemData(hwndList, index-1, pRecord);
								ListBox_SetCurSel(hwndList, index-1);
								PostMessage(hwndDlg, WM_COMMAND, MAKELONG(0, LBN_SELCHANGE), 0);
							}
						}
						break;
						case IDC_BUTTON_MOVEDOWN :
						{
							int index = ListBox_GetCurSel(hwndList);
							int count = ListBox_GetCount(hwndList);
							if (index < count - 1)
							{
								PhoneRecord* pRecord = (PhoneRecord*)ListBox_GetItemData(hwndList, index);
								ListBox_DeleteString(hwndList, index);
								ListBox_InsertString(hwndList, index+1, pRecord->wzName);
								ListBox_SetItemData(hwndList, index+1, pRecord);
								ListBox_SetCurSel(hwndList, index+1);
								PostMessage(hwndDlg, WM_COMMAND, MAKELONG(0, LBN_SELCHANGE), 0);
							}
						}
						break;
					}
				}
				switch (LOWORD(wParam))
				{
					case IDM_Flipper_SET_ACCEPT :
					case IDOK:
						{
							int index = 0;

							WCHAR wzName[MAX_PATH];
							DWORD dwName = sizeof(wzName);
							DWORD dwType;

							while (ERROR_SUCCESS == RegEnumValue(g_hKey, index, wzName, &dwName, NULL, &dwType, NULL, NULL))
							{
								dwName = sizeof(wzName);
								RegDeleteValue(g_hKey, wzName);
							}
							int count = ListBox_GetCount(hwndList);
							RegistrySetDWORD(g_hKey, NULL, L"Count", count);

							for (int i = 0; i < count; ++i)
							{
								WCHAR wzOid[10], wzName[10], wzPhone[10], wzCount[5];
								DWORD dwSize = 0;
								_itow(i, wzCount, 10);

								StringCchCopy(wzOid, ARRAYSIZE(wzOid), wzCount);
								StringCchCopy(wzPhone, ARRAYSIZE(wzPhone), wzCount);
								StringCchCopy(wzName, ARRAYSIZE(wzName), wzCount);

								StringCchCat(wzOid, ARRAYSIZE(wzOid), L"_OID");
								StringCchCat(wzPhone, ARRAYSIZE(wzPhone), L"_Phone");
								StringCchCat(wzName, ARRAYSIZE(wzName), L"_Name");

								PhoneRecord* pRecord = (PhoneRecord*)ListBox_GetItemData(hwndList, i);
								
								RegistrySetDWORD(g_hKey, NULL, wzOid, pRecord->oid);
								RegistrySetString(g_hKey, NULL, wzName, pRecord->wzName);
								RegistrySetString(g_hKey, NULL, wzPhone, pRecord->wzPhone);
							}

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
				if (g_pOutlook)
				{
					delete g_pOutlook;
				}
				CSpeedDial::DestroyPhoneItems(&launchItems);
                Sleep(0);
            }
            break;
    } 
    return FALSE; 
}

