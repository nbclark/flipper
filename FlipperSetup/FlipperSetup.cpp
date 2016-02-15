#include "stdafx.h"
#include <windows.h>
#include <regext.h>
#include <ce_setup.h>
#include <Tlhelp32.h>
#include "..\Flipper\FlipperPlugin.h"
#include "..\Flipper\Utils.h"

#define TH32CS_SNAPNOHEAPS   0x40000000   // optimization for text shell to not snapshot heaps 

extern "C"
__declspec(dllexport)
int GetPluginNames(HMODULE hModule, FlipperPlugin* pPlugins, DWORD nMaxCount)
{
    PFN_QueryAvailablePlugins pQuery = (PFN_QueryAvailablePlugins)GetProcAddress(hModule, L"QueryAvailablePlugins");

	if (pQuery)
	{
		pQuery(pPlugins, &nMaxCount);
	}
	return nMaxCount;
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

codeINSTALL_INIT Install_Init(
    HWND        hwndParent,
    BOOL        fFirstCall, 
    BOOL        fPreviouslyInstalled,
    LPCTSTR     pszInstallDir
    )
{
	DWORD dwDisp;

	HKEY hKeyFlipper, hKeyItems, hKeyItems0, hKeyItems1, hKeyItems0Data, hKeyItems1Data;
	RegCreateKeyEx(HKEY_CURRENT_USER, L"ControlPanel\\Flipper", 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKeyFlipper, &dwDisp);

	RegistrySetDWORD(hKeyFlipper, NULL, REG_EnableScrollAnimation, 1);
	RegistrySetDWORD(hKeyFlipper, NULL, REG_AutoClose, 0);
	RegistrySetDWORD(hKeyFlipper, NULL, REG_Disabled, 0);
	RegistrySetDWORD(hKeyFlipper, NULL, REG_TabAlignment, 0);
	RegistrySetDWORD(hKeyFlipper, NULL, REG_TabSize, 1);

	RegCreateKeyEx(hKeyFlipper, L"Items", 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKeyItems, &dwDisp);
	RegCreateKeyEx(hKeyItems, L"0", 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKeyItems0, &dwDisp);
	RegCreateKeyEx(hKeyItems, L"1", 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKeyItems1, &dwDisp);
	RegCreateKeyEx(hKeyItems0, L"Data", 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKeyItems0Data, &dwDisp);
	RegCreateKeyEx(hKeyItems1, L"Data", 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKeyItems1Data, &dwDisp);
	
	RegistrySetDWORD(hKeyItems0, NULL, L"Color", 8388672);
	RegistrySetDWORD(hKeyItems0, NULL, L"PluginId", 3);
	RegistrySetString(hKeyItems0, NULL, L"Assembly", L"FlipperPlugins.dll");
	RegistrySetString(hKeyItems0, NULL, L"Name", L"Quick Launch");
	
	RegistrySetDWORD(hKeyItems1, NULL, L"Color", 16512);
	RegistrySetDWORD(hKeyItems1, NULL, L"PluginId", 4);
	RegistrySetString(hKeyItems1, NULL, L"Assembly", L"FlipperPlugins.dll");
	RegistrySetString(hKeyItems1, NULL, L"Name", L"Weather");

	RegistrySetString(hKeyItems0Data, NULL, L"0_Path", L"\\Windows\\Start Menu\\Programs\\Flipper\\Flipper Settings.lnk");
	RegistrySetDWORD(hKeyItems0Data, NULL, L"Count", 1);

	RegistrySetDWORD(hKeyItems1Data, NULL, L"ZipCode", 98052);

	RegCloseKey(hKeyItems0Data);
	RegCloseKey(hKeyItems1Data);
	RegCloseKey(hKeyItems0);
	RegCloseKey(hKeyItems1);
	RegCloseKey(hKeyItems);
	RegCloseKey(hKeyFlipper);

    return codeINSTALL_INIT_CONTINUE;
}

codeINSTALL_EXIT Install_Exit(
    HWND    hwndParent,
    LPCTSTR pszInstallDir,
    WORD    cFailedDirs,
    WORD    cFailedFiles,
    WORD    cFailedRegKeys,
    WORD    cFailedRegVals,
    WORD    cFailedShortcuts
    )
{
    // We have to set these values here, because cabwiz doesn't like negative reg values
    KillProcess(TEXT("ctlpnl.exe"));

    SetToRegistry(REG_Disabled, 0);

    LaunchProgram(L"\\Program Files\\mobilesrc\\flipper\\FlipperConfig.exe", L"");
    return codeINSTALL_EXIT_DONE;
}

codeUNINSTALL_INIT Uninstall_Init(
    HWND        hwndParent,
    LPCTSTR     pszInstallDir
    )
{
    SetToRegistry(REG_Disabled, 1);
    // Kill process here
    KillProcess(TEXT("Flipper.exe"));
    return codeUNINSTALL_INIT_CONTINUE;
}



codeUNINSTALL_EXIT Uninstall_Exit(
    HWND    hwndParent
    )
{
    return codeUNINSTALL_EXIT_DONE;
}

