#include "stdafx.h"
#include <regext.h>
#include <aygshell.h>
#include <shellapi.h>
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <soundfile.h>
#include <time.h>
#include <notify.h>
#include <atltime.h>
#include <regext.h>
#include <tlhelp32.h>
#include "..\flipper\utils.h"
#pragma comment(lib, "toolhelp.lib") 
#include "TaskManager.h"
#include "ApplicationSelector.h"
#include "Resource.h"
#include "..\FlipperConfig\Anchor.h"

HMODULE CTaskManager::g_hInstance = NULL;

CTaskManager::CTaskManager(void)
{
    InitializeCriticalSection(&g_imgCriticalSection);
    g_hThumbThread = CreateThread(NULL, 0, &CTaskManager::PollThumbnails, this, 0, 0);
	SetThreadPriority(g_hThumbThread, THREAD_PRIORITY_BELOW_NORMAL);

    ZeroMemory(&g_sipInfo, sizeof(g_sipInfo));
    g_sipInfo.cbSize = sizeof(g_sipInfo);
    SHSipInfo(SPI_GETSIPINFO, 0, &g_sipInfo, 0);
}

CTaskManager::~CTaskManager(void)
{
    CleanupAppSwitchSettings();
    CleanupThumbnails();

    if (g_hThumbThread)
    {
        TerminateThread(g_hThumbThread, 0);
        CloseHandle(g_hThumbThread);
        g_hThumbThread = NULL;
    }
    DeleteCriticalSection(&g_imgCriticalSection);
}

void CTaskManager::ShowSettings(HWND hWndParent, HKEY hKey)
{
	DWORD x = DialogBoxParamW(g_hInstance, MAKEINTRESOURCE(IDD_DIALOG_TASKMANAGER), hWndParent, CTaskManager::TaskManagerProc, (LPARAM)hKey);
	DWORD error = GetLastError();
}

void CTaskManager::LoadSettings(IFlipperApp* pFlipper, HKEY hKey)
{
	g_pFlipper = pFlipper;
    CleanupAppSwitchSettings();
	LoadProcessItems(hKey, &(this->g_bAppSwitchEnableThumbnails), &g_reservedProcessList);
}

void CTaskManager::SaveProcessItems(HKEY hKey, list<ProcessRecord*>* pProcessList)
{
	list<ProcessRecord*>::iterator iter = pProcessList->begin();

	RegistrySetDWORD(hKey, NULL, L"Count", pProcessList->size());

	int i = 0;
	while (iter != pProcessList->end())
	{
		ProcessRecord* pRecord = *iter;
		WCHAR wzName[10], wzCount[5];
		DWORD dwSize = 0;
		_itow(i, wzCount, 10);

		StringCchCopy(wzName, ARRAYSIZE(wzName), wzCount);
		StringCchCat(wzName, ARRAYSIZE(wzName), L"_Name");
		RegistrySetString(hKey, NULL, wzName, pRecord->wzName);

		iter++;
		i++;
	}
}

void CTaskManager::LoadProcessItems(HKEY hKey, bool* pbEnableThumbnails, list<ProcessRecord*>* pProcessList)
{
    DWORD dwSize = sizeof(DWORD);
    DWORD dwType = 0, dwValue = 0;
    HRESULT hRes = ERROR_SUCCESS;
    if (hRes == ERROR_SUCCESS)
    {
        // Load stuff here
		RegistryGetDWORD(hKey, 0, REG_AppSwitch_EnableThumbnails, &dwValue);
        *pbEnableThumbnails = (dwValue != 0);
		
		DWORD dwCount = 0;
		RegistryGetDWORD(hKey, NULL, L"Count", &dwCount);

		for (DWORD i = 0; i < dwCount; ++i)
		{
			WCHAR wzName[10], wzCount[5];
			DWORD dwSize = 0;
			_itow(i, wzCount, 10);

			StringCchCopy(wzName, ARRAYSIZE(wzName), wzCount);
			StringCchCat(wzName, ARRAYSIZE(wzName), L"_Name");

			ProcessRecord* pRecord = new ProcessRecord();
			
			bool bDelete = true;

			if (ERROR_SUCCESS == RegistryGetString(hKey, NULL, wzName, pRecord->wzName, ARRAYSIZE(pRecord->wzName)))
			{
				bDelete = false;
			}
			if (bDelete)
			{
				delete pRecord;
			}
			else
			{
				pProcessList->push_back(pRecord);
			}
		}
    }
	//*pbEnableThumbnails = true;
}

void CTaskManager::DestroyProcessItems(list<ProcessRecord*>* pLaunchItems)
{
	list<ProcessRecord*>::iterator iter = pLaunchItems->begin();

	while (iter != pLaunchItems->end())
	{
		delete *iter;
		iter++;
	}

	pLaunchItems->clear();
}

void CTaskManager::CleanupAppSwitchSettings()
{
	DestroyProcessItems(&g_reservedProcessList);
}

void CTaskManager::CleanupThumbnails()
{
    PROCIMGMAP::iterator iter = g_imgMap.begin();
    while (iter != g_imgMap.end())
    {
        if (iter->second)
        {
            DeleteObject(iter->second);
            iter->second = NULL;
        }

        iter++;
    }
    PROCICONMAP::iterator iterIcon = g_iconMap.begin();
    while (iterIcon != g_iconMap.end())
    {
        if (iterIcon->second)
        {
            DestroyIcon(iterIcon->second);
            iterIcon->second = NULL;
        }

        iterIcon++;
    }
    g_iconMap.clear();
    g_imgMap.clear();
    g_procMap.clear();
    g_timeMap.clear();
}

node* CTaskManager::BuildNodeList(int* piNodeLen)
{
    node* pHeadNode = NULL;
    node* pCurr = NULL;
    uint uiMaxProcs = 20;

    DWORD* foundProcs = new DWORD[uiMaxProcs];

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS | TH32CS_SNAPNOHEAPS, 0);
    PROCESSENTRY32 pe = {0};
    pe.dwSize = sizeof(PROCESSENTRY32);

    int count = 0;
    if (hSnapshot != INVALID_HANDLE_VALUE)
    {
        HWND hwndCurrent = GetForegroundWindow();
        while (NULL != hwndCurrent)
        {
            long lStyle = GetWindowLong(hwndCurrent, GWL_STYLE);
            if ((0 == ((WS_POPUP | WS_MINIMIZEBOX) & lStyle) && IsWindowVisible(hwndCurrent) && GetWindowTextLength(hwndCurrent) > 0))
            {
                DWORD dwProcessId = 0;
                GetWindowThreadProcessId(hwndCurrent, &dwProcessId);
                if (Process32First(hSnapshot, &pe))
                {
                    do
                    {
                        if (pe.th32ProcessID == dwProcessId)
                        {
                            bool bFound = false;
                            for (int i = 0; i < count; ++i)
                            {
                                if (foundProcs[i] == dwProcessId)
                                {
                                    bFound = true;
                                    break;
                                }
                            }
                            if (bFound)
                            {
                                break;
                            }

							// TODO - search g_reservedProcessList instead
							/*
                            if (this->g_wzProcessList.find(pe.szExeFile) != this->g_wzProcessList.end())
                            {
                                if (this->g_wzProcessList[pe.szExeFile]->bIgnore)
                                {
                                    break;
                                }
                            }
							*/
							list<ProcessRecord*>::iterator iter = g_reservedProcessList.begin();

							bFound = false;
							while (iter != g_reservedProcessList.end())
							{
								if (0 == wcsicmp((*iter)->wzName, pe.szExeFile))
								{
									bFound = true;
									break;
								}
								iter++;
							}

							if (bFound)
							{
								break;
							}

                            node* pTemp = new node();
							ProcessItem* pProcess = new ProcessItem();

							pProcess->hWnd = hwndCurrent;
							pProcess->dwProcessId = dwProcessId;

							pTemp->pData = pProcess;

                            if (pHeadNode == NULL)
                            {
                                pHeadNode = pTemp;
                            }
                            GetWindowText(hwndCurrent, pTemp->wzText, ARRAYSIZE(pTemp->wzText));
							StringCchCopy(pTemp->wzDetails, ARRAYSIZE(pTemp->wzDetails), L"Select to Activate");
                            pTemp->prev = pCurr;
                            pTemp->next = NULL;
							pTemp->iIndex = count;

                            EnterCriticalSection(&g_imgCriticalSection);
                            if (g_imgMap.find(dwProcessId) != g_imgMap.end())
                            {
                                pTemp->hBmp = g_imgMap[dwProcessId];
                                pTemp->hIcon = NULL;
                            }
							bool bFoundIcon = false;
                            if (g_iconMap.find(dwProcessId) != g_iconMap.end())
                            {
                                pTemp->hIcon = g_iconMap[dwProcessId];
								bFoundIcon = true;
                            }
							if (!bFoundIcon && !pTemp->hIcon)
							{
								WCHAR wzClassName[MAX_PATH], wzWndText[MAX_PATH];
								GetClassName(hwndCurrent, wzClassName, MAX_PATH);
								GetWindowText(hwndCurrent, wzWndText, MAX_PATH);

								HMODULE hProcess = (HMODULE)OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, pe.th32ProcessID);
								WCHAR wzExe[MAX_PATH];
								GetModuleFileName(hProcess, wzExe, MAX_PATH);
								CloseHandle(hProcess);

								pTemp->hIcon = GetApplicationIcon(wzExe, false, pe.szExeFile, wzWndText, wzClassName);
								g_iconMap.insert(pair<DWORD, HICON>(pe.th32ProcessID, pTemp->hIcon));
							}
                            LeaveCriticalSection(&g_imgCriticalSection);

                            if (pCurr)
                            {
                                pCurr->next = pTemp;
                            }
                            pCurr = pTemp;

                            foundProcs[count] = dwProcessId;

                            count++;
                            (*piNodeLen)++;
                            break;
                        }
                    } while (Process32Next(hSnapshot, &pe));
                }
                if (count == uiMaxProcs)
                {
                    break;
                }
            }
            hwndCurrent = GetWindow(hwndCurrent, GW_HWNDNEXT);
        }
        CloseToolhelp32Snapshot(hSnapshot);
    }

    delete[] foundProcs;

    return pHeadNode;
}

void CTaskManager::EnsureCapture(HWND hwndCurrent, bool bForce)
{
    static DWORD dwLastUpdate = 0;
    if (!g_bAppSwitchEnableThumbnails)
    {
        return;
    }
    
    EnterCriticalSection(&g_imgCriticalSection);
    if ((GetTickCount() - dwLastUpdate) >= THUBMNAIL_REFRESH_INTERNVAL)
    {
        dwLastUpdate = GetTickCount();
        list<DWORD> procList;

        // Loop through existing thumbs and remove closed procs
        PROCIMGMAP::iterator iter = g_imgMap.begin();

        while (iter != g_imgMap.end())
        {
            DWORD dwProcess = iter->first;
            HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, false, dwProcess);

            if (!hProc)
            {
                // the process no longer exists
                procList.push_back(iter->first);
            }
            else
            {
                CloseHandle(hProc);
            }

            iter++;
        }

        if (procList.size() > 0)
        {
            list<DWORD>::iterator oldProcIter = procList.begin();
            while (oldProcIter != procList.end())
            {
                DWORD dwProcess = *oldProcIter;

                if (g_imgMap[dwProcess])
                {
                    DeleteObject(g_imgMap[dwProcess]);
                }
                if (g_iconMap[dwProcess])
                {
                    DestroyIcon(g_iconMap[dwProcess]);
                }
                g_imgMap.erase(dwProcess);
                g_iconMap.erase(dwProcess);
                g_timeMap.erase(dwProcess);
                g_procMap.erase(dwProcess);

                oldProcIter++;
            }
        }
    }

    DWORD dwProcessId = 0;
    GetWindowThreadProcessId(hwndCurrent, &dwProcessId);

    HWND wndTest = GetDesktopWindow();

    bool needsImage = false;
    if (g_imgMap.find(dwProcessId) == g_imgMap.end())
    {
        // add it here
        WCHAR wzClassName[MAX_PATH], wzWndText[MAX_PATH];
        GetClassName(hwndCurrent, wzClassName, MAX_PATH);
        GetWindowText(hwndCurrent, wzWndText, MAX_PATH);

        HMODULE hProcess = (HMODULE)OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, dwProcessId);
        WCHAR wzExe[MAX_PATH];
        GetModuleFileName(hProcess, wzExe, MAX_PATH);
        CloseHandle(hProcess);

        HICON hIcon = GetApplicationIcon(wzExe, false, wzExe, wzWndText, wzClassName);

        g_iconMap.insert(pair<DWORD, HICON>(dwProcessId, hIcon));
        g_imgMap.insert(pair<DWORD, HBITMAP>(dwProcessId, NULL));
        g_timeMap.insert(pair<DWORD, DWORD>(dwProcessId, GetTickCount()));
        g_procMap.insert(pair<DWORD, HWND>(dwProcessId, hwndCurrent));
        needsImage = true;
    }
    DWORD dwTime = g_timeMap[dwProcessId];
    if (bForce || ((GetTickCount() - dwTime) >= THUBMNAIL_REFRESH_INTERNVAL) || (g_procMap[dwProcessId] != hwndCurrent))
    {
        DeleteObject(g_imgMap[dwProcessId]);
        g_imgMap[dwProcessId] = NULL;
        g_timeMap[dwProcessId] = GetTickCount();

        needsImage = true;
    }
    g_procMap[dwProcessId] = hwndCurrent;

    if (needsImage)
    {
        HWND hwndDesk = HWND_DESKTOP;

        // take the screenshot here
        HDC hdcFore = GetDC(hwndDesk);
        HDC hdcThumb = CreateCompatibleDC(hdcFore);

        int width = (g_sipInfo.rcVisibleDesktop.right - g_sipInfo.rcVisibleDesktop.left);
        int height = (g_sipInfo.rcVisibleDesktop.bottom - g_sipInfo.rcVisibleDesktop.top);

        HBITMAP hbmpThumb = CreateCompatibleBitmap(hdcFore, width / 3, height / 3);
        HGDIOBJ hbmpOld = SelectObject(hdcThumb, hbmpThumb);

        BOOL bValue = StretchBlt(hdcThumb, 0, 0, width / 3, height / 3, hdcFore, g_sipInfo.rcVisibleDesktop.left, g_sipInfo.rcVisibleDesktop.top, width, height, SRCCOPY);

        SelectObject(hdcThumb, hbmpOld);
        DeleteDC(hdcThumb);
        DeleteDC(hdcFore);

        g_imgMap[dwProcessId] = hbmpThumb;
    }
    LeaveCriticalSection(&g_imgCriticalSection);
}

DWORD WINAPI CTaskManager::PollThumbnails(LPVOID lpParameter)
{
    CTaskManager* p_this = (CTaskManager*)lpParameter;
    HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, EVENT_FLIPPEREXIT);
    DWORD dwEvent = 0;

    while (WAIT_ABANDONED_0 != (dwEvent = WaitForSingleObject(hEvent, THUBMNAIL_POLL_INTERNVAL)))
    {
        // TODO, need to cleanup bitmaps every so often
        if (dwEvent == WAIT_TIMEOUT)
        {
            // TODO, put a critical section around creation of bitmaps
            HWND hwndCurrent = GetForegroundWindow();

            long lStyle = GetWindowLong(hwndCurrent, GWL_STYLE);
            if ((0 == (WS_MINIMIZEBOX & lStyle) && IsWindowVisible(hwndCurrent) && GetWindowTextLength(hwndCurrent) > 0))
            {
                p_this->EnsureCapture(hwndCurrent, false);
            }
        }
        else
        {
            break;
        }
    }
    CloseHandle(hEvent);

    return 0;
}

NodeSelectedApi(TaskManagerNodeSelected)
{
	CTaskManager* pTMan = (CTaskManager*)((FlipperPlugin*)pPlugin)->pData;
	*pbRefresh = false;

	ProcessItem* pProcess = (ProcessItem*)pNode->pData;

	HWND hWndFore = pProcess->hWnd;

	pTMan->g_pFlipper->Hide(pPlugin);
	SetForegroundWindow(hWndFore);
}

CloseOnSelectionApi(TaskManagerCloseOnSelection)
{
	CTaskManager* pTMan = (CTaskManager*)((FlipperPlugin*)pPlugin)->pData;
	return true;
}

BuildNodeListApi(TaskManagerBuildNodeList)
{
	CTaskManager* pTMan = (CTaskManager*)((FlipperPlugin*)pPlugin)->pData;
	return pTMan->BuildNodeList(piNodeLen);
}

SetSoftKeysApi(TaskManagerSetSoftKeys)
{
	StringCchCopy(pLeftSoftKey->wzText, ARRAYSIZE(pLeftSoftKey->wzText), L"Terminate");
	StringCchCopy(pRightSoftKey->wzText, ARRAYSIZE(pRightSoftKey->wzText), L"Exclude");

	pLeftSoftKey->bEnabled = true;
	pRightSoftKey->bEnabled = true;
}

SoftKeySelectedApi(TaskManagerSoftKeySelected)
{
	CTaskManager* pTMan = (CTaskManager*)((FlipperPlugin*)pPlugin)->pData;
	ProcessItem* pItem = (ProcessItem*)pNode->pData;

	if (bLeftSoftKey)
	{
		HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pItem->dwProcessId);
		if (hProcess)
		{
			TerminateProcess(hProcess, -1);
		}
		else
		{
			return;
		}
	}
	else
	{
		ProcessRecord* pRecord = new ProcessRecord();
		GetProcessName(pItem->dwProcessId, pRecord->wzName);

		pTMan->g_reservedProcessList.push_back(pRecord);

		HKEY hKey;
		pTMan->g_pFlipper->OpenRegistry(pPlugin, &hKey);
		CTaskManager::SaveProcessItems(hKey, &(pTMan->g_reservedProcessList));
		RegCloseKey(hKey);
	}
	pTMan->g_pFlipper->Refresh(pPlugin);

}

DestroyNodeListApi(TaskManagerDestroyNodeList)
{
	DestroyNodeList(pHeadNode);
}

BOOL CALLBACK CTaskManager::TaskManagerProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{
	static HWND hwndList;
	static HKEY g_hKey = NULL;
	static list<ProcessRecord*> g_processList;
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
			g_dlgAnchor.Add(IDC_LIST_TASKMANAGER, ANCHOR_ALL);
			g_dlgAnchor.Add(IDC_BUTTON_ADD, ANCHOR_RIGHT | ANCHOR_TOP);
			g_dlgAnchor.Add(IDC_BUTTON_REMOVE, ANCHOR_RIGHT | ANCHOR_TOP);
			g_dlgAnchor.Add(IDC_CHECK_ENABLETHUMBNAILS, ANCHOR_RIGHT | ANCHOR_BOTTOM | ANCHOR_LEFT);

			hwndList = GetDlgItem(hwndDlg, IDC_LIST_TASKMANAGER);

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
			mbi.hInstRes = CTaskManager::g_hInstance;
            mbi.nBmpId = 0;
            mbi.cBmpImages = 0;    

            SHCreateMenuBar(&mbi);

			RECT rect;
			GetWindowRect(hwndList, &rect);
			
			LV_COLUMN lvC;
			lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
			lvC.fmt = LVCFMT_LEFT;  // left-align column
			lvC.cx = rect.right - rect.left - 2;
			lvC.pszText = L"Name";

			HKEY hKey = (HKEY)lParam;
			g_hKey = hKey;

			bool bEnableThumbnails = false;
			CTaskManager::LoadProcessItems(hKey, &bEnableThumbnails, &g_processList);

			list<ProcessRecord*>::iterator iter = g_processList.begin();

			while (iter != g_processList.end())
			{
				ProcessRecord* pRecord = *iter;
				int index = ListBox_InsertString(hwndList, -1, pRecord->wzName);
				ListBox_SetItemData(hwndList, index, pRecord);
				iter++;
			}

			CheckDlgButton(hwndDlg, IDC_CHECK_ENABLETHUMBNAILS, bEnableThumbnails);

			SetFocus(hwndList);

            return 0;
        }
        break;
        case WM_COMMAND: 
			{
				if (HIWORD(wParam) == LBN_SELCHANGE)
				{
					int index = ListBox_GetCurSel(hwndList);
					int count = ListBox_GetCount(hwndList);

					bool bMoveUp = (index > 0);
					bool bMoveDown = (index < (count - 1));

					if (index < 0)
					{
						bMoveUp = bMoveDown = false;
					}

					EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON_REMOVE), index >= 0);
					EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON_MOVEUP), bMoveUp);
					EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON_MOVEDOWN), bMoveDown);
				}
				else if (HIWORD(wParam) == BN_CLICKED)
				{
					switch (LOWORD(wParam)) 
					{
						case IDC_BUTTON_ADD :
						{
							ProcessRecord* pRecord = new ProcessRecord();
							SetCursor(LoadCursor(NULL, IDC_WAIT));

							CApplicationSelector appSelector;

							if (appSelector.ShowDialog(CTaskManager::g_hInstance, hwndDlg, pRecord->wzName, true))
							{
								int index = ListBox_InsertString(hwndList, -1, pRecord->wzName);
								ListBox_SetItemData(hwndList, index, pRecord);
								ListBox_SetCurSel(hwndList, index);
								PostMessage(hwndDlg, WM_COMMAND, MAKELONG(0, LBN_SELCHANGE), 0);
							}
							else
							{
								delete pRecord;
							}
							SetCursor(NULL);
						}
						break;
						case IDC_BUTTON_REMOVE :
						{
							int index = ListBox_GetCurSel(hwndList);

							if (index >= 0)
							{
								ProcessRecord* pRecord = (ProcessRecord*)ListBox_GetItemData(hwndList, index);
								ListBox_DeleteString(hwndList, index);
								delete pRecord;
							}
							int count = ListBox_GetCount(hwndList);
							ListBox_SetCurSel(hwndList, min(count-1, index));
							PostMessage(hwndDlg, WM_COMMAND, MAKELONG(0, LBN_SELCHANGE), 0);
						}
						break;
						case IDC_BUTTON_MOVEUP :
						{
							int index = ListBox_GetCurSel(hwndList);
							if (index > 0)
							{
								ProcessRecord* pRecord = (ProcessRecord*)ListBox_GetItemData(hwndList, index);
								ListBox_DeleteString(hwndList, index);
								ListBox_InsertString(hwndList, index-1, pRecord->wzName);
								ListBox_SetItemData(hwndList, index-1, pRecord);
								ListBox_SetCurSel(hwndList, index-1);
								PostMessage(hwndDlg, WM_COMMAND, MAKELONG(0, LBN_SELCHANGE), 0);
							}
						}
						break;
						case IDC_BUTTON_MOVEDOWN :
						{
							int index = ListBox_GetCurSel(hwndList);
							int count = ListBox_GetCount(hwndList);
							if (index < count - 1)
							{
								ProcessRecord* pRecord = (ProcessRecord*)ListBox_GetItemData(hwndList, index);
								ListBox_DeleteString(hwndList, index);
								ListBox_InsertString(hwndList, index+1, pRecord->wzName);
								ListBox_SetItemData(hwndList, index+1, pRecord);
								ListBox_SetCurSel(hwndList, index+1);
								PostMessage(hwndDlg, WM_COMMAND, MAKELONG(0, LBN_SELCHANGE), 0);
							}
						}
						break;
					}
				}
				switch (LOWORD(wParam))
				{
					case IDM_Flipper_SET_ACCEPT :
					case IDOK:
						{
							int index = 0;

							WCHAR wzName[MAX_PATH];
							DWORD dwName = sizeof(wzName);
							DWORD dwType;
							DWORD dwEnableThumbnails = (IsDlgButtonChecked(hwndDlg, IDC_CHECK_ENABLETHUMBNAILS)) ? 1 : 0;

							while (ERROR_SUCCESS == RegEnumValue(g_hKey, index, wzName, &dwName, NULL, &dwType, NULL, NULL))
							{
								dwName = sizeof(wzName);
								RegDeleteValue(g_hKey, wzName);
							}
							int count = ListBox_GetCount(hwndList);

							list<ProcessRecord*> records;
							for (int i = 0; i < count; ++i)
							{
								ProcessRecord* pRecord = (ProcessRecord*)ListBox_GetItemData(hwndList, i);
								records.push_back(pRecord);
							}
							SaveProcessItems(g_hKey, &records);
							RegistrySetDWORD(g_hKey, NULL, REG_AppSwitch_EnableThumbnails, dwEnableThumbnails);

							EndDialog(hwndDlg, IDOK);
						}
						break;
					case IDM_Flipper_SET_CANCEL:
						{
							EndDialog(hwndDlg, IDOK);
						}
						break;
				}
            }
            break;
        case WM_DESTROY:
            {
                Sleep(0);
				CTaskManager::DestroyProcessItems(&g_processList);
            }
            break;
    } 
    return FALSE; 
}


