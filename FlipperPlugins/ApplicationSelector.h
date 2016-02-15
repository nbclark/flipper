#pragma once
#include "..\Flipper\FlipperPlugin.h"

class CApplicationSelector
{
public:
	CApplicationSelector(void);
	~CApplicationSelector(void);
	bool ShowDialog(HMODULE hInstance, HWND hwndParent, WCHAR* wzLnkLocation, bool bChooseProcess);

	static int ListViewCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static BOOL SelectLaunchProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static void GetLnkFiles(WCHAR* szPath, HIMAGELIST hSmall, list<LVITEM*>* plvItems, bool bReadOnly);

protected:
	static HIMAGELIST g_hSmall;
	static HINSTANCE g_hInstance;
	static list<LVITEM*> g_lvApplications;
	bool g_bChooseProcess;
	WCHAR* g_wzLnkLocation;
};
