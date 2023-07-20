// XTPWebView2.h : Configuration and inclusion of WebView2.h
//
// (c)1998-2023 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPWEBVIEW2_H__)
#	define __XTPWEBVIEW2_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "rpc.h"
#	include "rpcndr.h"
#	include "objidl.h"
#	include "oaidl.h"

// Defines the minimum supported _MSC_VER value that corresponds to Visual Studio 2015 for WebView2
// web browser provider to be enabled.
#	define XTP_BROWSER_WEBVIEW_MIN_VC_VERSION 1900

#	if _MSC_VER >= XTP_BROWSER_WEBVIEW_MIN_VC_VERSION
// Indicates that WebView2 web browser provider is supported by the current compiler.
#		define XTP_BROWSER_WEBVIEW_SUPPORTED
#	endif

//{{AFX_CODEJOCK_PRIVATE
#	ifdef DEFINE_ENUM_FLAG_OPERATORS
#		define XTP_DEFINE_ENUM_FLAG_OPERATORS_DISABLED
#		pragma push_macro("DEFINE_ENUM_FLAG_OPERATORS")
#		undef DEFINE_ENUM_FLAG_OPERATORS
#	endif

#	define DEFINE_ENUM_FLAG_OPERATORS(unused)

#	include "WebView2.h"

#	undef DEFINE_ENUM_FLAG_OPERATORS

#	ifdef XTP_DEFINE_ENUM_FLAG_OPERATORS_DISABLED
#		pragma pop_macro("DEFINE_ENUM_FLAG_OPERATORS")
#		undef XTP_DEFINE_ENUM_FLAG_OPERATORS_DISABLED
#	endif

#endif
//}}AFX_CODEJOCK_PRIVATE
