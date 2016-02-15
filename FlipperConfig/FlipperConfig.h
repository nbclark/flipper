//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__6D15A692_17DA_427F_878B_5DC8D8531F9A__INCLUDED_)
#define AFX_STDAFX_H__6D15A692_17DA_427F_878B_5DC8D8531F9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Insert your headers here
#define WIN32_LEAN_AND_MEAN        // Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <cpl.h>
#include <prsht.h>
#include <commctrl.h>
#include "resource.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__6D15A692_17DA_427F_878B_5DC8D8531F9A__INCLUDED_)

static HANDLE   g_hSensor = NULL;
static HWND     g_hWndCpl = NULL;