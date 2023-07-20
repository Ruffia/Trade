// XTPRibbonMarkupFrameTheme.h : interface for the CXTPRibbonMarkupFrameTheme class.
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
#if !defined(__XTPRIBBONMARKUPFRAMETHEME_H__)
#	define __XTPRIBBONMARKUPFRAMETHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPMarkupThemePart;

//===========================================================================
// Summary:
//     The CXTPRibbonMarkupFrameTheme class is used to enable a
//     system style theme for Command Bars.
// See Also:
//     CXTPPaintManager::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonMarkupFrameTheme : public CXTPFramePaintManager
{
	DECLARE_DYNAMIC(CXTPRibbonMarkupFrameTheme);

protected:
	enum XTPBorderPart
	{
		xtpBorderPartTop,
		xtpBorderPartLeft,
		xtpBorderPartLeftCaption,
		xtpBorderPartRight,
		xtpBorderPartRightCaption,
		xtpBorderPartBottom,
		xtpBorderPartCount
	};

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonMarkupFrameTheme object.
	// Parameters:
	//     pPaintManager - Pointer to a CXTPPaintManager object.
	//-----------------------------------------------------------------------
	CXTPRibbonMarkupFrameTheme(CXTPPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPRibbonMarkupFrameTheme object, handles cleanup and
	//     deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPRibbonMarkupFrameTheme();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the frame if the Ribbon was found.
	// Parameters:
	//      pDC        - Pointer to a valid device context.
	//      pFrameHook - Pointer to a CXTPCommandBarsFrameHook hook object.
	//-----------------------------------------------------------------------
	virtual void DrawFrame(CDC* pDC, CXTPCommandBarsFrameHook* pFrameHook);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the frame border.
	// Parameters:
	//      pDC        - Pointer to a valid device context.
	//      pFrameHook - Pointer to a CXTPCommandBarsFrameHook hook object.
	//-----------------------------------------------------------------------
	virtual void DrawFrameBorder(CDC* pDC, CXTPCommandBarsFrameHook* pFrameHook);

	//-----------------------------------------------------------------------
	// Summary:
	//     Re-loads markup parts from the specified ZIP source.
	// Parameters:
	//     hZip           - ZIP source to load markup parts from.
	//     pMarkupContext - Associated markup context.
	//-----------------------------------------------------------------------
	void ReloadMarkupParts(HZIP hZip, CXTPMarkupContext* pMarkupContext);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get the frame border part.
	// Parameters:
	//     nId     - Identifier of the frame border; can be one of the values defined
	//               by the CXTPRibbonMarkupFrameTheme::XTPBorderPart enumeration.
	//     bActive - TRUE if the frame border is active.
	//-----------------------------------------------------------------------
	virtual CXTPMarkupThemePart* GetFrameBorder(int nId, BOOL bActive) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads markup parts from the specified ZIP source.
	// Parameters:
	//     hZip           - ZIP source to load markup parts from.
	//     pMarkupContext - Associated markup context.
	//-----------------------------------------------------------------------
	virtual void LoadParts(HZIP hZip, CXTPMarkupContext* pMarkupContext) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads markup parts.
	// Parameters:
	//     bReload - Re-loads previously loaded parts if TRUE.
	//-----------------------------------------------------------------------
	virtual void LoadParts(BOOL bReload = FALSE) = 0;

protected:
	BOOL m_bMarkupPartsLoaded; // TRUE if markup parts have been loaded.
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPRIBBONMARKUPFRAMETHEME_H__)
