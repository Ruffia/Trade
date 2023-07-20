// XTPWebBrowserControlSite.h : Declaration of the CXTPWebBrowserControlSite class.
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

#if !defined(__XTPWEBBROWSERCONTROLSITE_H__)

//{{AFX_CODEJOCK_PRIVATE
#	define __XTPWEBBROWSERCONTROLSITE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"

#	pragma warning(push)
// C4464: relative include path contains '..'
#	pragma warning(disable : 4464)

#	pragma push_macro("AFX_DATA")
#	undef AFX_DATA
#	define AFX_DATA AFX_DATA_IMPORT
#	if (_MSC_VER >= 1300)
#		include <..\src\mfc\occimpl.h>
#	else
#		include <..\src\occimpl.h>
#	endif
#	pragma pop_macro("AFX_DATA")

#	pragma warning(pop)

#	include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

//{{AFX_CODEJOCK_PRIVATE

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPWebBrowserProvider;
class CXTPWebBrowserAggregate;

class CXTPWebBrowserControlSite : public COleControlSite
{
	CXTPWebBrowserProvider* m_pProvider;
	CXTPWebBrowserAggregate* m_pAggregate;

public:
	CXTPWebBrowserControlSite(COleControlContainer* pCtrlCont, CXTPWebBrowserProvider* pProvider);
	virtual ~CXTPWebBrowserControlSite();

	virtual BOOL OnCreateAggregates();

	DECLARE_INTERFACE_MAP()
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"

//}}AFX_CODEJOCK_PRIVATE

#endif
