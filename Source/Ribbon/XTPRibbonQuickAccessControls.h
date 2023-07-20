// XTPRibbonQuickAccessControls.h: interface for the CXTPRibbonQuickAccessControls class.
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
#if !defined(__XTPRIBBONQUICKACCESSCONTROLS_H__)
#	define __XTPRIBBONQUICKACCESSCONTROLS_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPRibbonControls is a CXTPControls derived class.
//     It represents a collection of controls for a Ribbon Bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonControls : public CXTPControls
{
	DECLARE_DYNCREATE(CXTPRibbonControls)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonControls object.
	//-----------------------------------------------------------------------
	CXTPRibbonControls();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a control is removed from the collection.
	// Parameters:
	//     pControl - Control that was removed from the collection.
	//-----------------------------------------------------------------------
	virtual void OnControlRemoved(CXTPControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a control is about to be removed from
	//     the collection.
	// Parameters:
	//     pControl - Control that is about to be removed from the collection.
	// Returns:
	//     TRUE to cancel the removal of the control.
	//-----------------------------------------------------------------------
	virtual BOOL OnControlRemoving(CXTPControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to check if a control has changed and must
	//     be saved.
	// Parameters:
	//     pControl - Control to check.
	// Returns:
	//     TRUE if the control must be saved.
	//-----------------------------------------------------------------------
	virtual BOOL ShouldSerializeControl(CXTPControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to copy controls.
	// Parameters:
	//     pControls  - Controls to be copied.
	//     bRecursive - TRUE to copy recursively.
	//-----------------------------------------------------------------------
	virtual void Copy(CXTPControls* pControls, BOOL bRecursive = FALSE);
};

//===========================================================================
// Summary:
//     CXTPRibbonQuickAccessControls is a CXTPControls derived class.
//     It represents a collection of Quick Access controls for a Ribbon Bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonQuickAccessControls : public CXTPControls
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonQuickAccessControls object.
	//-----------------------------------------------------------------------
	CXTPRibbonQuickAccessControls();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPRibbonQuickAccessControls object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPRibbonQuickAccessControls();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to remove a control.
	// Parameters:
	//     pControl - Control to be removed.
	//-----------------------------------------------------------------------
	void Remove(CXTPControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to reset Quick Access controls.
	//-----------------------------------------------------------------------
	void Reset();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to find a duplicate of a control.
	// Parameters:
	//     pControl - Control with a duplicate to find.
	// Returns:
	//     A pointer to a duplicate of the control if successful, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPControl* FindDuplicate(CXTPControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the index of a specified control from the Quick Access bar.
	// Parameters:
	//     pControl - Control whose index must be retrieved.
	// Returns:
	//     The index of the specified control if successful, otherwise -1.
	//-----------------------------------------------------------------------
	int IndexOf(CXTPControl* pControl) const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a control is added to the collection.
	// Parameters:
	//     pControl - Control that was added to the collection.
	//-----------------------------------------------------------------------
	virtual void OnControlAdded(CXTPControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a control is removed from the collection.
	// Parameters:
	//     pControl - Control that was removed from the collection.
	//-----------------------------------------------------------------------
	virtual void OnControlRemoved(CXTPControl* pControl);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	virtual void RefreshIndexes();
	//}}AFX_CODEJOCK_PRIVATE
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPRIBBONQUICKACCESSCONTROLS_H__)
