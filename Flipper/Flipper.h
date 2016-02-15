#pragma once
#include "utils.h"
#include <aygshell.h>
#include <ddraw.h>
#include "FlipperPlugin.h"
#pragma comment(lib, "ddraw.lib") 

#define WM_SHOWFLIPPER			(WM_USER + 1)
#define WM_HIDEFLIPPER			(WM_USER + 2)
#define WM_SELECTFLIPPER		(WM_USER + 3)
#define WM_NODESELECTED			(WM_USER + 4)
#define WM_SHOWTABS				(WM_USER + 5)
#define WM_SELECTTAB			(WM_USER + 6)
#define WM_DATAREADY			(WM_USER + 7)
#define WM_SELECTIONCLOSE		(WM_USER + 8)
#define WM_WILLCHANGESELECTION  (WM_USER + 9)
#define WM_PANELHIDING			(WM_USER + 10)
#define WM_SETTINGSCHANGED		(WM_USER + 11)

#define FLIPTAB_SIZE 30
#define FLIPTAB_DARKEN_AMOUNT (-20)

typedef stdext::hash_map<DWORD, HBITMAP> PROCIMGMAP;
typedef stdext::hash_map<DWORD, HICON> PROCICONMAP;
typedef stdext::hash_map<DWORD, DWORD> PROCTIMEMAP;
typedef stdext::hash_map<DWORD, HWND> PROCHWNDMAP;

#define THUBMNAIL_POLL_INTERNVAL (1000 * 1)
#define THUBMNAIL_REFRESH_INTERNVAL (1000 * 30)
#define THUMBNAIL_CAPTURE_WIDTH 
#define BLT_TRANSPARENT_COLOR	RGB(0xff, 0, 0xFF)

struct FlipperProcess
{
    bool bIgnore;
    bool bCanClose;
    bool bCanMinimize;
};

typedef bool (WINAPI * PFN_ClickAction)(void* pAltTab, HWND hwndDlg, node** ppNode);
class CFlipper : IFlipperApp
{
public:
	static CFlipper* g_pInstance;

    CFlipper(void);
    ~CFlipper(void);
    void Initialize();
    void ShowIndex(int i);
    void ShowLauncher();
    void ShowAltTab(int iDir);

    void OpenRegistry(void* pPlugin, HKEY* phKey);
	void Redraw(void* pPlugin, node* pHeadNode, int iNodeLen);
    void Hide(void* pPlugin);
	void Refresh(void* pPlugin);
	void DrawBackground(void* pPlugin, HDC hdcDest, RECT* pRect);

    void Hide();
	void Refresh();

	void Minimize();
	virtual void ShowTab(int index);//FlipperPlugin* pPlugin);
    bool MinimizeProcess(HWND hwndTop, WCHAR* wzProcess, WCHAR* wzClass);
    bool CloseProcess(HWND hwndTop, WCHAR* wzProcess, WCHAR* wzClass);
    void LoadSettings();
	void CancelBuild();
	static HRGN CreateRegionFromHDC(HDC hdc, int startX, int startY, int endX, int endY, COLORREF clrTransparent);

private:
    static LRESULT CALLBACK FlipperContentProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK FlipperTabProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
    node* BuildNodeList(int* piNodeLen);
    void EnsureCapture(HWND hwndCurrent, bool bForce);
    void LoadFlipperSettings();
    void CleanupFlipperSettings();
    void CleanupThumbnails();
    HRESULT Flip(HWND hwnd);
    HRESULT RestoreAll();
    BOOL InitSurfaces(DWORD dwColor, int x, int y, int width, int height, bool blendOnly);
    void ReleaseAllObjects();
	void UpdateLoadingScreen(WCHAR* wzText);
    void UpdateFrameAnimateIn(int iAnimateIterations, int iIteration);
    void UpdateFrameAnimateOut(HDC hdcThumb, int iAnimateIterations, int iIteration);
    void UpdateFrameScroll(FlipperPlugin* pPlugin, LPDIRECTDRAWSURFACE pBuffer, PFN_DrawThumbnail pfnDrawThumbnail, HDC hdcThumb, HWND hWnd, int xDiff, int yDiff, int xPos, int yPos, node* pHead, node** ppNewHead, int iActive, node** pUnderMouse, PFN_ClickAction* pClickAction, int* piDistanceFromCenter, int* piMouseFromCenter, bool bFinalStop);
	void DrawBackground(int transparentWidth);
	void DrawBackground(HDC hdcDest, RECT* pRect);
	void CreateTileBuffer(FlipperPlugin* pPlugin, PFN_DrawThumbnail pfnDrawThumbnail, HDC hdc, HDC hdcThumb, HWND hWnd, node* pHead);

    inline void DrawUpdatedFrame(HWND hwndDlg);
    inline void ResetAutoClose(HWND hwndDlg);
    inline void AbortAutoClose(HWND hwndDlg);
    void LoadFlipperDisplaySettings();

	WCHAR* GetCurrentTabName();
	static DWORD WINAPI LoadNodes(LPVOID lpParameter);

public:
    // these are appswitch variables
    bool g_bFlipperAutoClose;
    bool g_bFlipperEnableShrinkAnimation;
    bool g_bFlipperEnableScrollAnimation;
    bool g_bFlipperEnableThumbnails;
    int g_iDir;
	int g_iTabIndex;
	int g_iPadding;
    HWND g_hwndAltTab, g_hwndAltTabBorder;

	DWORD g_dwTabSize, g_dwTabAlign;
	DWORD g_dwTabWidth, g_dwTabHeight;

private:
	HANDLE g_hBuildNodeThread;
	FlipperPlugin* g_pPlugin;
	HANDLE g_hEventCanBuildNode, g_hEventBuildNodeReady;
	CancelEvent* g_pCancelEvent;
    void* g_pTouchHandler;
    WNDCLASS g_wndClassAltTab;
    WNDCLASS g_wndClassBorder;
    HINSTANCE g_hInstance;
    HANDLE hFinishedEvent;
    list<FlipperPlugin*> g_tabList;

    DWORD g_dwAnimateInterval;
    DWORD g_dwDeceleration;

    int g_width;
    int g_height;
    int g_vertShift;
    int g_bigThumbHeight;
    int g_bigThumbWidth;
    int g_bigThumbTop;
    int g_bigThumbMid;
    int g_bigThumbLeft;
    int g_labelWidth;
    int g_labelHeight;
    int g_labelSpacing;
    int g_labelLeft;
    int g_labelTop;
    int g_labelFont;
    int g_windowHeight;

    int g_buttonWidth;
    int g_buttonHeight;
    int g_buttonSpacing;
    int g_buttonLeft;
    int g_buttonTop;
    int g_buttonFont;

    SIPINFO g_sipInfo;

    LPDIRECTDRAW                g_pDD;
    LPDIRECTDRAWSURFACE         g_pDDSPrimary;
    LPDIRECTDRAWSURFACE         g_pDDSBuffer;
    LPDIRECTDRAWSURFACE         g_pDDSBackground;
    LPDIRECTDRAWSURFACE         g_pDDSBackClear;
    BOOL                        g_bActive;
    LPDIRECTDRAWCLIPPER         g_pDDClipper;
    HDC                         g_hdcLabel;
    HBITMAP                     g_hbmpLabel, g_hbmpLabelOld;
};