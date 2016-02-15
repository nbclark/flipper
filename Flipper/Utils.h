#pragma once

#include "..\Flipper\FlipperPlugin.h"

#define REG_FlipperKey                      TEXT("ControlPanel\\Flipper")
#define REG_RegstrationCode                 TEXT("RegCode")
#define REG_Disabled						TEXT("Disabled")
#define REG_EnableShrinkAnimation			TEXT("EnableShrinkAnimation")
#define REG_EnableScrollAnimation			TEXT("EnableScrollAnimation")
#define REG_AutoClose    					TEXT("AutoClose")
#define REG_TabSize	    					TEXT("TabSize")
#define REG_TabAlignment   					TEXT("TabAlignment")

#define REG_Items_Key                       TEXT("Items")
#define REG_Items_Color                     TEXT("Color")
#define REG_Items_Name                      TEXT("Name")
#define REG_Items_Assembly                  TEXT("Assembly")
#define REG_Items_PluginId                  TEXT("PluginId")
#define REG_Items_Data_Key                  TEXT("Data")

#define EVENT_FLIPPEREXIT					TEXT("FlipperExit")
#define EVENT_FLIPPERSETTINGS				TEXT("FlipperSettings")
#define EVENT_FLIPPER_MUTEX					TEXT("Flipper")

void LedOn(int id);
void LedOff(int id);
BOOL LaunchProgram(LPCTSTR lpFile, LPCTSTR lpParameters);
HRESULT GetFromRegistry(HKEY rootKey, TCHAR* szRegistryKey, TCHAR* szRegValueName, WCHAR wzValue[MAX_PATH]);
HRESULT GetFromRegistry(TCHAR* szRegValueName, WCHAR wzValue[MAX_PATH]);
HRESULT GetFromRegistry(HKEY rootKey, TCHAR* szRegistryKey, TCHAR* szRegValueName, DWORD *pdwValue);
HRESULT GetFromRegistry(TCHAR* szRegValueName, DWORD *pdwValue);
HRESULT SetToRegistry(TCHAR* szRegValueName, DWORD dwValue);
HRESULT SetStringToRegistry(TCHAR* szRegValueName, WCHAR* wzValue);
HICON GetApplicationIcon(WCHAR* szFilteredPath, bool bSmall, WCHAR* wzExeFile, WCHAR* wzWndText, WCHAR* wzClassName);
WCHAR* TrimEndSpaces(WCHAR* szEXEPath2);
bool RectContains(RECT* pRect, int xPos, int yPos);
bool GetProcessName(DWORD dwProcessId, WCHAR wzProcessName[MAX_PATH]);
bool FindLnkIcon(WCHAR szFoundPath[MAX_PATH], WCHAR* szPath, WCHAR* wzProcessName);
void ScaleProportional(UINT uFitToWidth, UINT uFitToHeight, UINT *puWidthToScale, UINT *puHeightToScale);
bool FileExists(WCHAR* wzPath, FILETIME* lpLastWrite);
void CreateDirectoryRecursively(WCHAR* wzPath);
COLORREF DarkenColor(COLORREF clr, int iDarkenAmount);
void DestroyNodeList(node* pHeadNode);

struct strltpred
{
   enum
   {
      bucket_size = 4,
      min_buckets = 8
   };

   size_t operator()(WCHAR* s) const
   {
      size_t h = 0;
      for (; *s; ++s)
         h = 5 * h + *s;
      return h;
   }

   bool operator()(WCHAR* a, WCHAR* b) const
   {
      return wcscmp(a, b) < 0;
   }
};