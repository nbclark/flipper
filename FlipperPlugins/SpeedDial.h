#pragma once
#include "POutlook.h"
#include "..\Flipper\FlipperPlugin.h"
#include "DestroyablePlugin.h"

struct PhoneRecord
{
	CEOID oid;
	WCHAR wzName[40];
	WCHAR wzPhone[15];
	HBITMAP hBmp;
	UINT uiWidth;
	UINT uiHeight;
};

class CSpeedDial : CDestroyablePlugin
{
public:
    CSpeedDial(void);
    ~CSpeedDial(void);

public:
	static HMODULE g_hInstance;
	static void ShowSettings(HWND hWndParent, HKEY hKey);
    node* BuildPhoneNodes(int* piNodeLen, int thumbWidth, int thumbHeight);
	void DestroyPhoneNodes(node* pHead);
	bool AddContact();
	void LoadSettings(IFlipperApp* pFlipper, HKEY hKey);

	static void DestroyPhoneItems(list<PhoneRecord*>* pLaunchItems);
	static void LoadPhoneItems(HKEY hKey, list<PhoneRecord*>* pLaunchItems, CPOutlook* pOutlook);
	static BOOL SpeedDialProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

public:
    list<PhoneRecord*> g_phoneList;
	CPOutlook g_outlookApp;
	IFlipperApp* g_pFlipper;

private:
	int g_iThumbWidth, g_iThumbHeight;
	node* g_pHeadNode;
	int g_iNodeLen;
};

DrawThumbnailApi(SpeedDialerDrawThumbnail);
NodeSelectedApi(SpeedDialerNodeSelected);
CloseOnSelectionApi(SpeedDialerCloseOnSelection);
BuildNodeListApi(SpeedDialerBuildNodeList);
DestroyNodeListApi(SpeedDialerDestroyNodeList);
SetSoftKeysApi(SpeedDialerSetSoftKeys);
SoftKeySelectedApi(SpeedDialerSoftKeySelected);