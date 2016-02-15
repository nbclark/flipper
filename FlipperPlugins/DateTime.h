#pragma once
#include "..\Flipper\Utils.h"
#include "..\Flipper\FlipperPlugin.h"
#include "DestroyablePlugin.h"

#define REG_TimeOffset L"TimeOffset"
//WCHAR* wzMonths = { L"January", L"February", L"March", L"April", L"May", L"June", L"July", L"August", L"September", L"October", L"November", L"December" };

class CDateTime : CDestroyablePlugin
{
public:
	CDateTime(IFlipperApp* pFlipper, void* pPlugin);
	~CDateTime(void);

public:
	static HMODULE g_hInstance;
	static void ShowSettings(HWND hWndParent, HKEY hKey);
    static DWORD WINAPI PollDateTime(LPVOID lpParameter);
	static BOOL DateTimeProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
    void LoadSettings(IFlipperApp* pFlipper, HKEY hKey);

	node* BuildNodeList(int* piNodeLen, int* piThumbWidth, int* piThumbHeight, int iMaxWidth, int iMaxHeight);
	bool DrawThumbnail(node* pNode, HDC hdc, HDC hdcThumb, RECT* pRect, bool bSelected);

	IFlipperApp* g_pFlipper;
	void* g_pPlugin;

public:
	DWORD g_dwOffset;
	node g_dateNode;
	bool g_bUpdating;
	HANDLE g_hThumbThread;
};

DrawThumbnailApi(DateTimeDrawThumbnail);
BuildNodeListApi(DateTimeBuildNodeList);
DestroyNodeListApi(DateTimeDestroyNodeList);
NodeSelectedApi(DateTimeNodeSelected);