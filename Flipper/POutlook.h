#pragma once
#include <pimstore.h>

void ScaleProportional(UINT uFitToWidth, UINT uFitToHeight, UINT *puWidthToScale, UINT *puHeightToScale);

struct CONTACTINFO
{
    CEOID   oid;             // ID of the contact
    HBITMAP hBitmap;         // handle to bitmap of the picture
    UINT    dxBitmap;        // bitmap width
    UINT    dyBitmap;        // bitmap height
    LPTSTR  pszPhoneNumber;  // phone number for contact
};

class CPOutlook
{
public:
	CPOutlook(void);
	~CPOutlook(void);
    HRESULT EnsurePOOM();
    void DrawBitmap(HDC hdc, int x, int y, CONTACTINFO* pContact, int nItem);
    HRESULT FindBitmap(CEOID oid, HBITMAP* phBitmap, UINT* pcxPic, UINT* pcyPic);
    HRESULT LoadContact(CEOID oid);
    IPOutlookApp2* m_pOutlookApp;           // interface to POOM
};
