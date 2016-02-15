#include "stdafx.h"
#include <regext.h>
#include <aygshell.h>
#include <shellapi.h>
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>   // includes the common control header
#include "QuickLaunch.h"
#include "resource.h"
#include "..\FlipperConfig\Anchor.h"

HMODULE CQuickLaunch::g_hInstance = NULL;

CQuickLaunch::CQuickLaunch(void)
{
	g_iThumbWidth = g_iThumbHeight = 0;
	g_pHeadNode = NULL;
}

CQuickLaunch::~CQuickLaunch(void)
{
	DestroyLaunchItems(&g_launchList);
	DestroyNodeList(g_pHeadNode);
	g_pHeadNode = NULL;
}

void CQuickLaunch::LoadSettings(IFlipperApp* pFlipper, HKEY hKey)
{
	g_pFlipper = pFlipper;
	LoadLaunchItems(hKey, &g_launchList);
}

void CQuickLaunch::ShowSettings(HWND hWndParent, HKEY hKey)
{
	DWORD x = DialogBoxParamW(g_hInstance, MAKEINTRESOURCE(IDD_DIALOG_LISTEDITOR), hWndParent, CQuickLaunch::QuickLaunchProc, (LPARAM)hKey);
	DWORD error = GetLastError();
}

void CQuickLaunch::SaveLaunchItems(HKEY hKey, list<LaunchItem*>* pLaunchItems)
{
	WCHAR wzName[MAX_PATH];
	DWORD dwName = sizeof(wzName);
	DWORD dwType;
	int index = 0;

	while (ERROR_SUCCESS == RegEnumValue(hKey, index, wzName, &dwName, NULL, &dwType, NULL, NULL))
	{
		dwName = sizeof(wzName);
		RegDeleteValue(hKey, wzName);
	}
	int count = pLaunchItems->size();
	RegistrySetDWORD(hKey, NULL, L"Count", count);

	index = 0;
	list<LaunchItem*>::iterator iter = pLaunchItems->begin();

	while (iter != pLaunchItems->end())
	{
		WCHAR wzPath[10], wzCount[5];
		DWORD dwSize = 0;
		_itow(index, wzCount, 10);

		StringCchCopy(wzPath, ARRAYSIZE(wzPath), wzCount);
		StringCchCat(wzPath, ARRAYSIZE(wzPath), L"_Path");

		LaunchItem* pRecord = *iter;
		RegistrySetString(hKey, NULL, wzPath, pRecord->wzPath);

		iter++;
		index++;
	}
}

void CQuickLaunch::LoadLaunchItems(HKEY hKey, list<LaunchItem*>* pLaunchItems)
{
	DWORD dwCount = 0;
	RegistryGetDWORD(hKey, NULL, L"Count", &dwCount);

	for (DWORD i = 0; i < dwCount; ++i)
	{
		WCHAR wzPath[10],wzCount[5];
		DWORD dwSize = 0;
		_itow(i, wzCount, 10);

		StringCchCopy(wzPath, ARRAYSIZE(wzPath), wzCount);
		StringCchCat(wzPath, ARRAYSIZE(wzPath), L"_Path");

		LaunchItem* pRecord = new LaunchItem();
		
		bool bDelete = true;

		if (ERROR_SUCCESS == RegistryGetString(hKey, NULL, wzPath, pRecord->wzPath, ARRAYSIZE(pRecord->wzPath)))
		{
			pRecord->hIcon = GetApplicationIcon(pRecord->wzPath, false, NULL, NULL, NULL);
			
			// TODO: Load default icon
			if (!pRecord->hIcon)
			{
				pRecord->hIcon = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_ICON1));
			}
			bDelete = false;
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

void CQuickLaunch::DestroyLaunchItems(list<LaunchItem*>* pLaunchItems)
{
	list<LaunchItem*>::iterator iter = pLaunchItems->begin();

	int iPhoneNodeListCount = 0;

	while (iter != pLaunchItems->end())
	{
		delete *iter;
		iter++;
	}

	pLaunchItems->clear();
}

void CQuickLaunch::DestroyPhoneNodes(node* pHead)
{
	//ASSERT(pHead == g_pHeadNode);
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

node* CQuickLaunch::BuildLaunchList(int* piNodeLen, int thumbWidth, int thumbHeight)
{
	if (g_iThumbHeight != thumbHeight || g_iThumbWidth != thumbWidth)
	{
		DestroyPhoneNodes(NULL);
	}
	if (!g_pHeadNode)
	{
		node* pHead = NULL;
		node* pPrev = NULL;

		list<LaunchItem*> pPhoneList = g_launchList;
		list<LaunchItem*>::iterator iter = pPhoneList.begin();

		int iPhoneNodeListCount = 0;

		while (iter != pPhoneList.end())
		{
			LaunchItem* pRecord = *iter;
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
			pRecord->hIcon = GetApplicationIcon(pRecord->wzPath, false, NULL, NULL, NULL);
			pCurr->prev = pPrev;
			pCurr->hIcon = pRecord->hIcon;
			pCurr->iIndex = iPhoneNodeListCount;
			pCurr->bCenter = true;
			pCurr->hBmp = NULL;

			Sleep(5);

			WCHAR* wzName = wcsrchr(pRecord->wzPath, L'\\');
			if (!wzName)
			{
				wzName = pRecord->wzPath;
			}
			else
			{
				wzName++;
			}

			StringCchCopy(pCurr->wzText, ARRAYSIZE(pCurr->wzText), wzName);
			WCHAR* wzExt = wcsrchr(pCurr->wzText, '.');
			if (wzExt)
			{
				wzExt[0] = 0;
			}

			StringCchCopy(pCurr->wzDetails, ARRAYSIZE(pCurr->wzDetails), L"Select to Launch");
			/*
			StringCchCopy(pCurr->wzDetails, ARRAYSIZE(pCurr->wzDetails), pRecord->wzPath);
			wzName = wcsrchr(pCurr->wzDetails, L'\\');
			if (wzName)
			{
				wzName[0] = 0;
			}
			*/

			pCurr->pData = pRecord;

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

DrawThumbnailApi(QuickLaunchDrawThumbnail)
{
	return false;
}

NodeSelectedApi(QuickLaunchNodeSelected)
{
	CQuickLaunch* pSD = (CQuickLaunch*)((FlipperPlugin*)pPlugin)->pData;
	*pbRefresh = false;
	LaunchItem* pRecord = (LaunchItem*)pNode->pData;
	if (pRecord->wzPath)
	{
		// Open it here
		LaunchProgram(pRecord->wzPath, NULL);
	}
}

CloseOnSelectionApi(QuickLaunchCloseOnSelection)
{
	return true;
}

BuildNodeListApi(QuickLaunchBuildNodeList)
{
	CQuickLaunch* pSD = (CQuickLaunch*)((FlipperPlugin*)pPlugin)->pData;

	//*piThumbHeight = (int)(GetSystemMetrics(SM_CXICON)) + 20;
	//*piThumbWidth = *piThumbHeight;

	return pSD->BuildLaunchList(piNodeLen, *piThumbWidth, *piThumbHeight);
}

SetSoftKeysApi(QuickLaunchSetSoftKeys)
{
	StringCchCopy(pLeftSoftKey->wzText, ARRAYSIZE(pLeftSoftKey->wzText), L"Add Item");
	StringCchCopy(pRightSoftKey->wzText, ARRAYSIZE(pRightSoftKey->wzText), L"Remove Item");

	pLeftSoftKey->bEnabled = true;
	pRightSoftKey->bEnabled = true;
}

SoftKeySelectedApi(QuickLaunchSoftKeySelected)
{
	CQuickLaunch* pSD = (CQuickLaunch*)((FlipperPlugin*)pPlugin)->pData;

	bool bChanged = false;
	if (bLeftSoftKey)
	{
		LaunchItem* pRecord = new LaunchItem();
		SetCursor(LoadCursor(NULL, IDC_WAIT));

		CApplicationSelector* appSelector = new CApplicationSelector();
		// TODO
		if (appSelector->ShowDialog(CQuickLaunch::g_hInstance, GetForegroundWindow(), pRecord->wzPath, false))
		{
			pRecord->hIcon = GetApplicationIcon(pRecord->wzPath, false, NULL, NULL, NULL);
			pSD->g_launchList.push_back(pRecord);
			bChanged = true;
		}
		else
		{
			delete pRecord;
		}
		delete appSelector;
		SetCursor(NULL);
	}
	else
	{
		if (pNode)
		{
			LaunchItem* pItem = (LaunchItem*)pNode->pData;
			pSD->g_launchList.remove(pItem);

			delete pItem;
			delete pNode;

			bChanged=true;
		}
	}
	if (bChanged)
	{
		HKEY hKey;
		pSD->g_pFlipper->OpenRegistry(pPlugin, &hKey);
		CQuickLaunch::SaveLaunchItems(hKey, &(pSD->g_launchList));
		RegCloseKey(hKey);

		int iNodeLen = 0;
		pSD->DestroyPhoneNodes(NULL);
		node* pHeadNode = pSD->BuildLaunchList(&iNodeLen, pSD->g_iThumbWidth, pSD->g_iThumbHeight);
		pSD->g_pFlipper->Redraw(pPlugin, pHeadNode, iNodeLen);
	}
}

DestroyNodeListApi(QuickLaunchDestroyNodeList)
{
	CQuickLaunch* pSD = (CQuickLaunch*)((FlipperPlugin*)pPlugin)->pData;

	if (bProcessExiting)
	{
		pSD->DestroyPhoneNodes(pHeadNode);
		pSD->DestroyLaunchItems(&(pSD->g_launchList));
	}
}

BOOL CALLBACK CQuickLaunch::QuickLaunchProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{
	static HWND hwndList;
	static HKEY g_hKey = NULL;
	static list<LaunchItem*> launchItems;

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

				LaunchItem* pRecord = (LaunchItem*)lpdis->itemData;
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

					WCHAR wzFullPath[MAX_PATH];
					WCHAR wzName[MAX_PATH];
					StringCchCopy(wzFullPath, ARRAYSIZE(wzFullPath), pRecord->wzPath);
					StringCchCopy(wzName, ARRAYSIZE(wzName), pRecord->wzPath);

					WCHAR* wzFile = wcsrchr(wzName, L'\\');

					if (!wzFile)
					{
						wzFile = wzName;
					}
					else
					{
						wzFile++;
					}
					WCHAR* wzExt = wcsrchr(wzFile, L'.');
					if (wzExt)
					{
						wzExt[0] = 0;
					}

					WCHAR* wzDir = wcsrchr(wzFullPath, L'\\');
					if (wzDir)
					{
						wzDir[0] = 0;
					}

					// todo - trim of .lnk
					DrawText(lpdis->hDC, wzFile, wcslen(wzFile), &rect, DT_TOP);
					
					LOGFONT lf;
					ZeroMemory(&lf, sizeof(lf));
					lf.lfHeight = -((rect.bottom - rect.top) / 4);
					lf.lfWeight = FW_BOLD;
					lf.lfCharSet = DEFAULT_CHARSET;
					HFONT hFont = CreateFontIndirect(&lf);
					HGDIOBJ hOldFont = SelectObject(lpdis->hDC, hFont);
					DrawText(lpdis->hDC, wzFullPath, wcslen(wzFullPath), &rect, DT_BOTTOM);
					SelectObject(lpdis->hDC, hOldFont);

					if (pRecord->hIcon)
					{
						DrawIcon(lpdis->hDC, 0, rect.top, pRecord->hIcon);
					}
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
			mbi.hInstRes = CQuickLaunch::g_hInstance;
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

			CQuickLaunch::LoadLaunchItems(hKey, &launchItems);

			list<LaunchItem*>::iterator iter = launchItems.begin();

			while (iter != launchItems.end())
			{
				LaunchItem* pRecord = *iter;
				int index = ListBox_InsertString(hwndList, -1, pRecord->wzPath);
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
							LaunchItem* pRecord = new LaunchItem();
							SetCursor(LoadCursor(NULL, IDC_WAIT));

							CApplicationSelector* appSelector = new CApplicationSelector();
							// TODO
							if (appSelector->ShowDialog(CQuickLaunch::g_hInstance, hwndDlg, pRecord->wzPath, false))
							{
								pRecord->hIcon = GetApplicationIcon(pRecord->wzPath, false, NULL, NULL, NULL);
								int index = ListBox_InsertString(hwndList, -1, pRecord->wzPath);
								ListBox_SetItemData(hwndList, index, pRecord);
								ListBox_SetCurSel(hwndList, index);
								PostMessage(hwndDlg, WM_COMMAND, MAKELONG(0, LBN_SELCHANGE), 0);
							}
							else
							{
								delete pRecord;
							}
							delete appSelector;
							SetCursor(NULL);
						}
						break;
						case IDC_BUTTON_REMOVE :
						{
							int index = ListBox_GetCurSel(hwndList);

							if (index >= 0)
							{
								LaunchItem* pRecord = (LaunchItem*)ListBox_GetItemData(hwndList, index);
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
								LaunchItem* pRecord = (LaunchItem*)ListBox_GetItemData(hwndList, index);
								ListBox_DeleteString(hwndList, index);
								ListBox_InsertString(hwndList, index-1, pRecord->wzPath);
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
								LaunchItem* pRecord = (LaunchItem*)ListBox_GetItemData(hwndList, index);
								ListBox_DeleteString(hwndList, index);
								ListBox_InsertString(hwndList, index+1, pRecord->wzPath);
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

							list<LaunchItem*> launchItems;
							int count = ListBox_GetCount(hwndList);

							for (int i = 0; i < count; ++i)
							{
								WCHAR wzPath[10], wzCount[5];
								DWORD dwSize = 0;
								_itow(i, wzCount, 10);

								StringCchCopy(wzPath, ARRAYSIZE(wzPath), wzCount);
								StringCchCat(wzPath, ARRAYSIZE(wzPath), L"_Path");

								LaunchItem* pRecord = (LaunchItem*)ListBox_GetItemData(hwndList, i);
								launchItems.push_back(pRecord);
							}

							CQuickLaunch::SaveLaunchItems(g_hKey, &launchItems);

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
				CQuickLaunch::DestroyLaunchItems(&launchItems);
                Sleep(0);
            }
            break;
    } 
    return FALSE; 
}