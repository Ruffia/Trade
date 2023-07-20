// CodejockSample.h
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

#if !defined(__CODEJOCKSAMPLE_H__)
#	define __CODEJOCKSAMPLE_H__

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	if !defined(_UNICODE) && (_MSC_VER < 1400)

#		if _MSC_VER > 1200					// MFC 7.0
#			include <..\src\mfc\afximpl.h> // MFC Global data
#		else
#			include <..\src\afximpl.h> // MFC Global data
#		endif

#	endif // !defined(_UNICODE) && (_MSC_VER < 1400)

#	include <XTToolkitPro.h> // Codejock ToolkitPro Component
#	include <AboutDlg.h>

#	ifdef XTP_INCLUDE_THEMEDIALOG

#		include <ThemeDef.h>
#		include <ThemeDlg.h>

#	endif // XTP_INCLUDE_THEMEDIALOG

inline void ReportNotImplemented(const char* lpCodeLocationName, int nLine)
{
	ASSERT(NULL != lpCodeLocationName);
	USES_CONVERSION;

	CString str;
	str.Format(_T("[%s:%i]: Implementation is beyond the scope of this sample"),
			   (LPCTSTR)A2T((LPSTR)lpCodeLocationName), nLine);
	AfxMessageBox(str);
}

#	if _MSC_VER < 1300
#		define REPORT_NOT_IMPLEMENTED() ReportNotImplemented(__FILE__, __LINE__)
#	else
#		define REPORT_NOT_IMPLEMENTED() ReportNotImplemented(__FUNCTION__, __LINE__)
#	endif

#endif // __CODEJOCKSAMPLE_H__
