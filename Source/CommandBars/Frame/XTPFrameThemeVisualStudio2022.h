// XTPFrameThemeVisualStudio2022.h
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
#if !defined(__XTPFRAMETHEMEVISUALSTUDIO2022_H__)
#	define __XTPFRAMETHEMEVISUALSTUDIO2022_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPPaintManager;
class CXTPCommandBarsFrameHook;
class CXTPRibbonBar;

class _XTP_EXT_CLASS CXTPFrameThemeVisualStudio2022
	: public CXTPFrameThemeOffice2013
	, private IXTPApplicationEvents
{
public:
	CXTPFrameThemeVisualStudio2022(CXTPPaintManager* pPaintManager);

	virtual ~CXTPFrameThemeVisualStudio2022();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the manager.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

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
	// <combine
	// CXTPFrameThemeVisualStudio2022::DrawFrameCaptionButton@CDC*@CRect@int@BOOL@BOOL@BOOL>
	//-----------------------------------------------------------------------
	virtual void DrawFrameCaptionButton(CDC* pDC, CRect rc, int nId, BOOL bSelected, BOOL bPressed,
										BOOL bActive, CXTPRibbonBar* pRibbonBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get/set the application state color for the
	//     frame border.
	// Parameters:
	//     clrState - RGB value representing the application state color. Use
	//                COLORREF_NULL to reset back to default value.
	// Returns:
	//     RGB value representing the application state color.
	//-----------------------------------------------------------------------
	virtual void SetStateColor(COLORREF clrState);
	virtual COLORREF
		GetStateColor() const; // <combine CXTPFrameThemeVisualStudio2022::SetStateColor@COLORREF>

	virtual CRect CalcFrameBorder(const CXTPCommandBarsFrameHook* pFrameHook) const;

	virtual CXTPFrameShadow* CreateShadow(CWnd* pFrame);

	virtual void OnShadowedFrameActiveStateChanged(BOOL bActive, CWnd* pFrame,
												   CXTPFrameShadow* pShadow);

private:
	virtual void OnAmbientPropertyChanged(CXTPApplication* pApplication,
										  XTPApplicationAmbientProperty nProperty);
	void UpdateStateColor();

	BOOL m_bAutomaticStateColor;

protected:
	COLORREF m_clrShadowColorActive;
	COLORREF m_clrShadowColorInactive;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPFRAMETHEMEVISUALSTUDIO2022_H__)
