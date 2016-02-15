#pragma once
#include "..\Flipper\Utils.h"
#include "..\Flipper\FlipperPlugin.h"
#include "DestroyablePlugin.h"

typedef stdext::hash_map<DWORD, HBITMAP> PROCIMGMAP;
typedef stdext::hash_map<DWORD, HICON> PROCICONMAP;
typedef stdext::hash_map<DWORD, DWORD> PROCTIMEMAP;
typedef stdext::hash_map<DWORD, HWND> PROCHWNDMAP;

#define REG_AppSwitch_EnableThumbnails       TEXT("EnableThumbnails")
#define REG_AppSwitch_ProcessKey             TEXT("Processes")
#define REG_AppSwitch_Process_Ignore         TEXT("Ignore")
#define REG_AppSwitch_Process_CanClose       TEXT("CanClose")

#define REG_AppSwitch_Process_CanMinimize    TEXT("CanMinimize")
#define THUBMNAIL_POLL_INTERNVAL (1000 * 1)
#define THUBMNAIL_REFRESH_INTERNVAL (1000 * 30)
#define THUMBNAIL_CAPTURE_WIDTH 

struct ProcessRecord
{
	WCHAR wzName[MAX_PATH];
};

struct ProcessItem
{
	HWND hWnd;
	DWORD dwProcessId;
};

struct AppSwitchProcess
{
    bool bIgnore;
    bool bCanClose;
    bool bCanMinimize;
};

BuildNodeListApi(TaskManagerBuildNodeList);
DestroyNodeListApi(TaskManagerDestroyNodeList);
NodeSelectedApi(TaskManagerNodeSelected);
SetSoftKeysApi(TaskManagerSetSoftKeys);
SoftKeySelectedApi(TaskManagerSoftKeySelected);

class CTaskManager : CDestroyablePlugin
{
public:
    CTaskManager(void);
    ~CTaskManager(void);

public:
	static HMODULE g_hInstance;
	static void ShowSettings(HWND hWndParent, HKEY hKey);
    static DWORD WINAPI PollThumbnails(LPVOID lpParameter);
    node* BuildNodeList(int* piNodeLen);
    void EnsureCapture(HWND hwndCurrent, bool bForce);
    void LoadSettings(IFlipperApp* pFlipper, HKEY hKey);
    void CleanupAppSwitchSettings();
    void CleanupThumbnails();

	static void SaveProcessItems(HKEY hKey, list<ProcessRecord*>* pProcessList);
	static void LoadProcessItems(HKEY hKey, bool* pbEnableThumbnails, list<ProcessRecord*>* pProcessList);
	static void DestroyProcessItems(list<ProcessRecord*>* pProcessList);
	static BOOL TaskManagerProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

public:
    // these are appswitch variables
    bool g_bAppSwitchEnableThumbnails;
	IFlipperApp* g_pFlipper;
	list<ProcessRecord*> g_reservedProcessList;

private:
    HANDLE g_hThumbThread;
    CRITICAL_SECTION g_imgCriticalSection;
    PROCIMGMAP g_imgMap;
    PROCICONMAP g_iconMap;
    PROCTIMEMAP g_timeMap;
    PROCHWNDMAP g_procMap;
    SIPINFO g_sipInfo;
};