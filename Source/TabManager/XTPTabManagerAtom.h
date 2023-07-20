// XTPTabManagerAtom.h
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
#if !defined(__XTPTABMANAGERATOM_H__)
#	define __XTPTABMANAGERATOM_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//=============================================================================
// Summary:
//     CXTPTabManagerAtom is a class that has only one purpose; to catch
//     the event when a property of the PaintManager is changed. When
//     a property of the PaintManager is changed, OnPropertyChanged is
//     called. When OnPropertyChanged is caught, Reposition is called
//     to recalculate self-layout.
// Remarks:
//     This is the base class for CXTPTabManager. When a property of
//     the CXTPTabPaintManager is changed, OnPropertyChanged is called
//     to recalculate the layout. The paint manager can be accessed
//     with the CXTPTabManager::GetPaintManager member.
// See Also:
//     CXTPTabManager::GetPaintManager
//=============================================================================
class _XTP_EXT_CLASS CXTPTabManagerAtom
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called when a property of the tab paint manager is changed.
	// See Also:
	//     CXTPTabManager::GetPaintManager
	//-------------------------------------------------------------------------
	virtual void OnPropertyChanged()
	{
		Reposition();
	}

	//-------------------------------------------------------------------------
	// Summary:
	//     Recalculates the layout of the tab manager and then repositions itself.
	// Remarks:
	//     This method must be overridden in derived classes.
	//-------------------------------------------------------------------------
	virtual void Reposition() = 0;

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTabManagerAtom object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	virtual ~CXTPTabManagerAtom()
	{
	}
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTABMANAGERATOM_H__)
