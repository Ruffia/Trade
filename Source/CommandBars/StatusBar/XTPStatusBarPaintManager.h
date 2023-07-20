// XTPStatusBarPaintManager.h
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
#if !defined(__XTPSTATUSBARPAINTMANAGER_H__)
#	define __XTPSTATUSBARPAINTMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPStatusBarPane;
class CXTPPaintManager;
class CXTPStatusBarColorSet;
class CXTPStatusBarSwitchPane;

class _XTP_EXT_CLASS CXTPStatusBarPaintManager : public CObject
{
	DECLARE_DYNAMIC(CXTPStatusBarPaintManager);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//      Constructs a status bar paint manager object.
	// Parameters:
	//      pPaintManager - Parent paint manager.
	//-----------------------------------------------------------------------
	CXTPStatusBarPaintManager(CXTPPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Handles object destruction
	//-----------------------------------------------------------------------
	virtual ~CXTPStatusBarPaintManager()
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill a status bar.
	// Parameters:
	//     pDC  - Pointer to a valid device context.
	//     pBar - Pointer to a CXTPStatusBar object.
	//-----------------------------------------------------------------------
	virtual void FillStatusBar(CDC* pDC, CXTPStatusBar* pBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw text for a single pane of the status bar.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     pPane - The status bar pane to draw.
	//     rcItem - Item rectangle.
	//-----------------------------------------------------------------------
	virtual void DrawStatusBarPaneEntry(CDC* pDC, CRect rcItem, CXTPStatusBarPane* pPane);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a status bar button.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rcItem - Item rectangle.
	//     pPane - The status bar pane to draw.
	//-----------------------------------------------------------------------
	virtual void DrawStatusBarButtonFace(CDC* pDC, CRect rcItem, CXTPStatusBarPane* pPane);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a status bar switch pane.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rcItem - Item rectangle.
	//     pPane - The status bar pane to draw.
	// Returns:
	//     Returns the width of the switch pane.
	//-----------------------------------------------------------------------
	virtual int DrawStatusBarSwitchPane(CDC* pDC, CRect rcItem, CXTPStatusBarSwitchPane* pPane);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a frame for a single cell of the status bar.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rc - CRect object specifying the size of the area.
	//     pPane - The status bar pane to draw.
	//     bGripperPane - TRUE if the pane is the last cell of the status bar.
	//-----------------------------------------------------------------------
	virtual void DrawStatusBarPaneBorder(CDC* pDC, CRect rc, CXTPStatusBarPane* pPane,
										 BOOL bGripperPane);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a status bar switch pane separator.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rc - CRect object specifying the size of the area.
	//     pPane - The status bar pane need to draw.
	//-----------------------------------------------------------------------
	virtual void DrawStatusBarPaneSeparator(CDC* pDC, CRect rc, CXTPStatusBarPane* pPane);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the status bar's gripper.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     rcClient - Client rectangle of the status bar.
	//-----------------------------------------------------------------------
	virtual void DrawStatusBarGripper(CDC* pDC, CRect rcClient);

	//-------------------------------------------------------------------------
	// Summary:
	//     Handles updates to paint manager colors and metrics.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

	virtual COLORREF GetTextColor(CXTPStatusBarPane* pPane);

public:
	CXTPPaintManager* m_pPaintManager;
	CXTPPaintManagerColor m_clrBackground;
	CXTPPaintManagerColor m_clrText;
	CXTPPaintManagerColor m_clrTextGray;	// Status bar gray text color.
	CXTPPaintManagerColor m_clrGripperLite; // Status bar gripper light color.
	CXTPPaintManagerColor m_clrGripperDark; // Status bar gripper dark color.
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPSTATUSBARPAINTMANAGER_H__)
