// TargetVer.h : Target version configuration
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

#if !defined(__TARGETVER_H__)
#	define __TARGETVER_H__

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	ifndef VC_EXTRALEAN
#		define VC_EXTRALEAN // Exclude rarely-used stuff from Windows headers
#	endif

#	ifndef _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#		define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS // some CString constructors will be explicit
#	endif

#	ifndef _AFX_ALL_WARNINGS
#		define _AFX_ALL_WARNINGS // turns off MFC's hiding of some common and often safely ignored
								  // warning messages
#	endif

// disable MBCS warning for Visual Studio 2013 and later
#	if _MSC_VER >= 1800 && !defined(NO_WARN_MBCS_MFC_DEPRECATION)
#		define NO_WARN_MBCS_MFC_DEPRECATION
#	endif

#	if _MSC_VER > 1400
#		include <sdkddkver.h>
#	endif // _MSC_VER > 1400

#endif // __TARGETVER_H__
