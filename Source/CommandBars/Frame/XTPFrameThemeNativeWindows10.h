// XTPFrameThemeNativeWindows10.h
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
#if !defined(__XTPFRAMETHEMENATIVEWINDOWS10_H__)
#	define __XTPFRAMETHEMENATIVEWINDOWS10_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPPaintManager;
class CXTPCommandBarsFrameHook;
class CXTPRibbonBar;

class _XTP_EXT_CLASS CXTPFrameThemeNativeWindows10
	: public CXTPFrameThemeOffice2013
	, public IXTPApplicationEvents
{
public:
	CXTPFrameThemeNativeWindows10(CXTPPaintManager* pPaintManager);

	virtual ~CXTPFrameThemeNativeWindows10();

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
	//      bActive - TRUE if frame is active.
	//-----------------------------------------------------------------------
	virtual void DrawFrameCaptionButton(CDC* pDC, CRect rc, int nId, BOOL bSelected, BOOL bPressed,
										BOOL bActive);
	//-----------------------------------------------------------------------
	// <combine
	// CXTPFrameThemeNativeWindows10::DrawFrameCaptionButton@CDC*@CRect@int@BOOL@BOOL@BOOL>
	//-----------------------------------------------------------------------
	virtual void DrawFrameCaptionButton(CDC* pDC, CRect rc, int nId, BOOL bSelected, BOOL bPressed,
										BOOL bActive, CXTPRibbonBar* pRibbonBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get/set the application state color for the
	//     frame border.
	// Parameters:
	//     clrState - RGB value representing the application state color.  Use
	//                COLORREF_NULL to reset back to default value.
	// Returns:
	//     RGB value representing the application state color.
	//-----------------------------------------------------------------------
	virtual void SetStateColor(COLORREF clrState);
	virtual COLORREF
		GetStateColor() const; // <combine CXTPFrameThemeNativeWindows10::SetStateColor@COLORREF>

	virtual CXTPFrameShadow* CreateShadow(CWnd* pFrame);

	virtual void OnShadowedFrameActiveStateChanged(BOOL bActive, CWnd* pFrame,
												   CXTPFrameShadow* pShadow);

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

private:
	void UpdateAccentColors();

	virtual void OnAmbientPropertyChanged(CXTPApplication* pApplication,
										  XTPApplicationAmbientProperty nProperty);

protected:
	COLORREF m_clrShadowColorActive;
	COLORREF m_clrShadowColorInactive;

	CXTPModernUIFrameCaptionButton* m_pButtonRestoreActive;
	CXTPModernUIFrameCaptionButton* m_pButtonRestoreInactive;
	CXTPModernUIFrameCaptionButton* m_pButtonRestorePressed;
	CXTPModernUIFrameCaptionButton* m_pButtonRestoreHot;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPFRAMETHEMENATIVEWINDOWS10_H__)
