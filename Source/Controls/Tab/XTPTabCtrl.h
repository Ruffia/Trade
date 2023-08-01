// XTPTabCtrl.h interface for the CXTPTabCtrl class.
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
#if !defined(__XTPTABCTRL_H__)
#	define __XTPTABCTRL_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPTabCtrlButtons;

DECLARE_TABCTRLEX_BASE(CXTPTabCtrlBase, CTabCtrl, CXTPTabExBase)

//===========================================================================
// Summary:
//     CXTPTabCtrl is a multiple inheritance class derived from CTabCtrl and
//     CXTPTabCtrlBase. It is used to create a CXTPTabCtrl class object. See
//     CXTPTabCtrlBase for additional functionality.
//===========================================================================
class _XTP_EXT_CLASS CXTPTabCtrl : public CXTPTabCtrlBase
{
	DECLARE_DYNAMIC(CXTPTabCtrl)

	friend class CXTPTabExBase;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabCtrl object.
	//-----------------------------------------------------------------------
	CXTPTabCtrl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTabCtrl object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPTabCtrl();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable/disable tooltip usage.
	// Parameters:
	//     bEnable - TRUE to enable tooltip usage, FALSE to disable.
	// Returns:
	//     TRUE if the tooltip control was found and updated, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL EnableToolTips(BOOL bEnable);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_VIRTUAL(CXTPTabCtrl)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPTabCtrl)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	//}}AFX_CODEJOCK_PRIVATE
};

//////////////////////////////////////////////////////////////////////

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPTABCTRL_H__)