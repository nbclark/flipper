#pragma once
#pragma once
#include "..\Flipper\FlipperPlugin.h"
#include "..\Flipper\Utils.h"
#include "ApplicationSelector.h"
#include "DestroyablePlugin.h"

struct LaunchItem
{
	WCHAR wzPath[MAX_PATH];
	HICON hIcon;
};

class CQuickLaunch : CDestroyablePlugin
{
public:
    CQuickLaunch(void);
    ~CQuickLaunch(void);

public:
	static HMODULE g_hInstance;
	static void ShowSettings(HWND hWndParent, HKEY hKey);
    node* BuildLaunchList(int* piNodeLen, int thumbWidth, int thumbHeight);
	void DestroyPhoneNodes(node* pHead);

	static void DestroyLaunchItems(list<LaunchItem*>* pLaunchItems);
	static void SaveLaunchItems(HKEY hKey, list<LaunchItem*>* pLaunchItems);
	static void LoadLaunchItems(HKEY hKey, list<LaunchItem*>* pLaunchItems);
	static BOOL QuickLaunchProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
	void LoadSettings(IFlipperApp* pFlipper, HKEY hKey);

public:
    list<LaunchItem*> g_launchList;
	IFlipperApp* g_pFlipper;
	int g_iThumbWidth, g_iThumbHeight;

private:
	node* g_pHeadNode;
	int g_iNodeLen;
};

DrawThumbnailApi(QuickLaunchDrawThumbnail);
NodeSelectedApi(QuickLaunchNodeSelected);
CloseOnSelectionApi(QuickLaunchCloseOnSelection);
BuildNodeListApi(QuickLaunchBuildNodeList);
DestroyNodeListApi(QuickLaunchDestroyNodeList);
SetSoftKeysApi(QuickLaunchSetSoftKeys);
SoftKeySelectedApi(QuickLaunchSoftKeySelected);