#include "StdAfx.h"
#include "KeyboardHook.h"

CKeyboardHook::CKeyboardHook(void)
{
}

CKeyboardHook::~CKeyboardHook(void)
{
	DeactivateKBHook();
}

LRESULT CKeyboardHook::LLKeyboardHookCallbackFunction( int nCode, WPARAM wParam, LPARAM lParam ) 
{
    static DWORD _dwDownTick = 0, _dwDownArrowTick = 0;

	// TODO: fix this
    CKeyboardHook* p_this = (CKeyboardHook*)GetWindowLong(NULL, GWL_USERDATA) ;

	KBDLLHOOKSTRUCT* pKeyHook = ((KBDLLHOOKSTRUCT*)lParam);

    return p_this->CallNextHookEx(p_this->g_hInstalledLLKBDhook, nCode, wParam, lParam);
    return 1;
}

BOOL CKeyboardHook::ActivateKBHook()
{
    //we need to manually load these standard Win32 API calls
    //MSDN states that these aren't supported in WinCE
    SetWindowsHookEx        = NULL;
    CallNextHookEx            = NULL;
    UnhookWindowsHookEx        = NULL;

    //now load the coredll.dll
    g_hHookApiDLL = LoadLibrary(_T("coredll.dll"));
    if(g_hHookApiDLL == NULL) 
    {
        //something is awfully wrong
        //the dll has to be present
        return false;
    }
    else
    {
        //load the SetWindowsHookEx API call
        //the SetWindowsHookEx function installs an application-defined hook procedure into a hook chain. 
        //You would install a hook procedure to monitor the system for certain types of events.
        //here we use use the hook to monitor kyeboard events
        SetWindowsHookEx = (_SetWindowsHookExW)GetProcAddress(g_hHookApiDLL, _T("SetWindowsHookExW"));
        if(SetWindowsHookEx == NULL)
        {
            //this means that MS has really stopped supporting this API in WinCE
            return false;
        }
        else
        {
            //install the KB hook
            //the hande needs to be saved for default processing of the events and to uninstall the hook, once we are done with it
            g_hInstalledLLKBDhook = SetWindowsHookEx(WH_KEYBOARD_LL, LLKeyboardHookCallbackFunction, g_hInstance, 0);
            if(g_hInstalledLLKBDhook == NULL)
            {
                return false;
            }
        }

        //load CallNextHookEx() API call
        //the CallNextHookEx function passes the hook information to the next hook procedure in the current hook chain. 
        //we use this call for default processing of events.
        CallNextHookEx = (_CallNextHookEx)GetProcAddress(g_hHookApiDLL, _T("CallNextHookEx"));
        if(CallNextHookEx == NULL)
        {
            return false;
        }

        //load UnhookWindowsHookEx() API
        //the UnhookWindowsHookEx function removes a hook procedure installed in a hook chain by the SetWindowsHookEx function.
        //we use this call to unistall the hook.
        UnhookWindowsHookEx = (_UnhookWindowsHookEx)GetProcAddress(g_hHookApiDLL, _T("UnhookWindowsHookEx"));
        if(UnhookWindowsHookEx == NULL) 
        {
            return false;
        }
    }
    //all the APIs are loaded and the application is hooked
    return true;
}

/** 
*    Function Name:DeactivateKBHook
*    
*    Function Desc:Uninstall the KB hook
*
*    Parameters:
*                 none
*    Returns:    
*                 true if we exit gracefully
*
*    Author:         Prathamesh Kulkarni
**/
BOOL CKeyboardHook::DeactivateKBHook()
{
    //unload the hook
    if(g_hInstalledLLKBDhook != NULL)
    {
        UnhookWindowsHookEx(g_hInstalledLLKBDhook);
        g_hInstalledLLKBDhook = NULL;
    }

    //unload the coredll.dll
    if(g_hHookApiDLL != NULL)
    {
        FreeLibrary(g_hHookApiDLL);
        g_hHookApiDLL = NULL;
    }
    
    //we have terminated gracefully
    return true;
}