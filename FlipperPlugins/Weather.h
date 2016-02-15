#pragma once
#include "..\Flipper\Utils.h"
#include "..\Flipper\FlipperPlugin.h"
#include "DestroyablePlugin.h"
#include <msxml.h>
#include <atlbase.h>
#include <wininet.h>
#include <urlmon.h>

#define REG_ZipCode L"ZipCode"
#define WEATHER_POLL_INTERVAL (1000*60*60*2)
#define WEATHER_POLL_INTERVAL_SECONDS (WEATHER_POLL_INTERVAL / 1000)
#define WEATHER_SAVE_PATH L"mobileSrc\\Flipper\\Weather"
#define WEATHER_IMAGES_PATH L"images"
#define WEATHER_SERVICE_URL L"http://www.google.com/ig/api?weather=%s"
//L"http://www.webservicex.net/WeatherForecast.asmx/GetWeatherByZipCode?ZipCode=%s"

typedef CComPtr<IXMLDOMDocument>    XMLDOMDocumentPtr;
typedef CComPtr<IXMLDOMNode>        XMLNodePtr;
typedef CComPtr<IXMLDOMElement>     XMLElementPtr;
typedef CComPtr<IXMLDOMNodeList>    XMLNodeListPtr;

struct ForecastDay
{
	WCHAR wzDay[100];
	WCHAR wzCondition[100];
	WCHAR wzImage[MAX_PATH];
	short shMaxTempF;
	short shMinTempF;
	short shMaxTempC;
	short shMinTempC;
};

class CWeather : CDestroyablePlugin
{
public:
	CWeather(void* pPlugin);
	~CWeather(void);

public:
	static HMODULE g_hInstance;
	static void ShowSettings(HWND hWndParent, HKEY hKey);
    static DWORD WINAPI PollWeather(LPVOID lpParameter);
    node* BuildNodeList(int* piNodeLen, int* piThumbWidth, int* piThumbHeight, int iMaxWidth, int iMaxHeight);
    void UpdateWeather();
    void LoadSettings(IFlipperApp* pFlipper, HKEY hKey);

	static bool DownloadItem(WCHAR* wzUrl, WCHAR* wzPath, bool bOverwriteExisting, bool requireXml);
	bool RefreshWeatherItems();
	bool DrawThumbnail(void* pPlugin, node* pNode, HDC hdc, HDC hdcThumb, RECT* pRect, bool bSelected, bool* pbDrawLabels);

	void LoadWeatherItems(list<ForecastDay*>* pForecastDays);
	void DeserializeXml(list<ForecastDay*>* pForecastList);
	void CreateNodes(list<ForecastDay*>* pForecastList);

	static void DestroyWeatherItems(list<ForecastDay*>* pForecastDays);
	static BOOL WeatherProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
    static DWORD WINAPI SearchLocation(LPVOID lpParameter);

	IFlipperApp* g_pFlipper;
	void* g_pPlugin;
	WCHAR* g_wzMessage;
	static WCHAR* g_wzError;
	static WCHAR* g_wzUpdating;

private:

	HANDLE g_hSemUpdate;
    HANDLE g_hThumbThread;
	list<ForecastDay*> g_forecastDays;
	WCHAR g_zipCode[200];
	WCHAR g_wzLocation[250];
	WCHAR g_wzWeatherUrl[MAX_PATH];
	WCHAR g_wzWeatherPath[MAX_PATH];
	WCHAR g_wzSettingsPath[MAX_PATH];
	WCHAR g_wzImagesPath[MAX_PATH];
	FILETIME g_lastUpdated;
	node g_updatingNode;
	node g_forecastNodes[7];
	int g_iNodeCount;
	bool g_bUpdating;
	bool g_bCelsius;

	hash_map<WCHAR*, HBITMAP, strltpred> g_cachedImages;
};

DrawThumbnailApi(WeatherDrawThumbnail);
BuildNodeListApi(WeatherBuildNodeList);
DestroyNodeListApi(WeatherDestroyNodeList);
NodeSelectedApi(WeatherNodeSelected);
SetSoftKeysApi(WeatherSetSoftKeys);
SoftKeySelectedApi(WeatherSoftKeySelected);