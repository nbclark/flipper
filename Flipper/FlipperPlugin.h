#pragma once

struct node
{
    WCHAR wzText[40];
    WCHAR wzDetails[40];
    node* next;
    node* prev;
    HBITMAP hBmp;
    HICON hIcon;
	int iIndex;
	bool bCenter;
	void* pData;
};

struct FlipperSoftKey
{
	WCHAR wzText[20];
	bool  bEnabled;
};

#define ARRAYSIZE(s) (sizeof(s) / sizeof(s[0]))

class IFlipperApp
{
	public:
		virtual void OpenRegistry(void* pPlugin, HKEY* phKey) {}
		virtual void Redraw(void* pPlugin, node* pHeadNode, int iNodeLen) {}
		virtual void Refresh(void* pPlugin) {}
		virtual void Hide(void* pPlugin) {}
		virtual void DrawBackground(void* pPlugin, HDC hdcDest, RECT* pRect) {}
};

class CancelEvent
{
	private:
		IFlipperApp* m_hwndApp;
		bool m_bCancelled;

	public:
		CancelEvent(IFlipperApp* hwndApp)
		{
			m_hwndApp = hwndApp;
			m_bCancelled = false;
		}
		bool IsCancelled()
		{
			return m_bCancelled;
		}
		void SetCancelled(bool bCancelled)
		{
			m_bCancelled = bCancelled;
		}
};

typedef bool (WINAPI * PFN_DrawThumbnail)(void* pPlugin, node* pNode, HDC hdc, HDC hdcThumb, RECT* pRect, bool bSelected, bool* pbDrawLabels);
typedef void (WINAPI * PFN_NodeSelected)(void* pPlugin, node* pNode, bool* bRefresh);
typedef bool (WINAPI * PFN_CloseOnSelection)(void* pPlugin, node* pNode);
typedef node* (WINAPI * PFN_BuildNodeList)(void* pPlugin, CancelEvent* pCancel, int* piNodeLen, int* piThumbWidth, int* piThumbHeight, int iMaxWidth, int iMaxHeight);
typedef void (WINAPI * PFN_DestroyNodeList)(void* pPlugin, node* pHeadNode, bool bProcessExiting);
typedef void (WINAPI * PFN_SetSoftKeys)(void* pPlugin, FlipperSoftKey* pLeftSoftKey, FlipperSoftKey* pRightSoftKey);
typedef void (WINAPI * PFN_SoftKeySelected)(void* pPlugin, node* pNode, bool bLeftSoftKey);
typedef void (WINAPI * PFN_ShowSettings)(HWND hWndParent, HKEY hKey);

struct FlipperPlugin
{
	int pluginId;
	WCHAR wzName[30];
	COLORREF dwColor;
	PFN_DrawThumbnail pfnDrawThumbnail;
	PFN_NodeSelected pfnNodeSelected;
	PFN_CloseOnSelection pfnCloseOnSelection;
	PFN_BuildNodeList pfnBuildNodeList;
	PFN_DestroyNodeList pfnDestroyNodeList;
	PFN_SetSoftKeys pfnSetSoftKeys;
	PFN_SoftKeySelected pfnSoftKeySelected;
	PFN_ShowSettings pfnShowSettings;
	void* pData;
	HMODULE hLib;
};

// You must implement these methods
typedef bool (WINAPI * PFN_QueryAvailablePlugins)(FlipperPlugin* pPluginList, DWORD* dwLength);
typedef bool (WINAPI * PFN_CreatePluginInstance)(IFlipperApp* pFlipper, int pluginId, FlipperPlugin** ppPlugin, HKEY hKey, int thumbWidth, int thumbHeight);
typedef void (WINAPI * PFN_DestroyPluginInstance)(FlipperPlugin* pPlugin);
typedef PFN_ShowSettings (WINAPI * PFN_GetSettingsProc)(int iPluginId);

#define DrawThumbnailApi(szFunction) \
	extern "C" __declspec(dllexport) bool WINAPI szFunction(void* pPlugin, node* pNode, HDC hdc, HDC hdcThumb, RECT* pRect, bool bSelected, bool* pbDrawLabels)
#define NodeSelectedApi(szFunction) \
	extern "C" __declspec(dllexport) void WINAPI szFunction(void* pPlugin, node* pNode, bool* pbRefresh)
#define CloseOnSelectionApi(szFunction) \
	extern "C" __declspec(dllexport) bool WINAPI szFunction(void* pPlugin, node* pNode)
#define BuildNodeListApi(szFunction) \
	extern "C" __declspec(dllexport) node* WINAPI szFunction(void* pPlugin, CancelEvent* pCancel, int* piNodeLen, int* piThumbWidth, int* piThumbHeight, int iMaxWidth, int iMaxHeight)
#define DestroyNodeListApi(szFunction) \
	extern "C" __declspec(dllexport) void WINAPI szFunction(void* pPlugin, node* pHeadNode, bool bProcessExiting)
#define SetSoftKeysApi(szFunction) \
	extern "C" __declspec(dllexport) void WINAPI szFunction(void* pPlugin, FlipperSoftKey* pLeftSoftKey, FlipperSoftKey* pRightSoftKey)
#define SoftKeySelectedApi(szFunction) \
	extern "C" __declspec(dllexport) void WINAPI szFunction(void* pPlugin, node* pNode, bool bLeftSoftKey)
#define ShowSettingsApi(szFunction) \
	extern "C" __declspec(dllexport) void WINAPI szFunction(WCHAR* wzRegistryPath)
