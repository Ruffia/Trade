// XTPStatusBarThemeVisualStudio2022.h
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
#if !defined(__XTPXTPSTATUSBARTHEMEVISUALSTUDIO2022_H__)
#	define __XTPXTPSTATUSBARTHEMEVISUALSTUDIO2022_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//    The CXTPStatusBarThemeVisualStudio2022 class is used to enable the
//    Visual Studio 2022 style theme for the status bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPStatusBarThemeVisualStudio2022
	: public CXTPStatusBarThemeOffice2013
	, private IXTPApplicationEvents
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPStatusBarThemeVisualStudio2022 object.
	// Parameters:
	//     pPaintManager - Pointer to the parent CXTPPaintManager object.
	//-------------------------------------------------------------------------
	CXTPStatusBarThemeVisualStudio2022(CXTPPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles object destruction.
	//-----------------------------------------------------------------------
	~CXTPStatusBarThemeVisualStudio2022();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the status bar's gripper.
	// Parameters:
	//     pDC      - Pointer to a valid device context
	//     rcClient - Client rectangle of the status bar.
	//-----------------------------------------------------------------------
	virtual void DrawStatusBarGripper(CDC* pDC, CRect rcClient);

	//-----------------------------------------------------------------------
	// Summary:
	//    This method is called to refresh colors and other theme specific
	//    metrics for the paint manager.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//    Call this member function to get/set the application state color for the
	//    status bar.
	// Parameters:
	//     clrState - COLORREF value representing the application state color. Use
	//                COLORREF_NULL to reset back to default color.
	//-----------------------------------------------------------------------
	virtual void SetStateColor(COLORREF clrState);
	virtual COLORREF GetStateColor()
		const; // <combine CXTPStatusBarThemeVisualStudio2022::SetStateColor@COLORREF>

private:
	virtual void OnAmbientPropertyChanged(CXTPApplication* pApplication,
										  XTPApplicationAmbientProperty nProperty);
	void UpdateStateColor();

	BOOL m_bAutomaticStateColor;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPXTPSTATUSBARTHEMEVISUALSTUDIO2022_H__)
