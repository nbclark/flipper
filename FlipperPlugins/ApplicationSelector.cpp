#include "stdafx.h"
#include <regext.h>
#include <aygshell.h>
#include <shellapi.h>
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <soundfile.h>
#include <time.h>
#include <notify.h>
#include <atltime.h>
#include <regext.h>
#include <tlhelp32.h>
#include "resource.h"
#include "ApplicationSelector.h"
#include "..\flipper\utils.h"
#include "..\FlipperConfig\Anchor.h"

HMODULE CApplicationSelector::g_hInstance = NULL;
list<LVITEM*> CApplicationSelector::g_lvApplications;
HIMAGELIST CApplicationSelector::g_hSmall = NULL;

CApplicationSelector::CApplicationSelector(void)
{
	Sleep(0);
}

CApplicationSelector::~CApplicationSelector(void)
{
	Sleep(0);
}

bool CApplicationSelector::ShowDialog(HINSTANCE hInstance, HWND hwndParent, WCHAR* wzLnkLocation, bool bChooseProcess)
{
	g_hInstance = hInstance;
	g_wzLnkLocation = wzLnkLocation;
	g_bChooseProcess = bChooseProcess;
	return IDOK == DialogBoxParamW(hInstance, MAKEINTRESOURCE(IDD_SELECTLINK_DIALOG), hwndParent, CApplicationSelector::SelectLaunchProc, (LPARAM)this);
}

void CApplicationSelector::GetLnkFiles(WCHAR* szPath, HIMAGELIST hSmall, list<LVITEM*>* plvItems, bool bReadOnly)
{
    if (!plvItems && !bReadOnly)
    {
        return;
    }
    WCHAR szFilteredPath[MAX_PATH];
    wcscpy(szFilteredPath, szPath);
    wcscat(szFilteredPath, L"*");

    WIN32_FIND_DATAW data;
    HANDLE hFile = FindFirstFile(szFilteredPath, &data);

    do
    {
        if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            // recurse
            WCHAR szNewPath[MAX_PATH];
            wcscpy(szNewPath, szPath);
            wcscat(szNewPath, data.cFileName);
            wcscat(szNewPath, L"\\");

            GetLnkFiles(szNewPath, hSmall, plvItems, bReadOnly);
        }
        else
        {
            WCHAR* wzExt = wcsrchr(data.cFileName, L'.');
            if (wzExt && 0 == wcsicmp(wzExt, L".lnk") && 0 != wcsicmp(data.cFileName, L"icon.lnk"))
            {
                if (!bReadOnly)
                {
                    wcscpy(szFilteredPath, szPath);
                    wcscat(szFilteredPath, data.cFileName);

					LVITEM* plvItem = new LVITEM();
					memset(plvItem, 0, sizeof(LVITEM));
					plvItem->mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE | LVIF_PARAM;
					plvItem->state = 0;      
					plvItem->stateMask = 0;

					plvItems->push_back(plvItem);

                    HICON hIcon = GetApplicationIcon(szFilteredPath, true, NULL, NULL, NULL);

                    if (hIcon)
                    {
                        int iIndex = ImageList_ReplaceIcon(hSmall, -1, hIcon);
                        plvItem->iImage = iIndex;

                        DestroyIcon(hIcon);
                    }
                    else
                    {
                        plvItem->iImage = 0;
                    }

                    int iLen = wcslen(data.cFileName)+1;
                    plvItem->pszText = new WCHAR[iLen];
                    wcscpy(plvItem->pszText, data.cFileName);
                    plvItem->cchTextMax = iLen;

                    WCHAR* wzExt = wcschr(plvItem->pszText, L'.');

                    if (wzExt)
                    {
                        wzExt[0] = 0;
                    }
                    
                    iLen = wcslen(szFilteredPath)+1;
                    plvItem->lParam = (LPARAM)new WCHAR[iLen];
                    wcscpy((WCHAR*)plvItem->lParam, szFilteredPath);
                }
            }
        }
    } while (FindNextFile(hFile, &data));
}


int CALLBACK CApplicationSelector::ListViewCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    WCHAR* wzItem1 = (WCHAR*)lParam1;
    WCHAR* wzItem2 = (WCHAR*)lParam2;

    WCHAR* wzName1 = wcsrchr(wzItem1, L'\\');
    WCHAR* wzName2 = wcsrchr(wzItem2, L'\\');

    return (wcsicmp(wzName1, wzName2));
}

BOOL CALLBACK CApplicationSelector::SelectLaunchProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{
    HWND hwndList;
    static HWND hWndMenu = NULL;
	static WCHAR* wzLnkLocation = NULL;
	static CApplicationSelector* p_this = NULL;
	static CDlgAnchor g_dlgAnchor;

    switch (message) 
    { 
		case WM_SIZE :
		{
			g_dlgAnchor.OnSize();
		}
		break;
        case WM_INITDIALOG:
        {
            hwndList = GetDlgItem(hwndDlg, IDC_LIST_LINK);
			p_this = (CApplicationSelector*)lParam;
			wzLnkLocation = p_this->g_wzLnkLocation;

            RECT rect, rectText;
            GetClientRect(hwndDlg, &rect);
            GetWindowRect(GetDlgItem(hwndDlg, IDC_EDIT_LINK), &rectText);

			if (p_this->g_bChooseProcess)
			{
				MoveWindow(GetDlgItem(hwndDlg, IDC_EDIT_LINK), rect.left + 5, rect.top + 5, (rect.right - rect.left) - 10, (rectText.bottom - rectText.top), TRUE);
				MoveWindow(hwndList, rect.left + 5, rect.top + 10 + (rectText.bottom - rectText.top), (rect.right - rect.left) - 10, (rect.bottom - rect.top) - 15 - (rectText.bottom - rectText.top), TRUE);
			}
			else
			{
				ShowWindow(GetDlgItem(hwndDlg, IDC_EDIT_LINK), SW_HIDE);
				MoveWindow(hwndList, rect.left + 5, rect.top + 5, (rect.right - rect.left) - 10, (rect.bottom - rect.top) - 10, TRUE);
			}
			g_dlgAnchor.Init(hwndDlg);
			g_dlgAnchor.Add(IDC_LIST_LINK, ANCHOR_ALL);
			g_dlgAnchor.Add(IDC_EDIT_LINK, ANCHOR_ALL & ~ANCHOR_BOTTOM);
            
            SHMENUBARINFO mbi;
            memset(&mbi, 0, sizeof(SHMENUBARINFO));
            mbi.cbSize = sizeof(SHMENUBARINFO);
            mbi.hwndParent = hwndDlg;
            mbi.nToolBarId = IDR_Flipper_SET_MENUBAR;
			mbi.hInstRes = CApplicationSelector::g_hInstance;
            mbi.nBmpId = 0;
            mbi.cBmpImages = 0;    
            
            if (FALSE == SHCreateMenuBar(&mbi))
            {
                MessageBox(hwndDlg, L"SHCreateMenuBar Failed", L"Error", MB_OK);
            }
            hWndMenu = mbi.hwndMB;
            
            SHINITDLGINFO shidi;
            // Create a Done button and size it.
            shidi.dwMask = SHIDIM_FLAGS;
            shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN;
            shidi.hDlg = hwndDlg;
            SHInitDialog(&shidi);

            LV_COLUMN lvC;
            lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
            lvC.fmt = LVCFMT_LEFT;  // left-align column
            lvC.cx = 400;            // width of column in pixels
            lvC.pszText = L"File";

            //0x00A00000 | 
            SetWindowLong(hwndList, GWL_STYLE, WS_BORDER | WS_VSCROLL | LVS_NOCOLUMNHEADER | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_SHAREIMAGELISTS);
            SetWindowLong(hwndList, GWL_EXSTYLE, LVS_EX_NOHSCROLL | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES);

            if (NULL == CApplicationSelector::g_hSmall)
            {
				int iconSize = GetSystemMetrics(SM_CXSMICON);
                CApplicationSelector::g_hSmall = ImageList_Create(iconSize, iconSize, ILC_COLOR, 0, 32);
                ImageList_SetBkColor(CApplicationSelector::g_hSmall, RGB(0xFF, 0xFF, 0xFF));

				HICON hIcon = LoadIconW(CApplicationSelector::g_hInstance, MAKEINTRESOURCE(IDI_ICON1));
                DRA::ImageList_AddIcon(CApplicationSelector::g_hSmall, hIcon);
				DestroyIcon(hIcon);

                int dwApplications = 0;

                TCHAR szStartMenu[MAX_PATH];
                if (SHGetSpecialFolderPath(NULL, szStartMenu, CSIDL_STARTMENU, FALSE))
                {
                    wcscat(szStartMenu, L"\\");
                }
                else
                {
                    wcscpy(szStartMenu, L"\\windows\\start menu\\");
                }
                GetLnkFiles(szStartMenu, CApplicationSelector::g_hSmall, &(CApplicationSelector::g_lvApplications), false);
            }

            ListView_InsertColumn(hwndList, 0, &lvC);
            ListView_SetImageList(hwndList, CApplicationSelector::g_hSmall, LVSIL_SMALL);
            int iIndex = 0;

			for (list<LVITEM*>::iterator iter = CApplicationSelector::g_lvApplications.begin(); iter != CApplicationSelector::g_lvApplications.end(); ++iter)
            {
				LVITEM* pItem = (*iter);
                // Always append to the end
                pItem->iItem = iIndex++;
                iIndex = ListView_InsertItem(hwndList, pItem);
                
                if (wzLnkLocation && 0 == wcsicmp((WCHAR*)pItem->lParam, wzLnkLocation))
                {
                    ListView_SetItemState(hwndList, iIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED)
                    ListView_SetSelectionMark(hwndList, iIndex);
                }
            }

            ListView_SortItems(hwndList, ListViewCompareProc, 0);

            int iSelIndex = ListView_GetSelectionMark(hwndList);
            ListView_EnsureVisible(hwndList, iSelIndex, FALSE);
            
            SetCursor(NULL);

            return 0;
        }
        break;
        case WM_NOTIFY :
        {
            hwndList = GetDlgItem(hwndDlg, IDC_LIST_LINK);
            NMHDR * pnmh = (NMHDR *)lParam;

            switch (pnmh->code)
            {
                case NM_SETFOCUS :
                {
                    int iItem = ListView_GetSelectionMark(hwndList);
                    SetFocus(hwndList);
                    SetActiveWindow(hwndList);
                }
                break;
				case LVN_ITEMCHANGED :
				{
					LPNMLISTVIEW pnmv = (LPNMLISTVIEW) lParam;

					if (pnmv->uNewState & LVIS_SELECTED)
					{
						WCHAR* wzSlash = NULL;
						if (p_this->g_bChooseProcess)
						{
							WCHAR wzExe[MAX_PATH];
							SHGetShortcutTarget((WCHAR*)pnmv->lParam, wzExe, ARRAYSIZE(wzExe));

							WCHAR* wzReturn = wcschr(wzExe, L'\r');

							if (wzReturn)
							{
								wzReturn[0] = '\0';
							}

							while (wzExe[0] == L':')
							{
								WCHAR* wzSpace = wcschr(wzExe, L' ');

								if (wzSpace)
								{
									wzSpace[0] = L'\0';
								}

								HKEY hKey = NULL;
								if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Shell\\Rai", 0, KEY_READ, &hKey))
								{
									bool bSuccess = false;
									if (ERROR_SUCCESS == RegistryGetString(hKey, wzExe, L"1", wzExe, ARRAYSIZE(wzExe)))
									{
										bSuccess = true;
									}
									RegCloseKey(hKey);

									if (!bSuccess)
									{
										break;
									}
								}
							}

							wzSlash = wcsrchr(wzExe, L'\\');
							if (!wzSlash)
							{
								wzSlash = wzExe;
							}

							WCHAR* wzDelim = wcschr(wzSlash, L' ');
							if (wzDelim)
							{
								wzDelim[0] = L'\0';
							}
							wzDelim = wcschr(wzSlash, L',');
							if (wzDelim)
							{
								wzDelim[0] = L'\0';
							}
						}
						else
						{
							WCHAR wzExe[MAX_PATH];

							StringCchCopy(wzExe, ARRAYSIZE(wzExe), (WCHAR*)pnmv->lParam);
							wzSlash = wzExe;
						}

						SetDlgItemTextW(hwndDlg, IDC_EDIT_LINK, wzSlash);
					}
				}
				break;
			}
            Sleep(0);
        }
        break;
        case WM_COMMAND: 
            switch (LOWORD(wParam)) 
            {
                case IDOK:
                    {
                        EndDialog(hwndDlg, IDOK);
                    }
                    break;
                case IDM_Flipper_SET_ACCEPT:
                    {
                        EndDialog(hwndDlg, IDOK);
                    }
                    break;
                case IDM_Flipper_SET_CANCEL:
                    {
                        EndDialog(hwndDlg, IDCANCEL);
                    }
                    break;
            }
            break;
        case WM_DESTROY:
            {
				//g_dlgAnchor.RemoveAll();
				GetDlgItemText(hwndDlg, IDC_EDIT_LINK, wzLnkLocation, MAX_PATH);

                if (hWndMenu)
                {
                    DestroyWindow(hWndMenu);
					hWndMenu = NULL;
                }
            }
            break;
    } 
    return DefWindowProcW(hwndDlg, message, wParam, lParam); 
}

