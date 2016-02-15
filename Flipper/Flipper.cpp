#include "stdafx.h"
#include <windows.h>
#include <soundfile.h>
#include <time.h>
#include <notify.h>
#include <atltime.h>
#include <regext.h>
#include <tlhelp32.h>
#include <cpl.h>   // includes the common control header
#include <Imm.h>
#include "utils.h"
#include "resource.h"
#include "Flipper.h"
#include "rpn.h"

// use GDI+ (Platform SDK needed)      //
/////////////////////////////////////////
#define ULONG_PTR ULONG                //
#include <gdiplus.h>
using namespace Gdiplus;
using namespace Gdiplus::DllExports;
#pragma comment(lib, "..\\LibGDIPlusCE\\build\\vc200x-ce\\Windows Mobile 6 Professional SDK (ARMV4I)\\Debug\\LibGDIPlus.lib")
/////////////////////////////////////////

CFlipper* CFlipper::g_pInstance = NULL;
ULONG_PTR _gdiplusToken;

CFlipper::CFlipper(void)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&_gdiplusToken, &gdiplusStartupInput, NULL);

	g_pInstance = this;
    g_hInstance = GetModuleHandle(NULL);
	g_iTabIndex = -1;

    // these are appswitch variables
    g_bFlipperAutoClose = 0;
    g_bFlipperEnableShrinkAnimation = true;
    g_bFlipperEnableScrollAnimation = true;
    g_bFlipperEnableThumbnails = true;

    LoadSettings();

    memset(&g_wndClassAltTab, 0, sizeof(WNDCLASS));
    g_wndClassAltTab.lpszClassName = L"flipperContent";
    g_wndClassAltTab.lpfnWndProc = &FlipperContentProc;
    g_wndClassAltTab.style = 0;
    g_wndClassAltTab.hInstance = g_hInstance;
    g_wndClassAltTab.hIcon = NULL;
    ATOM a = RegisterClassW(&g_wndClassAltTab);

    memset(&g_wndClassBorder, 0, sizeof(WNDCLASS));
    g_wndClassBorder.lpszClassName = L"flipperTabs";
    g_wndClassBorder.lpfnWndProc = &FlipperTabProc;
    g_wndClassBorder.style = 0;
    g_wndClassBorder.hInstance = g_hInstance;
    g_wndClassBorder.hIcon = NULL;
    a = RegisterClassW(&g_wndClassBorder);

    g_hwndAltTab = CreateWindowExW
    (
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
        g_wndClassAltTab.lpszClassName,
        L"",
        WS_POPUP | WS_CLIPSIBLINGS,
        0,g_sipInfo.rcVisibleDesktop.top,g_width,g_height, NULL, NULL, g_hInstance, this
    );

    g_hwndAltTabBorder = CreateWindowExW
    (
        WS_EX_NOACTIVATE | WS_EX_TOPMOST,
        g_wndClassBorder.lpszClassName,
        L"",
        WS_POPUP | WS_CLIPSIBLINGS,
        0,g_sipInfo.rcVisibleDesktop.top/2-30,20,60, NULL, NULL, g_hInstance, NULL
    );

    RECT rect;
    GetWindowRect(g_hwndAltTab, &rect);
    SetWindowLong(g_hwndAltTab, GWL_USERDATA, (LONG)this);
    SetWindowLong(g_hwndAltTabBorder, GWL_USERDATA, (LONG)this);

    DDCAPS ddCaps;
    DDCAPS ddHelCaps;
    DDSURFACEDESC ddsd;
    HRESULT hRet;

    hRet = DirectDrawCreate(NULL, &g_pDD, NULL);
    if (hRet != DD_OK)
    {
        // we failed
    }

    // Get exclusive mode
    hRet = g_pDD->SetCooperativeLevel(g_hwndAltTab, DDSCL_NORMAL );
    if (hRet != DD_OK)
    {
        // we failed
    }

    g_pDD->GetCaps(&ddCaps, &ddHelCaps);

    if (!(ddCaps.ddsCaps.dwCaps & DDSCAPS_BACKBUFFER)) 
    {
        // we failed
    }

    if (!(ddCaps.ddsCaps.dwCaps & DDSCAPS_FLIP)) 
    {
        // we failed
    }

    int maxDimension = max(g_width, g_height);

    // Create the primary surface with 1 back buffer
    memset(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    hRet = g_pDD->CreateSurface(&ddsd, &g_pDDSPrimary, NULL);
    if (hRet != DD_OK)
    {
        // we failed
    }

    // Create a offscreen bitmap.
    ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.dwHeight = maxDimension;
    ddsd.dwWidth = maxDimension;
    hRet = g_pDD->CreateSurface(&ddsd, &g_pDDSBackground, NULL);
    if (hRet != DD_OK)
    {
        // we failed
    }

    hRet = g_pDD->CreateSurface(&ddsd, &g_pDDSBuffer, NULL);
    if (hRet != DD_OK)
    {
        // we failed
    }
    
    hRet = g_pDD->CreateSurface(&ddsd, &g_pDDSBackClear, NULL);
    if (hRet != DD_OK)
    {
        // we failed
    }

    int maxLabelDimension = max(max(g_buttonWidth, g_labelWidth), max(g_buttonHeight, g_labelHeight));

    HDC hdc = GetDC(0);
    g_hdcLabel = CreateCompatibleDC(hdc);
    g_hbmpLabel = CreateCompatibleBitmap(hdc, maxLabelDimension, maxLabelDimension);
    g_hbmpLabelOld = (HBITMAP)SelectObject(g_hdcLabel, g_hbmpLabel);

    hRet = g_pDD->CreateClipper(0, &g_pDDClipper, NULL);
    if (hRet != DD_OK)
    {
        // we failed
    }

    hRet=g_pDDClipper->SetHWnd(0, g_hwndAltTab);
    hRet=g_pDDSPrimary->SetClipper(g_pDDClipper);

    RGNDATA rgnData = {0};

    DWORD dwSize = 0;
    hRet=g_pDDClipper->GetClipList(NULL, NULL, &dwSize);

    hFinishedEvent = NULL;
}

void CFlipper::Initialize()
{
    hFinishedEvent = CreateEvent(NULL, FALSE, TRUE, L"CFlipper");
	g_hEventCanBuildNode = CreateEvent(NULL, FALSE, TRUE, L"CFlipperBuildNodeCan");
	g_hEventBuildNodeReady = CreateEvent(NULL, FALSE, FALSE, L"CFlipperBuildNodeReady");
	
	SendMessage(g_hwndAltTabBorder, WM_SHOWFLIPPER, 0, 0);

	if (!g_bFlipperAutoClose)
	{
		ShowWindow(g_hwndAltTabBorder, SW_SHOW);
	}

	g_hBuildNodeThread = CreateThread(NULL, 0, &CFlipper::LoadNodes, this, 0, 0);
	SetThreadPriority(g_hBuildNodeThread, THREAD_PRIORITY_BELOW_NORMAL);
}

CFlipper::~CFlipper(void)
{
    ReleaseAllObjects();
    CleanupFlipperSettings();

    CloseHandle(hFinishedEvent);
	DestroyWindow(g_hwndAltTab);
	DestroyWindow(g_hwndAltTabBorder);

	if (g_hBuildNodeThread)
	{
		TerminateThread(g_hBuildNodeThread, 0);
		CloseHandle(g_hBuildNodeThread);
		g_hBuildNodeThread = NULL;
	}
	Gdiplus::GdiplusShutdown(_gdiplusToken);
}

void CFlipper::LoadSettings()
{
    LoadFlipperSettings();
    LoadFlipperDisplaySettings();
	
	bool demoMode = false;
	if (!CheckIfRegistered(REG_RegstrationCode, RPN_CVALUE, RPN_VARIANT))
	{
		demoMode = true;
		if (IDYES == MessageBox(NULL, L"Only 2 plugins may be added in demo mode.\n\nWould you like information on how to register?", L"Demo Mode", MB_YESNO | MB_ICONQUESTION))
		{
			LaunchProgram(L"http://www.mobilesrc.com/Flipper.aspx", L"");
		}
	}

    HKEY hKey;
    DWORD dwSize = sizeof(DWORD);
    DWORD dwType = 0, dwValue = 0;
    HRESULT hRes = RegOpenKeyEx(HKEY_CURRENT_USER, REG_FlipperKey, 0, KEY_READ, &hKey);
    if (hRes == ERROR_SUCCESS)
    {
        HKEY hKeyItems;
        hRes = RegOpenKeyEx(hKey, REG_Items_Key, 0, KEY_READ | KEY_ENUMERATE_SUB_KEYS, &hKeyItems);
        if (hRes == ERROR_SUCCESS)
        {
            int i = 0;

            HKEY hKeyItem;

			list<int> idList;
            while (true)
            {
				WCHAR wzName[3];
                DWORD dwItemName = 3;
                hRes = RegEnumKeyEx(hKeyItems, i++, wzName, &dwItemName, 0, 0, 0, 0);

				if (demoMode && idList.size() >= 2)
				{
					break;
				}
                if (hRes == ERROR_SUCCESS)
                {
					idList.push_back(_wtoi(wzName));
                }
				else
				{
					break;
				}
            }
			idList.sort();

			list<int>::iterator iter = idList.begin();

			while (iter != idList.end())
			{
                bool bSucceeded = false;
				WCHAR* wzItemName = new WCHAR[3];
				_itow(*iter, wzItemName, 10);

                hRes = RegOpenKeyEx(hKeyItems, wzItemName, 0, KEY_READ | KEY_ENUMERATE_SUB_KEYS, &hKeyItem);

                if (hRes == ERROR_SUCCESS)
                {
					WCHAR* wzName = new WCHAR[100];
					dwSize = sizeof(WCHAR) * 100;
					dwType = REG_SZ;

                    RegQueryValueExW(hKeyItem, REG_Items_Name, 0, &dwType, (LPBYTE)wzName, &dwSize);

					WCHAR wzAssembly[100];
					dwSize = sizeof(WCHAR) * 100;
					dwType = REG_SZ;
                    RegQueryValueExW(hKeyItem, REG_Items_Assembly, 0, &dwType, (LPBYTE)wzAssembly, &dwSize);

					DWORD dwPluginId = 0;
					dwType = REG_DWORD;
					dwSize = sizeof(DWORD);
                    RegQueryValueExW(hKeyItem, REG_Items_PluginId, 0, &dwType, (LPBYTE)&dwPluginId, &dwSize);

					DWORD dwColor = 0;
					dwType = REG_DWORD;
					dwSize = sizeof(DWORD);
                    RegQueryValueExW(hKeyItem, REG_Items_Color, 0, &dwType, (LPBYTE)&dwColor, &dwSize);


					HKEY hKeyData = NULL;
					hRes = RegOpenKeyEx(hKeyItem, REG_Items_Data_Key, 0, KEY_READ | KEY_ENUMERATE_SUB_KEYS, &hKeyData);

					if (hRes == ERROR_SUCCESS)
					{
						WCHAR wzFullAssembly[MAX_PATH];
						StringCchCopy(wzFullAssembly, ARRAYSIZE(wzFullAssembly), L"\\program files\\mobilesrc\\flipper\\plugins\\");
						StringCchCat(wzFullAssembly, ARRAYSIZE(wzFullAssembly), wzAssembly);

						HMODULE hLib = LoadLibrary(wzFullAssembly);

						if (hLib)
						{
							PFN_CreatePluginInstance pfnCreatePluginInstance = (PFN_CreatePluginInstance)GetProcAddress(hLib, L"CreatePluginInstance");

							FlipperPlugin* pPlugin = NULL;
							pfnCreatePluginInstance(this, dwPluginId, &pPlugin, hKeyData, g_bigThumbWidth, g_bigThumbHeight);
							pPlugin->dwColor = dwColor;
							g_tabList.push_back(pPlugin);

							pPlugin->hLib = hLib;
						}
						else
						{
							bSucceeded = false;
							MessageBox(NULL, L"Error Loading Plugin", L"Flipper", MB_OK);
						}

						RegCloseKey(hKeyData);
					}
                    RegCloseKey(hKeyItem);

                    bSucceeded = true;
                }
                if (!bSucceeded)
                {
                    delete[] wzItemName;
                }
				iter++;
			}

            RegCloseKey(hKeyItems);
        }

        RegCloseKey(hKey);
    }
}

void CFlipper::LoadFlipperDisplaySettings()
{
    ZeroMemory(&g_sipInfo, sizeof(g_sipInfo));
    g_sipInfo.cbSize = sizeof(g_sipInfo);
    SHSipInfo(SPI_GETSIPINFO, 0, &g_sipInfo, 0);

    g_width = GetSystemMetrics(SM_CXSCREEN);
    g_height = GetSystemMetrics(SM_CYSCREEN);

	int divisor = 5;

	switch (g_dwTabSize)
	{
		case 0 :
		{
			divisor = 6;
		}
		break;
		case 1 :
		{
			divisor = 5;
		}
		break;
		default :
		{
			divisor = 4;
		}
		break;
	}

	g_dwTabWidth = g_width / 15;

	g_bigThumbHeight = g_height / divisor;
	g_bigThumbWidth = g_width / 4;

    int labelTop = g_height / 5;

	int maxDim = max(g_height, g_width);
	
	g_iPadding = (int)(((double)maxDim / 640.0) * 4);

    g_labelWidth = g_width - (g_width / 6);
    g_labelHeight = maxDim / 14;
    g_labelSpacing = 5;
    g_labelLeft = (g_width - g_labelWidth) >> 1;
    g_labelFont = (int)(g_labelHeight * 2.0 / 3);
    g_buttonWidth = g_width - (g_width / 6);
    g_buttonHeight = maxDim / 2;
    g_buttonSpacing = g_height / 5;
    g_buttonLeft = (g_width - g_buttonWidth) >> 1;
    g_buttonFont = g_buttonHeight / 2;
    g_dwAnimateInterval = 50;
    g_dwDeceleration = 75;

    g_bigThumbTop = g_labelSpacing;
    g_bigThumbLeft = (g_width - g_bigThumbWidth) / 2;

    g_bigThumbMid = g_bigThumbTop + (g_bigThumbHeight / 2);
    g_labelTop = g_labelSpacing;
    g_buttonTop = g_bigThumbTop + g_bigThumbHeight + g_buttonSpacing;
    g_height = (g_height - g_sipInfo.rcVisibleDesktop.top);

	g_windowHeight = g_bigThumbHeight + (g_labelSpacing * 4) + g_labelHeight * 2;
}

void CFlipper::LoadFlipperSettings()
{
    CleanupFlipperSettings();

    HKEY hKey;
    DWORD dwSize = sizeof(DWORD);
    DWORD dwType = 0, dwValue = 0;
    HRESULT hRes = RegOpenKeyEx(HKEY_CURRENT_USER, REG_FlipperKey, 0, KEY_READ, &hKey);
    if (hRes == ERROR_SUCCESS)
    {
        // Load stuff here
        RegQueryValueExW(hKey, REG_AutoClose, 0, &dwType, (LPBYTE)&dwValue, &dwSize);
        this->g_bFlipperAutoClose = (dwValue != 0);
        RegQueryValueExW(hKey, REG_EnableShrinkAnimation, 0, &dwType, (LPBYTE)&dwValue, &dwSize);
        this->g_bFlipperEnableShrinkAnimation = (dwValue != 0);
        RegQueryValueExW(hKey, REG_EnableScrollAnimation, 0, &dwType, (LPBYTE)&dwValue, &dwSize);
        this->g_bFlipperEnableScrollAnimation = (dwValue != 0);
        RegQueryValueExW(hKey, REG_EnableScrollAnimation, 0, &dwType, (LPBYTE)&dwValue, &dwSize);
        this->g_bFlipperEnableShrinkAnimation = (dwValue != 0);
        RegQueryValueExW(hKey, REG_TabSize, 0, &dwType, (LPBYTE)&(g_dwTabSize), &dwSize);
        RegQueryValueExW(hKey, REG_TabAlignment, 0, &dwType, (LPBYTE)&(g_dwTabAlign), &dwSize);

        RegCloseKey(hKey);
    }
}

void CFlipper::CleanupFlipperSettings()
{
	list<FlipperPlugin*>::iterator iter = g_tabList.begin();

	while (iter != g_tabList.end())
	{
		PFN_DestroyPluginInstance pfnDestroyPluginInstance = (PFN_DestroyPluginInstance)GetProcAddress((*iter)->hLib, L"DestroyPluginInstance");

		if (pfnDestroyPluginInstance)
		{
			pfnDestroyPluginInstance(*iter);
		}

		delete *iter;
		iter++;
	}
	g_tabList.clear();
}

WCHAR* CFlipper::GetCurrentTabName()
{
	list<FlipperPlugin*>::iterator iter = g_tabList.begin();
	int index = 0;
	while (iter != g_tabList.end())
	{
		if (index == g_iTabIndex)
		{
			return (*iter)->wzName;
		}
		index++;
		iter++;
	}
	return L"";
}

void CFlipper::Hide()
{
	if (IsWindowVisible(g_hwndAltTab))
	{
		SendMessage(g_hwndAltTab, WM_HIDEFLIPPER, 0, 0);
		ShowWindow(g_hwndAltTab, SW_HIDE);
		SendMessage(g_hwndAltTabBorder, WM_PANELHIDING, 0, 0);
	}

	if (g_bFlipperAutoClose)
	{
		ShowWindow(g_hwndAltTabBorder, SW_HIDE);
	}


	g_iTabIndex = -1;
}

void CFlipper::Minimize()
{
	if (IsWindowVisible(g_hwndAltTab))
	{
		ResetEvent(hFinishedEvent);
		SendMessage(g_hwndAltTab, WM_SELECTFLIPPER, 1, 1);
	}
	else
	{
		SetEvent(hFinishedEvent);
	}
}

void CFlipper::Refresh()
{
	// TODO
	// We want to trigger the event here
	CancelBuild();
	g_pCancelEvent = new CancelEvent(this);
	//g_pPlugin = pPlugin;

	LoadFlipperDisplaySettings();
	SetEvent(g_hEventBuildNodeReady);
	//SendMessage(g_hwndAltTab, WM_SHOWFLIPPER, (WPARAM)pPlugin, MAKELONG(0, !isVisible));
	BOOL isVisible = IsWindowVisible(g_hwndAltTab);
	//SendMessage(g_hwndAltTab, WM_SHOWFLIPPER, NULL, MAKELONG(0, !isVisible));

	//BOOL isVisible = IsWindowVisible(g_hwndAltTab);
	//SendMessage(g_hwndAltTab, WM_SHOWFLIPPER, NULL, MAKELONG(0, !isVisible));
}

void CFlipper::OpenRegistry(void* pPlugin, HKEY* phKey)
{
	*phKey = NULL;
	WCHAR wzKey[MAX_PATH];

	list<FlipperPlugin*>::iterator iter = g_tabList.begin();

	int index = -1;
	int i = 0;
	while (iter != g_tabList.end())
	{
		if (*iter == pPlugin)
		{
			index = i;
			break;
		}

		i++;
		iter++;
	}

	if (index >= 0)
	{
		StringCchPrintfW(wzKey, ARRAYSIZE(wzKey), L"%s\\%s\\%d\\%s", REG_FlipperKey, REG_Items_Key, index, REG_Items_Data_Key);
	    HRESULT hRes = RegOpenKeyEx(HKEY_CURRENT_USER, wzKey, 0, KEY_ALL_ACCESS, phKey);
		Sleep(0);
	}
}


void CFlipper::Redraw(void* pPlugin, node* pHeadNode, int iNodeLen)
{
	if (pPlugin == g_pPlugin)
	{
		SendMessage(g_hwndAltTab, WM_DATAREADY, (WPARAM)pHeadNode, iNodeLen);
	}
}

void CFlipper::Hide(void* pPlugin)
{
	if (pPlugin == g_pPlugin)
	{
		Hide();
	}
}

void CFlipper::Refresh(void* pPlugin)
{
	if (pPlugin == g_pPlugin)
	{
		Refresh();
	}
}

void CFlipper::DrawBackground(void* pPlugin, HDC hdcDest, RECT* pRect)
{
	if (pPlugin == g_pPlugin)
	{
		DrawBackground(hdcDest, pRect);
	}
}

void CFlipper::ShowIndex(int i)
{
	BOOL isVisible = IsWindowVisible(g_hwndAltTab);

    // Show the window here
    LoadFlipperDisplaySettings();
    ResetEvent(hFinishedEvent);
    //SHSipPreference(GetForegroundWindow(), SIP_FORCEDOWN);
    //SHSipPreference(g_hwndAltTab, SIP_FORCEDOWN);

	FlipperPlugin* pPlugin = NULL;
	list<FlipperPlugin*>::iterator iter = g_tabList.begin();
	int index = 0;
	while (iter != g_tabList.end())
	{
		if (index == i)
		{
			pPlugin = *iter;
			break;
		}
		index++;
		iter++;
	}
	if (!pPlugin)
	{
		return;
	}
	g_iTabIndex = i;
	
	// We want to trigger the event here
	CancelBuild();
	g_pCancelEvent = new CancelEvent(this);
	g_pPlugin = pPlugin;

	SendMessage(g_hwndAltTab, WM_SHOWFLIPPER, (WPARAM)pPlugin, MAKELONG(0, !isVisible));
	SetEvent(g_hEventBuildNodeReady);
}

void CFlipper::CancelBuild()
{
	if (g_pCancelEvent)
	{
		g_pCancelEvent->SetCancelled(true);
		WaitForSingleObject(g_hEventCanBuildNode, INFINITE);

		delete g_pCancelEvent;
		g_pCancelEvent = NULL;
	}
}

DWORD WINAPI CFlipper::LoadNodes(LPVOID lpParameter)
{
    CFlipper* p_this = (CFlipper*)lpParameter;

	while (WAIT_OBJECT_0 == ::WaitForSingleObject(p_this->g_hEventBuildNodeReady, INFINITE))
	{
		node* pHeadNode = NULL;
		int iNodeLen = 0;
		ResetEvent(p_this->g_hEventBuildNodeReady);
		FlipperPlugin* pPlugin = p_this->g_pPlugin;
		CancelEvent* pCancel = p_this->g_pCancelEvent;

		if (pPlugin->pfnBuildNodeList)
		{
			if (!pCancel->IsCancelled())
			{
				int width, height;
				height = p_this->g_windowHeight;
				width = p_this->g_width-p_this->g_dwTabWidth;

				pHeadNode = pPlugin->pfnBuildNodeList(pPlugin, pCancel, &iNodeLen, &(p_this->g_bigThumbWidth), &(p_this->g_bigThumbHeight), width, height);
			}
			else
			{
				Sleep(0);
			}
		}
		if (!pCancel->IsCancelled())
		{
			// Do something here...
			if (!pHeadNode)
			{
				iNodeLen = 0;
			}
			p_this->Redraw(pPlugin, pHeadNode, iNodeLen);
		}

		SetEvent(p_this->g_hEventCanBuildNode);
	}
	return 0;
}

void CFlipper::ShowTab(int index)
{
	if (IsWindowVisible(g_hwndAltTab))
	{
		this->Hide();
	}
	else
	{
		SendMessage(g_hwndAltTabBorder, WM_SELECTTAB, 0, 0);
	}
}

void CFlipper::ReleaseAllObjects(void)
{
    if (g_pDDSBuffer != NULL)
    {
        g_pDDSBuffer->Release();
        g_pDDSBuffer = NULL;
    }
    if (g_pDDSPrimary != NULL)
    {
        g_pDDSPrimary->Release();
        g_pDDSPrimary = NULL;
    }
    if (g_pDDSBackground != NULL)
    {
        g_pDDSBackground->Release();
        g_pDDSBackground = NULL;
    }
    if (g_pDDSBackClear != NULL)
    {
        g_pDDSBackClear->Release();
        g_pDDSBackClear = NULL;
    }
    if (g_hdcLabel != NULL)
    {
        SelectObject(g_hdcLabel, g_hbmpLabelOld);
        DeleteObject(g_hbmpLabel);
        DeleteDC(g_hdcLabel);
        g_hdcLabel = NULL;
        g_hbmpLabel = NULL;
    }
    if (g_pDD != NULL)
    {
        g_pDD->Release();
        g_pDD = NULL;
    }
}

HRESULT CFlipper::Flip(HWND hwnd)
{
    HRESULT ddrval;
    RECT rcRectSrc;
    RECT rcRectDest;
    POINT p;

    // if we're windowed do the blit, else just Flip
    if (true)
    {
        // first we need to figure out where on the primary surface our window lives
        p.x = 0; p.y = 0;
        ClientToScreen(hwnd, &p);
        GetClientRect(hwnd, &rcRectDest);
        OffsetRect(&rcRectDest, p.x, p.y);
        GetClientRect(hwnd, &rcRectSrc);

        ddrval = g_pDDSPrimary->Blt( &rcRectDest, g_pDDSBuffer, &rcRectSrc, 0, NULL);

    }
    return ddrval;
}

HRESULT CFlipper::RestoreAll()
{
    HRESULT hRet;

    hRet = g_pDDSPrimary->Restore();
    if (hRet == DD_OK)
    {
        hRet = g_pDDSBackground->Restore();
        if (hRet == DD_OK)
        {
            hRet = g_pDDSBackClear->Restore();
            if (hRet == DD_OK)
            {
                InitSurfaces(0, 0, 0, g_width, g_height, false);
            }
        }
    }
    return hRet;
}



        static void CreateRoundRectangle(RECT* rectangle, GpPath* gPath, int radius)
        {
            float l = rectangle->left;
            float t = rectangle->top;
            float w = rectangle->right - rectangle->left;
            float h = rectangle->bottom - rectangle->top;
            float d = radius << 1;

			GdipAddPathArc(gPath, l, t, d, d, 180, 90); // topleft
            GdipAddPathLine(gPath, l + radius, t, l + w - radius, t); // top
            GdipAddPathArc(gPath, l + w - d, t, d, d, 270, 90); // topright
            GdipAddPathLine(gPath, l + w, t + radius, l + w, t + h - radius); // right
            GdipAddPathArc(gPath, l + w - d, t + h - d, d, d, 0, 90); // bottomright
            GdipAddPathLine(gPath, l + w - radius, t + h, l + radius, t + h); // bottom
            GdipAddPathArc(gPath, l, t + h - d, d, d, 90, 90); // bottomleft
            GdipAddPathLine(gPath, l, t + h - radius, l, t + radius); // left

			GdipClosePathFigure(gPath);
        }

        static void  CreateTopRoundRectangle(RECT* rectangle, GpPath* gPath, int radius)
        {
            float l = rectangle->left;
            float t = rectangle->top;
            float w = rectangle->right - rectangle->left;
            float h = rectangle->bottom - rectangle->top;
            float d = radius << 1;

            GdipAddPathArc(gPath, l, t, d, d, 180, 90); // topleft
            GdipAddPathLine(gPath, l + radius, t, l + w - radius, t); // top
            GdipAddPathArc(gPath, l + w - d, t, d, d, 270, 90); // topright
            GdipAddPathLine(gPath, l + w, t + radius, l + w, t + h); // right
            GdipAddPathLine(gPath, l + w, t + h, l, t + h); // bottom
            GdipAddPathLine(gPath, l, t + h, l, t + radius); // left

			GdipClosePathFigure(gPath);
        }

        static void CreateBottomRadialPath(RECT* rectangle, GpPath* gPath)
        {
            RectF rect;

			rect.X = rectangle->left;
			rect.Y = rectangle->top;
			rect.Width = rectangle->right - rectangle->left;
			rect.Height = rectangle->bottom - rectangle->top;

            rect.X -= rect.Width * .35f;
            rect.Y -= rect.Height * .15f;
            rect.Width *= 1.7f;
            rect.Height *= 2.3f;
            GdipAddPathEllipse(gPath, rect.X,rect.Y,rect.Width,rect.Height);

			GdipClosePathFigure(gPath);
        }

#define RGB_TO_565(r,g,b) (WORD) ((r & 0xf8 )<<8) | ((g&0xfc)<<3) | ((b&0xf8)>>3)
#define COLOR32_TO_565(c) (WORD) (((c >> 0) & 0xf8 )<<8) | (((c >> 8) &0xfc)<<3) | (((c >> 16) &0xf8)>>3)
#define COLOR_DARKEN(color, amount) (DWORD) RGB(((color >> 0) & 0xFF) - amount, ((color >> 8) & 0xFF) - amount, ((color >> 16) & 0xFF) - amount)

BOOL CFlipper::InitSurfaces(DWORD dwColor, int x, int y, int width, int height, bool blendOnly)
{
    // TODO: Get the back handle here
    HRESULT hr;
    HDC hdcClear;
    RECT windowRect;
    windowRect.left = windowRect.top = 0;
    windowRect.right = width;
    windowRect.bottom = height;

    if (!blendOnly && (hr = g_pDDSBackClear->GetDC(&hdcClear)) == DD_OK)
    {
		// Let's take a screenshot here
		HDC hdcDesk = GetDC(HWND_DESKTOP);

		StretchBlt(hdcClear, 0, 0, width, height, hdcDesk, x, y, width, height, SRCCOPY);
		g_pDDSBackClear->ReleaseDC(hdcClear);
		DeleteDC(hdcDesk);
	}

	DDBLTFX ddbltfx;
	memset(&ddbltfx, 0, sizeof(ddbltfx));
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.dwROP = SRCCOPY;
	DWORD dwX = RGB(0xff, 0xff, 0);
	//ddbltfx.dwFillColor = RGB_TO_565(0xFF, 0xFF, 0);
	int r = (dwColor >> 0) & 0xFF;
	int g = (dwColor >> 8) & 0xFF;
	int b = (dwColor >> 16) & 0xFF;

	DWORD dwColorLight = DarkenColor(dwColor, FLIPTAB_DARKEN_AMOUNT);
	
	int rLight = (dwColorLight >> 0) & 0xFF;
	int gLight = (dwColorLight >> 8) & 0xFF;
	int bLight = (dwColorLight >> 16) & 0xFF;
	
	DWORD dwColorVeryLight = DarkenColor(dwColor, FLIPTAB_DARKEN_AMOUNT - 30);
	
	int rVeryLight = (dwColorVeryLight >> 0) & 0xFF;
	int gVeryLight = (dwColorVeryLight >> 8) & 0xFF;
	int bVeryLight = (dwColorVeryLight >> 16) & 0xFF;
	
	DWORD dwColorVeryDark = DarkenColor(dwColor, 30);
	
	int rVeryDark = (dwColorVeryDark >> 0) & 0xFF;
	int gVeryDark = (dwColorVeryDark >> 8) & 0xFF;
	int bVeryDark = (dwColorVeryDark >> 16) & 0xFF;

	ddbltfx.dwFillColor = COLOR32_TO_565(dwColorLight);

	DDALPHABLTFX alphaFx;
	alphaFx.dwSize = sizeof(DDALPHABLTFX);
	alphaFx.ddargbScaleFactors.alpha = 0x70;
	alphaFx.ddargbScaleFactors.red = 0x00;
	alphaFx.ddargbScaleFactors.green = 0x00;
	alphaFx.ddargbScaleFactors.blue = 0x00;

	RECT destRect;
	memcpy(&destRect, &windowRect, sizeof(RECT));
	//destRect.left += 4;
	//destRect.top += 4;
	//destRect.right -= 4;
	//destRect.bottom -= 4;

	g_pDDSBackground->Blt(&destRect, NULL, &windowRect, DDBLT_COLORFILL, &ddbltfx);
	hr = g_pDDSBackground->AlphaBlt(&windowRect, g_pDDSBackClear, &windowRect, 0, &alphaFx);

	RECT rect = {0};
	memcpy(&rect, &windowRect, sizeof(RECT));

	HDC hdc;
	g_pDDSBackground->GetDC(&hdc);
	GpGraphics* gx;

	SmoothingMode smooth;
	GdipCreateFromHDC(hdc, &gx);

	GdipGetSmoothingMode(gx, &smooth);
	GdipSetSmoothingMode(gx, SmoothingMode::SmoothingModeAntiAlias);

	rect.left += 2;
	rect.top += 2;
    rect.right-=g_dwTabWidth+2;
    rect.bottom-=2;
	GpPath* gPath;
	GpPen* gPen;
	ARGB aRgb = RGB(0xff,0xff,0xff) | (0xff << 24);
	GdipCreatePath(FillMode::FillModeAlternate, &gPath);
	GdipCreatePen1(aRgb, 4, GpUnit::UnitPixel, &gPen);
	CreateRoundRectangle(&rect, gPath, 4);
	//GdipDrawPath(gx, gPen, gPath);
	GdipDeletePen(gPen);
	GdipDeletePath(gPath);

    rect.left++;
    rect.top++;
    rect.right -= 1;
    rect.bottom -= 1;

	RECT rect2 = {0};
	memcpy(&rect2, &rect, sizeof(RECT));
	rect2.bottom -= ((rect2.bottom - rect2.top) >> 1);

	GpSolidFill* gBrush;
	aRgb = RGB(0,0,0) | (0x7f << 24);

	GdipCreatePath(FillMode::FillModeAlternate, &gPath);
	GdipCreateSolidFill(aRgb, &gBrush);
	CreateRoundRectangle(&rect, gPath, 2);
	GdipFillPath(gx, gBrush, gPath);
	GdipDeleteBrush(gBrush);
	GdipDeletePath(gPath);

    rect2.bottom++;
	GdipCreatePath(FillMode::FillModeAlternate, &gPath);
	CreateTopRoundRectangle(&rect2, gPath, 2);
    int opacity = 0x99;

	GpRect gpRect;
	gpRect.X = rect2.left - 20;
	gpRect.Y = rect2.top;
	gpRect.Width = (rect2.right - rect2.left) + 50;
	gpRect.Height = (rect2.bottom - rect2.top);

	GpPointF point1, point2;
	point1.X = gpRect.X;
	point1.Y = gpRect.Y;
	point2.X = gpRect.Width;
	point2.Y = gpRect.Height;

	ARGB aRgb1 = RGB(0xFF,0xFF,0xFF) | ((opacity&0xff) << 24);
	ARGB aRgb2 = RGB(0xFF,0xFF,0xFF) | (((opacity/3)&0xfF) << 24);

	GpLineGradient* gGradient;
	GdipCreateLineBrush(&point1, &point2, aRgb1, aRgb2, GpWrapMode::WrapModeTile, &gGradient);
	GdipFillPath(gx, gGradient, gPath);

	rect2.bottom-=2;
	GdipDeleteBrush((GpBrush*)&gGradient);
	GdipDeletePath(gPath);

	/*
	GpPath* gPath2;
	GpPathGradient* gPathGrad;
	GdipCreatePath(FillMode::FillModeAlternate, &gPath);
	GdipCreatePath(FillMode::FillModeAlternate, &gPath2);
	CreateRoundRectangle(&rect, gPath, 2);
	CreateBottomRadialPath(&rect, gPath2);

	GdipCreatePathGradientFromPath(gPath2, &gPathGrad);

	float glowOpacity = 254;
    opacity = (int)(0xb2 * glowOpacity + 0.5f);
	
	point1.X = (rect.left + rect.right) / 2.0f;
	point1.Y = (rect.top + rect.bottom) / 2.0f;

	aRgb = dwColorLight | ((opacity & 0xFF) << 24);

	GdipSetPathGradientCenterPoint(gPathGrad, &point1);
	GdipSetPathGradientCenterColor(gPathGrad, aRgb);
	int iCount = 1;
	aRgb = dwColorLight | ((0 & 0xFF) << 24);
	GdipSetPathGradientSurroundColorsWithCount(gPathGrad, &aRgb, &iCount);

	//GdipFillPath(gx, gPathGrad, gPath2);

	GdipDeleteBrush((GpBrush*)gPathGrad);
	GdipDeletePath(gPath);
	GdipDeletePath(gPath2);
	*/
/*
	
                using (GraphicsPath clip = CreateRoundRectangle(rect, 2))
                {
                   
                    //g.SetClip(clip, CombineMode.CombineModeIntersect);
                    using (GraphicsPath brad = CreateBottomRadialPath(rect))
                    {
                        using (PathGradientBrush pgr = new PathGradientBrush(brad))
                        {
                            unchecked
                            {
                                int opacity = (int)(0xb2 * glowOpacity + .5f);
                                RectangleF bounds = new RectangleF(rect.X, rect.Y, rect.Width, rect.Height);// brad.getbounds();
                                pgr.SetCenterPoint(new GpPointF((bounds.Left + bounds.Right) / 2f, (bounds.Top + bounds.Bottom) / 2f));
                                pgr.SetCenterColor(Color.Empty.FromArgb(opacity, glowColor));
                                pgr.SetSurroundColors(new Color[] { Color.Empty.FromArgb(0, glowColor) });
                            }
                            g.FillPath(pgr, brad);
                        }
                    }
                    //g.ResetClip();
                }
*/
	/*
	GdipCreatePath(FillMode::FillModeAlternate, &gPath);
	CreateRoundRectangle(&rect, gPath, 3);
	aRgb = RGB(0x00, 0x00, 0x00) | (0xFF << 24);
	GdipCreatePen1(aRgb, 1, GpUnit::UnitPixel, &gPen);
	GdipDrawPath(gx, gPen, gPath);
	GdipDeletePen(gPen);
	GdipDeletePath(gPath);

	*/
	GdipSetSmoothingMode(gx, smooth);

	GdipReleaseDC(gx, hdc);
	GdipDeleteGraphics(gx);
	g_pDDSBackground->ReleaseDC(hdc);
	
	//hr = g_pDDSBackground->AlphaBlt(&windowRect, g_pDDSBackClear, &windowRect, 0, &alphaFx);

    return TRUE;
}

bool DrawThumbnail(void* pPlugin, node* pNode, HDC hdc, HDC hdcThumb, RECT* pRect, bool bSelected, bool* pbDrawLabels)
{
	int padding = CFlipper::g_pInstance->g_iPadding;
	DWORD dwColor = (bSelected) ? BLACK_BRUSH : WHITE_BRUSH;
	DWORD dwPen = (bSelected) ? BLACK_PEN : WHITE_PEN;
	byte dwOpaque = (bSelected) ? 255 : 100;
	HBITMAP hbmpWindow = pNode->hBmp;
	HICON hIconWindow = pNode->hIcon;
	bool bCenter = pNode->bCenter;

	int thumbLeft = pRect->left;
	int thumbTop = pRect->top;
	int thumbWidth = (pRect->right - pRect->left);
	int thumbHeight = (pRect->bottom - pRect->top);

    BLENDFUNCTION blendFunc;
    blendFunc.AlphaFormat = 0;
    blendFunc.BlendFlags = 0;
    blendFunc.BlendOp = AC_SRC_OVER;
    blendFunc.SourceConstantAlpha = dwOpaque;

    RECT rect;
    BITMAP bitmap;
    int iconWidth = GetSystemMetrics(SM_CXICON);
    int iconHeight = GetSystemMetrics(SM_CYICON);

    thumbLeft += padding*2;
    thumbTop += padding*2;
    thumbWidth -= padding*4;
    thumbHeight -= padding*4;

    rect.left = thumbLeft - padding;
    rect.right = thumbLeft + thumbWidth + padding;
    rect.top = thumbTop - padding;
    rect.bottom = thumbTop + thumbHeight + padding;
    
    HGDIOBJ hBrushOld = SelectObject(hdc, (HBRUSH)GetStockObject(dwColor));
	HGDIOBJ hPenOld = SelectObject(hdc, (HPEN)GetStockObject(dwPen));
    RoundRect(hdc, rect.left, rect.top, rect.right, rect.bottom, padding << 1, padding << 1);
    SelectObject(hdc, hBrushOld);

    int borderWidth = max(1, padding / 2);
    rect.left += borderWidth;
    rect.top += borderWidth;
    rect.right -= borderWidth;
    rect.bottom -= borderWidth;
    
    hBrushOld = SelectObject(hdc, (HBRUSH)GetStockObject(dwColor ^ 4));
    RoundRect(hdc, rect.left, rect.top, rect.right, rect.bottom, padding << 1, padding << 1);
    SelectObject(hdc, hBrushOld);
	SelectObject(hdc, hPenOld);

    if (hbmpWindow)
    {
        ::GetObject(hbmpWindow, sizeof(BITMAP), &bitmap);
        HGDIOBJ hbmpOld = SelectObject(hdcThumb, hbmpWindow);

		int left = thumbLeft;
		int top = thumbTop;
		int width = thumbWidth;
		int height = thumbHeight;

		if (bCenter)
		{
			int newWidth = bitmap.bmWidth;
			int newHeight = bitmap.bmHeight;
			::ScaleProportional(width, height, (UINT*)&newWidth, (UINT*)&newHeight);

			width = newWidth;
			height = newHeight;

			left = thumbLeft + ((thumbWidth - width) >> 1);
			top = thumbTop + ((thumbHeight - height) >> 1);
		}

        BOOL bValue = AlphaBlend(hdc, left, top, width, height, hdcThumb, 0, 0, bitmap.bmWidth, bitmap.bmHeight, blendFunc);
        SelectObject(hdcThumb, hbmpOld);
    }
    bool bNeedsDestroy = false;
    if (!hIconWindow)
    {
        hIconWindow = LoadIconW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
        bNeedsDestroy = true;
    }

    DrawIcon(hdc, rect.left + (rect.right - rect.left - iconWidth) / 2, rect.top + (rect.bottom - rect.top - iconHeight) / 2, hIconWindow);

    if (bNeedsDestroy)
    {
        DestroyIcon(hIconWindow);
    }
	return true;
}

bool HideTaskSwitch(void* pVoid, HWND hwndDlg, node** ppNode)
{
    ShowWindow(hwndDlg, SW_HIDE);
    return false;
}

inline void CFlipper::DrawUpdatedFrame(HWND hwndDlg)
{
    HRESULT hRet;
    while (TRUE)
    {
        hRet = this->Flip(hwndDlg);
        if (hRet == DD_OK)
        {
            break;
        }
        if (hRet == DDERR_SURFACELOST)
        {
            hRet = this->RestoreAll();
            if (hRet != DD_OK)
            {
                break;
            }
        }
        if (hRet == DDERR_SURFACEBUSY)
        {
			g_pDDSPrimary->Unlock(NULL);
            break;
        }
        if (hRet != DDERR_WASSTILLDRAWING)
        {
            break;
        }
    }
}

HRGN CFlipper::CreateRegionFromHDC(HDC hdc, int startX, int startY, int endX, int endY, COLORREF clrTransparent)
{
	HRGN hRgn = NULL;
	int minX = 0;
	for (int i = startY; i < endY; ++i)
	{
		bool bRowHasData = false;
		int xStart = -1, xEnd = -1;
		for (int j = startX; j < endX; ++j)
		{
			COLORREF clrPixel = GetPixel(hdc, j, i);

			if (clrPixel != clrTransparent)
			{
				HRGN hRgnTemp = CreateRectRgn(j, i, j+1, i+1);

				if (NULL == hRgn)
				{
					hRgn = hRgnTemp;
				}
				else
				{
					int ret = CombineRgn(hRgn, hRgn, hRgnTemp, RGN_OR);
					DeleteObject(hRgnTemp);
				}
			}
		}
	}
	return hRgn;
}

LRESULT CFlipper::FlipperTabProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{
    CFlipper* p_this = (CFlipper*)GetWindowLong(hwndDlg, GWL_USERDATA);
	static HBITMAP hbmp = NULL;
	static HDC hdcMem = NULL;
	static int _width, _height;
	static HRGN hRgn = NULL;
	static HDC hdcBack = NULL;
	static HBITMAP hbmpBack = NULL, hbmpOld = NULL;
	static int _iIndex = 0;
	static int _iCount = 0;

    switch (message) 
    {
		case WM_SETTINGCHANGE :
		{
			WCHAR* w = (WCHAR*)lParam;
			Sleep(0);
		}
		break;
		case WM_CREATE :
		{
			Sleep(0);
		}
		break;
		case WM_SHOWFLIPPER:
		{
			_iIndex = -1;
			_iCount = p_this->g_tabList.size();

			if (_iCount == 0)
			{
				return 1;
			}

			BITMAP bm;

			HDC hdcParent = GetDC(HWND_DESKTOP);
			hdcMem = CreateCompatibleDC(hdcParent);
			_height = p_this->g_windowHeight;
			_width = p_this->g_dwTabWidth;
			hbmp = CreateCompatibleBitmap(hdcParent, _width, _height+1);
			hbmpOld = (HBITMAP)SelectObject(hdcMem, hbmp);

			HPEN hPen = ::CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
			SelectObject(hdcMem, hPen);
			SelectObject(hdcMem, CreateSolidBrush(RGB(0xd8, 0xe2, 0xFF)));

			RECT rect;
			rect.left = rect.top = 0;
			rect.right = _width;
			rect.bottom = _height+1;

			FillRect(hdcMem, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
			
			int left = 0;
			
			if (p_this->g_dwTabAlign == 1)
			{
				left = GetSystemMetrics(SM_CXSCREEN) - _width;
				//RoundRect(hdcMem, 2, 3, _width*2, _height-2, 25,25);
				Rectangle(hdcMem, 2, 3, _width*2, _height-2);
			}
			else
			{
				//RoundRect(hdcMem, -_width, 3, _width-2, _height-2, 25,25);
				Rectangle(hdcMem, -_width, 3, _width-2, _height-2);
			}

			// do stuff here
			GetObject(hbmp, sizeof(bm), &bm);
			COLORREF clrTransparent = GetPixel(hdcMem, 0, 0);

			// Create bitmap, draw rounded rect and use that for the region

			hRgn = CreateRectRgn(0, 0, _width, _height);//CreateRegionFromHDC(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, clrTransparent);

			DeleteObject(hPen);
			SetWindowRgn(hwndDlg, hRgn, FALSE);

			MoveWindow(hwndDlg, left, (p_this->g_height - _height) /2, _width, _height, FALSE);
			//SetWindowPos(hwndDlg, NULL, 0, (p_this->g_height - _height) /2, _width, _height, SWP_SHOWWINDOW);
		}
		break;
		case WM_SELECTTAB :
			{
				if (!IsWindowVisible(hwndDlg))
				{
					ShowWindow(hwndDlg, SW_SHOW);
				}
				_iIndex = (int)lParam;

				p_this->Minimize();
				SetTimer(hwndDlg, 1, 10, NULL);

				InvalidateRect(hwndDlg, NULL, FALSE);
				UpdateWindow(hwndDlg);
			}
			return 0;
		case WM_WILLCHANGESELECTION :
			{
				int iDir = (int)lParam;

				int tempIndex = _iIndex + iDir;

				if (tempIndex >= 0 && tempIndex < _iCount)
				{
					return 1;
				}
			}
			return 0;
		case WM_TIMER :
			{
				if (WAIT_OBJECT_0 == WaitForSingleObject(p_this->hFinishedEvent, 0))
				{
					KillTimer(hwndDlg, 1);
					ResetEvent(p_this->hFinishedEvent);
					//InvalidateRect(hwndDlg, NULL, TRUE);
					p_this->ShowIndex(_iIndex);
				}
			}
			break;
        case WM_LBUTTONDOWN :
            {
                int y = HIWORD(lParam);
				double size = (double)_height / _iCount;
				int index = (int)(y / size);

				if (index == _iIndex)
				{
					p_this->Minimize();
					p_this->Hide();

					_iIndex = -1;
				}
				else
				{
					_iIndex = index;

					p_this->Minimize();
					SetTimer(hwndDlg, 1, 10, NULL);
				}
				InvalidateRect(hwndDlg, NULL, FALSE);
				UpdateWindow(hwndDlg);
			}
			break;
		case WM_PANELHIDING :
			{
				_iIndex = -1;
				InvalidateRect(hwndDlg, NULL, TRUE);
				Sleep(0);
			}
			break;
		case WM_KEYDOWN :
			{
                switch (wParam)
                {
                    case VK_UP :
						{
							if (_iIndex > 0)
							{
								_iIndex--;
								InvalidateRect(hwndDlg, NULL, FALSE);
								UpdateWindow(hwndDlg);
								p_this->Minimize();
								SetTimer(hwndDlg, 1, 10, NULL);
								//p_this->ShowIndex(_iIndex);
							}
						}
						break;
                    case VK_DOWN :
						{
							if (_iIndex < _iCount-1)
							{
								_iIndex++;
								InvalidateRect(hwndDlg, NULL, FALSE);
								UpdateWindow(hwndDlg);
								p_this->Minimize();
								SetTimer(hwndDlg, 1, 10, NULL);
								//p_this->ShowIndex(_iIndex);
							}
						}
						break;
				}
			}
			break;
        case WM_PAINT :
            {
                if (NULL == hdcBack)
                {
					RECT windowRect;
					GetWindowRect(hwndDlg, &windowRect);
                    int width = GetSystemMetrics(SM_CXSCREEN);
                    int height = GetSystemMetrics(SM_CYSCREEN);

                    BLENDFUNCTION blendFunc;

                    HDC hdcDesk = GetDC(HWND_DESKTOP);
                    hdcBack = CreateCompatibleDC(hdcDesk);
                    hbmpBack = CreateCompatibleBitmap(hdcDesk, _width, _height);
                    hbmpOld = (HBITMAP)SelectObject(hdcBack, hbmpBack);
					
     //               blendFunc.AlphaFormat = 0;
     //               blendFunc.BlendFlags = 0;
     //               blendFunc.BlendOp = AC_SRC_OVER;
     //               blendFunc.SourceConstantAlpha = 255;

					//BitBlt(hdcBack, 0, 0, _width, _height, hdcDesk, windowRect.left, windowRect.top, SRCCOPY);
     //               AlphaBlend(hdcBack, 0, 0, _width, _height, hdcMem, 0, 0, _width, _height, blendFunc);

                    // Draw highlight areas here
                    DeleteDC(hdcDesk);
                }
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwndDlg, &ps);
				
				//BitBlt(hdc, 0, 0, _width, _height, hdcBack, 0, 0, SRCCOPY);

				list<FlipperPlugin*>::iterator iter = p_this->g_tabList.begin();

				int i = 0;
				
				RECT rect;
				rect.left = rect.top = 0;
				rect.bottom = _height / _iCount;
				rect.right = _width;

				GpGraphics* gx;

				SmoothingMode smooth;
				GdipCreateFromHDC(hdcBack, &gx);

				GdipGetSmoothingMode(gx, &smooth);
				GdipSetSmoothingMode(gx, SmoothingMode::SmoothingModeAntiAlias);

				while (iter != p_this->g_tabList.end())
				{
					rect.top = (long)(i * (double)_height / _iCount);
					rect.bottom = (long)((i+1) * (double)_height / _iCount);

					COLORREF color = (*iter)->dwColor;
					
					if (_iIndex == i) 
					{
						color = DarkenColor(color, FLIPTAB_DARKEN_AMOUNT);
					}

					HBRUSH hBrush = CreateSolidBrush(color);
					FillRect(hdcBack, &rect, hBrush);
					DeleteObject(hBrush);

					int opacity = 0x7f;

					GpRect gpRect;
					gpRect.X = rect.left;
					gpRect.Y = rect.top;
					gpRect.Width = (rect.right - rect.left);
					gpRect.Height = (rect.bottom - rect.top);

					GpPointF point1, point2;
					point1.X = gpRect.X;
					point1.Y = gpRect.Y;
					point2.X = gpRect.Width;
					point2.Y = gpRect.Y;

					ARGB aRgb1 = RGB(0,0,0) | (((opacity)&0xfF) << 24);
					ARGB aRgb2 = RGB(0xff,0xff,0xff) | (((opacity)&0xfF) << 24);

					GpLineGradient* gGradient;
					GdipCreateLineBrush(&point1, &point2, aRgb1, aRgb2, GpWrapMode::WrapModeTile, &gGradient);
					//GdipFillPath(gx, gGradient, gPath);
					GdipFillRectangle(gx, gGradient, rect.left, rect.top, (rect.right-rect.left), (rect.bottom-rect.top));

					GdipDeleteBrush((GpBrush*)&gGradient);

					if (_iIndex == i) 
					{
						//RECT rcBorder;
						//memcpy(&rcBorder, &rect, sizeof(RECT));
						//rcBorder.top += 5;
						//rcBorder.bottom -= 5;
						//rcBorder.right = rcBorder.left + 10;
						//FillRect(hdc, &rcBorder, (HBRUSH)GetStockObject(BLACK_BRUSH));
						
						ARGB aRgb1 = RGB(0xff,0xff,0xff) | (((0xff)&0xfF) << 24);
						ARGB aRgb2 = RGB(0,0,0) | (((0x7f)&0xfF) << 24);

						point1.X = gpRect.X;
						point2.X = gpRect.Width;
						point2.Y = gpRect.Y;

						GdipCreateLineBrush(&point1, &point2, aRgb1, aRgb2, GpWrapMode::WrapModeTile, &gGradient);
						GdipFillRectangle(gx, gGradient, rect.left, rect.top, (rect.right-rect.left), (rect.bottom-rect.top));
						GdipDeleteBrush((GpBrush*)&gGradient);
					}
					i++;
					iter++;
				}
				GdipSetSmoothingMode(gx, smooth);

				GdipReleaseDC(gx, hdc);
				GdipDeleteGraphics(gx);
				//g_pColors
				HPEN hPen = ::CreatePen(PS_DASH, 4, RGB(0,0,0));
				HGDIOBJ hOld = SelectObject(hdcBack, hPen);
				SetBkColor(hdcBack, TRANSPARENT);
				MoveToEx(hdcBack, 0, 0, NULL);
				LineTo(hdcBack, _width, 0);
				LineTo(hdcBack, _width, _height);
				LineTo(hdcBack, 0, _height);
				LineTo(hdcBack, 0, 0);
				SelectObject(hdcBack, hOld);
				BitBlt(hdc, 0, 0, _width, _height, hdcBack, 0, 0, SRCCOPY);
				//TransparentBlt(hdc, 0, 0, _width, _height, hdcBack, 0, 0, _width, _height, RGB(0xd8, 0xe2, 0xFF));

                EndPaint(hwndDlg, &ps);
                Sleep(0);
            }
            break;
		case WM_ERASEBKGND :
			{
				return 0;
                if (NULL == hdcBack)
                {
					RECT windowRect;
					GetWindowRect(hwndDlg, &windowRect);
                    int width = GetSystemMetrics(SM_CXSCREEN);
                    int height = GetSystemMetrics(SM_CYSCREEN);

                    BLENDFUNCTION blendFunc;

                    HDC hdcDesk = GetDC(HWND_DESKTOP);
                    hdcBack = CreateCompatibleDC(hdcDesk);
                    hbmpBack = CreateCompatibleBitmap(hdcDesk, _width, _height);
                    hbmpOld = (HBITMAP)SelectObject(hdcBack, hbmpBack);
					
                    blendFunc.AlphaFormat = 0;
                    blendFunc.BlendFlags = 0;
                    blendFunc.BlendOp = AC_SRC_OVER;
                    blendFunc.SourceConstantAlpha = 255;

					BitBlt(hdcBack, 0, 0, _width, _height, hdcDesk, windowRect.left, windowRect.top, SRCCOPY);
                    AlphaBlend(hdcBack, 0, 0, _width, _height, hdcMem, 0, 0, _width, _height, blendFunc);

                    // Draw highlight areas here
                    DeleteDC(hdcDesk);
                }
			}
			break;
	}
    return DefWindowProc(hwndDlg, message, wParam, lParam);
}



//-----------------------------------------------------------------------------
// Name: DDColorMatch()
// Desc: Convert a RGB color to a pysical color.
//       We do this by leting GDI SetPixel() do the color matching
//       then we lock the memory and see what it got mapped to.
//-----------------------------------------------------------------------------
extern "C" DWORD
DDColorMatch(IDirectDrawSurface * pdds, COLORREF rgb)
{
    COLORREF                rgbT;
    HDC                     hdc;
    DWORD                   dw = CLR_INVALID;
    DDSURFACEDESC           ddsd;
    HRESULT                 hres;

    //
    //  Use GDI SetPixel to color match for us
    //
    if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK)
    {
        rgbT = GetPixel(hdc, 0, 0);     // Save current pixel value
        SetPixel(hdc, 0, 0, rgb);       // Set our value
        pdds->ReleaseDC(hdc);
    }
    else
    {
        return dw;
    }
    //
    // Now lock the surface so we can read back the converted color
    //
    ddsd.dwSize = sizeof(ddsd);
    while ((hres = pdds->Lock(NULL, &ddsd, 0, NULL)) == DDERR_WASSTILLDRAWING)
        ;
    if (hres == DD_OK)
    {
        dw = *(DWORD *) ddsd.lpSurface;                 // Get DWORD
        if (ddsd.ddpfPixelFormat.dwRGBBitCount < 32)
            dw &= (1 << ddsd.ddpfPixelFormat.dwRGBBitCount) - 1;  // Mask it to bpp
        pdds->Unlock(NULL);
    }
    //
    //  Now put the color that was there back.
    //
    if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK)
    {
        SetPixel(hdc, 0, 0, rgbT);
        pdds->ReleaseDC(hdc);
    }
    return dw;
}

//-----------------------------------------------------------------------------
// Name: DDSetColorKey()
// Desc: Set a color key for a surface, given a RGB.
//       If you pass CLR_INVALID as the color key, the pixel
//       in the upper-left corner will be used.
//-----------------------------------------------------------------------------
extern "C" HRESULT
DDSetColorKey2(IDirectDrawSurface * pdds, COLORREF rgb)
{
    DDCOLORKEY ddck;

    ddck.dwColorSpaceLowValue = DDColorMatch(pdds, rgb);
    ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
    return pdds->SetColorKey(DDCKEY_SRCBLT, &ddck);
}

LRESULT CFlipper::FlipperContentProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{
    CFlipper* p_this = (CFlipper*)GetWindowLong(hwndDlg, GWL_USERDATA);
	static FlipperPlugin* g_pPluginData;

	static WCHAR* _wzLoading = L"Loading...";
	static WCHAR* _wzNoData = L"No Data...";
	static WCHAR* _wzMessage = NULL;

	static int _height, _width;

    static int _x = 0;
    static int _y = 0;

	static list<int> _prevXs;
	static list<int> _prevTimes;

    static int _xDiff = 0;
    static int _timeDiff = 0;

    static int _iAccel = 0;
    static int _iAccelStart = 0;
    static int _iDecel = 1;
    static bool _bMouseDown = false;
    static bool _bMouseMove = false;
    static node* _pHead = NULL;
    static node* _pUnderMouse = NULL;
	static node* _pStart = NULL;
	static node* _pStartNext = NULL;
	static node* _pStartFinal = NULL;
	static node* _pSelected = NULL;
    static int _iDistance = 0;
    static int _iDistanceNeeded = 0;
    static int _iVelocity = 0;
	static int _iOffset = 0;
    static int _iEnd = 0;
    static int _iMouseDistance = 0;
    static int _iNodLen = 0;
    static DWORD _dwDownTick = 0;

	static int _iIndex = 0, _iMinIndex = 0, _iMaxIndex = 0, _iPerPage = 0;

    static PFN_ClickAction _pClickAction = NULL;
	static PFN_DrawThumbnail _pDrawThumbnail = &DrawThumbnail;

    // 1 = in, 2 = out, 3 = scroll
    static int _iAnimation = 0;
    static int _iIteration = 0;
    static int _iIterations = 5;
    static int _iInitialDir = 0;
    static bool _isAnimating = false;
    static bool _isLoaded = false, _isLoading = false, _isError = false;
    static double _decelRateConst = 0.0, _decelRate = 0.0;
    static DWORD _dwLastAnimation = 0;
    static DWORD _dwEstimatedTime = 0;
	static HWND _hwndMenu = NULL;

	static LPDIRECTDRAWSURFACE _ddTiles = NULL;

    // img stuff
    static HDC _hdcThumb = NULL;

	HWND hwnd = GetFocus();
    switch (message) 
    {
		case WM_KEYUP :
		{
			switch (wParam)
			{
				case VK_TBACK :
				{
					p_this->Hide();
				}
				return 1;
			}
		}
		break;
		case WM_DESTROY :
			{
				if (_hwndMenu)
				{
					DestroyWindow(_hwndMenu);
					_hwndMenu = NULL;
				}
			}
			break;
        case WM_SHOWFLIPPER:
            {
				_isLoading = true;
				_wzMessage = _wzLoading;
				_iIndex = 0;
				_iInitialDir = (int)LOWORD(lParam);
				if (wParam)
				{
					g_pPluginData = (FlipperPlugin*)wParam;
				}
				
				SendMessage(hwndDlg, SHCMBM_OVERRIDEKEY, VK_TBACK, MAKELPARAM(SHMBOF_NODEFAULT | SHMBOF_NOTIFY, SHMBOF_NODEFAULT | SHMBOF_NOTIFY));

				if (!_hwndMenu)
				{
					SHMENUBARINFO mbi;
					memset(&mbi, 0, sizeof(SHMENUBARINFO));
					mbi.cbSize = sizeof(SHMENUBARINFO);
					mbi.hwndParent = hwndDlg;
					mbi.nToolBarId = IDR_Flipper_SET_MENUBAR;
					mbi.hInstRes = GetModuleHandle(NULL);
					mbi.nBmpId = 0;
					mbi.cBmpImages = 0;    

					SHCreateMenuBar(&mbi);
					_hwndMenu = mbi.hwndMB;
					Sleep(0);
				}

                HDC hdc = GetDC(NULL);
                _hdcThumb = CreateCompatibleDC(hdc);
                DeleteDC(hdc);

				//p_this->g_bFlipperEnableShrinkAnimation = true;
				p_this->g_bFlipperEnableScrollAnimation = true;

                if (p_this->g_bFlipperEnableShrinkAnimation)
                {
                    _iIterations = 10;
                }
                else
                {
                    _iIterations = 0;
                }
                if (p_this->g_bFlipperEnableScrollAnimation)
                {
                    _decelRate = _decelRateConst = p_this->g_dwDeceleration / 100.0;
                }
                else
                {
                    _decelRate = _decelRateConst = 0;
                }
                _dwEstimatedTime = _iIterations * p_this->g_dwAnimateInterval;

				_iNodLen = 0;
				
				_height = p_this->g_windowHeight;
				_width = p_this->g_width - p_this->g_dwTabWidth;

				if (true)
				{
					int left = p_this->g_dwTabWidth;

					if (p_this->g_dwTabAlign == 1)
					{
						left = 0;
					}

					p_this->InitSurfaces(g_pPluginData->dwColor, left, (p_this->g_height - _height) / 2, p_this->g_width, _height, !HIWORD(lParam));
				}
				_isError = false;
				_isLoaded = true;
                
                _iAnimation = 1;
				int left = p_this->g_dwTabWidth;

				if (p_this->g_dwTabAlign == 1)
				{
					left = 0;
				}

				SetWindowPos(hwndDlg, NULL, left, (p_this->g_height - _height) / 2, _width, _height, SWP_SHOWWINDOW);
                SetForegroundWindow(hwndDlg);
				SetActiveWindow(hwndDlg);
				SetFocus(hwndDlg);

                // fade in
                _iIteration = _iIterations;
                if (p_this->g_bFlipperEnableShrinkAnimation)
                {
                    _iIteration = 0;
                }
                SetTimer(hwndDlg, 1, p_this->g_dwAnimateInterval, NULL);
            }
            break;
		case WM_DATAREADY :
			{
				_pSelected = NULL;
				node* pHead = (node*)wParam;
				_iNodLen = (int)lParam;

				bool bSameHead = (pHead == _pHead);
				_pHead = pHead;

				if (!bSameHead)
				{
					_iIndex = 0;
				}

				//if (g_pPluginData->pfnBuildNodeList)
				//{
				//	_pHead = g_pPluginData->pfnBuildNodeList(g_pPluginData, &_iNodLen, &(p_this->g_bigThumbWidth), &(p_this->g_bigThumbHeight), _width, _height);
				//}
				if (_hwndMenu)
				{
					ShowWindow(_hwndMenu, SW_SHOW);
					FlipperSoftKey softKeys[2];
					ZeroMemory(softKeys, sizeof(softKeys));

					if (g_pPluginData->pfnSetSoftKeys)
					{
						g_pPluginData->pfnSetSoftKeys(g_pPluginData, &softKeys[0], &softKeys[1]);
					}

					TBBUTTONINFO buttonInfo = {0};
					buttonInfo.cbSize = sizeof(TBBUTTONINFO);
					buttonInfo.dwMask = TBIF_STATE | TBIF_TEXT;
					buttonInfo.idCommand = IDM_Flipper_Menu_Left;
					buttonInfo.pszText = softKeys[0].wzText;
					buttonInfo.fsState = (softKeys[0].bEnabled) ? TBSTATE_ENABLED : TBSTATE_HIDDEN;

					SendMessage(_hwndMenu, TB_SETBUTTONINFO, IDM_Flipper_Menu_Left, (LPARAM)&buttonInfo);

					buttonInfo.idCommand = IDM_Flipper_Menu_Right;
					buttonInfo.pszText = softKeys[1].wzText;
					buttonInfo.fsState = (softKeys[1].bEnabled) ? TBSTATE_ENABLED : TBSTATE_HIDDEN;

					SendMessage(_hwndMenu, TB_SETBUTTONINFO, IDM_Flipper_Menu_Right, (LPARAM)&buttonInfo);
				}
				if (!_pHead)
				{
					_wzMessage = _wzNoData;
					_isLoading = false;
					_isError = true;
				}
				else
				{
					if (_ddTiles)
					{
						_ddTiles->Release();
						_ddTiles = NULL;
					}

					_iMinIndex = 0;
					_iPerPage = (_width) / p_this->g_bigThumbWidth;
					_iMaxIndex = max(0, _iNodLen - _iPerPage);

					_iIndex = min(_iIndex, _iNodLen-1);

					p_this->g_bigThumbWidth = min(_width, p_this->g_bigThumbWidth);
					p_this->g_bigThumbHeight = min(_height, p_this->g_bigThumbHeight);

					_pDrawThumbnail = &DrawThumbnail;
					if (g_pPluginData->pfnDrawThumbnail)
					{
						_pDrawThumbnail = g_pPluginData->pfnDrawThumbnail;
					}

					if (_iNodLen > 1)
					{
						// We could try building a big list here...
						// Create the primary surface with 1 back buffer
						DDSURFACEDESC ddsd;
						memset(&ddsd, 0, sizeof(ddsd));
						ddsd.dwSize = sizeof(ddsd);
						ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH | DDSD_CKSRCBLT;
						ddsd.ddckCKSrcBlt.dwColorSpaceLowValue = 1;
						ddsd.ddckCKSrcBlt.dwColorSpaceHighValue = RGB(0xFF, 0xFF, 0xFF);
						ddsd.dwHeight = _height;
						ddsd.dwWidth = _iNodLen * (p_this->g_bigThumbWidth+p_this->g_iPadding);
						HRESULT hRet = p_this->g_pDD->CreateSurface(&ddsd, &_ddTiles, NULL);

						if (hRet != DD_OK)
						{
							// we failed
							Sleep(0);
						}

						DWORD dwFill = DDColorMatch(_ddTiles, RGB(0xFF,0,0xFF));

						DDBLTFX fillFx;
						fillFx.dwSize = sizeof(fillFx);
						fillFx.dwFillColor = dwFill;
						fillFx.dwROP = SRCCOPY;

						DDCOLORKEY ddck;
						ddck.dwColorSpaceLowValue = dwFill;
						ddck.dwColorSpaceHighValue = dwFill;

						hRet = _ddTiles->Blt(NULL, NULL, NULL, DDBLT_COLORFILL, &fillFx);
						hRet = _ddTiles->SetColorKey(DDCKEY_SRCBLT, &ddck);

						HDC hdcBuffer;
						_ddTiles->GetDC(&hdcBuffer);
						p_this->CreateTileBuffer(g_pPluginData, _pDrawThumbnail, hdcBuffer, _hdcThumb, hwndDlg, _pHead);
						_ddTiles->ReleaseDC(hdcBuffer);
					}
				}
				_pStart = _pStartNext = _pHead;
				_isLoading = false;

				if (_iAnimation == 3)
				{
					if (_isError)
					{
						p_this->UpdateLoadingScreen(_wzMessage);
						p_this->DrawUpdatedFrame(hwndDlg);
					}
					else
					{
						int x;
						p_this->UpdateFrameScroll(g_pPluginData, _ddTiles, _pDrawThumbnail, _hdcThumb, hwndDlg, p_this->g_bigThumbWidth * _iInitialDir, 0, 0, 0, _pStart, NULL, _iIndex, &_pUnderMouse, &_pClickAction, &x, &x, true);
						p_this->DrawUpdatedFrame(hwndDlg);
					}
				}
			}
			break;

		case WM_COMMAND :
			{
				switch (LOWORD(wParam))
				{
					case IDM_Flipper_Menu_Left :
					{
						node* pSelected = _pHead;
						for (int i = 0; i < _iNodLen; ++i)
						{
							if (_iIndex == pSelected->iIndex)
							{
								break;
							}
							else
							{
								pSelected = pSelected->next;
							}
						}
						if (g_pPluginData->pfnSoftKeySelected)
						{
							g_pPluginData->pfnSoftKeySelected(g_pPluginData, pSelected, true);
						}
					}
					break;
					case IDM_Flipper_Menu_Right :
					{
						node* pSelected = _pHead;
						for (int i = 0; i < _iNodLen; ++i)
						{
							if (_iIndex == pSelected->iIndex)
							{
								break;
							}
							else
							{
								pSelected = pSelected->next;
							}
						}
						if (g_pPluginData->pfnSoftKeySelected)
						{
							g_pPluginData->pfnSoftKeySelected(g_pPluginData, pSelected, false);
						}
					}
					break;
				}
			}
			break;
        case WM_ACTIVATE :
            {
                if (!LOWORD(wParam))
                {
                    if (_iAnimation == 1)
                    {
                        Sleep(0);
                    }
					p_this->Hide();

					if (_hwndMenu)
					{
						ShowWindow(_hwndMenu, SW_HIDE);
					}
                }
				else
				{
				}
            }
            break;
        case WM_KILLFOCUS :
            {
                Sleep(0);
            }
            break;
        case WM_SETFOCUS :
            {
				Sleep(0);
            }
            break;
        case WM_HIDEFLIPPER :
            {
				p_this->CancelBuild();
                if (_isLoaded)
                {
                    _isLoaded = false;

                    _x = _y = 0;

                    KillTimer(hwndDlg, 1);
                    KillTimer(hwndDlg, 2);

                    _iIteration = 0;
                    _iDistance = _iDecel = _iDistanceNeeded = 0;

                    SetEvent(p_this->hFinishedEvent);
                    if (_hdcThumb)
                    {
                        DeleteDC(_hdcThumb);
                        _hdcThumb = NULL;
                    }
					if (_pHead)
					{
						if (g_pPluginData->pfnDestroyNodeList)
						{
							g_pPluginData->pfnDestroyNodeList(g_pPluginData, _pHead, false);
						}
					}
					_pHead = NULL;
                }
				else
				{
                    SetEvent(p_this->hFinishedEvent);
				}
            }
            return 1;
		case WM_SELECTIONCLOSE :
			{
				node* pHead = (node*)lParam;
				if (g_pPluginData->pfnCloseOnSelection && pHead)
				{
					return g_pPluginData->pfnCloseOnSelection(g_pPluginData, pHead);
				}
				return 1;
			}
		case WM_NODESELECTED :
			{
				node* pHead = (node*)lParam;
				if (pHead)
				{
					if (g_pPluginData->pfnNodeSelected)
					{
						bool bRefresh = false;
						g_pPluginData->pfnNodeSelected(g_pPluginData, pHead, &bRefresh);

						if (bRefresh)
						{
							p_this->Refresh();
							return 1;
						}
					}
					else
					{
					}
						p_this->Hide();
						ShowWindow(hwndDlg, SW_HIDE);
				}
				else
				{
					//p_this->Hide();
				}
			}
			return 1;
        case WM_SELECTFLIPPER :
            {
				if (wParam)
				{
					_pSelected = NULL;
				}
				else
				{
					if (_pHead)
					{
						_pSelected = _pHead;
						for (int i = 0; i < _iNodLen; ++i)
						{
							if (_iIndex == _pSelected->iIndex)
							{
								break;
							}
							else
							{
								_pSelected = _pSelected->next;
							}
						}
					}
				}

				if (SendMessage(hwndDlg, WM_SELECTIONCLOSE, NULL, (LPARAM)_pSelected))
				{
					// let's select this guy
					KillTimer(hwndDlg, 1);
					if (_iAnimation == 1)
					{
						_iIteration = max(0, _iIterations - _iIteration);
					}
					if (_iAnimation == 3)
					{
						_iIteration = 0;
					}
					_iAnimation = 2;
					SetTimer(hwndDlg, 1, p_this->g_dwAnimateInterval, NULL);
				}
				else if (!wParam)
				{
					SendMessage(hwndDlg, WM_NODESELECTED, NULL, (LPARAM)_pSelected);
				}
				else
				{
					SendMessage(hwndDlg, WM_HIDEFLIPPER, NULL, NULL);
				}
            }
            return 1;
                case WM_KEYDOWN :
            {
				// Handle any non-accelerated key commands
				switch (wParam)
				{
					case VK_UP :
					case VK_DOWN :
						{
							if (SendMessage(p_this->g_hwndAltTabBorder, WM_WILLCHANGESELECTION, 0, (wParam == VK_UP) ? -1 : 1))
							{
								SetEvent(p_this->hFinishedEvent);
								KillTimer(hwndDlg, 1);
								PostMessage(p_this->g_hwndAltTabBorder, WM_KEYDOWN, wParam, lParam);
							}
							return 1;
						}
				}
                if (_iAnimation == 3)
                {
                    // Handle any non-accelerated key commands
                    switch (wParam)
                    {
						case VK_RETURN:
                        case VK_ACTION:
                            {
                                //p_this->AbortAutoClose(hwndDlg);
                                SendMessage(hwndDlg, WM_SELECTFLIPPER, 0, 0);
                            }
                            break;
                        case VK_LEFT :
                            {
								node* pTemp = _pStart;
								if (_iDistanceNeeded != 0)
								{
									pTemp = _pStartFinal;
								}
								if (_iIndex < pTemp->iIndex)
								{
									_iIndex = pTemp->iIndex;
								}
								if (_iIndex > (pTemp->iIndex + _iPerPage))
								{
									_iIndex = pTemp->iIndex + _iPerPage;
								}
								if (_iIndex > 0)
								{
									_iIndex--;

									//p_this->AbortAutoClose(hwndDlg);
									KillTimer(hwndDlg, 1);
									if (_iIndex < pTemp->iIndex)
									{
										_iDistanceNeeded += p_this->g_bigThumbWidth;
										_pStartFinal = pTemp->prev;
									}
									SetTimer(hwndDlg, 1, p_this->g_dwAnimateInterval, NULL);
								}
                            }
                            break;
                        case VK_RIGHT :
                            {
								node* pTemp = _pStart;
								if (_iDistanceNeeded != 0)
								{
									pTemp = _pStartFinal;
								}
								if (_iIndex < pTemp->iIndex)
								{
									_iIndex = pTemp->iIndex;
								}
								if (_iIndex > (pTemp->iIndex + _iPerPage))
								{
									_iIndex = pTemp->iIndex + _iPerPage;
								}
								if (_iIndex < _iNodLen-1)
								{
									_iIndex++;

									//p_this->AbortAutoClose(hwndDlg);
									KillTimer(hwndDlg, 1);
									if (_iIndex >= pTemp->iIndex + _iPerPage)
									{
										_iDistanceNeeded += -p_this->g_bigThumbWidth;
										_pStartFinal = pTemp->next;
									}
									SetTimer(hwndDlg, 1, p_this->g_dwAnimateInterval, NULL);
								}
                            }
                            break;
                    }
                }
            }
            return 1;
        case WM_LBUTTONDOWN :
            {
                // if the timer is active...we need to adjust our x offset here
                
                if (_iAnimation == 3)
                {
                    //p_this->AbortAutoClose(hwndDlg);
                    int x = _x;
                    int y = _y;
                    _x = LOWORD(lParam);
                    _y = HIWORD(lParam);
					
					if (_iDistanceNeeded != 0)
					{
						_isAnimating = false;
						_bMouseDown = true;
						_bMouseMove = true;
						KillTimer(hwndDlg, 1);
						_pStart = _pStartFinal;

						//p_this->UpdateFrameScroll(g_pPluginData, _ddTiles, _pDrawThumbnail, _hdcThumb, hwndDlg, 0, 0, _x, _y, _pStart, &_pStartNext, _iIndex, &_pUnderMouse, &_pClickAction, &_iDistance, &_iMouseDistance, true);

						p_this->UpdateFrameScroll(g_pPluginData, _ddTiles, _pDrawThumbnail, _hdcThumb, hwndDlg, -_iDistanceNeeded, 0, _x, _y, _pStart, &_pStartNext, _iIndex, &_pUnderMouse, &_pClickAction, &_iDistance, &_iMouseDistance, true);
                        p_this->DrawUpdatedFrame(hwndDlg);
						
						//_x += _iDistanceNeeded;
						//_iDistanceNeeded = 0;
						Sleep(0);
						_dwDownTick = GetTickCount();
					}
					else
					{
	                    p_this->UpdateFrameScroll(g_pPluginData, _ddTiles, _pDrawThumbnail, _hdcThumb, hwndDlg, 0, 0, _x, _y, _pStart, &_pStartNext, _iIndex, &_pUnderMouse, &_pClickAction, &_iDistance, &_iMouseDistance, true);

						if (_pUnderMouse)
						{
							_iIndex = _pUnderMouse->iIndex;
							p_this->UpdateFrameScroll(g_pPluginData, _ddTiles, _pDrawThumbnail, _hdcThumb, hwndDlg, 0, 0, _x, _y, _pStart, &_pStartNext, _iIndex, &_pUnderMouse, &_pClickAction, &_iDistance, &_iMouseDistance, true);
							p_this->DrawUpdatedFrame(hwndDlg);
						}

						_bMouseMove = false;
						_pClickAction = NULL;

						if (_y >= p_this->g_bigThumbTop)
						{
							_bMouseDown = true;

							if (_y <= (p_this->g_bigThumbTop + p_this->g_bigThumbHeight))
							{
								_bMouseMove = true;
								KillTimer(hwndDlg, 1);

								_dwDownTick = GetTickCount();
							}
						}
						else
						{
							_pClickAction = &HideTaskSwitch;
						}
					}
                    if (!_bMouseMove && _isAnimating)
                    {
                        _x = x;
                        _y = y;
                        _bMouseDown = false;
                    }
                }
            }
            break;
        case WM_LBUTTONUP :
            {
                if (_iAnimation == 3)
                {
                    int x = LOWORD(lParam);
                    int y = HIWORD(lParam);

					//_pStart = _pStartNext;
					int tempIndex = _pStartNext->iIndex;
                    DWORD dwUpTick = GetTickCount();
                    DWORD dwDiff = (dwUpTick - _dwDownTick);
                    int xDiff = (x - _x);

                    if (_bMouseMove)
                    {
						int iDesiredIndex = tempIndex;

                        if (abs(xDiff) >= 20)
						{
                            // between -10 & 10
                            int count = max(-10, min(10, xDiff / p_this->g_bigThumbWidth));

                            if (count == 0)
                            {
                                if (xDiff < 0)
                                {
                                    count = -1;
                                }
                                else
                                {
                                    count = 1;
                                }
                            }
                            else
                            {
                                count = count << 1;
                            }

							if (_prevXs.size() > 1)
							{
								list<int>::iterator iter = _prevXs.begin();
								int iVelocity = (*iter - *(++iter)) / 2;

								double test = (1 + abs(iVelocity)) * (iVelocity/2.0);

								_prevXs.clear();

								int tempCount = (int)((1 + abs(iVelocity)) * (iVelocity/2.0) / p_this->g_bigThumbWidth);

								if (tempCount)
								{
									count = max(-10, min(10, tempCount));
								}
								else
								{
									count = 0;
									Sleep(0);
								}
							}
							else
							{
								count = 0;
							}

							if (tempIndex-count < 0 || tempIndex-count >= _iNodLen)
							{
								iDesiredIndex = tempIndex-count;
							}
							else
							{
								// _iDistance is how far we need to go to get to our current item
								if (count > 0)
								{
									iDesiredIndex += (_pStartNext->iIndex - tempIndex);
									iDesiredIndex -= abs(count);
								}
								else if (count < 0)
								{
									iDesiredIndex += (_pStartNext->iIndex - tempIndex);
									iDesiredIndex += abs(count);
								}
							}
                            _isAnimating = true;
                            //SetTimer(hwndDlg, 1, p_this->g_dwAnimateInterval, NULL);
                            Sleep(0);
                            // Start a timer here
                        }
						else if (dwDiff > 500 || _iDistanceNeeded)
						{
							iDesiredIndex = tempIndex;
						}
                        else if (_pUnderMouse)
                        {
							_iIndex = _pUnderMouse->iIndex;
							SendMessage(hwndDlg, WM_KEYDOWN, VK_ACTION, 0);
                        }
                        else if (abs(xDiff) < 20)
                        {
							// we click elsewhere -- close it
							ShowWindow(hwndDlg, SW_HIDE);
                        }
                        else
                        {
							if (_pStartNext)
							{
								// Add acceleration here - we want to snap to the active tab
								_iDecel = x - _x;
								_iDistanceNeeded = _iDistance;
							}
                        }
						if (iDesiredIndex != _iIndex || _pStartNext)
						{
							iDesiredIndex = max(min(iDesiredIndex, _iNodLen-1), 0);

							if (!_pStartNext)
							{
								_pStartNext = _pStart;
							}
							int iCurrIndex = _pStart->iIndex;
							int iStartIndex = iDesiredIndex;
							if (iStartIndex < _iMinIndex)
							{
								iStartIndex = _iMinIndex;
							}
							if (iStartIndex > _iMaxIndex)
							{
								iStartIndex = _iMaxIndex;
							}
							node* pIter = _pHead;
							for (int i = 0; i <= iStartIndex; ++i)
							{
								if (i == iStartIndex)
								{
									break;
								}
								pIter = pIter->next;
							}

							int scaleDiff = (iCurrIndex - iStartIndex);

							//_iIndex = iDesiredIndex;
							_pStartFinal = pIter;
							_iDistanceNeeded = _iDistance + scaleDiff * p_this->g_bigThumbWidth;
							_iDecel = -_iDistance;//x - _x;

							SetTimer(hwndDlg, 1, p_this->g_dwAnimateInterval, NULL);
						}
                    }
                    else if (_pClickAction != NULL)
                    {
                        if (_pClickAction(p_this, hwndDlg, &_pHead))
                        {
                            p_this->UpdateFrameScroll(g_pPluginData, _ddTiles, _pDrawThumbnail, _hdcThumb, hwndDlg, 0, 0, 0, 0, _pStart, NULL, _iIndex, &_pUnderMouse, &_pClickAction, &_iDistance, &_iMouseDistance, true);
                            p_this->DrawUpdatedFrame(hwndDlg);
                        }
                    }

                    _bMouseMove = false;
                    _bMouseDown = false;
                }
                _pClickAction = NULL;
            }
            break;
        case WM_MOUSEMOVE :
            {
                if (_iAnimation == 3 && _bMouseDown)
                {
                    int x = LOWORD(lParam);
                    int y = HIWORD(lParam);

					_prevXs.push_front(x);
					_prevTimes.push_front(GetTickCount());

                    int xDiff = 0;
                    int yDiff = 0;

                    if (_bMouseMove)
                    {
                        xDiff = (x - _x) - _iDistanceNeeded;
                        yDiff = (y - _y);
                    }

                    if (_bMouseMove || !_isAnimating)
                    {
                        p_this->UpdateFrameScroll(g_pPluginData, _ddTiles, _pDrawThumbnail, _hdcThumb, hwndDlg, xDiff, yDiff, x, y, _pStart, &_pStartNext, _iIndex, &_pUnderMouse, &_pClickAction, &_iDistance, &_iMouseDistance, true);
                        p_this->DrawUpdatedFrame(hwndDlg);
                    }
                }
            }
            break;
        case WM_TIMER :
            {
                // Update and flip surfaces
                if (wParam == 2)
                {
                    // auto-select here
                    //p_this->AbortAutoClose(hwndDlg);
                    SendMessage(hwndDlg, WM_SELECTFLIPPER, 0, 0);
                }
                else if (1 == wParam && _iAnimation > 0)
                {
                    int x;
                    DWORD dwTick = GetTickCount();
                    DWORD dwDiff = dwTick - _dwLastAnimation;
                    bool bSkipFrame = (dwDiff < 500) && (_dwLastAnimation > 0) && (dwDiff / p_this->g_dwAnimateInterval)> 1;
                    _dwLastAnimation = dwTick;

                    if (bSkipFrame)
                    {
                        Sleep(0);
                    }

                    switch (_iAnimation)
                    {
                    case 1 :
                        {
                            if (_iIteration <= _iIterations)
                            {
                                if (bSkipFrame)
                                {
									_iIteration++;
                                }
                                _iIteration++;
								_iIteration = min(_iIterations+1, _iIteration);

                                p_this->UpdateFrameAnimateIn(_iIterations, _iIteration);
                            }
                            else
                            {
                                _iAnimation = 3;
                                KillTimer(hwndDlg, 1);
                                _isAnimating = false;
                                // Slide left or right here depending on scroll

								if (!_isLoading && !_isError)
								{
									if (_iInitialDir < 0)
									{
										SendMessage(hwndDlg, WM_KEYDOWN, VK_LEFT, 0);
									}
									else if (_iInitialDir > 0)
									{
										SendMessage(hwndDlg, WM_KEYDOWN, VK_RIGHT, 0);
									}
									else
									{
										p_this->UpdateFrameScroll(g_pPluginData, _ddTiles, _pDrawThumbnail, _hdcThumb, hwndDlg, p_this->g_bigThumbWidth * _iInitialDir, 0, _x, _y, _pStart, &_pStartNext, _iIndex, &_pUnderMouse, &_pClickAction, &x, &x, true);
									}
								}
								else
								{
									p_this->UpdateLoadingScreen(_wzMessage);
								}
								SetActiveWindow(hwndDlg);
								SetFocus(hwndDlg);
                            }
                        }
                        break;
                    case 2 :
                        {
                            if (_iIteration <= _iIterations)
                            {
                                _iIteration++;
                                p_this->UpdateFrameAnimateOut(_hdcThumb, _iIterations, _iIterations-_iIteration);
                            }
                            else
                            {
                                // Hide window here
                                KillTimer(hwndDlg, 1);
                                _isAnimating = false;

								SendMessage(hwndDlg, WM_NODESELECTED, NULL, (LPARAM)_pSelected);
								SetEvent(p_this->hFinishedEvent);

                                return 1;
                            }
                        }
                        break;
                    case 3 :
                        {
                            // We are scrolling here
                            if (_iDistanceNeeded == 0)
                            {
                                _iDecel = 0;
                                KillTimer(hwndDlg, 1);
                                _isAnimating = false;
                                p_this->UpdateFrameScroll(g_pPluginData, _ddTiles, _pDrawThumbnail, _hdcThumb, hwndDlg, 0, 0, _x, _y, _pStart, &_pStartNext, _iIndex, &_pUnderMouse, &_pClickAction, &_iDistance, &_iMouseDistance, true);

                                //p_this->ResetAutoClose(hwndDlg);
                            }
                            else
                            {
                                double decelRate = _decelRate;

                                if (bSkipFrame)
                                {
                                    //decelRate = _decelRate * _decelRate;
                                }
								if (abs(_iDistanceNeeded) < (p_this->g_bigThumbWidth / 2))
								{
									decelRate = decelRate * .8;
								}
                                _iDecel += (_iDistanceNeeded - (int)(_iDistanceNeeded * decelRate));
                                // todo add logic here

                                bool bFinalStop = (abs(_iDistanceNeeded) < p_this->g_bigThumbWidth / 2) || (abs(_iDecel) < p_this->g_bigThumbWidth / 2) || (decelRate == 0);

                                if (bFinalStop)
                                {
                                    Sleep(0);
                                }

                                p_this->UpdateFrameScroll(g_pPluginData, _ddTiles, _pDrawThumbnail, _hdcThumb, hwndDlg, _iDecel, 0, _x, _y, _pStart, &_pStartNext, _iIndex, &_pUnderMouse, &_pClickAction, &_iDistance, &_iMouseDistance, bFinalStop);
                                _iDistanceNeeded = (int)(_iDistanceNeeded * decelRate);

                                if (_iDistanceNeeded == 0 && _pStartNext)
                                {
                                    _pStart = _pStartFinal;
									_pStartNext = _pStart;
                                }
                            }
                        }
                    }
                    
                    p_this->DrawUpdatedFrame(hwndDlg);
                }
            }
            return 1;
        case WM_ERASEBKGND:
            {
            }
            return 1;
    }
    return DefWindowProc(hwndDlg, message, wParam, lParam);
}

void CFlipper::DrawBackground(int transparentWidth)
{
    DDBLTFX ddbltfx;
    memset(&ddbltfx, 0, sizeof(ddbltfx));
    ddbltfx.dwSize = sizeof(ddbltfx);
    ddbltfx.dwROP = SRCCOPY;
	RECT clientRect, windowRect;

	GetWindowRect(g_hwndAltTab, &windowRect);
	clientRect.top = clientRect.left = 0;
	clientRect.right = windowRect.right - windowRect.left - transparentWidth;
	clientRect.bottom = windowRect.bottom - windowRect.top;

	if (g_dwTabAlign == 1)
	{
		clientRect.left = transparentWidth;
		clientRect.right = windowRect.right;
	}

    g_pDDSBuffer->Blt(&clientRect, g_pDDSBackground, &clientRect, DDBLT_ROP, &ddbltfx);
	
	if (g_dwTabAlign == 1)
	{
		clientRect.right = clientRect.left;
		clientRect.left = 0;
	}
	else
	{
		clientRect.left = clientRect.right;
		clientRect.right += transparentWidth;
	}

    g_pDDSBuffer->Blt(&clientRect, g_pDDSBackClear, &clientRect, DDBLT_ROP, &ddbltfx);
}

void CFlipper::DrawBackground(HDC hdcDest, RECT* pRect)
{
    DDBLTFX ddbltfx;
    memset(&ddbltfx, 0, sizeof(ddbltfx));
    ddbltfx.dwSize = sizeof(ddbltfx);
    ddbltfx.dwROP = SRCCOPY;
	
	HDC hdcSrc;
	g_pDDSBackground->GetDC(&hdcSrc);
	BitBlt(hdcDest, pRect->left, pRect->top, pRect->right, pRect->bottom, hdcSrc, pRect->left, pRect->top, SRCCOPY);
	g_pDDSBackground->ReleaseDC(hdcSrc);
	Sleep(0);
}

void CFlipper::UpdateFrameAnimateIn(int iAnimateIterations, int iIteration)
{
	DrawBackground((int)((g_width - g_dwTabWidth) * (1.0 * (iAnimateIterations - iIteration) / iAnimateIterations)));
}

void CFlipper::UpdateFrameAnimateOut(HDC hdcThumb, int iAnimateIterations, int iIteration)
{
	DrawBackground((int)((g_width - g_dwTabWidth) * (1.0 * (iAnimateIterations - iIteration) / iAnimateIterations)));
}

void CFlipper::UpdateLoadingScreen(WCHAR* wzText)
{
	DrawBackground(0);

	int _width = (g_width - g_dwTabWidth);
	int _height = (g_windowHeight);

	RECT rect;
	rect.left = rect.top = 0;
	rect.bottom = _height;
	rect.right = _width ;

	HDC hdc;
	g_pDDSBuffer->GetDC(&hdc);

	LOGFONT lf;
	ZeroMemory(&lf, sizeof(lf));
	lf.lfHeight = -(_height / 6);
	lf.lfWeight = FW_BOLD;
	lf.lfCharSet = DEFAULT_CHARSET;
	HFONT hFont = CreateFontIndirect(&lf);

	SetBkMode(hdc, TRANSPARENT);
	SetBkColor(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(0xff,0xff,0xff));
	HGDIOBJ hOldFont = SelectObject(hdc, hFont);

	DrawText(hdc, wzText, wcslen(wzText), &rect, DT_VCENTER | DT_CENTER | DT_CALCRECT);

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

	BLENDFUNCTION blendFunc;
	blendFunc.AlphaFormat = 0;
	blendFunc.BlendFlags = 0;
	blendFunc.BlendOp = AC_SRC_OVER;
	blendFunc.SourceConstantAlpha = 100;
	AlphaBlend(hdc, 0, rectFill.top, _width, rectFill.bottom - rectFill.top, hdcBlack, 0, 0, rect.right - rect.left, rect.bottom - rect.top, blendFunc);
	
	SelectObject(hdcBlack, hObjOld);
	DeleteObject(hbmpBlack);
	DeleteDC(hdcBlack);

	DrawText(hdc, wzText, wcslen(wzText), &rectFill, DT_VCENTER | DT_CENTER);

	SelectObject(hdc, hOldFont);
	DeleteObject(hFont);

	g_pDDSBuffer->ReleaseDC(hdc);
}

void CFlipper::CreateTileBuffer(FlipperPlugin* pPlugin, PFN_DrawThumbnail pfnDrawThumbnail, HDC hdc, HDC hdcThumb, HWND hWnd, node* pHead)
{
	DrawBackground(0);

    int bigLeft = (g_width - g_bigThumbWidth) / 2;
    int bigRight = bigLeft + g_bigThumbWidth;
    int bigMid = 5 + g_bigThumbWidth / 2;

    node* pNodeCurr = pHead;
	node* pNodeActive = pHead;
	bool bDrawLabels = false;

    for (int i = 0; i < 100; ++i)
    {
        int xMid = bigMid + (g_bigThumbWidth * i);

		int x = xMid;
		int y = g_bigThumbMid + g_labelSpacing + g_labelHeight;

		int anWidth = g_bigThumbWidth;
		int anHeight = g_bigThumbHeight;

		// this will animate the list sliding from side to side

		RECT rect;
		rect.left = x - anWidth/2;
		rect.top = y - anHeight/2;
		rect.right = rect.left + anWidth;
		rect.bottom = rect.top + anHeight;

		RECT rectCoord;
		memcpy(&rectCoord, &rect, sizeof(RECT));

		rectCoord.top = g_labelHeight + 5;
		rectCoord.bottom = g_windowHeight - g_labelHeight - 5;

		if (rect.right > 0)
		{
			RECT displayRect;
			displayRect.left = rect.left;
			displayRect.top = rect.top;
			displayRect.right = rect.left + anWidth;
			displayRect.bottom = rect.top + anHeight;

			if (!pfnDrawThumbnail(pPlugin, pNodeCurr, hdc, hdcThumb, &rect, FALSE, &bDrawLabels))
			{
				DrawThumbnail(pPlugin, pNodeCurr, hdc, hdcThumb, &rect, FALSE, &bDrawLabels);
			}
		}
		pNodeCurr = pNodeCurr->next;

		if (!pNodeCurr)
		{
			break;
		}
	}
}

void CFlipper::UpdateFrameScroll(FlipperPlugin* pPlugin, LPDIRECTDRAWSURFACE pBuffer, PFN_DrawThumbnail pfnDrawThumbnail, HDC hdcThumb, HWND hWnd, int xDiff, int yDiff, int xPos, int yPos, node* pHead, node** ppNewHead, int iActive, node** pUnderMouse, PFN_ClickAction* pClickAction, int* piDistanceFromCenter, int* piMouseFromCenter, bool bFinalStop)
{
	DrawBackground(0);

	if (!pHead)
	{
		return;
	}

	if (pBuffer)
	{
		DDSURFACEDESC ddDesc;
		ddDesc.dwSize = sizeof(ddDesc);
		ddDesc.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
		pBuffer->GetSurfaceDesc(&ddDesc);

		int xDiffMod = xDiff - (pHead->iIndex * g_bigThumbWidth);

		DDBLTFX ddbltfx;
		memset(&ddbltfx, 0, sizeof(ddbltfx));
		ddbltfx.dwSize = sizeof(ddbltfx);
		ddbltfx.dwROP = SRCCOPY;

		RECT destRect = {0}, srcRect = {0};
		destRect.left = destRect.top = 0;
		destRect.right = g_width-g_dwTabWidth;
		destRect.bottom = g_windowHeight;

		memcpy(&srcRect, &destRect, sizeof(srcRect));

		if (xDiffMod < 0)
		{
			Sleep(0);
			srcRect.left -= xDiffMod;
			srcRect.right -= xDiffMod;
		}
		else if (xDiffMod > 0)
		{
			srcRect.left -= xDiffMod;
			srcRect.right -= xDiffMod;
		}

		if (srcRect.left < 0)
		{
			int diff = -srcRect.left;
			srcRect.left = 0;
			destRect.left += diff;
		}
		if ((DWORD)srcRect.right > ddDesc.dwWidth)
		{
			int diff = srcRect.right - ddDesc.dwWidth;
			srcRect.right -= diff;
			destRect.right -= diff;
		}

		g_pDDSBuffer->Blt(&destRect, pBuffer, &srcRect, DDBLT_KEYSRC, &ddbltfx);
	}
	// Draw the selected node here...

    *pUnderMouse = NULL;
    *pClickAction = NULL;

    int bigLeft = (g_width - g_bigThumbWidth) / 2;
    int bigRight = bigLeft + g_bigThumbWidth;
    int bigMid = 5 + g_bigThumbWidth / 2;

    HDC hdc;
    g_pDDSBuffer->GetDC(&hdc);
    RECT activeRect;
    bool bHasActive = false;
	bool bDrawLabels = true;
    double percentAway = 1.0;

    *piDistanceFromCenter = 500;

	int iDir = 1;
	if (pHead->iIndex > iActive)
	{
		iDir = -1;
	}

    node* pNodeCurr = pHead;
	node* pNodeActive = pHead;
    for (int iDir = -1; iDir < 2; iDir+=2)
    {
        node* pNodeCurr = pHead;
        for (int i = 0; i < 20; ++i)
        {
            if (iDir > 0 && i == 0)
            {
                //
            }
			else
			{
                int xMid = bigMid + (g_bigThumbWidth * iDir * i);

				int x = xMid + xDiff;
				int y = g_bigThumbMid + g_labelSpacing + g_labelHeight;

				int anWidth = g_bigThumbWidth;
				int anHeight = g_bigThumbHeight;

				if (abs(x) < abs(*piDistanceFromCenter))
				{
					*piDistanceFromCenter = -xDiff;

					if (ppNewHead)
					{
						*ppNewHead = pNodeCurr;
					}
				}

				// this will animate the list sliding from side to side
				RECT rect;
				rect.left = x - anWidth/2;
				rect.top = y - anHeight/2;
				rect.right = rect.left + anWidth;
				rect.bottom = rect.top + anHeight;

				RECT rectCoord;
				memcpy(&rectCoord, &rect, sizeof(RECT));

				rectCoord.top = g_labelHeight + 5;
				rectCoord.bottom = g_windowHeight - g_labelHeight - 5;

				if (RectContains(&rectCoord, xPos, yPos))
				{
					*pUnderMouse = pNodeCurr;
					*piMouseFromCenter = (bigMid - x);
				}

				if (pNodeCurr->iIndex == iActive && !bHasActive)
				{
					bHasActive = true;
					memcpy(&activeRect, &rect, sizeof(RECT));
					pNodeActive = pNodeCurr;

					RECT displayRect;
					displayRect.left = rect.left;
					displayRect.top = rect.top;
					displayRect.right = rect.left + anWidth;
					displayRect.bottom = rect.top + anHeight;

					if (!pfnDrawThumbnail(pPlugin, pNodeCurr, hdc, hdcThumb, &rect, (pNodeCurr->iIndex == iActive), &bDrawLabels)) //dwBrush, (byte)dwOpaque, true))
					{
						DrawThumbnail(pPlugin, pNodeCurr, hdc, hdcThumb, &rect, (pNodeCurr->iIndex == iActive), &bDrawLabels);
					}
				}
            }
            pNodeCurr = (iDir < 0) ? pNodeCurr->prev : pNodeCurr->next;

			if (!pNodeCurr)
			{
				break;
			}
        }
    }

	if (bDrawLabels)
	{
		int labelYMid = (g_labelSpacing) + (g_labelHeight) / 2;
		g_labelWidth = (g_width-g_dwTabWidth) - 10;
		int labelXMid = (g_width - g_dwTabWidth) / 2;
		int labelYHalf = (g_labelHeight) / 2;
		int labelXHalf = (g_labelWidth) / 2;

		RECT rect, rectBottom;
		rect.left = 5;
		rect.top = g_labelTop;
		rect.right = rect.left + g_labelWidth;
		rect.bottom = g_labelTop + g_labelHeight;

		memcpy(&rectBottom, &rect, sizeof(RECT));
		rectBottom.top = g_windowHeight - g_labelHeight - 5;
		rectBottom.bottom = rectBottom.top + g_labelHeight;

		COLORREF rgb = RGB(0xE5, 0xEA, 0xFF);
		HBRUSH hBrush = CreateSolidBrush(rgb);
		HGDIOBJ hOldBrush = SelectObject(hdc, hBrush);
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
		Rectangle(hdc, rectBottom.left, rectBottom.top, rectBottom.right, rectBottom.bottom);
		SelectObject(hdc, hOldBrush);
		DeleteObject(hBrush);

		LOGFONT lf;
		ZeroMemory(&lf, sizeof(lf));
		lf.lfHeight = -(g_labelFont);
		lf.lfWeight = FW_BOLD;
		lf.lfCharSet = DEFAULT_CHARSET;
		HFONT hFont = CreateFontIndirect(&lf);

		SetBkMode(hdc, TRANSPARENT);
		SetBkColor(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(0,0,0));
		HGDIOBJ hOldFont = SelectObject(hdc, hFont);

		DrawText(hdc, pNodeActive->wzText, wcslen(pNodeActive->wzText), &rect, DT_VCENTER | DT_CENTER | DT_END_ELLIPSIS);
		DrawText(hdc, pNodeActive->wzDetails, wcslen(pNodeActive->wzDetails), &rectBottom, DT_VCENTER | DT_CENTER | DT_END_ELLIPSIS);

		SelectObject(hdc, hOldFont);
		DeleteObject(hFont);
	}

    g_pDDSBuffer->ReleaseDC(hdc);
}