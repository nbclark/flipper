#include <aygshell.h>
#include <regext.h>
#include <shellapi.h>
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>   // includes the common control header
#include <string.h>   // includes the common control header
#include <Tlhelp32.h>
#include "stdafx.h"
#include "DeviceResolutionAware.h"
#include "FlipperConfig.h"
#include "..\Flipper\Flipper.h"
#include "..\Flipper\FlipperPlugin.h"
#include "..\Flipper\Utils.h"
#include "..\Flipper\RPN.h"
#include "Anchor.h"
#include "DlgResizer.h"


// Foreground and background colors
DWORD g_fgColor = 0xff0000;
DWORD g_bgColor = 0x00ff00;

// Basic colors in color dialogbox.
DWORD g_crItems[] = {
	RGB(  0,   0,   0),	RGB( 64,   0,   0),	RGB(128,   0,   0),	RGB(128,  64,  64),	RGB(255,   0,   0),	RGB(255, 128, 128),
	RGB(255, 255, 128),	RGB(255, 255,   0),	RGB(255, 128,  64),	RGB(255, 128,   0),	RGB(128,  64,   0),	RGB(128, 128,   0),
	RGB(128, 128,  64),	RGB(  0,  64,   0),	RGB(  0, 128,   0),	RGB(  0, 255,   0),	RGB(128, 255,   0),	RGB(128, 255, 128),
	RGB(  0, 255, 128),	RGB(  0, 255,  64),	RGB(  0, 128, 128),	RGB(  0, 128,  64),	RGB(  0,  64,  64),	RGB(128, 128, 128),
	RGB( 64, 128, 128),	RGB(  0,   0, 128),	RGB(  0,   0, 255),	RGB(  0,  64, 128),	RGB(  0, 255, 255), RGB(128, 255, 255),
	RGB(  0, 128, 255),	RGB(  0, 128, 192),	RGB(128, 128, 255),	RGB(  0,   0, 160),	RGB(  0,   0,  64),	RGB(192, 192, 192),
	RGB( 64,   0,  64),	RGB( 64,   0,  64),	RGB(128,   0, 128),	RGB(128,   0,  64),	RGB(128, 128, 192),	RGB(255, 128, 192),
	RGB(255, 128, 255),	RGB(255,   0, 255), RGB(255,   0, 128),	RGB(128,   0, 255), RGB( 64,   0, 128),	RGB(255, 255, 255),
};

#define NUM_APPLETS        1

typedef struct tagApplets
{
    int icon;           // icon resource identifier
    int namestring;     // name-string resource identifier
    int descstring;     // description-string resource identifier
} APPLETS;

struct PluginSetting
{
	DWORD dwPluginId;
	DWORD dwItemId;
	WCHAR wzName[100];
	COLORREF dwColor;
	HMODULE hAssembly;
	PFN_ShowSettings pfnShowSettings;
};

struct AssemblyPlugin
{
	WCHAR* wzName;
	WCHAR* wzAssembly;
	HMODULE hModule;
	DWORD dwPluginId;
};

const APPLETS SystemApplets[] =
{
    {APPLET_ICON, APPLET_NAME, APPLET_DESC}
    // add more struct members here if supporting more than on applet
};

HINSTANCE     g_hInstance    = NULL;
HIMAGELIST    g_hSmall = NULL;
HICON         g_hIcon = NULL;
list<HMODULE> g_hModules;
list<AssemblyPlugin*> g_wzPlugins;
AssemblyPlugin* g_pSelectedPlugin = NULL;
int			  g_iMaxItemId = 0;

BOOL CreatePropertySheet(HWND hWnd, int iApplet);

////////////////////////////////////////////////////////
//    DllMain
//
////////////////////////////////////////////////////////
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
                     )
{
    if (DLL_PROCESS_ATTACH == ul_reason_for_call)
        g_hInstance = (HINSTANCE)hModule;

    return TRUE;
}


////////////////////////////////////////////////////////
//    Do initialization, eg memory allocations, etc.
//  return TRUE if all OK and FALSE if applet shouldn't
//  be loaded
//
////////////////////////////////////////////////////////
BOOL InitApplet(HWND hwndParent)
{
    return TRUE;
}


////////////////////////////////////////////////////////
//    Do cleanup here
//  
////////////////////////////////////////////////////////
void TermApplet()
{
    return;
}

////////////////////////////////////////////////////////
//    This is the entry point called by ctlpnl.exe
//  
////////////////////////////////////////////////////////
extern "C"
__declspec(dllexport)
LONG WINAPI CPlApplet(HWND hwndCPL, UINT uMsg, LONG lParam1, LONG lParam2)
{
    static int        iInitCount = 0;
    int                iApplet;
    static DWORD    dwThread = 0;

    switch (uMsg)
    {
        // First message sent. It is sent only once to
        // allow the dll to initialize it's applet(s)
        case CPL_INIT:
            if (!iInitCount)
            {
                if (!InitApplet(hwndCPL))
                    return FALSE;
            }
            iInitCount++;
            return TRUE;
            
        // Second message sent. Return the count of applets supported
        // by this dll
        case CPL_GETCOUNT:
            // Return the number of applets we support
            return (LONG)((sizeof(SystemApplets))/(sizeof(SystemApplets[0])));

        // Third message sent. Sent once for each applet supported by this dll.
        // The lParam1 contains the number that indicates which applet this is
        // for, from 0 to 1 less than the count of applets.
        // lParam2 is a NEWCPLINFO that should be filled with information about
        // this applet before returning
        case CPL_NEWINQUIRE:
            {
                LPNEWCPLINFO    lpNewCPlInfo;

                lpNewCPlInfo                = (LPNEWCPLINFO)lParam2;
                iApplet                        = (int)lParam1;
                lpNewCPlInfo->dwSize        = (DWORD)sizeof(NEWCPLINFO);
                lpNewCPlInfo->dwFlags        = 0;
                lpNewCPlInfo->dwHelpContext    = 0;
                lpNewCPlInfo->lData            = SystemApplets[iApplet].icon;
                lpNewCPlInfo->hIcon            = LoadIcon(g_hInstance,(LPCTSTR)MAKEINTRESOURCE(SystemApplets[iApplet].icon));
                lpNewCPlInfo->szHelpFile[0]    = '\0';

                LoadString(g_hInstance,SystemApplets[iApplet].namestring,lpNewCPlInfo->szName,32);
                LoadString(g_hInstance,SystemApplets[iApplet].descstring,lpNewCPlInfo->szInfo,64);
            }

            break;

        // This is sent whenever the user clicks an icon in Settings for one of
        // the applets supported by this dll. lParam1 contains the number indicating
        // which applet. Return 0 if applet successfully launched, non-zero otherwise
        case CPL_DBLCLK:
            iApplet = (UINT)lParam1;
            if (!CreatePropertySheet(hwndCPL,iApplet))
                return 1;
            break;
            
        // Sent once per applet, before CPL_EXIT
        case CPL_STOP:
            break;
            
        // Sent once before the dll is unloaded
        case CPL_EXIT:
            iInitCount--;
            if (!iInitCount)
                TermApplet();
            break;
            
        default:
            break;
    }

    return 0;
}



BOOL CALLBACK RegistrationProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{
    static int iRetryAttempts = 0;
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
			g_dlgAnchor.Add(IDC_STATIC_OWNER, ANCHOR_LEFT | ANCHOR_TOP);
			g_dlgAnchor.Add(IDC_STATIC_CODE, ANCHOR_LEFT | ANCHOR_TOP);
			g_dlgAnchor.Add(IDC_KEY, ANCHOR_LEFT | ANCHOR_TOP | ANCHOR_RIGHT);
			g_dlgAnchor.Add(IDC_PPC_ID, ANCHOR_LEFT | ANCHOR_TOP | ANCHOR_RIGHT);
			g_dlgAnchor.Add(IDC_REGISTER_BUTTON, ANCHOR_LEFT | ANCHOR_TOP | ANCHOR_RIGHT);
			g_dlgAnchor.Add(IDC_DEMO_BUTTON, ANCHOR_LEFT | ANCHOR_TOP | ANCHOR_RIGHT);

            SHMENUBARINFO mbi;
            memset(&mbi, 0, sizeof(SHMENUBARINFO));
            mbi.cbSize = sizeof(SHMENUBARINFO);
            mbi.hwndParent = hwndDlg;
            mbi.nToolBarId = 0;
            mbi.hInstRes = g_hInstance;
            mbi.nBmpId = 0;
            mbi.cBmpImages = 0;    

            SHCreateMenuBar(&mbi);

            SHINITDLGINFO shidi;
            // Create a Done button and size it.
            shidi.dwMask = SHIDIM_FLAGS;
            shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN;
            shidi.hDlg = hwndDlg;
            SHInitDialog(&shidi);

            WCHAR wzOwner[255];
            GetFromRegistry(HKEY_CURRENT_USER, L"ControlPanel\\Owner", L"Name", wzOwner);

            SetDlgItemText(hwndDlg, IDC_PPC_ID, wzOwner);

            return 0;
        }
        break;
        case WM_COMMAND: 
            switch (LOWORD(wParam)) 
            {
                case IDOK:
                    {
                        EndDialog(hwndDlg, IDCANCEL);
                    }
                    break;
                case IDC_REGISTER_BUTTON :
                    {
                        WCHAR wzCustomer[255];
                        GetDlgItemText(hwndDlg, IDC_PPC_ID, wzCustomer, 255);

                        WCHAR wzKey[10];
                        GetDlgItemText(hwndDlg, IDC_KEY, wzKey, 10);
                        // Validate here
                        UINT uiVal = EvaluateRPN(wzCustomer, RPN_CVALUE, RPN_VARIANT);
                        UINT uiRpn = _wtoi(wzKey);

                        if (uiVal == uiRpn)
                        {
                            SetToRegistry(REG_RegstrationCode, uiRpn);
                            // registration complete - write to registry
                            MessageBox(hwndDlg, L"You have successfully activated Flipper.", L"Activation Successful", MB_OK | MB_ICONINFORMATION);
                            EndDialog(hwndDlg, IDOK);
                        }
                        else
                        {
                            MessageBox(hwndDlg, L"You have entered an invalid activation code.", L"Activation Failed", MB_OK | MB_ICONERROR);
                            SetDlgItemText(hwndDlg, IDC_KEY, L"");

                            iRetryAttempts++;

                            if (iRetryAttempts > 5)
                            {
                                EndDialog(hwndDlg, IDABORT);
                            }

                        }
                        Sleep(0);
                    }
                    break;
                case IDC_DEMO_BUTTON :
                    {
                        EndDialog(hwndDlg, IDCANCEL);
                    }
                    break;
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


int CALLBACK AppearanceSettingsPageProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
    LPPROPSHEETPAGE ppsp;
	static CDlgAnchor g_dlgAnchor;

    switch (uMsg)
    {
		case WM_SIZE :
		{
			g_dlgAnchor.OnSize();
		}
		break;
        case WM_INITDIALOG:
            {
				g_dlgAnchor.Init(hwndDlg);
				g_dlgAnchor.Add(IDC_CHECK_AUTOHIDE, ANCHOR_LEFT | ANCHOR_TOP | ANCHOR_RIGHT);
				g_dlgAnchor.Add(IDC_CHECK_ENABLEANIMATIONS, ANCHOR_LEFT | ANCHOR_TOP | ANCHOR_RIGHT);
				g_dlgAnchor.Add(IDC_STATIC_ALIGN, ANCHOR_LEFT | ANCHOR_TOP);
				g_dlgAnchor.Add(IDC_COMBO_ALIGNMENT, ANCHOR_LEFT | ANCHOR_TOP | ANCHOR_RIGHT);
				g_dlgAnchor.Add(IDC_COMBO_SIZE, ANCHOR_LEFT | ANCHOR_TOP | ANCHOR_RIGHT);
				g_dlgAnchor.Add(IDC_CHECK_DISABLE, ANCHOR_LEFT | ANCHOR_BOTTOM | ANCHOR_RIGHT);

                SHMENUBARINFO mbi;
                memset(&mbi, 0, sizeof(SHMENUBARINFO));
                mbi.cbSize = sizeof(SHMENUBARINFO);
                mbi.hwndParent = hwndDlg;
                mbi.nToolBarId = CheckIfRegistered(REG_RegstrationCode, RPN_CVALUE, RPN_VARIANT) ? 0 : IDR_Flipper_Main_MENUBAR;
                mbi.hInstRes = g_hInstance;
                mbi.nBmpId = 0;
                mbi.cBmpImages = 0;    

                SHCreateMenuBar(&mbi);

                // lParam contains the PROPSHEETPAGE struct; store the HWND into the dword at the end of the struct
                ppsp = (LPPROPSHEETPAGE)lParam;
                *((HWND*)(ppsp + 1)) = hwndDlg;

				// Set Checks here
				DWORD dwAlign = 0, dwSize = 1, dwDisabled = 0, dwAutoHide = 1, dwAnimate = 1, dwDisp;

				HKEY hKey = NULL;

				if (SUCCEEDED(RegCreateKeyEx(HKEY_CURRENT_USER, REG_FlipperKey, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, &dwDisp)))
				{
					RegistryGetDWORD(hKey, NULL, REG_TabSize, &dwSize);
					RegistryGetDWORD(hKey, NULL, REG_TabAlignment, &dwAlign);
					RegistryGetDWORD(hKey, NULL, REG_Disabled, &dwDisabled);
					RegistryGetDWORD(hKey, NULL, REG_AutoClose, &dwAutoHide);
					RegistryGetDWORD(hKey, NULL, REG_EnableScrollAnimation, &dwAnimate);

					RegCloseKey(hKey);
				}

				HWND hwndAlign = GetDlgItem(hwndDlg, IDC_COMBO_ALIGNMENT);
				HWND hwndSize = GetDlgItem(hwndDlg, IDC_COMBO_SIZE);
				HWND hwndDisabled = GetDlgItem(hwndDlg, IDC_CHECK_DISABLE);
				HWND hwndAutoHide = GetDlgItem(hwndDlg, IDC_CHECK_AUTOHIDE);
				HWND hwndAnimate = GetDlgItem(hwndDlg, IDC_CHECK_ENABLEANIMATIONS);

				CheckDlgButton(hwndDlg, IDC_CHECK_DISABLE, (dwDisabled != 0) ? BST_CHECKED : BST_UNCHECKED);
				CheckDlgButton(hwndDlg, IDC_CHECK_AUTOHIDE, (dwAutoHide != 0) ? BST_CHECKED : BST_UNCHECKED);
				CheckDlgButton(hwndDlg, IDC_CHECK_ENABLEANIMATIONS, (dwAnimate != 0) ? BST_CHECKED : BST_UNCHECKED);

				ComboBox_AddString(hwndAlign, L"Left");
				ComboBox_AddString(hwndAlign, L"Right");
				ComboBox_SetCurSel(hwndAlign, dwAlign);

				ComboBox_AddString(hwndSize, L"Small");
				ComboBox_AddString(hwndSize, L"Medium");
				ComboBox_AddString(hwndSize, L"Large");

				ComboBox_SetCurSel(hwndSize, dwSize);
            }
            return TRUE;
        case WM_DESTROY :
            {
				// Save checks here
				DWORD dwAlign = 0, dwSize = 1, dwDisabled = 0, dwAutoHide = 1, dwAnimate = 1, dwDisp = 0;
				HKEY hKey = NULL;

				HWND hwndAlign = GetDlgItem(hwndDlg, IDC_COMBO_ALIGNMENT);
				HWND hwndSize = GetDlgItem(hwndDlg, IDC_COMBO_SIZE);
				HWND hwndDisabled = GetDlgItem(hwndDlg, IDC_CHECK_DISABLE);
				HWND hwndAutoHide = GetDlgItem(hwndDlg, IDC_CHECK_AUTOHIDE);

				dwDisabled = (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DISABLE) == BST_CHECKED);
				dwAutoHide = (IsDlgButtonChecked(hwndDlg, IDC_CHECK_AUTOHIDE) == BST_CHECKED);
				dwAnimate = (IsDlgButtonChecked(hwndDlg, IDC_CHECK_ENABLEANIMATIONS) == BST_CHECKED);

				dwAlign = ComboBox_GetCurSel(hwndAlign);
				dwSize = ComboBox_GetCurSel(hwndSize);

				if (SUCCEEDED(RegCreateKeyEx(HKEY_CURRENT_USER, REG_FlipperKey, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, &dwDisp)))
				{
					RegistrySetDWORD(hKey, NULL, REG_TabSize, dwSize);
					RegistrySetDWORD(hKey, NULL, REG_TabAlignment, dwAlign);
					RegistrySetDWORD(hKey, NULL, REG_Disabled, dwDisabled);
					RegistrySetDWORD(hKey, NULL, REG_AutoClose, dwAutoHide);
					RegistrySetDWORD(hKey, NULL, REG_EnableScrollAnimation, dwAnimate);

					RegCloseKey(hKey);
				}

				Sleep(0);
            }
			
        case WM_COMMAND: 
			{
				switch (LOWORD(wParam)) 
				{
					case IDOK:
					case IDM_Flipper_SET_ACCEPT :
						{
							EndDialog(hwndDlg, IDOK);
						}
						break;
					case IDM_Flipper_SET_CANCEL :
						{
							SHInitExtraControls();
							int iRegCodeRet = DialogBoxW(g_hInstance, MAKEINTRESOURCE(IDD_REGISTER_DIALOG), hwndDlg, (DLGPROC)RegistrationProc);
						}
						break;
				}
			}
			break;
        case WM_HSCROLL:
            switch LOWORD(wParam) 
            {
                case TB_BOTTOM:
                case TB_THUMBPOSITION:
                case TB_LINEUP:
                case TB_LINEDOWN:
                case TB_PAGEUP:
                case TB_PAGEDOWN:
                case TB_TOP:
                    {
                    }

                    return TRUE;
                default:
                    // Default case
                    return FALSE;
            }
            break;
    }

    return FALSE;
}

void GetSettingsRegistryPath(WCHAR* wzBuffer, DWORD dwLength, int iPluginId)
{
	WCHAR wzNumber[5];
	_itow(iPluginId, wzNumber, 10);

	StringCchCopy(wzBuffer, dwLength, REG_FlipperKey);
	StringCchCat(wzBuffer, dwLength, L"\\");
	StringCchCat(wzBuffer, dwLength, REG_Items_Key);
	StringCchCat(wzBuffer, dwLength, L"\\");
	StringCchCat(wzBuffer, dwLength, wzNumber);
	StringCchCat(wzBuffer, dwLength, L"\\Data");
}

bool CloneRecursively(HKEY hKeyOrig, HKEY hKeyParent, LPCTSTR pszClone) 
{ 
	HKEY hKeyClone = NULL; 
	DWORD dwDisp; 
	_TCHAR szName[MAX_PATH];  // Buffer for registry names. 
	DWORD dwcNameLen = MAX_PATH; // Length of buffer. 
	DWORD dwcSubKeys = 0;   // Number of sub keys. 
	DWORD dwcMaxSubKey;   // Longest sub key size. 
	DWORD dwcMaxClass;   // Longest class string. 
	DWORD dwcValues = 0;   // Number of values for this key. 
	DWORD dwcMaxValueName;  // Longest Value name. 
	DWORD dwcMaxValueData;  // Longest Value data. 
	DWORD dwcSecDesc;    // Security descriptor. 
	FILETIME ftLastWriteTime;  // Last write time. 


	*szName = _T('\0'); 

	if (ERROR_SUCCESS != ::RegQueryInfoKey( 
		hKeyOrig,   // Key handle. 
		szName,    // Buffer for class name. 
		&dwcNameLen,  // Length of class string. 
		NULL,    // Reserved. 
		&dwcSubKeys,  // Number of sub keys. 
		&dwcMaxSubKey,  // Longest sub key size. 
		&dwcMaxClass,  // Longest class string. 
		&dwcValues,   // Number of values for this key. 
		&dwcMaxValueName, // Longest Value name. 
		&dwcMaxValueData, // Longest Value data. 
		&dwcSecDesc,  // Security descriptor. 
		&ftLastWriteTime // Last write time. 
		))
	{
		return false; 
	}

	dwcMaxValueName++; // allow for terminating nulls 
	dwcMaxValueData++; 

	// Clone this key: 
	if (ERROR_SUCCESS != ::RegCreateKeyEx(hKeyParent, pszClone, 0, szName, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL, &hKeyClone, &dwDisp))
	{
		return false;
	}

	// Clone all its values: 
	if (dwcValues)
	{ 
		DWORD dwcValueLen = dwcMaxValueData; 
		DWORD dwIndex = 0; 
		DWORD dwType; 
		LPBYTE pszData = (LPBYTE)GlobalAllocPtr(GMEM_MOVEABLE, dwcValueLen);

		if (pszData == NULL)
		{ 
			::RegCloseKey(hKeyClone); 
			return false; 
		} 

		dwcNameLen = MAX_PATH; 

		while (::RegEnumValue(hKeyOrig, dwIndex++, szName, &dwcNameLen, NULL, &dwType, pszData, &dwcValueLen) == ERROR_SUCCESS) 
		{ 
			::RegSetValueEx(hKeyClone, szName, NULL, dwType, pszData, dwcValueLen); 
			dwcNameLen = MAX_PATH; 
			dwcValueLen = dwcMaxValueData; 
		} 

		GlobalFreePtr(pszData); 
	} 


	// Clone all its subkeys: 
	while (dwcSubKeys)
	{ 
		dwcNameLen = MAX_PATH; 

		if (::RegEnumKeyEx(hKeyOrig, --dwcSubKeys, szName, &dwcNameLen, NULL, NULL, NULL, &ftLastWriteTime) == ERROR_SUCCESS)
		{ 
			HKEY hSubKey; 

			if (::RegOpenKeyEx(hKeyOrig, szName, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS)
			{ 
				CloneRecursively(hSubKey, hKeyClone, szName); 
				::RegCloseKey(hSubKey); 
			} 
		} 
	} 

	::RegCloseKey(hKeyClone); 

	return true; 
} 


///////////////////////////////////////////////////////////////////////////­// 
// DeleteRecursively: 
// Recursively delete all subtrees: 

bool DeleteRecursively(HKEY hKeyParent, LPCTSTR pszSubKey) 
{ 
	HKEY hKey; 

	if (RegOpenKeyEx(hKeyParent, pszSubKey, 0, KEY_READ|KEY_WRITE, &hKey) != ERROR_SUCCESS)
	{
		return true;
	}

	TCHAR  szName[MAX_PATH];   // Buffer for name. 
	DWORD  dwNameLen = MAX_PATH;  // Length of string. 
	FILETIME ftLastWriteTime;   // Last write time. 

	int i = 0;
	while (::RegEnumKeyEx(hKey, i, szName, &dwNameLen, NULL, NULL, NULL, &ftLastWriteTime) == ERROR_SUCCESS)
	{ 
		// Delete subkeys at the next level: 
		if (::RegDeleteKey(hKey, szName) != ERROR_SUCCESS)
		{
			DeleteRecursively(hKey, szName);

			HKEY hKeyTemp;
			if (ERROR_SUCCESS == RegOpenKeyEx(hKey, szName, 0, KEY_READ, &hKeyTemp))
			{
				i++;
				RegCloseKey(hKeyTemp);
			}
		}

		dwNameLen = MAX_PATH; 
	} 

	// Finally, delete this subkey: 
	::RegCloseKey(hKey); 

	return ::RegDeleteKey(hKeyParent, pszSubKey) == ERROR_SUCCESS; 
} 

BOOL CALLBACK AddPluginProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{
	static HWND hwndList;
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
			g_dlgAnchor.Add(IDC_LIST_PLUGINS, ANCHOR_ALL);

			g_pSelectedPlugin = NULL;
			hwndList = GetDlgItem(hwndDlg, IDC_LIST_PLUGINS);

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
            mbi.hInstRes = g_hInstance;
            mbi.nBmpId = 0;
            mbi.cBmpImages = 0;    

            SHCreateMenuBar(&mbi);

			list<AssemblyPlugin*>::iterator iter = g_wzPlugins.begin();

			int height = ListBox_GetItemHeight(hwndList, 0);
			ListBox_SetItemHeight(hwndList, 0, height * 1.5);
			while (iter != g_wzPlugins.end())
			{
				AssemblyPlugin* pPlugin = (AssemblyPlugin*)*iter;
				int index = ListBox_AddString(hwndList, pPlugin->wzName);
				ListBox_SetItemData(hwndList, index, pPlugin);
				iter++;
			}

            return 0;
        }
        break;
        case WM_COMMAND: 
			{
				switch (LOWORD(wParam)) 
				{
					case IDOK:
					case IDM_Flipper_SET_ACCEPT :
						{
							int index = ListBox_GetCurSel(hwndList);

							if (index >= 0)
							{
								g_pSelectedPlugin = (AssemblyPlugin*)ListBox_GetItemData(hwndList, index);
								EndDialog(hwndDlg, IDOK);
							}
							else
							{
								EndDialog(hwndDlg, IDCANCEL);
							}
						}
						break;
					case IDM_Flipper_SET_CANCEL :
						{
							EndDialog(hwndDlg, IDCANCEL);
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

PluginSetting* AddPlugin(HKEY hKeyItems, HIMAGELIST hImgList, HWND hwndList, HDC hdc, int itemId)
{
	DWORD dwSize = 0;
	DWORD dwType = REG_DWORD;
	HKEY hKeyItem;
	HRESULT hRes = S_OK;
	bool bSucceeded = false;
	WCHAR wzItemName[3];
	_itow(itemId, wzItemName, 10);

	PluginSetting* pSetting = NULL;

	hRes = RegOpenKeyEx(hKeyItems, wzItemName, 0, KEY_READ | KEY_ENUMERATE_SUB_KEYS, &hKeyItem);

	if (hRes == ERROR_SUCCESS)
	{
		WCHAR wzAssembly[MAX_PATH];
		pSetting = new PluginSetting();

		dwSize = ARRAYSIZE(pSetting->wzName);
		dwType = REG_SZ;

		RegQueryValueExW(hKeyItem, REG_Items_Name, 0, &dwType, (LPBYTE)pSetting->wzName, &dwSize);

		dwSize = ARRAYSIZE(wzAssembly);
		dwType = REG_SZ;
		RegQueryValueExW(hKeyItem, REG_Items_Assembly, 0, &dwType, (LPBYTE)wzAssembly, &dwSize);

		DWORD dwPluginId = 0;
		dwType = REG_DWORD;
		dwSize = sizeof(DWORD);
		RegQueryValueExW(hKeyItem, REG_Items_PluginId, 0, &dwType, (LPBYTE)&pSetting->dwPluginId, &dwSize);

		DWORD dwColor = 0;
		dwType = REG_DWORD;
		dwSize = sizeof(DWORD);
		RegQueryValueExW(hKeyItem, REG_Items_Color, 0, &dwType, (LPBYTE)&pSetting->dwColor, &dwSize);

		pSetting->dwItemId = itemId;

		HKEY hKeyData;
		hRes = RegOpenKeyEx(hKeyItem, REG_Items_Data_Key, 0, KEY_READ | KEY_ENUMERATE_SUB_KEYS, &hKeyData);

		WCHAR wzFullAssembly[MAX_PATH];
		StringCchCopy(wzFullAssembly, ARRAYSIZE(wzFullAssembly), L"\\program files\\mobilesrc\\flipper\\plugins\\");
		StringCchCat(wzFullAssembly, ARRAYSIZE(wzFullAssembly), wzAssembly);

		pSetting->hAssembly = LoadLibrary(wzFullAssembly);
		PFN_GetSettingsProc pfnGetSettingsProc = (PFN_GetSettingsProc)GetProcAddress(pSetting->hAssembly, L"GetSettingsProc");
		pSetting->pfnShowSettings = (pfnGetSettingsProc != NULL) ? pfnGetSettingsProc(pSetting->dwPluginId) : NULL;

		LV_ITEM lvItem;
		ZeroMemory(&lvItem, sizeof(LV_ITEM));
		lvItem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
		lvItem.state = 0;      
		lvItem.stateMask = 0;
		lvItem.lParam = (LPARAM)pSetting;
		lvItem.cchTextMax = ARRAYSIZE(pSetting->wzName);
		lvItem.pszText = pSetting->wzName;

		HBITMAP hbmp = CreateBitmap(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CXSMICON), 1, 16, NULL);// CreateCompatibleBitmap(hdc, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CXSMICON));
		HGDIOBJ hOld = SelectObject(hdc, hbmp);
		RECT fillRect;
		fillRect.top = fillRect.left = 0;
		fillRect.bottom = fillRect.right = GetSystemMetrics(SM_CXSMICON);
		HBRUSH hBrush = CreateSolidBrush(pSetting->dwColor); //pSetting->dwItemId*85,pSetting->dwItemId*85,0));//COLOR32_TO_565(pSetting->dwColor));
		FillRect(hdc, &fillRect, hBrush);
		DeleteObject(hBrush);

		SelectObject(hdc, hOld);
		
		lvItem.iImage = ImageList_Add(hImgList, hbmp, NULL);
		lvItem.iItem = itemId;

		DeleteObject(hbmp);

		ListView_InsertItem(hwndList, &lvItem);
		RegCloseKey(hKeyItem);
	}
	return pSetting;
}

int CALLBACK ItemsSettingsPageProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
    LPPROPSHEETPAGE ppsp;
    static HWND hwndList;
	static HIMAGELIST g_hImageList;
	static CDlgAnchor g_dlgAnchor;

    switch (uMsg)
    {
		case WM_SIZE :
		{
			g_dlgAnchor.OnSize();
		}
		break;
        case WM_INITDIALOG:
            {
				RECT rc;
				GetWindowRect(hwndDlg,&rc);
				Sleep(0);

				g_dlgAnchor.Init(hwndDlg);
				g_dlgAnchor.Add(IDC_LIST_ITEMS, ANCHOR_ALL);
				g_dlgAnchor.Add(IDC_LIST_COLORS, ANCHOR_LEFT | ANCHOR_RIGHT | ANCHOR_BOTTOM);
				g_dlgAnchor.Add(IDC_BUTTON_ADD, ANCHOR_RIGHT | ANCHOR_BOTTOM);
				g_dlgAnchor.Add(IDC_BUTTON_REMOVE, ANCHOR_RIGHT | ANCHOR_TOP);
				g_dlgAnchor.Add(IDC_BUTTON_MOVEDOWN, ANCHOR_RIGHT | ANCHOR_TOP);
				g_dlgAnchor.Add(IDC_BUTTON_MOVEUP, ANCHOR_RIGHT | ANCHOR_TOP);
				g_dlgAnchor.Add(IDC_BUTTON_OPTIONS, ANCHOR_RIGHT | ANCHOR_TOP);

                SetCursor(LoadCursor(NULL, IDC_WAIT));
				hwndList = GetDlgItem(hwndDlg, IDC_LIST_ITEMS);

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
				mbi.nToolBarId = CheckIfRegistered(REG_RegstrationCode, RPN_CVALUE, RPN_VARIANT) ? 0 : IDR_Flipper_Main_MENUBAR;
                mbi.hInstRes = g_hInstance;
                mbi.nBmpId = 0;
                mbi.cBmpImages = 0;    

                SHCreateMenuBar(&mbi);

                // lParam contains the PROPSHEETPAGE struct; store the HWND into the dword at the end of the struct
                ppsp = (LPPROPSHEETPAGE)lParam;

				if (ppsp)
				{
					*((HWND*)(ppsp + 1)) = hwndDlg;
				}

				SetWindowLong(hwndList, GWL_STYLE, WS_BORDER | WS_VSCROLL | LVS_NOCOLUMNHEADER | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS);
				SetWindowLong(hwndList, GWL_EXSTYLE, LVS_EX_NOHSCROLL | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
				
				int nColor;
				for (nColor = 0; nColor < sizeof(g_crItems)/sizeof(DWORD); nColor++)
				{
					SendDlgItemMessage(hwndDlg, IDC_LIST_COLORS, LB_ADDSTRING, nColor, (LPARAM) "");
					SendDlgItemMessage(hwndDlg, IDC_LIST_COLORS, LB_SETITEMDATA , nColor, (LPARAM) g_crItems[nColor]);
					//SendDlgItemMessage(hwndDlg, IDC_LIST_COLORS, LB_ADDSTRING, 0, (LPARAM) g_crItems[nColor]);
					if (g_fgColor == g_crItems[nColor])
					{
						SendDlgItemMessage(hwndDlg, IDC_LIST_COLORS, LB_SETCURSEL, nColor, 0);
					}
				}

				int index = ListBox_GetCount(GetDlgItem(hwndDlg, IDC_LIST_COLORS));

				RECT rect;
				GetWindowRect(hwndList, &rect);
				
				LV_COLUMN lvC;
				lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
				lvC.fmt = LVCFMT_LEFT;  // left-align column
				lvC.cx = rect.right - rect.left - 2;
				lvC.pszText = L"Name";

				g_hImageList = ImageList_Create(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CXSMICON), ILC_COLOR, 5, 5);
                ImageList_SetBkColor(g_hImageList, RGB(0x00, 0xFF, 0xFF));

				ListView_InsertColumn(hwndList, 0, &lvC);

				// Set Checks here
				HKEY hKey = NULL;
				DWORD dwType = 0, dwValue = 0, dwSize = 0;
				HRESULT hRes = RegOpenKeyEx(HKEY_CURRENT_USER, REG_FlipperKey, 0, KEY_READ, &hKey);
				if (hRes == ERROR_SUCCESS)
				{
					HKEY hKeyItems;
					hRes = RegOpenKeyEx(hKey, REG_Items_Key, 0, KEY_READ | KEY_ENUMERATE_SUB_KEYS, &hKeyItems);
					if (hRes == ERROR_SUCCESS)
					{
						int i = 0;

						list<int> idList;
						while (true)
						{
							WCHAR wzName[3];
							DWORD dwItemName = 3;
							hRes = RegEnumKeyEx(hKeyItems, i++, wzName, &dwItemName, 0, 0, 0, 0);

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

						HDC hdc = CreateCompatibleDC(GetDC(hwndList));
						list<int>::iterator iter = idList.begin();
						while (iter != idList.end())
						{
							AddPlugin(hKeyItems, g_hImageList, hwndList, hdc, *iter);
							g_iMaxItemId = max(*iter, g_iMaxItemId);
							iter++;
						}
						DeleteDC(hdc);
						ListView_SetImageList(hwndList, g_hImageList, LVSIL_SMALL);
					}
				}
                SetCursor(NULL);
			}
            return TRUE;
        case WM_DESTROY :
            {
				// Save checks here
				Sleep(0);

				int iCount = ListView_GetItemCount(hwndList);
				HKEY hKeyFlipper, hKeyItems;
				RegOpenKeyEx(HKEY_CURRENT_USER, REG_FlipperKey, 0, KEY_READ, &hKeyFlipper);
				RegOpenKeyEx(hKeyFlipper, REG_Items_Key, 0, KEY_READ, &hKeyItems);

				for (int i = 0; i < iCount; ++i)
				{
					WCHAR wzNewItemPrefix[6], wzNewItem[5], wzOldItem[5];
					LVITEM lvItem;
					ZeroMemory(&lvItem, sizeof(LVITEM));
					lvItem.mask = LVIF_PARAM;
					lvItem.iItem = i;

					ListView_GetItem(hwndList, &lvItem);
					PluginSetting* pSetting = (PluginSetting*)lvItem.lParam;

					_itow(i, wzNewItem, 10);
					_itow(pSetting->dwItemId, wzOldItem, 10);

					HKEY hKeyItem;
					RegOpenKeyEx(hKeyItems, wzOldItem, 0, KEY_ALL_ACCESS, &hKeyItem);

					StringCchCopy(wzNewItemPrefix, ARRAYSIZE(wzNewItemPrefix), L"_");
					StringCchCat(wzNewItemPrefix, ARRAYSIZE(wzNewItemPrefix), wzNewItem);

					// Save the color here
					HRESULT hr2 = RegistrySetDWORD(hKeyItem, NULL, REG_Items_Color, pSetting->dwColor);

					// Clone here
					CloneRecursively(hKeyItem, hKeyItems, wzNewItemPrefix);

					// Close & delete
					RegCloseKey(hKeyItem);

					DeleteRecursively(hKeyItems, wzOldItem);
				}

				for (int i = 0; i < iCount; ++i)
				{
					WCHAR wzNewItemPrefix[6], wzNewItem[5];
					LVITEM lvItem;
					ZeroMemory(&lvItem, sizeof(LVITEM));
					lvItem.mask = LVIF_PARAM;
					lvItem.iItem = i;

					ListView_GetItem(hwndList, &lvItem);
					PluginSetting* pSetting = (PluginSetting*)lvItem.lParam;

					_itow(i, wzNewItem, 10);

					StringCchCopy(wzNewItemPrefix, ARRAYSIZE(wzNewItemPrefix), L"_");
					StringCchCat(wzNewItemPrefix, ARRAYSIZE(wzNewItemPrefix), wzNewItem);

					HKEY hKeyItem;
					RegOpenKeyEx(hKeyItems, wzNewItemPrefix, 0, KEY_ALL_ACCESS, &hKeyItem);

					CloneRecursively(hKeyItem, hKeyItems, wzNewItem);
					RegCloseKey(hKeyItem);

					DeleteRecursively(hKeyItems, wzNewItemPrefix);

					delete pSetting;
				}
				RegCloseKey(hKeyFlipper);
				RegCloseKey(hKeyItems);
            }
			break;
		case WM_NOTIFY :
			{
				switch (wParam)
				{
					case IDC_LIST_ITEMS :
					{
						LPNMLISTVIEW pnmv = (LPNMLISTVIEW) lParam;

						switch (pnmv->hdr.code)
						{
							case LVN_ITEMCHANGED :
							{
								bool bHasSettings = false;
								bool bMoveUp = false;
								bool bMoveDown = false;

								if (pnmv->iItem >= 0 && (pnmv->uNewState & LVIS_SELECTED))
								{
									PluginSetting* pSetting = (PluginSetting*)pnmv->lParam;
									bHasSettings = pSetting->pfnShowSettings != 0;
									
									bMoveUp = (pnmv->iItem > 0);
									bMoveDown = (pnmv->iItem < ListView_GetItemCount(hwndList)-1);

									int index = ListBox_FindItemData(GetDlgItem(hwndDlg, IDC_LIST_COLORS), 0, pSetting->dwColor);

									if (index < 0)
									{
										srand( GetTickCount() );
										pSetting->dwColor = g_crItems[rand() % ARRAYSIZE(g_crItems)];
									}
									ListBox_SelectItemData(GetDlgItem(hwndDlg, IDC_LIST_COLORS), 0, pSetting->dwColor);
								}

								int count = ListView_GetSelectedCount(hwndList);

								EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON_OPTIONS), bHasSettings);
								EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON_MOVEUP), bMoveUp);
								EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON_MOVEDOWN), bMoveDown);
								EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON_REMOVE), (count > 0));
								EnableWindow(GetDlgItem(hwndDlg, IDC_LIST_COLORS), (count > 0));

								Sleep(0);
							}
							break;
						}
					}
					break;
				}
			}
			break;
		case WM_COMMAND :
			{
				switch (LOWORD(wParam))
				{
					case IDOK:
					case IDM_Flipper_SET_ACCEPT :
						{
							EndDialog(hwndDlg, 0);
						}
						return 0;
					case IDM_Flipper_SET_CANCEL :
						{
							SHInitExtraControls();
							int iRegCodeRet = DialogBoxW(g_hInstance, MAKEINTRESOURCE(IDD_REGISTER_DIALOG), hwndDlg, (DLGPROC)RegistrationProc);
						}
						return 0;
				}
				if (HIWORD(wParam) == LBN_SELCHANGE)
				{
					// Set the color here --
					int iSel = ListView_GetSelectionMark(hwndList);
					LV_ITEM lvItem;
					ZeroMemory(&lvItem, sizeof(LV_ITEM));
					lvItem.mask = LVIF_IMAGE | LVIF_PARAM;
					lvItem.iItem = iSel;
					ListView_GetItem(hwndList, &lvItem);
					
					PluginSetting* pSetting = (PluginSetting*)lvItem.lParam;

					iSel = ListBox_GetCurSel(GetDlgItem(hwndDlg, IDC_LIST_COLORS));
					COLORREF clr = (COLORREF)ListBox_GetItemData(GetDlgItem(hwndDlg, IDC_LIST_COLORS), iSel);

					if (pSetting->dwColor != clr)
					{
						pSetting->dwColor = clr;

						HDC hdc = CreateCompatibleDC(GetDC(hwndDlg));
						HBITMAP hbmp = CreateBitmap(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CXSMICON), 1, 16, NULL);

						HGDIOBJ hOld = SelectObject(hdc, hbmp);
						RECT fillRect;
						fillRect.top = fillRect.left = 0;
						fillRect.bottom = fillRect.right = GetSystemMetrics(SM_CXSMICON);
						HBRUSH hBrush = CreateSolidBrush(pSetting->dwColor);
						FillRect(hdc, &fillRect, hBrush);
						DeleteObject(hBrush);
						SelectObject(hdc, hOld);
						DeleteDC(hdc);

						ImageList_Replace(g_hImageList, lvItem.iImage, hbmp, NULL);
						DeleteObject(hbmp);

						ListView_RedrawItems(hwndList, lvItem.iItem, lvItem.iItem);
					}
				}
				else if (HIWORD(wParam) == BN_CLICKED)
				{
					switch (LOWORD(wParam)) 
					{
						case IDC_BUTTON_ADD :
						{
							bool canAdd = true;

							if (!CheckIfRegistered(REG_RegstrationCode, RPN_CVALUE, RPN_VARIANT))
							{
								if (ListView_GetItemCount(hwndList) >= 2)
								{
									canAdd = false;

									if (IDYES == MessageBox(NULL, L"Only 2 plugins may be added in demo mode.\n\nWould you like information on how to register?", L"Demo Mode", MB_YESNO | MB_ICONQUESTION))
									{
										LaunchProgram(L"http://www.mobilesrc.com/Flipper.aspx", L"");
									}
								}
							}

							if (canAdd)
							{
								g_pSelectedPlugin = NULL;
								DialogBoxW(g_hInstance, MAKEINTRESOURCE(IDD_ADD_DIALOG), hwndDlg, AddPluginProc);

								if (g_pSelectedPlugin)
								{
									g_iMaxItemId++;

									HKEY hKey = NULL, hKeyData = NULL;
									DWORD dwType = 0, dwValue = 0, dwSize = 0, dwDisp;
									HRESULT hRes = RegOpenKeyEx(HKEY_CURRENT_USER, REG_FlipperKey, 0, KEY_READ, &hKey);
									if (hRes == ERROR_SUCCESS)
									{
										HKEY hKeyItems;
										hRes = RegCreateKeyEx(hKey, REG_Items_Key, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKeyItems, &dwDisp);
										if (hRes == ERROR_SUCCESS)
										{
											WCHAR wzItem[5];
											_itow(g_iMaxItemId, wzItem, 10);
											hRes = RegCreateKeyEx(hKeyItems, wzItem, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, &dwDisp);
											if (hRes == ERROR_SUCCESS)
											{
												WCHAR wzValue[MAX_PATH];
												RegCreateKeyEx(hKey, REG_Items_Data_Key, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKeyData, &dwDisp);

												StringCchCopy(wzValue, ARRAYSIZE(wzValue), g_pSelectedPlugin->wzName);
												RegSetValueExW(hKey,REG_Items_Name,0,REG_SZ,(LPBYTE)wzValue, sizeof(wzValue));

												StringCchCopy(wzValue, ARRAYSIZE(wzValue), g_pSelectedPlugin->wzAssembly);
												RegSetValueExW(hKey,REG_Items_Assembly,0,REG_SZ,(LPBYTE)wzValue, sizeof(wzValue));

												RegSetValueExW(hKey,REG_Items_PluginId,0,REG_DWORD,(LPBYTE)&(g_pSelectedPlugin->dwPluginId), sizeof(DWORD));

												srand( GetTickCount() );
												dwValue = g_crItems[rand() % ARRAYSIZE(g_crItems)];
												RegSetValueExW(hKey,REG_Items_Color,0,REG_DWORD,(LPBYTE)&dwValue, sizeof(DWORD));

												RegCloseKey(hKeyData);
												RegCloseKey(hKey);

												HDC hdc = CreateCompatibleDC(GetDC(hwndList));
												PluginSetting* pSetting = AddPlugin(hKeyItems, g_hImageList, hwndList, hdc, g_iMaxItemId);
												DeleteDC(hdc);

												if (pSetting && pSetting->pfnShowSettings)
												{
													WCHAR wzSettings[MAX_PATH];
													GetSettingsRegistryPath(wzSettings, MAX_PATH, pSetting->dwItemId);

													HKEY hKeyData;
													RegCreateKeyEx(HKEY_CURRENT_USER, wzSettings, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKeyData, &dwDisp);
													pSetting->pfnShowSettings(hwndDlg, hKeyData);
													RegCloseKey(hKeyData);
												}
											}
										}
									}
								}
							}
							Sleep(0);
						}
						break;
						case IDC_BUTTON_REMOVE :
						{
							int iSel = ListView_GetSelectionMark(hwndList);
							LV_ITEM lvItem;
							ZeroMemory(&lvItem, sizeof(LV_ITEM));
							lvItem.mask = LVIF_IMAGE | LVIF_PARAM;
							lvItem.iItem = iSel;
							ListView_GetItem(hwndList, &lvItem);
							
							PluginSetting* pSetting = (PluginSetting*)lvItem.lParam;

							HKEY hKey = NULL;
							HRESULT hRes = RegOpenKeyEx(HKEY_CURRENT_USER, REG_FlipperKey, 0, KEY_READ, &hKey);
							if (hRes == ERROR_SUCCESS)
							{
								HKEY hKeyItems;
								hRes = RegOpenKeyEx(hKey, REG_Items_Key, 0, KEY_READ, &hKeyItems);
								if (hRes == ERROR_SUCCESS)
								{
									WCHAR wzItem[5];
									_itow(pSetting->dwItemId, wzItem, 10);
									if (DeleteRecursively(hKeyItems, wzItem))
									{
										ListView_DeleteItem(hwndList, iSel);
									}
								}
							}
						}
						break;
						case IDC_BUTTON_MOVEUP :
						{
							int iSel = ListView_GetSelectionMark(hwndList);
							LV_ITEM lvItem;
							ZeroMemory(&lvItem, sizeof(LV_ITEM));
							lvItem.mask = LVIF_IMAGE | LVIF_PARAM;
							lvItem.iItem = iSel;
							ListView_GetItem(hwndList, &lvItem);

							PluginSetting* pSetting = (PluginSetting*)lvItem.lParam;

							lvItem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
							lvItem.cchTextMax = ARRAYSIZE(pSetting->wzName);
							lvItem.pszText = pSetting->wzName;
							lvItem.iItem = iSel - 1;
							lvItem.state = LVIS_SELECTED | LVIS_FOCUSED;
							lvItem.stateMask = -1;

							ListView_DeleteItem(hwndList, iSel);
							ListView_InsertItem(hwndList, &lvItem);

							ListView_SetSelectionMark(hwndList, lvItem.iItem);
						}
						break;
						case IDC_BUTTON_MOVEDOWN :
						{
							int iSel = ListView_GetSelectionMark(hwndList);
							LV_ITEM lvItem;
							ZeroMemory(&lvItem, sizeof(LV_ITEM));
							lvItem.mask = LVIF_IMAGE | LVIF_PARAM;
							lvItem.iItem = iSel;
							ListView_GetItem(hwndList, &lvItem);

							PluginSetting* pSetting = (PluginSetting*)lvItem.lParam;

							lvItem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
							lvItem.cchTextMax = ARRAYSIZE(pSetting->wzName);
							lvItem.pszText = pSetting->wzName;
							lvItem.iItem = iSel + 1;
							lvItem.state = LVIS_SELECTED | LVIS_FOCUSED;
							lvItem.stateMask = -1;

							ListView_DeleteItem(hwndList, iSel);
							ListView_InsertItem(hwndList, &lvItem);

							ListView_SetSelectionMark(hwndList, lvItem.iItem);
						}
						break;
						case IDC_BUTTON_OPTIONS :
						{
							int iSel = ListView_GetSelectionMark(hwndList);
							LV_ITEM lvItem;
							ZeroMemory(&lvItem, sizeof(LV_ITEM));
							lvItem.mask = LVIF_PARAM;
							lvItem.iItem = iSel;
							ListView_GetItem(hwndList, &lvItem);

							PluginSetting* pSetting = (PluginSetting*)lvItem.lParam;
							if (pSetting->pfnShowSettings)
							{
								WCHAR wzSettings[MAX_PATH];
								GetSettingsRegistryPath(wzSettings, MAX_PATH, pSetting->dwItemId);

								DWORD dwDisp;
								HKEY hKeyData;
								RegCreateKeyEx(HKEY_CURRENT_USER, wzSettings, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKeyData, &dwDisp);
								pSetting->pfnShowSettings(hwndDlg, hKeyData);
								RegCloseKey(hKeyData);
							}
						}
						break;
					}
				}
			}
			break;
        case WM_HSCROLL:
            switch LOWORD(wParam) 
            {
                case TB_BOTTOM:
                case TB_THUMBPOSITION:
                case TB_LINEUP:
                case TB_LINEDOWN:
                case TB_PAGEUP:
                case TB_PAGEDOWN:
                case TB_TOP:
                    {
                    }

                    return TRUE;
                default:
                    // Default case
                    return FALSE;
            }
            break;

	case WM_MEASUREITEM:
		LPMEASUREITEMSTRUCT lpmis; 
		lpmis = (LPMEASUREITEMSTRUCT) lParam; 
		lpmis->itemWidth = 10;
		lpmis->itemHeight = 30; 
		break;

	case WM_DRAWITEM:
		{
			RECT rc;
			DRAWITEMSTRUCT * pdis;

			pdis = (DRAWITEMSTRUCT *)lParam;
			rc = pdis->rcItem;

			// NULL object ?
			if (pdis->itemData == -1)
			{
				return 0;
			}
			
			HBRUSH hBrush = NULL;
			SetBkMode(pdis->hDC, TRANSPARENT);
			if ((pdis->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
			{
				if ((pdis->itemState & ODS_SELECTED))
				{
					hBrush = GetSysColorBrush(COLOR_HIGHLIGHT);
				}
				else
				{
					hBrush = GetSysColorBrush(COLOR_WINDOW);
				}
			}

			if (hBrush)
			{
				COLORREF cr = (COLORREF) pdis->itemData;
				FillRect(pdis->hDC, &rc, hBrush);
				InflateRect(&rc, -3, -3);

				hBrush = CreateSolidBrush(cr);
				FillRect(pdis->hDC, &rc, hBrush);
				DeleteObject(hBrush);
			}
		}
		break;
    }

    return FALSE;
}

int CALLBACK ListViewCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    WCHAR* wzItem1 = (WCHAR*)lParam1;
    WCHAR* wzItem2 = (WCHAR*)lParam2;

    WCHAR* wzName1 = wcsrchr(wzItem1, L'\\');
    WCHAR* wzName2 = wcsrchr(wzItem2, L'\\');

    return (wcsicmp(wzName1, wzName2));
}

////////////////////////////////////////////////////////
//    This callback is called when each property page gets
//  created and when it gets released.
////////////////////////////////////////////////////////
UINT CALLBACK PropSheetPageProc(HWND hwnd,UINT uMsg,LPPROPSHEETPAGE ppsp)
{
    HWND    hwndDlg;

    // Get the HWND of this property page that was tucked at the
    // end of the propsheetpage struct in the WM_INITDIALOG for this page
    hwndDlg = *((HWND*)(ppsp + 1));

    switch(uMsg)
    {
    case PSPCB_CREATE:
        //Return any non zero value to indicate success
        return 1;
        break;

    case PSPCB_RELEASE:
        //Make this a code block so declaration of iPage is local
        {
            // return value is ignored for this message
            return 0;
        }
        break;
    default:
        // We should never see this case.  If we do we want to know in debug builds.
        ASSERT(false);
        break;
    }
    //return for default case above
    return (UINT)-1;
}
////////////////////////////////////////////////////////
//    The PropSheetProc for the property sheet.
//
////////////////////////////////////////////////////////
int CALLBACK PropSheetProc(HWND hwndDlg,UINT uMsg,LPARAM lParam)
{
    switch (uMsg)
    {
        // Returning COMCTL32_VERSION to this message makes the
        // style of the property sheet to be that of the native
        // Pocket PC ones
        case PSCB_GETVERSION:
            return COMCTL32_VERSION;
        
        // The text copied into the lParam on receipt of this message
        // is displayed as the title of the propertysheet
        case PSCB_GETTITLE:
            wcscpy((TCHAR*)lParam,TEXT("Flipper Settings"));
            break;
        
        // The text copied into the lParam on receipt of this message
        // gets displayed as the link text at the bottom of the
        // propertysheet
        case PSCB_GETLINKTEXT:
            //wcscpy((TCHAR*)lParam,TEXT("Go to <file:ctlpnl.exe cplmain.cpl,2,1{another}> applet."));
            break;
            
        default:
            break;
    }
    
    return 0;
}

void LoadPlugins()
{

	SetCursor(LoadCursor(NULL, IDC_WAIT));

	FlipperPlugin plugins[100];
	WCHAR wzAssembly[MAX_PATH];
    WIN32_FIND_DATAW data;
    HANDLE hFile = FindFirstFile(L"\\program files\\mobilesrc\\flipper\\plugins\\*.dll", &data);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				WCHAR* wzFile = wcsstr(data.cFileName, L".dll");
				if (wzFile)
				{
					StringCchCopy(wzAssembly, ARRAYSIZE(wzAssembly), L"\\program files\\mobilesrc\\flipper\\plugins\\");
					StringCchCat(wzAssembly, ARRAYSIZE(wzAssembly), data.cFileName);
					HMODULE hMod = LoadLibrary(wzAssembly);

					if (hMod)
					{
						bool bSuccess = false;
						PFN_QueryAvailablePlugins pfnQuery = (PFN_QueryAvailablePlugins)GetProcAddress(hMod, L"QueryAvailablePlugins");

						if (pfnQuery)
						{
							DWORD dwPlugins = ARRAYSIZE(plugins);
							if (pfnQuery(plugins, &dwPlugins))
							{
								for (int i = 0; i < (int)dwPlugins; ++i)
								{
									int iStrLen = wcslen(plugins[i].wzName)+1;
									WCHAR* wzName = new WCHAR[iStrLen];
									AssemblyPlugin* pPlugin = new AssemblyPlugin();
									StringCchCopy(wzName, iStrLen, plugins[i].wzName);

									iStrLen = wcslen(data.cFileName)+1;
									pPlugin->hModule = hMod;
									pPlugin->dwPluginId = plugins[i].pluginId;
									pPlugin->wzName = wzName;
									pPlugin->wzAssembly = new WCHAR[iStrLen];
									StringCchCopy(pPlugin->wzAssembly, iStrLen, data.cFileName);

									g_wzPlugins.push_back(pPlugin);
								}
								bSuccess = true;
							}
						}

						if (bSuccess)
						{
							g_hModules.push_back(hMod);
						}
						else
						{
							FreeLibrary(hMod);
						}
					}
				}
			}
		} while (FindNextFile(hFile, &data));
	}
	FindClose(hFile);

	SetCursor(NULL);
}

////////////////////////////////////////////////////////
//    Called to display this applet as a property sheet
//
////////////////////////////////////////////////////////
BOOL CreatePropertySheet(HWND hwndParent, int iApplet)
{
    BOOL bReturn = FALSE;
	InitCommonControls();

	//LoadPlugins();
/*
    if (!CheckIfRegistered(REG_RegstrationCode, RPN_CVALUE, RPN_VARIANT))
    {
        SHInitExtraControls();
        int iRegCodeRet = DialogBoxW(g_hInstance, MAKEINTRESOURCE(IDD_REGISTER_DIALOG), hwndParent, (DLGPROC)RegistrationProc);

        switch (iRegCodeRet)
        {
            case IDOK :
                {
                    // successfully registered
                }
                break;
            case IDCANCEL :
                {
                    // running in demo mode
                }
                break;
            default :
                {
                    return bReturn;
                }
                break;
        }
    }
*/
    PROPSHEETHEADER psh;
    PROPSHEETPAGE ppsp;
    HPROPSHEETPAGE hpsp[2];
    int nPages=2;

    // Set all values for first property page
    ppsp.dwSize = sizeof(PROPSHEETPAGE)+sizeof(HWND); // Extra space at end of struct to tuck page's hwnd in when it's created
    ppsp.dwFlags = PSP_DEFAULT|PSP_USETITLE|PSP_USECALLBACK;
    ppsp.hInstance = g_hInstance;
    ppsp.pszTemplate = (LPTSTR)MAKEINTRESOURCE(IDD_PROPPAGEAPPEARANCE);
    ppsp.hIcon = NULL;
    ppsp.pszTitle = TEXT("Appearance");
    ppsp.pfnDlgProc = AppearanceSettingsPageProc;
    ppsp.pfnCallback = PropSheetPageProc;
    ppsp.pcRefParent = NULL;

    hpsp[0] = CreatePropertySheetPage(&ppsp);
    if (NULL == hpsp[0]) {
        //Fail out of function
        return bReturn;
    }

    // Set all values for second perperty page
    ppsp.dwSize = sizeof(PROPSHEETPAGE)+sizeof(HWND); // Extra space at end of struct to tuck page's hwnd in when it's created
    ppsp.dwFlags = PSP_DEFAULT|PSP_USETITLE|PSP_USECALLBACK;
    ppsp.hInstance = g_hInstance;
    ppsp.pszTemplate = (LPTSTR)MAKEINTRESOURCE(IDD_PROPPAGEITEMS);
    ppsp.hIcon = NULL;
    ppsp.pszTitle = TEXT("Items");
    ppsp.pfnDlgProc = ItemsSettingsPageProc;
    ppsp.pfnCallback = PropSheetPageProc;
    ppsp.pcRefParent = NULL;

    hpsp[1] = CreatePropertySheetPage(&ppsp);
    if (NULL ==hpsp[1]) {
        //Clean up the page we have created
        DestroyPropertySheetPage(hpsp[0]);

        //Fail out of function
        return bReturn;
    }
    
    psh.dwSize                = sizeof(psh);
    psh.dwFlags                = PSH_USECALLBACK|PSH_MAXIMIZE;
    psh.hwndParent            = hwndParent;
    psh.hInstance            = g_hInstance;
    psh.pszCaption            = L"Flipper Settings";
    psh.phpage                = hpsp;
    psh.nPages                = nPages;
    psh.nStartPage            = iApplet > (nPages-1)? (nPages-1): iApplet;
    psh.pfnCallback            = PropSheetProc;

    if(-1 != PropertySheet(&psh))
    {
        bReturn = TRUE;
    }
    else 
    {
        //Clean up PropertySheetPages as they weren't used by the PropertySheet
        DestroyPropertySheetPage(hpsp[0]);
        DestroyPropertySheetPage(hpsp[1]);
    }

	list<HMODULE>::iterator modIter = g_hModules.begin();

	while (modIter != g_hModules.end())
	{
		FreeLibrary(*modIter);
		modIter++;
	}

	list<AssemblyPlugin*>::iterator pluginIter = g_wzPlugins.begin();

	while (pluginIter != g_wzPlugins.end())
	{
		delete[] (*pluginIter)->wzName;
		delete[] (*pluginIter)->wzAssembly;

		delete *pluginIter;

		pluginIter++;
	}
/*
    if (hEvent)
    {
        SetEvent(hEvent);
        CloseHandle(hEvent);
    }
    else if (!dwDisable)
    {
        TCHAR szStatic[MAX_PATH], szStartup[MAX_PATH];
        if (SHGetSpecialFolderPath(NULL, szStartup, CSIDL_STARTUP, FALSE))
        {
            _tcscat(szStartup, L"\\Flipper.lnk");
            if (SHGetShortcutTarget(szStartup, szStatic, MAX_PATH))
            {
                LaunchProgram(szStatic, NULL);
            }
        }
    }
*/
    if (g_hIcon)
    {
        DestroyIcon(g_hIcon);
        g_hIcon = NULL;
    }

    if (g_hSmall)
    {
        ImageList_Destroy(g_hSmall);
        g_hSmall = NULL;
    }

    return bReturn;
}


DWORD FindProcess(TCHAR *szName) 
{ 
  HINSTANCE            hProcessSnap   = NULL; 
  PROCESSENTRY32    pe32           = {0}; 
  DWORD                dwTaskCount    = 0; 
  
  hProcessSnap = (HINSTANCE)CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS | TH32CS_SNAPNOHEAPS, 0); 
  if (hProcessSnap == (HANDLE)-1) return 0; 

  dwTaskCount = 0; 
  pe32.dwSize = sizeof(PROCESSENTRY32);   // must be filled out before use 
  if (Process32First(hProcessSnap, &pe32)) { 
    do { 
      if (_wcsicmp(pe32.szExeFile,szName)==0) 
     { 
        CloseToolhelp32Snapshot(hProcessSnap); 
        return pe32.th32ProcessID; 
     } 
    } 
    while (Process32Next(hProcessSnap, &pe32)); 
  } 
  CloseToolhelp32Snapshot(hProcessSnap); 
  return 0; 
} 

void KillProcess(TCHAR *szName) 
{ 
   DWORD dwPID = FindProcess(szName); 
   HANDLE hProcess; 
    
   if (dwPID) 
   { 
      hProcess = OpenProcess(PROCESS_ALL_ACCESS,false,dwPID); 
      TerminateProcess(hProcess,0); 
      CloseHandle(hProcess); 
   } 
}

int _tmain(int argc, _TCHAR* argv[])
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	g_hInstance = GetModuleHandle(NULL);
	InitCommonControls();
	LoadPlugins();
	//DialogBoxW(g_hInstance, MAKEINTRESOURCE(IDD_PROPPAGEITEMS), GetForegroundWindow(), ItemsSettingsPageProc);
	CreatePropertySheet(GetForegroundWindow(), 0);
	
	DWORD dwDisabled = 0;
    GetFromRegistry(REG_Disabled, &dwDisabled);

	if (dwDisabled)
	{
		KillProcess(L"flipper.exe");
	}
	else
	{
		HWND hwndFlipper = FindWindow(L"flipperTabs", NULL);

		if (hwndFlipper)
		{
			PostMessage(hwndFlipper, WM_SETTINGSCHANGED, 0, 0);
		}
		else
		{
			LaunchProgram(L"\\program files\\mobilesrc\\flipper\\flipper.exe", NULL);
		}
	}
	CoUninitialize();
	return 0;
}