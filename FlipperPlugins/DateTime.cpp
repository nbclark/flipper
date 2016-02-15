#include "StdAfx.h"
#include <time.h>
#include <regext.h>
#include <aygshell.h>
#include <shellapi.h>
#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include "DateTime.h"
#include "..\FlipperConfig\Anchor.h"

HMODULE CDateTime::g_hInstance = NULL;

CDateTime::CDateTime(IFlipperApp* pFlipper, void* pPlugin)
{
	g_pFlipper = pFlipper;
	g_pPlugin = pPlugin;
	ZeroMemory(&g_dateNode, sizeof(node));

    g_hThumbThread = CreateThread(NULL, 0, &CDateTime::PollDateTime, this, 0, 0);
	SetThreadPriority(g_hThumbThread, THREAD_PRIORITY_BELOW_NORMAL);
}

CDateTime::~CDateTime(void)
{
    if (g_hThumbThread)
    {
        TerminateThread(g_hThumbThread, 0);
        CloseHandle(g_hThumbThread);
        g_hThumbThread = NULL;
    }
}

void CDateTime::LoadSettings(IFlipperApp* pFlipper, HKEY hKey)
{
	RegistryGetDWORD(hKey, NULL, REG_TimeOffset, &g_dwOffset); 
}

void CDateTime::ShowSettings(HWND hWndParent, HKEY hKey)
{
	DWORD x = DialogBoxParamW(g_hInstance, MAKEINTRESOURCE(IDD_DIALOG_CLOCK), hWndParent, CDateTime::DateTimeProc, (LPARAM)hKey);
	DWORD error = GetLastError();
}

node* CDateTime::BuildNodeList(int* piNodeLen, int* piThumbWidth, int* piThumbHeight, int iMaxWidth, int iMaxHeight)
{
	*piThumbWidth = iMaxWidth;
	*piNodeLen = 1;
	return &g_dateNode;
}

bool CDateTime::DrawThumbnail(node* pNode, HDC hdc, HDC hdcThumb, RECT* pRect, bool bSelected)
{
	pRect->right -= (pRect->left*2);

	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0xff, 0xff, 0xff));
	HGDIOBJ hPenOld = SelectObject(hdc, hPen);
    HGDIOBJ hBrushOld = SelectObject(hdc, (HBRUSH)GetStockObject(BLACK_BRUSH));
    RoundRect(hdc, pRect->left, pRect->top, pRect->right, pRect->bottom, 8, 8);
    SelectObject(hdc, hBrushOld);
	SelectObject(hdc, hPenOld);
	DeleteObject(hPen);

    LOGFONT lf;
    ZeroMemory(&lf, sizeof(lf));
    lf.lfHeight = -(pRect->bottom - pRect->top) / 2;
    lf.lfWeight = FW_BOLD;
    lf.lfCharSet = DEFAULT_CHARSET;
    HFONT hFont = CreateFontIndirect(&lf);

    SetBkMode(hdc, TRANSPARENT);
    SetBkColor(hdc, TRANSPARENT);
    HGDIOBJ hOldFont = SelectObject(hdc, hFont);

	SYSTEMTIME st;
	GetLocalTime(&st);
	//GetSystemTime(&st);
	WCHAR wzTime[20];
	TIME_ZONE_INFORMATION info = {0};
	GetTimeZoneInformation(&info);

	wsprintf(wzTime, L"%d:%d %s", (st.wHour > 12) ? (st.wHour - 12) : ((st.wHour == 0) ? 12 : st.wHour), st.wMinute, (st.wHour >= 12) ? L"PM" : L"AM");
	StringCchCopy(g_dateNode.wzText, ARRAYSIZE(g_dateNode.wzText), info.StandardName);

	GetTimeFormat(NULL, NULL, &st, L"h:mm tt", wzTime, ARRAYSIZE(wzTime));

	GetDateFormat(NULL, NULL, &st, L"dddd, MMMM dd, yyyy", g_dateNode.wzDetails, ARRAYSIZE(g_dateNode.wzDetails));

	SetTextColor(hdc, RGB(0xff,0xff,0xff));
	DrawTextW(hdc, wzTime, wcslen(wzTime), pRect, DT_VCENTER | DT_CENTER);

	SelectObject(hdc, hOldFont);
	DeleteObject(hFont);

	return true;
}

DWORD WINAPI CDateTime::PollDateTime(LPVOID lpParameter)
{
    CDateTime* p_this = (CDateTime*)lpParameter;

	HANDLE hEvents[1];
	hEvents[0] = OpenEvent(EVENT_ALL_ACCESS, FALSE, EVENT_FLIPPEREXIT);

    DWORD dwEvent = 0;
	DWORD dwSleep = 1000 * 60;

    while (WAIT_ABANDONED_0 != (dwEvent = WaitForMultipleObjects(ARRAYSIZE(hEvents), hEvents, FALSE, dwSleep)))
    {
        // Update weather here
		if (dwEvent == WAIT_OBJECT_0)
		{
			break;
		}
		else
		{
			SYSTEMTIME st;
			GetLocalTime(&st);

			dwSleep = 1000 * (60 - st.wSecond);

			p_this->g_pFlipper->Redraw(p_this->g_pPlugin, &(p_this->g_dateNode), 1);
		}
    }
    CloseHandle(hEvents[0]);

    return 0;
}

DrawThumbnailApi(DateTimeDrawThumbnail)
{
	CDateTime* pDateTime = (CDateTime*)((FlipperPlugin*)pPlugin)->pData;
	return pDateTime->DrawThumbnail(pNode, hdc, hdcThumb, pRect, bSelected);
}

NodeSelectedApi(DateTimeNodeSelected)
{
	CDateTime* pDateTime = (CDateTime*)((FlipperPlugin*)pPlugin)->pData;
	*pbRefresh = false;

	// TODO - show webpage?
	pDateTime->g_pFlipper->Hide(pPlugin);
}

CloseOnSelectionApi(DateTimeCloseOnSelection)
{
	CDateTime* pDateTime = (CDateTime*)((FlipperPlugin*)pPlugin)->pData;
	return true;
}

BuildNodeListApi(DateTimeBuildNodeList)
{
	CDateTime* pDateTime = (CDateTime*)((FlipperPlugin*)pPlugin)->pData;
	return pDateTime->BuildNodeList(piNodeLen, piThumbWidth, piThumbHeight, iMaxWidth, iMaxHeight);
}

DestroyNodeListApi(DateTimeDestroyNodeList)
{
	// we don't clean up here since we have a static # of items
	return;
}

BOOL CALLBACK CDateTime::DateTimeProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{
	static HWND hwndTimeZone;
	static HKEY g_hKey = NULL;
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
			g_dlgAnchor.Init(hwndDlg);
			g_dlgAnchor.Add(IDC_COMBO_TIMEZONE, ANCHOR_ALL &~ ANCHOR_BOTTOM);

			hwndTimeZone = GetDlgItem(hwndDlg, IDC_COMBO_TIMEZONE);

			TIME_ZONE_INFORMATION info = {0};
			GetTimeZoneInformation(&info);

			int index = ComboBox_AddString(hwndTimeZone, info.StandardName);
			index = ComboBox_SetItemData(hwndTimeZone, index, info.StandardBias);

			g_hKey = (HKEY)lParam;
			DWORD dwOffset = 0;
			RegistryGetDWORD(g_hKey, NULL, REG_TimeOffset, &dwOffset); 

			ComboBox_SetCurSel(hwndTimeZone, 0);

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
			mbi.hInstRes = CDateTime::g_hInstance;
            mbi.nBmpId = 0;
            mbi.cBmpImages = 0;    

            SHCreateMenuBar(&mbi);

			SetFocus(hwndTimeZone);

            return 0;
        }
        break;
        case WM_COMMAND: 
			{
				switch (LOWORD(wParam))
				{
					case IDM_Flipper_SET_ACCEPT :
					case IDOK:
						{
							WCHAR wzName[MAX_PATH];
							DWORD dwName = ARRAYSIZE(wzName);

							int index = ComboBox_GetCurSel(hwndTimeZone);
							DWORD dwTimeZone = ComboBox_GetItemData(hwndTimeZone, index);
							
							RegistrySetDWORD(g_hKey, NULL, REG_TimeOffset, dwTimeZone);

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
                Sleep(0);
            }
            break;
    } 
    return FALSE; 
}
