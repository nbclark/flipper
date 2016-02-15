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
#include "FlipperLoader.h"
#include "resource.h"

// Modes
// 0 = disabled
// 1 = scroll only
// 2 = keydown only
// 3 = smart mode


int _tmain(int argc, _TCHAR* argv[])
{
	/*
    CComPtr<IXMLDOMDocument> spXMLDoc;
    XMLElementPtr spRootElem;
    CComBSTR bstr( L"" );
    VARIANT_BOOL vbLoadResult = VARIANT_FALSE;
	spXMLDoc.CoCreateInstance(CLSID_DOMDocument);
	spXMLDoc->loadXML(bstr, &vbLoadResult);

	CComBSTR bstrNode(L"");
	XMLNodeListPtr spNodeList;
	spXMLDoc->selectNodes(bstrNode spNodeList);
*/
	
    HANDLE hEvent = CreateEvent(NULL, FALSE, TRUE, EVENT_APPSWITCH_MUTEX);

    if (!hEvent)
    {
        return -1;
    }
    if (ERROR_ALREADY_EXISTS == GetLastError())
    {
        // the event already exists
        CloseHandle(hEvent);

		HWND hwndFlipper = FindWindow(L"flipperTabs", NULL);

		if (hwndFlipper)
		{
			PostMessage(hwndFlipper, WM_SHOWTABS, 0, 0);
		}

        return -2;
    }

	CoInitializeEx(NULL, COINIT_MULTITHREADED);


	DWORD w = sizeof(FlipperPlugin);
	HANDLE hEventExit = CreateEvent(NULL, TRUE, FALSE, EVENT_FLIPPEREXIT);
    /*
    // The beta won't check for keys but will expire after 30 days or something
    SYSTEMTIME curTime;
    GetSystemTime(&curTime);

    // May 15
    if (((curTime.wMonth > 4) && (curTime.wDay > 14)) || (curTime.wYear > 2009))
    {
        MessageBox(NULL, L"The Flipper beta period has expired. Visit http://www.mobilesrc.com/ for more information.", L"Flipper Beta Expired", MB_OK | MB_ICONWARNING);
        return 0;
    }
    //BOOL bSuccess = CeSetThreadPriority(GetCurrentThread(), 100);
	*/

    MSG msg;
	
	CFlipper* pFlipperApp = new CFlipper();
    pFlipperApp->Initialize();
	//pFlipperApp->ShowTab(0);

    while ( GetMessage( &msg, NULL, 0, 0 ) )
    {
		if (msg.message == WM_SHOWTABS)
		{
			int index = (int)msg.lParam;
			pFlipperApp->ShowTab(index);
			Sleep(0);
		}
		else if (msg.message == WM_SETTINGSCHANGED)
		{
			delete pFlipperApp;
			pFlipperApp = new CFlipper();
			pFlipperApp->Initialize();
		}
		else
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
			if (GetForegroundWindow() != pFlipperApp->g_hwndAltTab)
			{
				//ShowWindow(g_FlipperApp.g_hwndAltTab, SW_HIDE);
				//SendMessage(g_FlipperApp.g_hwndAltTab, WM_HIDEFLIPPER, 0, 0);
				//SetFocus(NULL);
			}
		}
    }

	CloseHandle(hEventExit);
	CloseHandle(hEvent);

	delete pFlipperApp;

	CoUninitialize();

    return 0;
}