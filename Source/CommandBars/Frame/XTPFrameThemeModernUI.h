// XTPFrameThemeModernUI.h : interface for the CXTPFrameThemeModernUI class.
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
#if !defined(__XTPFRAMETHEMEMODERNUI_H__)
#	define __XTPFRAMETHEMEMODERNUI_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPRibbonBar;
class CXTPPaintManager;
class CXTPCommandBarsFrameHook;
class CXTPColorSetCaptionButton;

//---------------------------------------------------------------------------

class _XTP_EXT_CLASS CXTPModernUIFrameBorder : public CXTPMarkupThemePart
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method sets the border color of the caption button.
	//-----------------------------------------------------------------------
	void SetBorderColor(COLORREF clrBorder, COLORREF clrBackground);
};

//---------------------------------------------------------------------------

class _XTP_EXT_CLASS CXTPModernUIFrameCaptionButton : public CXTPMarkupThemePart
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method sets the background, outline and fill color of the caption button.
	//-----------------------------------------------------------------------
	void SetColors(CXTPColorSetCaptionButton* pColorSet);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method sets the background color of the caption button.
	//-----------------------------------------------------------------------
	void SetBackgroundColor(COLORREF clrBackground);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method sets the outline color of the caption button.
	//-----------------------------------------------------------------------
	void SetOutlineColor(COLORREF clrOutline);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method sets the stroke color of the caption button.
	//-----------------------------------------------------------------------
	void SetStrokeColor(COLORREF clrStroke);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method sets the fill color of the caption button.
	//-----------------------------------------------------------------------
	void SetFillColor(COLORREF clrFill);
};

//---------------------------------------------------------------------------

class _XTP_EXT_CLASS CXTPFrameColorSet
{
public:
	COLORREF m_clrFrameCaptionActive;   // Caption color (active)
	COLORREF m_clrFrameCaptionInactive; // Caption color (inactive)
};

class _XTP_EXT_CLASS CXTPFrameThemeModernUI : public CXTPRibbonMarkupFrameTheme
{
	DECLARE_DYNAMIC(CXTPFrameThemeModernUI);

public:
	CXTPFrameThemeModernUI(CXTPPaintManager* pPaintManager);

	virtual ~CXTPFrameThemeModernUI();

public:
	virtual HRGN CalcFrameRegion(CXTPCommandBarsFrameHook* pFrameHook, CSize sz) const;

	virtual CRect CalcFrameBorder(const CXTPCommandBarsFrameHook* pFrameHook) const;

	virtual void DrawFrame(CDC* pDC, CXTPCommandBarsFrameHook* pFrameHook);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw frame caption text.
	// Parameters:
	//     pDC - Pointer to device context.
	//     rcCaptionText - Bounding rectangle of the window text.
	//     pSite - Frame to draw.
	//     bActive - TRUE if the frame is active.
	//-----------------------------------------------------------------------
	virtual void DrawCaptionText(CDC* pDC, CRect rcCaptionText, CWnd* pSite, BOOL bActive);
	//-----------------------------------------------------------------------
	// <combine CXTPFrameThemeModernUI::DrawCaptionText@CDC*@CRect@CWnd*@BOOL>
	//-----------------------------------------------------------------------
	virtual void DrawCaptionText(CDC* pDC, CRect rcCaptionText, CWnd* pSite, BOOL bActive,
								 CXTPRibbonBar* pRibbonBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the caption button.
	// Parameters:
	//      pDC - Pointer to device context.
	//      rc - Bounding rectangle of the button.
	//      nId - Identifier of the button.
	//      bSelected - TRUE if the button is highlighted.
	//      bPressed -  TRUE if the button is pressed.
	//      bActive - TRUE if the frame is active.
	//-----------------------------------------------------------------------
	virtual void DrawFrameCaptionButton(CDC* pDC, CRect rc, int nId, BOOL bSelected, BOOL bPressed,
										BOOL bActive);
	//-----------------------------------------------------------------------
	// <combine CXTPFrameThemeModernUI::DrawFrameCaptionButton@CDC*@CRect@int@BOOL@BOOL@BOOL>
	//-----------------------------------------------------------------------
	virtual void DrawFrameCaptionButton(CDC* pDC, CRect rc, int nId, BOOL bSelected, BOOL bPressed,
										BOOL bActive, CXTPRibbonBar* pRibbonBar);

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
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the manager.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Every derived class has to implement this method and return the
	//     name of the STYLE resource that contains all style information.
	//-----------------------------------------------------------------------
	virtual LPCTSTR GetStyleResourceName() const = 0;

	virtual CXTPMarkupThemePart* GetFrameBorder(int nId, BOOL bActive);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATED_IN_FAVOR(CXTPRibbonMarkupFrameTheme::m_bMarkupPartsLoaded)
	BOOL m_bLoaded;
	//}}AFX_CODEJOCK_PRIVATE

	using CXTPRibbonMarkupFrameTheme::m_bMarkupPartsLoaded;

	CXTPPaintManagerColor m_clrFrameBorderActive;
	CXTPPaintManagerColor m_clrFrameBorderInactive;

	CXTPModernUIFrameBorder* m_pBorderLeft;
	CXTPModernUIFrameBorder* m_pBorderTop;
	CXTPModernUIFrameBorder* m_pBorderRight;
	CXTPModernUIFrameBorder* m_pBorderBottom;

	CXTPColorSetCaptionButton m_csFrameCaptionButtonInactive;
	CXTPColorSetCaptionButton m_csFrameCaptionButtonActive;
	CXTPColorSetCaptionButton m_csFrameCaptionButtonHot;
	CXTPColorSetCaptionButton m_csFrameCaptionButtonPressed;

	CXTPModernUIFrameCaptionButton* m_pButtonMinimize;
	CXTPModernUIFrameCaptionButton* m_pButtonMaximize;
	CXTPModernUIFrameCaptionButton* m_pButtonRestore;
	CXTPModernUIFrameCaptionButton* m_pButtonClose;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPFRAMETHEMEMODERNUI_H__)
