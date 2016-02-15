// FlipperPlugins.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "TaskManager.h"
#include "QuickLaunch.h"
#include "SpeedDial.h"
#include "Weather.h"
#include "DateTime.h"
#include "DestroyablePlugin.h"
#include "..\Flipper\FlipperPlugin.h"

HANDLE g_hInstance = NULL;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	g_hInstance = hModule;
	CSpeedDial::g_hInstance = (HMODULE)hModule;
	CTaskManager::g_hInstance = (HMODULE)hModule;
	CQuickLaunch::g_hInstance = (HMODULE)hModule;
	CWeather::g_hInstance = (HMODULE)hModule;
	CDateTime::g_hInstance = (HMODULE)hModule;
    return TRUE;
}

extern "C"
__declspec(dllexport)
bool WINAPI QueryAvailablePlugins(FlipperPlugin* pPluginList, DWORD* dwLength)
{
	if (!dwLength || !pPluginList)
	{
		return false;
	}
	if (*dwLength < 5)
	{
		*dwLength = 5;
		return false;
	}
	*dwLength = 5;
	StringCchCopy(pPluginList[0].wzName, ARRAYSIZE(pPluginList[0].wzName), L"Task Manager");
	StringCchCopy(pPluginList[1].wzName, ARRAYSIZE(pPluginList[1].wzName), L"Speed Dialer");
	StringCchCopy(pPluginList[2].wzName, ARRAYSIZE(pPluginList[2].wzName), L"Quick Launch");
	StringCchCopy(pPluginList[3].wzName, ARRAYSIZE(pPluginList[3].wzName), L"Weather");
	StringCchCopy(pPluginList[4].wzName, ARRAYSIZE(pPluginList[4].wzName), L"Date/Time");

	pPluginList[0].pluginId = 1;
	pPluginList[1].pluginId = 2;
	pPluginList[2].pluginId = 3;
	pPluginList[3].pluginId = 4;
	pPluginList[4].pluginId = 5;

	return true;
}

extern "C"
__declspec(dllexport)
PFN_ShowSettings WINAPI GetSettingsProc(int pluginId)
{
	switch (pluginId)
	{
		case 1 :
			{
				return CTaskManager::ShowSettings;
			}
			break;
		case 2 :
			{
				return CSpeedDial::ShowSettings;
			}
			break;
		case 3 :
			{
				return CQuickLaunch::ShowSettings;
			}
			break;
		case 4 :
			{
				return CWeather::ShowSettings;
			}
			break;
		case 5 :
			{
				return CDateTime::ShowSettings;
			}
			break;
	}
	return NULL;
}

extern "C"
__declspec(dllexport)
bool WINAPI CreatePluginInstance(IFlipperApp* pFlipper, int pluginId, FlipperPlugin** ppPlugin, HKEY hKey, int thumbWidth, int thumbHeight)
{
	*ppPlugin = NULL;

	switch (pluginId)
	{
		case 1 :
		{
			FlipperPlugin* pPlugin = new FlipperPlugin();
			CTaskManager* pTaskMan = new CTaskManager();
			pTaskMan->LoadSettings(pFlipper, hKey);

			pPlugin->pData = pTaskMan;
			pPlugin->pluginId = pluginId;
			pPlugin->pfnDrawThumbnail = NULL;
			pPlugin->pfnCloseOnSelection = NULL;
			pPlugin->pfnBuildNodeList = &TaskManagerBuildNodeList;
			pPlugin->pfnDestroyNodeList = &TaskManagerDestroyNodeList;
			pPlugin->pfnNodeSelected = &TaskManagerNodeSelected;
			pPlugin->pfnSetSoftKeys = &TaskManagerSetSoftKeys;
			pPlugin->pfnSoftKeySelected = &TaskManagerSoftKeySelected;

			*ppPlugin = pPlugin;
		}
		return true;

		case 2 :
		{
			FlipperPlugin* pPlugin = new FlipperPlugin();

			CSpeedDial* pSD = new CSpeedDial();
			pSD->LoadSettings(pFlipper, hKey);

			pPlugin->pData = pSD;
			pPlugin->pluginId = pluginId;
			pPlugin->pfnDrawThumbnail = NULL;
			pPlugin->pfnCloseOnSelection = NULL;
			pPlugin->pfnBuildNodeList = &SpeedDialerBuildNodeList;
			pPlugin->pfnDestroyNodeList = &SpeedDialerDestroyNodeList;
			pPlugin->pfnNodeSelected = &SpeedDialerNodeSelected;
			pPlugin->pfnSetSoftKeys = &SpeedDialerSetSoftKeys;
			pPlugin->pfnSoftKeySelected = &SpeedDialerSoftKeySelected;

			*ppPlugin = pPlugin;
		}
		return true;

		case 3 :
		{
			FlipperPlugin* pPlugin = new FlipperPlugin();

			CQuickLaunch* pSD = new CQuickLaunch();
			pSD->LoadSettings(pFlipper, hKey);

			pPlugin->pData = pSD;
			pPlugin->pluginId = pluginId;
			pPlugin->pfnDrawThumbnail = NULL;
			pPlugin->pfnCloseOnSelection = NULL;
			pPlugin->pfnBuildNodeList = &QuickLaunchBuildNodeList;
			pPlugin->pfnDestroyNodeList = &QuickLaunchDestroyNodeList;
			pPlugin->pfnNodeSelected = &QuickLaunchNodeSelected;
			pPlugin->pfnSetSoftKeys = &QuickLaunchSetSoftKeys;
			pPlugin->pfnSoftKeySelected = &QuickLaunchSoftKeySelected;

			*ppPlugin = pPlugin;
		}
		return true;

		case 4 :
		{
			FlipperPlugin* pPlugin = new FlipperPlugin();

			CWeather* pSD = new CWeather(pPlugin);
			pSD->LoadSettings(pFlipper, hKey);

			pPlugin->pData = pSD;
			pPlugin->pluginId = pluginId;
			pPlugin->pfnDrawThumbnail = &WeatherDrawThumbnail;
			pPlugin->pfnCloseOnSelection = NULL;
			pPlugin->pfnBuildNodeList = &WeatherBuildNodeList;
			pPlugin->pfnDestroyNodeList = &WeatherDestroyNodeList;
			pPlugin->pfnNodeSelected = &WeatherNodeSelected;
			pPlugin->pfnSetSoftKeys = &WeatherSetSoftKeys;
			pPlugin->pfnSoftKeySelected = &WeatherSoftKeySelected;

			*ppPlugin = pPlugin;
		}
		return true;

		case 5 :
		{
			FlipperPlugin* pPlugin = new FlipperPlugin();

			CDateTime* pSD = new CDateTime(pFlipper, pPlugin);
			pSD->LoadSettings(pFlipper, hKey);

			pPlugin->pData = pSD;
			pPlugin->pluginId = pluginId;
			pPlugin->pfnDrawThumbnail = &DateTimeDrawThumbnail;
			pPlugin->pfnCloseOnSelection = NULL;
			pPlugin->pfnBuildNodeList = &DateTimeBuildNodeList;
			pPlugin->pfnDestroyNodeList = &DateTimeDestroyNodeList;
			pPlugin->pfnNodeSelected = &DateTimeNodeSelected;
			pPlugin->pfnSetSoftKeys = NULL;
			pPlugin->pfnSoftKeySelected = NULL;

			*ppPlugin = pPlugin;
		}
		return true;
	}
	return false;
}

extern "C"
__declspec(dllexport)
void WINAPI DestroyPluginInstance(FlipperPlugin* pPlugin)
{
	if (pPlugin->pData)
	{
		delete ((CDestroyablePlugin*)pPlugin->pData);
	}
}