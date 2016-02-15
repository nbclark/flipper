#include "StdAfx.h"
#include "POutlook.h"
#include "Image.h"

#include <initguid.h>
#define INITGUID
#include <imaging.h>
#include <windowsx.h>

#define _ErrorLabel Error

#define CHR(hResult) \
    if(FAILED(hResult)) { hr = (hResult); goto _ErrorLabel;} 

#define CPR(pPointer) \
    if(NULL == (pPointer)) { hr = (E_OUTOFMEMORY); goto _ErrorLabel;} 

#define CBR(fBool) \
    if(!(fBool)) { hr = (E_FAIL); goto _ErrorLabel;} 

#define ARRAYSIZE(s) (sizeof(s) / sizeof(s[0]))

#define RELEASE_OBJ(s)  \
    if (s != NULL)      \
    {                   \
        s->Release();   \
        s = NULL;       \
    }

HRESULT  GetStreamSize(IStream* pStream, ULONG* pulSize);
HBITMAP  HBITMAPFromImage(IImage * pImage, COLORREF crBackColor);
HRESULT  GetBitmapFromStream(IStream* pStream, HBITMAP* phBitmap, UINT* iWidth, UINT* iHeight);
void     DrawRect(HDC hdc, LPRECT prc, COLORREF clr);

CPOutlook::CPOutlook(void)
{
	m_pOutlookApp = NULL;
}

CPOutlook::~CPOutlook(void)
{
}

// **************************************************************************
// Function Name: CPictureDial::EnsurePOOM
// 
// Purpose: Instantiate the POOM object if necessary
//
// Arguments:
//    NONE
//
// Return Values:
//    HRESULT - S_OK if success, failure code if not
//

HRESULT CPOutlook::EnsurePOOM()
{
    HRESULT hr;

    if (m_pOutlookApp == NULL)
    {
        hr = CoCreateInstance(__uuidof(Application), NULL, CLSCTX_INPROC_SERVER,
                              __uuidof(IPOutlookApp2), (LPVOID*) &m_pOutlookApp);
        CHR(hr);

        hr = m_pOutlookApp->Logon(NULL);
        CHR(hr);
    }

    hr = S_OK;

Error:
    if (FAILED(hr))
    {
        // If we failed to log on, don't keep the object around
        RELEASE_OBJ(m_pOutlookApp);
    }

    return hr;
}


// **************************************************************************
// Function Name: CPictureDial::DrawBitmap
// 
// Purpose: Draws the bitmap within the entry square
//
// Arguments:
//    IN HDC hdc - DC for drawing
//    IN int x   - horizontal location of upper left hand corner of drawing area
//    IN int y   - vertical location of upper left hand corner of drawing area
//    IN CONTACTINFO pContact - contains bitmap info for the contact to draw
//    IN int nItem - index of current item
//
// Return Values:
//    NONE
//

void CPOutlook::DrawBitmap(HDC hdc, int x, int y, CONTACTINFO* pContact, int nItem)
{
//    HRESULT  hr;
//    HBITMAP  hOldBitmap = NULL;
//    HDC      hdcSrc = NULL;
//    RECT     rc = {x, y, x+m_uItemWidth, y+m_uItemHeight};
//    COLORREF clrBack;
//    UINT     uItemWidth  = m_uItemWidth-BORDERSIZE-CXGUTTER*2; 
//    UINT     uItemHeight = m_uItemHeight-BORDERSIZE-CYGUTTER*2; 
//    UINT     xOffset = (uItemWidth  >= pContact->dxBitmap ? (uItemWidth-pContact->dxBitmap)/2 : 0);
//    UINT     yOffset = (uItemHeight >= pContact->dyBitmap ? (uItemHeight-pContact->dyBitmap)/2 : 0);
//
//    hdcSrc = CreateCompatibleDC(hdc);
//    CBR(hdcSrc != NULL);
//
//    hOldBitmap = SelectBitmap(hdcSrc, pContact->hBitmap);
//    CBR(hOldBitmap != NULL);
//
//    // Select the color depending on if we're drawing the active entry
//    clrBack = (m_iSel == nItem ? GetSysColor(COLOR_HIGHLIGHT) : GetSysColor(COLOR_WINDOW));
//
//    // Color the background first
//    DrawRect(hdc, &rc, clrBack);
//    
//    // Draw the image
//    BitBlt(hdc, x+CXGUTTER+xOffset, y+CYGUTTER+yOffset, pContact->dxBitmap, pContact->dyBitmap, hdcSrc, 0, 0, SRCCOPY);
//
//Error:    
//    if (hOldBitmap)
//    {
//        SelectBitmap(hdcSrc, hOldBitmap);
//    }
//
//    if (hdcSrc)
//    {
//        DeleteDC(hdcSrc);
//    }

    return;
}


// **************************************************************************
// Function Name: CPictureDial::FindBitmap
// 
// Purpose: Given an oid, find the bitmap and its dimensions
//
// Arguments:
//    IN  CEOID    oid      - oid of the contact
//    IN  HBITMAP* phBitmap - bitmap of the contact's picture
//    OUT UINT*    puWidth  - width of the bitmap
//    OUT UINT*    puHeight - height of the bitmap
//
// Return Values:
//    HRESULT - S_OK if success, failure code if not
//

HRESULT CPOutlook::FindBitmap
(
    CEOID    oid,
    HBITMAP* phBitmap,
    UINT*    puWidth,
    UINT*    puHeight
)
{
    HRESULT   hr;
    IItem*    pItem = NULL;
    IStream*  pStream = NULL;
    ULONG     ulSize;

    // Make sure we can access POOM
    hr = EnsurePOOM();
    CHR(hr);

    hr = m_pOutlookApp->GetItemFromOidEx(oid, 0, &pItem);
    CHR(hr);
    
    // Extract the picture from the contact
    hr = pItem->OpenProperty(PIMPR_PICTURE, GENERIC_READ, &pStream);
    CHR(hr);
    
    hr = GetStreamSize(pStream, &ulSize);
    CHR(hr);

    // In some cases, the property may exist even if there is no picture.
    // Make sure we can access the stream and don't have a 0 byte stream
    CBR(ulSize > 0);

    hr = GetBitmapFromStream(pStream, phBitmap, puWidth, puHeight);
    CHR(hr);

Error:
    RELEASE_OBJ(pItem);
    RELEASE_OBJ(pStream);

    return hr;
}



// **************************************************************************
// Function Name: CPictureDial::LoadContact
// 
// Purpose: Load a single contact from the registry
//
// Arguments:
//    IN int  nItem - index of the contact to load
//
// Return Values:
//    HRESULT - S_OK if success, failure code if not
//

HRESULT CPOutlook::LoadContact(CEOID oid)
{
    HRESULT hr;
	/*
    LRESULT lResult;
    HKEY    hKey;
    TCHAR   szKey[ARRAYSIZE(c_szRegSaveFormat)+10];
    TCHAR   szPhoneNumber[MAXPHONE];
    LPTSTR  pszPhoneNumber = NULL;
    UINT    cchPhoneNumber;
    CEOID   oid;
    HBITMAP hBitmap = NULL;
    UINT    dx = m_uItemWidth-BORDERSIZE-CXGUTTER*2;
    UINT    dy = m_uItemHeight-BORDERSIZE-CYGUTTER*2;
    DWORD   dwSize;

    StringCchPrintf(szKey, ARRAYSIZE(szKey), c_szRegSaveFormat, nItem);

    lResult = RegOpenKeyEx(HKEY_CURRENT_USER, szKey, 0, KEY_QUERY_VALUE, &hKey);
    CBR(lResult == ERROR_SUCCESS);

    dwSize = sizeof(szPhoneNumber);
    lResult = RegQueryValueEx(hKey, c_szRegValuePhoneNumber, NULL, NULL, (LPBYTE) szPhoneNumber, &dwSize);
    CBR(lResult == ERROR_SUCCESS);

    dwSize = sizeof(oid);
    lResult = RegQueryValueEx(hKey, c_szRegValueOID, NULL, NULL, (LPBYTE) &oid, &dwSize);
    CBR(lResult == ERROR_SUCCESS);

    cchPhoneNumber = lstrlen(szPhoneNumber)+1;
    pszPhoneNumber = new TCHAR[cchPhoneNumber];
    CPR(pszPhoneNumber);

    StringCchCopy(pszPhoneNumber, cchPhoneNumber, szPhoneNumber);

    FindBitmap(oid, &hBitmap, &dx, &dy);

    m_rgContact[nItem].oid = oid;
    m_rgContact[nItem].pszPhoneNumber = pszPhoneNumber;
    m_rgContact[nItem].hBitmap = hBitmap;
    m_rgContact[nItem].dxBitmap = dx;
    m_rgContact[nItem].dyBitmap = dy;
    hBitmap = NULL;
    pszPhoneNumber = NULL;

    hr = S_OK;

Error:
    delete [] pszPhoneNumber;

    if (hKey)
    {
        RegCloseKey(hKey);
    }

    if (hBitmap)
    {
        DeleteBitmap(hBitmap);
    }
	*/
    return hr;
}

// **************************************************************************
// Function Name: GetStreamSize
// 
// Purpose: Given an IStream, returns the size of the stream.  This is needed
//          for streams that do not support the Stat method
//
// Arguments:
//    IN  IStream*  pStream - stream to determine size for
//    OUT ULONG*    pulSize - size of stream
//
// Return Values:
//    HRESULT - S_OK if success, failure code if not
//
// Side Effects:
//    The stream pointer always resets to the beginning
//

HRESULT GetStreamSize(IStream* pStream, ULONG* pulSize)
{
    HRESULT hr;
    LARGE_INTEGER  li = {0};
    ULARGE_INTEGER uliZero = {0};
    ULARGE_INTEGER uli;

    CBR(pStream != NULL && pulSize != NULL);

    hr = pStream->Seek(li, STREAM_SEEK_END, &uli);
    CHR(hr);

    *pulSize = uli.LowPart;
    hr = S_OK;

Error:
    if (SUCCEEDED(hr))
    {
        // Move the stream back to the beginning of the file
        hr = pStream->Seek(li, STREAM_SEEK_SET, &uliZero);
    }

    return hr;
}


// **************************************************************************
// Function Name: HBITMAPFromImage
// 
// Purpose: Convert IImage to HBITMAP.  If bitmap has transparency, the
//    background will be filled with the color passed in
//
// Arguments:
//    IN  IImage*   pImage      - pointer to the IImage
//    IN  COLORREF  crBackColor - color of the background
//
// Return Values:
//    HRESULT - S_OK if success, failure code if not
//
HBITMAP HBITMAPFromImage
(
    IN IImage * pImage,
    IN COLORREF crBackColor
    )
{
    HRESULT    hr;
    HBITMAP    hbmResult = NULL;
    ImageInfo  ii;
    HDC        hDC = NULL;
    HBITMAP    hbmNew = NULL;
    void *     pv;
    BITMAPINFO bmi = { 0 };
    HBITMAP    hbmOld = NULL;
    RECT       rc = { 0 };

    CBR(pImage != NULL);

    // Get image width/height
    hr = pImage->GetImageInfo(&ii);
    CHR(hr);

    // Create HDC
    hDC = CreateCompatibleDC(NULL);
    CBR(hDC != NULL);

    // Create DIB section
    bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth       = ii.Width;
    bmi.bmiHeader.biHeight      = ii.Height;
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biBitCount    = (SHORT) max(16, GetDeviceCaps(hDC, BITSPIXEL));
    bmi.bmiHeader.biCompression = BI_RGB;

    hbmNew = CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS, &pv, NULL, 0);
    CBR(hbmNew != NULL);

    // Select DIB into DC
    hbmOld = (HBITMAP)SelectObject(hDC, hbmNew);

    rc.right = ii.Width;
    rc.bottom = ii.Height;

    // Clear the bitmap using the background color
    //DrawRect(hDC, &rc, crBackColor); 

    // Draw into DC/DIB
    hr = pImage->Draw(hDC, &rc, NULL);
    CHR(hr);

    hbmResult = hbmNew;
    hbmNew = NULL;

Error:
    if (hbmNew)
    {
        DeleteObject(hbmNew);       
    }

    if (hDC)
    {
        if (hbmOld)
        {
            SelectObject(hDC, hbmOld);
        }

        DeleteDC(hDC);
    }

    return hbmResult;
}


// **************************************************************************
// Function Name: ScaleProportional
// 
// Purpose: Scale the width and height to fit the given width and height
//          but maintain the proportion
//
// Arguments:
//    IN     UINT  uFitToWidth     - width of source image
//    IN     UINT  uFitToHeight    - height of source image
//    IN/OUT UINT* puWidthToScale  - width of image to scale to
//    IN/OUT UINT* puHeightToScale - height of image to scale to
//
// Return Values:
//    HRESULT - S_OK if success, failure code if not
//
void ScaleProportional(UINT uFitToWidth, UINT uFitToHeight, UINT *puWidthToScale, UINT *puHeightToScale)
{
    HRESULT hr;

    CBR(puWidthToScale != NULL && puHeightToScale != NULL);

    // Scale (*puWidthToScale, *puHeightToScale) to fit within (uFitToWidth, uFitToHeight), while
    // maintaining the aspect ratio
    int nScaledWidth = MulDiv(*puWidthToScale, uFitToHeight, *puHeightToScale);

    // If we didn't overflow and the scaled width does not exceed bounds
    if (nScaledWidth >= 0 && nScaledWidth <= (int)uFitToWidth)
    {
        *puWidthToScale  = nScaledWidth;
        *puHeightToScale = uFitToHeight;
    }
    else
    {
        *puHeightToScale = MulDiv(*puHeightToScale, uFitToWidth, *puWidthToScale);
        
        // The height *must* be within the bounds [0, uFitToHeight] since we overflowed
        // while fitting to height
        ASSERT(*puHeightToScale >= 0 && *puHeightToScale <= uFitToHeight);
        
        *puWidthToScale  = uFitToWidth;
    }

Error:
    return;
}


// **************************************************************************
// Function Name: GetBitmapFromStream
// 
// Purpose: Convert an IStream to an HBITMAP and return the new dimensions
//
// Arguments:
//    IN     UINT  uFitToWidth     - width of source image
//    IN     UINT  uFitToHeight    - height of source image
//    OUT UINT* puWidth  - width of image to scale to
//    OUT UINT* puHeight - height of image to scale to
//
// Return Values:
//    HRESULT - S_OK if success, failure code if not
//
HRESULT GetBitmapFromStream(IStream* pStream, HBITMAP* phBitmap, UINT* puWidth, UINT* puHeight)
{
    HRESULT hr;
    HBITMAP           hBitmap = NULL;

    IImagingFactory*  pFactory = NULL;
    IImage*           pImage   = NULL;
    IImage*           pThumbnail = NULL;
    ImageInfo         imgInfo = {0};

    CBR(pStream != NULL && phBitmap != NULL && puWidth != NULL && puHeight != NULL);

    // Use a little imaging help
    hr = CoCreateInstance(CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void**) &pFactory);
    CHR(hr);
    
    hr = pFactory->CreateImageFromStream(pStream, &pImage);
    CHR(hr);

    hr = pImage->GetImageInfo(&imgInfo);
    CHR(hr);
    CBR(imgInfo.Width > 0 && imgInfo.Height > 0);

    // Scale to the new size
    ScaleProportional(*puWidth, *puHeight, &imgInfo.Width, &imgInfo.Height);

    // Get the new image
    hr = pImage->GetThumbnail(imgInfo.Width, imgInfo.Height, &pThumbnail);
    CHR(hr);

    // Convert this to HBITMAP, our target format
    hBitmap = HBITMAPFromImage(pThumbnail, RGB(255,255,255));
    CBR(hBitmap != NULL);

    *puWidth = imgInfo.Width;
    *puHeight = imgInfo.Height;
    *phBitmap = hBitmap;
    hBitmap = NULL;

Error:
    RELEASE_OBJ(pFactory);
    RELEASE_OBJ(pImage);
    RELEASE_OBJ(pThumbnail);

    if (hBitmap)
    {
        DeleteBitmap(hBitmap);
    }

    return hr;

}