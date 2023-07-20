// XTPMarqueeCtrl.h : header file
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
#if !defined(__XTPMARQUEECTRL_H__)
#	define __XTPMARQUEECTRL_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPWinThemeWrapper;
class CXTPMarqueeCtrlPaintManager;

// ---------------------------------------------------------------------
// Summary:
//     CXTPMarqueeCtrl is a CProgressCtrl derived class. It is used to
//     create a customizable Marquee style progress control. Using the
//     SetMarquee method, you can set the speed, gap between marquee
//     chunks, width, and transparency for each chunk. See the sample
//     located in the Controls/MarqueeProgress for more details.
// ---------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPMarqueeCtrl : public CProgressCtrl
{
	DECLARE_DYNAMIC(CXTPMarqueeCtrl)

public:
	// -----------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarqueeCtrl object.
	// -----------------------------------------------------------------
	CXTPMarqueeCtrl();

	// -----------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMarqueeCtrl object, handles cleanup and
	//     deallocation.
	// -----------------------------------------------------------------
	virtual ~CXTPMarqueeCtrl();

	// ----------------------------------------------------------------------
	// Summary:
	//     This member function obtains a pointer to the associated paint
	//     manager object. The paint manager object is used for drawing
	//     the date picker window.
	// Returns:
	//     Pointer to the associated paint manager object.
	// See Also:
	//     SetTheme, SetPaintManager
	// ----------------------------------------------------------------------
	CXTPMarqueeCtrlPaintManager* GetPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the new control drawing theme.
	// Parameters:
	//     pPaintManager - Pointer to the new paint manager object or derived
	//                     class. If NULL, then the default paint manager
	//                     object is set.
	// Remarks:
	//     Call this member function to set the paint manager object that is
	//     used for drawing a date picker window.
	// See Also:
	//     GetPaintManager
	//-----------------------------------------------------------------------
	void SetTheme(CXTPMarqueeCtrlPaintManager* pPaintManager = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to switch the visual theme of the control.
	// Parameters:
	//     nTheme - New visual theme. Can be any of the values listed in the
	//              Remarks section.
	// Remarks:
	//     nStyle can be one of the following:
	//     * <b>xtpControlThemeDefault</b>: Standard appearance style.
	//     * <b>xtpControlThemeFlat</b>: Flat appearance style.
	//     * <b>xtpControlThemeUltraFlat</b>: Ultra flat appearance style.
	//     * <b>xtpControlThemeOffice2000</b>: Office 2000 appearance style.
	//     * <b>xtpControlThemeOfficeXP</b>: Office XP appearance style.
	//     * <b>xtpControlThemeOffice2003</b>: Office 2003 appearance style.
	//     * <b>xtpControlThemeResource</b>: Office 2007 appearance style.
	//-----------------------------------------------------------------------
	void SetTheme(XTPControlTheme nTheme);

	// -----------------------------------------------------------------
	// Summary:
	//     This member is called to update the color and chunk bitmaps used
	//     by the marquee progress control.
	// -----------------------------------------------------------------
	void RefreshMetrics();

	// -----------------------------------------------------------------
	// Summary:
	//     Turns marquee mode on/off for the current progress bar control.
	// Parameters:
	//     bMarqueeMode - TRUE to turn marquee mode on,
	//                    FALSE to turn marquee mode off.
	//     nInterval    - Time in milliseconds between updates of the
	//                    marquee animation.
	//     nGap         - Size in pixels of the gap between each chunk
	//                    displayed in the marquee progress bar.
	//     cx           - Size in pixels of each chunk displayed in the
	//                    marquee progress bar.
	//     piTrans      - Array of 5 integers representing the amount of
	//                    transparency for each chunk displayed in the
	//                    marquee progress bar as a percent ranging from
	//                    0 to 100. This method assumes that the array
	//                    contains 5 integers.
	// Example:
	//    The following code example starts and stops the marquee
	//    scrolling animation:
	// <code>
	// int piAlpha[5] = { 25, 50, 75, 100, 100 };
	// m_wndProgress.SetMarquee(TRUE, 50, 2, 12, piAlpha);
	// </code>
	// -----------------------------------------------------------------
	void SetMarquee(BOOL bMarqueeMode = TRUE, int nInterval = 50, int nGap = 2, int cx = 12,
					int* piTrans = NULL);

	// -----------------------------------------------------------------
	// Summary:
	//     Determines if the PBS_SMOOTH style has been set for the marquee
	//     progress bar.
	// Returns:
	//     TRUE if the progress bar has PBS_SMOOTH style set, otherwise FALSE.
	// -----------------------------------------------------------------
	BOOL IsSmooth();

	// -----------------------------------------------------------------
	// Summary:
	//     Determines if the PBS_VERTICAL style has been set for the marquee
	//     progress bar.
	// Returns:
	//     TRUE if the progress bar has PBS_VERTICAL style set, otherwise FALSE.
	// -----------------------------------------------------------------
	BOOL IsVertical();

	// -----------------------------------------------------------------
	// Summary:
	//     Determines if the PBS_MARQUEE style has been set for the marquee
	//     progress bar.
	// Returns:
	//     TRUE if the progress bar has PBS_MARQUEE style set, otherwise FALSE.
	// -----------------------------------------------------------------
	BOOL IsMarquee();

	// -----------------------------------------------------------------
	// Summary:
	//     This member function sets the background and progress indicator
	//     colors for the progress bar.
	// Parameters:
	//     crBack  - A COLORREF value that specifies the new background color.
	//     crChunk - A COLORREF value that specifies the new progress indicator
	//               bar color.
	// -----------------------------------------------------------------
	void SetColors(COLORREF crBack, COLORREF crChunk);

protected:
	// -----------------------------------------------------------------
	// Summary:
	//     This member function is called when the progress bar has been
	//     created or subclassed.
	// Returns:
	//     TRUE if the control has been successfully initialized,
	//     otherwise FALSE.
	// -----------------------------------------------------------------
	virtual BOOL OnInitControl();

	// -----------------------------------------------------------------
	// Summary:
	//     This member function is called to determine if Windows themes
	//     are enabled for the calling application.
	// Returns:
	//     TRUE if Windows themes are enabled, otherwise FALSE.
	// -----------------------------------------------------------------
	virtual BOOL UseWinThemes();

	//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_VIRTUAL(CXTPMarqueeCtrl)
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL
	//{{AFX_MSG(CXTPMarqueeCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	afx_msg LRESULT OnSetTheme(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	//}}AFX_CODEJOCK_PRIVATE

protected:
	int m_nGap;			  // Size in pixels between each chunk in the marquee.
	int m_nPos;			  // Current position of the marquee animation.
	int m_cxChunk;		  // Size in pixels of each chunk in the marquee.
	BYTE m_chAlpha[5];	// Represents the transparency for each chunk in the marquee.
	BOOL m_bIsSubclassed; // TRUE if the progress bar was subclassed.
						  // progress.
	CXTPMarqueeCtrlPaintManager* m_pPaintManager; // Pointer to the progress bar paint manager.

	friend class CXTPMarqueeCtrlPaintManager;
	friend class CXTPMarqueeCtrlThemeResource;
};

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE CXTPMarqueeCtrlPaintManager* CXTPMarqueeCtrl::GetPaintManager() const
{
	return m_pPaintManager;
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPMARQUEECTRL_H__)
