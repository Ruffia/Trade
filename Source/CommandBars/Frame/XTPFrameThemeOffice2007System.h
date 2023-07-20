// XTPFrameThemeOffice2007System.h : interface for the CXTPFrameThemeOffice2007System class.
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
#if !defined(__XTPRIBBONOFFICE2007SYSTEMFRAMETHEME_H__)
#	define __XTPRIBBONOFFICE2007SYSTEMFRAMETHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPMarkupThemePart;
class CXTPRibbonBar;

//===========================================================================
// Summary:
//     The CXTPFrameThemeOffice2007System class is used to enable a system style
//     theme for Command Bars.
// See Also:
//     CXTPPaintManager::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPFrameThemeOffice2007System : public CXTPRibbonMarkupFrameTheme
{
	// Construction / Destruction
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFrameThemeOffice2007System object.
	// Parameters:
	//     pPaintManager - Reference to a CXTPPaintManager object.
	//-----------------------------------------------------------------------
	CXTPFrameThemeOffice2007System(CXTPPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFrameThemeOffice2007System object, handles cleanup and
	//     deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPFrameThemeOffice2007System();

	// Frame border
public:
	virtual HRGN CalcFrameRegion(CXTPCommandBarsFrameHook* pFrameHook, CSize sz) const;

	virtual CRect CalcFrameBorder(const CXTPCommandBarsFrameHook* pFrameHook) const;

public:
	virtual CXTPMarkupThemePart* GetFrameBorder(int nId, BOOL bActive);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the frame caption bar.
	// Parameters:
	//     pDC - Pointer to device context.
	//     rcCaptionBar - Caption bar rectangle.
	//     pSite - Frame to draw.
	//     bActive - TRUE if the frame is active.
	//-----------------------------------------------------------------------
	virtual void DrawCaptionBar(CDC* pDC, CRect rcCaptionBar, CWnd* pSite, BOOL bActive);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw caption button.
	// Parameters:
	//      pDC - Pointer to device context.
	//      rc - Bounding rectangle of the button.
	//      nId - Identifier of the button.
	//      bSelected - TRUE if the button is highlighted.
	//      bPressed -  TRUE if the button is pressed.
	//      bActive - TRUE if the frame is active.
	//-----------------------------------------------------------------------
	void DrawFrameCaptionButton(CDC* pDC, CRect rc, int nId, BOOL bSelected, BOOL bPressed,
								BOOL bActive);
	//-----------------------------------------------------------------------
	// <combine
	// CXTPFrameThemeOffice2007System::DrawFrameCaptionButton@CDC*@CRect@int@BOOL@BOOL@BOOL>
	//-----------------------------------------------------------------------
	virtual void DrawFrameCaptionButton(CDC* pDC, CRect rc, int nId, BOOL bSelected, BOOL bPressed,
										BOOL bActive, CXTPRibbonBar* pRibbonBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the manager.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads markup parts from the specified ZIP source.
	// Parameters:
	//     hZip           - ZIP source to load markup parts from.
	//     pMarkupContext - Associated markup context.
	//-----------------------------------------------------------------------
	virtual void LoadParts(HZIP hZip, CXTPMarkupContext* pMarkupContext);

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads markup parts.
	// Parameters:
	//     bReload - Re-loads previously loaded parts if TRUE.
	//-----------------------------------------------------------------------
	virtual void LoadParts(BOOL bReload = FALSE);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATED_IN_FAVOR(LoadParts)
	BOOL LoadResource();

	_XTP_DEPRECATED_IN_FAVOR(CXTPRibbonMarkupFrameTheme::m_bMarkupPartsLoaded)
	BOOL m_bLoaded;
	//}}AFX_CODEJOCK_PRIVATE

	using CXTPRibbonMarkupFrameTheme::m_bMarkupPartsLoaded;

	CXTPMarkupThemePart* m_pCaptionActive;
	CXTPMarkupThemePart* m_pCaptionInactive;
	CXTPMarkupThemePart* m_pBorderLeftActive;
	CXTPMarkupThemePart* m_pBorderLeftInactive;
	CXTPMarkupThemePart* m_pBorderLeftCaptionActive;
	CXTPMarkupThemePart* m_pBorderLeftCaptionInactive;
	CXTPMarkupThemePart* m_pBorderTopActive;
	CXTPMarkupThemePart* m_pBorderTopInactive;
	CXTPMarkupThemePart* m_pBorderRightActive;
	CXTPMarkupThemePart* m_pBorderRightInactive;
	CXTPMarkupThemePart* m_pBorderRightCaptionActive;
	CXTPMarkupThemePart* m_pBorderRightCaptionInactive;
	CXTPMarkupThemePart* m_pBorderBottomActive;
	CXTPMarkupThemePart* m_pBorderBottomInactive;

	CXTPMarkupThemePart* m_pCaptionButtonSelected;
	CXTPMarkupThemePart* m_pCaptionButtonPressed;

	CXTPMarkupThemePart* m_pGlyphMinimize9;
	CXTPMarkupThemePart* m_pGlyphMinimize11;
	CXTPMarkupThemePart* m_pGlyphMaximize9;
	CXTPMarkupThemePart* m_pGlyphMaximize11;
	CXTPMarkupThemePart* m_pGlyphRestore9;
	CXTPMarkupThemePart* m_pGlyphRestore11;
	CXTPMarkupThemePart* m_pGlyphClose9;
	CXTPMarkupThemePart* m_pGlyphClose11;

	CXTPMarkupContext* m_pMarkupContext; // Store pointer to CXTPMarkupContext object.
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPRIBBONOFFICE2007SYSTEMFRAMETHEME_H__)
