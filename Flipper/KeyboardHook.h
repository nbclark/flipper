#pragma once

//used for passing to SetWindowsHookEx funtion to set a Low level (LL) keyboard hook
#define WH_KEYBOARD_LL           20
// For the low level keyboard hook, your keyboards procedures is passed a pointer to KBDLLHOOKSTRUCT instance
typedef struct {
    DWORD vkCode;
    DWORD scanCode;
    DWORD flags;
    DWORD time;
    ULONG_PTR dwExtraInfo;
} KBDLLHOOKSTRUCT, *PKBDLLHOOKSTRUCT;

// Define the function types used by hooks
typedef LRESULT    (CALLBACK* HOOKPROC)(int code, WPARAM wParam, LPARAM lParam);
typedef HHOOK     (WINAPI *_SetWindowsHookExW)(int, HOOKPROC, HINSTANCE, DWORD);
typedef LRESULT    (WINAPI *_CallNextHookEx)(HHOOK, int, WPARAM, LPARAM);
typedef LRESULT    (WINAPI *_UnhookWindowsHookEx)(HHOOK);

class CKeyboardHook
{
public:
	CKeyboardHook(void);
	~CKeyboardHook(void);
    static LRESULT CALLBACK LLKeyboardHookCallbackFunction( int nCode, WPARAM wParam, LPARAM lParam );
    BOOL ActivateKBHook();
    BOOL DeactivateKBHook();
protected :
    HINSTANCE g_hInstance;
    HINSTANCE  g_hHookApiDLL;
    HHOOK g_hInstalledLLKBDhook;
    bool g_bIsHookActive;
    
    _SetWindowsHookExW         SetWindowsHookEx;
    _UnhookWindowsHookEx    UnhookWindowsHookEx;
    _CallNextHookEx          CallNextHookEx;    
};
