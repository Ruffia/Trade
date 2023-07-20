// XTPTabThemeExcel2003.h
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
#if !defined(__XTPTABTHEMEEXCEL2003_H__)
#	define __XTPTABTHEMEEXCEL2003_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPTabThemeExcel2003 implements an Excel tab appearance.
// Remarks:
//     To use the Excel appearance, SetAppearance is used to apply
//     the xtpTabAppearanceExcel2003 XTPTabAppearanceStyle.
//
//     CXTPTabThemeExcel2003 supports the following XTPTabColorStyle:
//     * <b>xtpTabColorDefault</b>: Default color style.
//     * <b>xtpTabColorVisualStudio2003</b>: Visual Studio color style.
//     * <b>xtpTabColorOffice2003</b>: Office 2003 color style.
// See Also:
//     XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance,
//     GetAppearanceSet, SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
//===========================================================================
class _XTP_EXT_CLASS CXTPTabThemeExcel2003 : public CXTPTabThemeFlat
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabThemeExcel2003 object.
	//-----------------------------------------------------------------------
	CXTPTabThemeExcel2003()
	{
		m_rcHeaderMargin.SetRect(2, 3, 2, 0);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the top, left, right, and bottom margins of the tab header.
	// Returns:
	//     The top, left, right, and bottom margins of the tab header.
	// Remarks:
	//     The margins of the tab header are the space placed around
	//     the tab buttons inside the tab header.
	// See Also:
	//     CXTPTabPaintManager::GetHeaderRect
	//-----------------------------------------------------------------------
	virtual CRect GetHeaderMargin()
	{
		return CRect(m_rcHeaderMargin.left + m_nButtonHeight / 2, m_rcHeaderMargin.top,
					 m_rcHeaderMargin.right + m_nButtonHeight / 2, m_rcHeaderMargin.bottom);
	}

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
	// See Also:
	//     CXTPTabPaintManager::DrawTabControlEx,
	//     CXTPTabPaintManager::DrawSingleButtonIconAndText
	//-----------------------------------------------------------------------
	virtual void DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the bounding rectangle of a specified drawing item.
	// Parameters:
	//     pItem - Pointer to the drawing item with
	//             the bounding rectangle to retrieve.
	// Returns:
	//     The bounding rectangle of the specified drawing item.
	// See Also:
	//     CXTPTabManagerItem::GetRect
	//-----------------------------------------------------------------------
	virtual CRect GetButtonDrawRect(const CXTPTabManagerItem* pItem);
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTABTHEMEEXCEL2003_H__)
