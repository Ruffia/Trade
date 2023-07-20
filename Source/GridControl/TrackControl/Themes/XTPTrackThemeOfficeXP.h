// XTPTrackThemeOfficeXP.h: interface for the CXTPTrackPaintManager class.
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
#if !defined(__XTPTRACKTHEMEOfficeXP_H__)
#	define __XTPTRACKTHEMEOfficeXP_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPTrackThemeOfficeXP is a CXTPTrackPaintManager derived
//     class that represents an Office XP style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPTrackThemeOfficeXP : public CXTPTrackPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor, handles properties initialization.
	//-----------------------------------------------------------------------
	CXTPTrackThemeOfficeXP();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the background of a column using
	//     the current column style.
	// Parameters:
	//     pDC                 - Pointer to the used device context.
	//     pColumn             - Pointer to the column whose background must be drawn.
	//     rcColumn            - Bounding rectangle of the column.
	//     bColumnPressed      - TRUE if the column is pressed by the user.
	//     bDraggingFromHeader - TRUE if the column is being dragged from the header area.
	//     pHeader             - Pointer to the header that the column belongs to.
	//-----------------------------------------------------------------------
	virtual void DrawColumnBackground(CDC* pDC, CXTPGridColumn* pColumn, CRect rcColumn,
									  BOOL& bColumnPressed, BOOL& bDraggingFromHeader,
									  CXTPGridHeader* pHeader = NULL);
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPTRACKTHEMEOfficeXP_H__)
