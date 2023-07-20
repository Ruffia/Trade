// XTPTabThemePropertyPage2007.h
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
#if !defined(__XTPTABTHEMEPROPERTYPAGE2007_H__)
#	define __XTPTABTHEMEPROPERTYPAGE2007_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CAppearanceSetPropertyPage2007 implements an Office 2007 tab appearance.
// Remarks:
//     To use the Office 2007 appearance, SetAppearance is used to apply
//     the xtpTabAppearancePropertyPage2007 XTPTabAppearanceStyle.
// See Also:
//     XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance,
//     GetAppearanceSet, SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
//===========================================================================
class _XTP_EXT_CLASS CAppearanceSetPropertyPage2007 : public CXTPTabPaintManagerTheme
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CAppearanceSetPropertyPage2007 object.
	//-----------------------------------------------------------------------
	CAppearanceSetPropertyPage2007()
	{
		m_rcHeaderMargin.SetRect(4, 1, 0, 0);
	}

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Refreshes the appearance metrics.
	//     Override this member to change the appearance metrics.
	// Remarks:
	//     This method is used to refresh the appearance metrics
	//     such as font, button height, margin size, etc.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a single tab button in the tab client header area.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     pItem - Pointer to the tab button to draw.
	// Remarks:
	//     This method is only used to draw the tab button. This method does not
	//     draw the button's icon or text.
	//
	//     DrawSingleButton calls CXTPTabPaintManager::DrawSingleButtonIconAndText
	//     after the button is drawn to draw the button's icon and text.
	//
	//     If m_bDoubleHighlightedBorder is TRUE, then a double border is
	//     drawn on mouse over when hot tracking is enabled.
	// See Also:
	//     CXTPTabPaintManager::DrawTabControlEx,
	//     CXTPTabPaintManager::DrawSingleButtonIconAndText, m_bDoubleHighlightedBorder
	//-----------------------------------------------------------------------
	virtual void DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills both the tab header and the tab client area and then
	//     draws the tab client frame.
	// Parameters:
	//     pTabManager - Pointer to the tab manager to fill.
	//     pDC         - Pointer to a valid device context.
	//     rcControl   - Bounding rectangle of the tab manager control.
	//                   See CXTPTabManager::GetControlRect.
	// Returns:
	//     The bounding rectangle of the tab header.
	// Remarks:
	//     If CXTPTabPaintManager::m_bFillBackground is TRUE, then the tab client space
	//     is filled using CXTPTabPaintManagerColorSet::m_clrControlFace.
	//     CXTPTabPaintManager::m_bFillBackground is only FALSE when drawing
	//     docking pane panel auto-hide tabs because there is no client area to draw.
	// See Also:
	//     CXTPTabPaintManagerTheme::GetHeaderRect, CXTPTabPaintManager::m_bFillBackground,
	//     XTPTabClientFrame, CXTPTabPaintManager::m_clientFrame
	//-----------------------------------------------------------------------
	virtual CRect FillTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the default value of the color set for this theme.
	// Returns:
	//     The default value of the color set for this theme.
	// See Also:
	//     GetSupportedColorSets
	//-----------------------------------------------------------------------
	virtual XTPTabColorStyle GetDefaultColorSet()
	{
		return xtpTabColorResource;
	}

	//{{AFX_CODEJOCK_PRIVATE
protected:
	typedef void(AFX_CDECL* LPFNDRAWROTATEDBITS)(int cx, int cy, UINT* pSrcBits, UINT* pDestBits);
	void DrawRotatedButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem, BOOL bSwap,
						   LPFNDRAWROTATEDBITS pfnRotatedProcBack,
						   LPFNDRAWROTATEDBITS pfnRotatedProc);
	virtual void DrawButtonBackground(CDC* pDC, CXTPTabManagerItem* pItem, CRect rc);
	//}}AFX_CODEJOCK_PRIVATE
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTABTHEMEPROPERTYPAGE2007_H__)
