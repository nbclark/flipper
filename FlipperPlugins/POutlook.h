#pragma once
#include <pimstore.h>
#include <windows.h>
#include "..\flipper\utils.h"

struct CONTACTINFO
{
    CEOID   oid;             // ID of the contact
    HBITMAP hBitmap;         // handle to bitmap of the picture
    UINT    dxBitmap;        // bitmap width
    UINT    dyBitmap;        // bitmap height
    LPTSTR  pszPhoneNumber;  // phone number for contact
};

HBITMAP LoadImageFromFile(WCHAR* wzFile, UINT width, UINT height);

class CPOutlook
{
public:
	CPOutlook(void);
	~CPOutlook(void);
    HRESULT EnsurePOOM();
    void DrawBitmap(HDC hdc, int x, int y, CONTACTINFO* pContact, int nItem);
    HRESULT FindBitmap(CEOID oid, HBITMAP* phBitmap, UINT* pcxPic, UINT* pcyPic);
    HRESULT ShowContact(CEOID oid, HWND hwndParent);
	HRESULT SelectContact(HWND hWnd, WCHAR* wzName, DWORD dwName, WCHAR* wzPhone, DWORD dwPhone, CEOID* pOid);
	HRESULT UpdateContact(CEOID oid, WCHAR* wzName, size_t cchName, HBITMAP* phBmp, UINT* puiWidth, UINT* puiHeight);
	HRESULT CreateSMS(WCHAR* wzPhone, WCHAR* wzSubject);

private:
    IPOutlookApp2* m_pOutlookApp;           // interface to POOM
};
