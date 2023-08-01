// XTPRibbonPaintManager.h: interface for the CXTPRibbonPaintManager class.
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
#if !defined(__XTPRIBBONPAINTMANAGER_H__)
#	define __XTPRIBBONPAINTMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPRibbonGroup;
class CXTPRibbonBar;
class CXTPRibbonTab;
class CXTPResourceImage;
class CXTPResourceImages;
class CXTPTabPaintManager;
class CXTPRibbonTabContextHeaders;
class CXTPPopupToolBar;
class CXTPPaintManager;
class CXTPRibbonControlSystemButton;

_XTP_EXT_CLASS BOOL AFX_CDECL IsCompositeRect(CXTPCommandBar* pRibbonBar, const CRect& rc);

//===========================================================================
// Summary:
//     Standalone class used by CXTPRibbonBar as a paint manager.
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonPaintManager : public CXTPCmdTarget
{
	DECLARE_DYNAMIC(CXTPRibbonPaintManager)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonPaintManager object.
	// Parameters:
	//     pPaintManager - Pointer to a CXTPPaintManager object.
	//-----------------------------------------------------------------------
	CXTPRibbonPaintManager(CXTPPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPRibbonPaintManager object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPRibbonPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//      This method is called to refresh the visual metrics of the manager.
	// Remarks:
	//      Refreshes all of the colors in the ribbon bar. Many of the colors
	//      used are set with the GROUP_COLOR structure.
	// See Also:
	//     GROUP_COLOR
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the specified CXTPRibbonGroup.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     pGroup - Pointer to the CXTPRibbonGroup to be drawn.
	// Remarks:
	//     Draws the entire group including background, caption, and text.
	//     This will not draw the option button or the group pop-up.
	// See Also:
	//     GROUP_COLOR, FillRibbonBar, FillGroupRect, RefreshMetrics
	//-----------------------------------------------------------------------
	virtual void DrawGroup(CDC* pDC, CXTPRibbonGroup* pGroup) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the specified control which is a part of the Quick Access panel.
	// Parameters:
	//     pDC     - Pointer to a valid device context
	//     pContrl - Control to draw.
	//-----------------------------------------------------------------------
	virtual void DrawQuickAccessControl(CDC* pDC, CXTPRibbonBar* pRibbon, CXTPControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill the background of a CXTPRibbonGroup.
	// Parameters:
	//     pDC        - Pointer to a valid device context.
	//     pActiveTab - Pointer to the tab that is currently active.
	//     rcGroups   - Bounding rectangle of the group.
	// Remarks:
	//     Performs a gradient fill using the GROUP_COLOR::clrBackground for
	//     the main group background and GROUP_COLOR::clrCaption to fill the
	//     group caption rectangle.
	// See Also:
	//     CXTPRibbonGroup::GetRect, GROUP_COLOR
	//-----------------------------------------------------------------------
	virtual void FillGroupRect(CDC* pDC, CXTPRibbonTab* pActiveTab, CRect rcGroups) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the group pop-up button.
	// Parameters:
	//     pDC                - Pointer to a valid device context
	//     pControlGroupPopup - Pointer to a CXTPControlPopup object.
	//     bDraw              - TRUE to draw the group pop-up button,
	//                          FALSE to retrieve the size of the control.
	// Remarks:
	//     A single group button will be displayed if there is not enough room
	//     to display the items in the group. A group pop-up is displayed
	//     when the button is clicked. The pop-up contains all the items in
	//     the group. When a group button is clicked, a CXTPControlPopup
	//     is displayed that contains the items of the group.
	// Returns:
	//     If bDraw is FALSE, then the size of the group pop-up button control
	//     will be returned.
	// See Also:
	//     CXTPRibbonGroup::GetControlGroupPopup
	//-----------------------------------------------------------------------
	virtual CSize DrawControlGroupPopup(CDC* pDC, CXTPControlPopup* pControlGroupPopup,
										BOOL bDraw) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill the background of a ribbon bar control.
	// Parameters:
	//     pDC        - Pointer to a valid device context
	//     pRibbonBar - Pointer to the parent ribbon bar.
	//-----------------------------------------------------------------------
	virtual void FillRibbonBar(CDC* pDC, CXTPRibbonBar* pRibbonBar) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the group option button.
	// Parameters:
	//     pDC                 - Pointer to a valid device context.
	//     pControlGroupOption - Pointer to a CXTPControl object.
	//     bDraw               - TRUE to draw option button,
	//                           FALSE to retrieve the size of the control.
	// Remarks:
	//     The option button is typically used to launch a dialog associated with the group.
	//     It is drawn on the right side of the group caption.
	// Returns:
	//     If bDraw is FALSE, then the size of the option button control
	//     will be returned.
	// See Also:
	//     CXTPRibbonGroup::ShowOptionButton, CXTPRibbonGroup::GetControlGroupOption
	//-----------------------------------------------------------------------
	virtual CSize DrawControlGroupOption(CDC* pDC, CXTPControl* pControlGroupOption,
										 BOOL bDraw) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw ribbon context headers.
	// Parameters:
	//     pDC             - Pointer to a valid device context.
	//     pContextHeaders - Collection of context headers.
	//     pRibbonBar      - Pointer to the parent ribbon bar.
	//-----------------------------------------------------------------------
	virtual void DrawRibbonFrameContextHeaders(CDC* pDC, CXTPRibbonBar* pRibbonBar,
											   CXTPRibbonTabContextHeaders* pContextHeaders) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a single tab of the ribbon bar.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     pItem - Ribbon tab to draw.
	//-----------------------------------------------------------------------
	virtual void DrawRibbonTab(CDC* pDC, CXTPRibbonTab* pItem) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member fills in the tab header and tab client area. It
	//     will then draw tab client frame.
	// Parameters:
	//     pRibbonBar  - Pointer to the ribbon bar to draw the tab control for.
	//     pDC         - Pointer to a valid device context.
	//     rcControl   - Bounding rectangle of the tab control.
	//-----------------------------------------------------------------------
	virtual void FillRibbonTabControl(CDC* pDC, CXTPRibbonBar* pRibbonBar, CRect rcControl) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the pop-up toolbar of a reduced group.
	// Parameters:
	//     pDC         - Pointer to a valid device context.
	//     pGroup      - Parent group of pop-up'ed toolbar.
	//     pCommandBar - Pop-up toolbar to draw.
	//-----------------------------------------------------------------------
	virtual void FillGroupPopupToolBar(CDC* pDC, CXTPRibbonGroup* pGroup,
									   CXTPCommandBar* pCommandBar) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the ribbon caption if EnableFrameTheme
	//     was not called.
	// Parameters:
	//     pDC        - Pointer to a valid device context.
	//	   pRibbonBar - Pointer to the ribbon bar to draw the caption for.
	//-----------------------------------------------------------------------
	virtual void DrawRibbonFrameCaptionBar(CDC* pDC, CXTPRibbonBar* pRibbonBar, BOOL bActive) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the Ribbon caption.
	// Parameters:
	//     pDC        - Pointer to a valid device context.
	//	   pRibbonBar - Pointer to the ribbon bar to draw the caption for.
	//     bActive    - TRUE if the ribbon frame is active.
	//-----------------------------------------------------------------------
	virtual void DrawRibbonFrameCaption(CDC* pDC, CXTPRibbonBar* pRibbonBar, BOOL bActive) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the Quick Access customize button.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     pControl - Pointer to the Quick Access customize button.
	// See Also:
	//     DrawRibbonQuickAccessMoreButton
	//-----------------------------------------------------------------------
	virtual void DrawRibbonQuickAccessButton(CDC* pDC, CXTPControlPopup* pControl) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the Quick Access more button.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     pControl - Pointer to the Quick Access more button.
	// See Also:
	//     DrawRibbonQuickAccessButton
	//-----------------------------------------------------------------------
	virtual void DrawRibbonQuickAccessMoreButton(CDC* pDC, CXTPControlPopup* pControl) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the top-left system button.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     pControl - System button control.
	//     rc       - Bounding rectangle of the control.
	//-----------------------------------------------------------------------
	virtual void DrawRibbonFrameSystemButton(CDC* pDC, CXTPRibbonControlSystemButton* pControl,
											 CRect rc) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw scroll group buttons.
	// Parameters:
	//     pDC         - Pointer to a valid device context.
	//     pControl    - Scroll button to draw.
	//     bScrollLeft - TRUE to draw the left button, FALSE to draw the right.
	//-----------------------------------------------------------------------
	virtual void DrawRibbonScrollButton(CDC* pDC, CXTPControl* pControl, BOOL bScrollLeft) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the background of a minimized pop-up bar
	// Parameters:
	//     pDC           - Pointer to a valid device context.
	//     pPopupToolBar - Pointer to a pop-up toolbar.
	//-----------------------------------------------------------------------
	virtual void FillTabPopupToolBar(CDC* pDC, CXTPPopupToolBar* pPopupToolBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the ribbon frame if EnableFrameTheme
	//     was not called.
	// Parameters:
	//     pDC        - Pointer to a valid device context.
	//     pRibbonBar - Ribbon bar to draw.
	//-----------------------------------------------------------------------
	virtual void DrawRibbonFramePart(CDC* pDC, CXTPRibbonBar* pRibbonBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the pop-up bar of the more button.
	// Parameters:
	//      pDC  - Pointer to a valid device context.
	//      pBar - Toolbar to fill.
	//-----------------------------------------------------------------------
	virtual void FillMorePopupToolBarEntry(CDC* pDC, CXTPCommandBar* pBar) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the pop-up bar of the system button.
	// Parameters:
	//      pDC  - Pointer to a valid device context.
	//      pBar - Toolbar to fill.
	//-----------------------------------------------------------------------
	virtual void FillSystemPopupBarEntry(CDC* pDC, CXTPPopupBar* pBar) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a pop-up bar button.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     pButton - Pop-up bar button to draw.
	//-----------------------------------------------------------------------
	virtual void DrawSystemPopupBarButton(CDC* pDC, CXTPControl* pButton) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the ribbon group when the group is
	//     minimized and only a button is displayed. When the button is
	//     pressed, the group is displayed as a pop-up.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     pButton - Button to draw.
	//-----------------------------------------------------------------------
	virtual BOOL DrawRibbonGroupControlEntry(CDC* pDC, CXTPControl* pButton) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the ribbon group pop-up toolbar.
	// Parameters:
	//     pDC          - Pointer to a valid device context.
	//     pRibbonGroup - Group to draw.
	//-----------------------------------------------------------------------
	virtual void DrawRibbonGroupPopupToolBar(CDC* pDC, CXTPRibbonGroup* pRibbonGroup);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the edit control height.
	// Returns:
	//     The edit control height.
	//-----------------------------------------------------------------------
	int GetEditHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the font height used for Ribbon.
	// Parameters:
	//     nFontHeight - New font height.
	// See Also:
	//     GetFontHeight
	//-----------------------------------------------------------------------
	void SetFontHeight(int nFontHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the font height used for Ribbon.
	// Returns:
	//     The font height used for Ribbon.
	// See Also:
	//     SetFontHeight
	//-----------------------------------------------------------------------
	int GetFontHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the ribbon caption height.
	// Parameters:
	//     pRibbonBar - Pointer to the parent ribbon bar.
	// Returns:
	//     The ribbon caption height.
	//-----------------------------------------------------------------------
	virtual int GetRibbonCaptionHeight(const CXTPRibbonBar* pRibbonBar) const = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the ribbon group option size.
	// Returns:
	//     The ribbon group option size.
	//-----------------------------------------------------------------------
	virtual CSize GetControlGroupOptionSize() const = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the height of the group caption bar.
	// Returns:
	//     The height of the group caption bar.
	// See Also:
	//     CXTPRibbonGroup::RepositionControls, CXTPRibbonGroup::SetRect
	//-----------------------------------------------------------------------
	int GetGroupCaptionHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads a bitmap image from the specified path.
	// Parameters:
	//     lpszFileName - The name of the file that contains the bitmaps
	//                    to load.
	// Remarks:
	//     The images are the bitmaps that represent all the visual components
	//     of the Ribbon Bar. For example tab buttons, group buttons, menu buttons,
	//     toolbar buttons, option button, toolbar dropdown, etc.
	//     The images are loaded using LoadImage and are stored in the m_pImages
	//     image collection.
	//     Images for the Office 2007 theme can be found in the \\Source\\Ribbon\\Res
	//     folder.
	// Returns:
	//     New CXTPResourceImage containing the loaded bitmap.
	// See Also:
	//     SetImageHandle, m_pImages
	//-----------------------------------------------------------------------
	CXTPResourceImage* LoadImage(LPCTSTR lpszFileName);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get a pointer to the CXTPResourceImages object.
	// Returns:
	//     A pointer to the CXTPResourceImages object.
	//-----------------------------------------------------------------------
	CXTPResourceImages* GetImages() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get a pointer to the tab paint manager.
	// Remarks:
	//     The tab paint manager is used to customize the appearance of
	//     CXTPTabManagerItem objects and the tab manager (i.e. tab colors,
	//     styles, etc.). This member must be overridden in derived classes.
	// Returns:
	//     A pointer to the CXTPTabPaintManager that contains the
	//     visual elements of the tabs.
	//-----------------------------------------------------------------------
	CXTPTabPaintManager* GetTabPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get a pointer to the frame paint manager.
	// Returns:
	//     A pointer to the frame paint manager.
	//-----------------------------------------------------------------------
	CXTPFramePaintManager* GetFramePaintManager() const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a CXTPTabPaintManager object used to store the color and
	//     appearance information of the tabs in the ribbon bar.
	// Returns:
	//     A pointer to the newly created CXTPTabPaintManager object.
	//-----------------------------------------------------------------------
	virtual CXTPTabPaintManager* CreateTabPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a ribbon bar application icon.
	// Parameters:
	//     pDC        - Pointer to a valid device context.
	//     pRibbonBar - Pointer to the ribbon bar to draw the application icon for.
	//     rc         - Bounding rectangle of the icon.
	//     hIcon      - Pointer to the application icon to use.
	//-----------------------------------------------------------------------
	void DrawRibbonApplicationIcon(CDC* pDC, CXTPRibbonBar* pRibbonBar, CRect rc, HICON hIcon);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw caption text for an aero-transparent caption.
	// Parameters:
	//     pDC           - Pointer to a valid device context.
	//     rcCaptionText - Caption text rectangle.
	//     strText       - Caption text.
	//     pSite         - Frame to draw.
	//     bActive       - TRUE if the frame is active.
	//-----------------------------------------------------------------------
	void DrawDwmCaptionText(CDC* pDC, CRect rcCaptionText, CString strText, CWnd* pSite,
							BOOL bActive);

#	ifdef _XTP_COMMANDBARS_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	LPFONTDISP OleGetCaptionFont();
	void OleSetCaptionFont(LPFONTDISP pFontDisp);
	LPFONTDISP OleGetTitleFont();
	void OleSetTitleFont(LPFONTDISP pFontDisp);

	DECLARE_DISPATCH_MAP()
	DECLARE_OLETYPELIB_EX(CXTPRibbonTheme);
	DECLARE_INTERFACE_MAP()

//}}AFX_CODEJOCK_PRIVATE
#	endif

public:
	BOOL m_bHotTrackingGroups; // TRUE to highlight the group under the cursor.
	BOOL m_bFlatFrame;		   // TRUE to draw a flat frame.

	CXTPPaintManagerFont m_fontGroupCaption; // Font used to draw text in the group caption.

	COLORREF m_clrRecentFileListEdgeShadow;	// Color of the File List edge shadow.
	COLORREF m_clrRecentFileListEdgeHighLight; // Color of the File List edge highlight.

	int m_nTabsHeight;		   // Tabs heights.
	int m_nGroupSpacing;	   // Group spacing.
	int m_nGroupCaptionHeight; // Group caption bar height.

protected:
	COLORREF m_clrRibbonFace;			  // Ribbon face color.
	COLORREF m_clrRibbonBorder;			  // Ribbon border color.
	COLORREF m_clrRibbonGroupCaptionText; // Ribbon group captions color.
	COLORREF m_clrRibbonInactiveFace;	 // Color of the ribbon when it is inactive.

	COLORREF m_clrFileButtonText; // Text caption to display when the system button is displayed as
								  // a tab button (Windows 7 style). Button style must be button
								  // caption.

	COLORREF m_clrMinimizedFrameEdgeHighLight; // Minimized bottom edge.
	COLORREF m_clrMinimizedFrameEdgeShadow;	// Minimized bottom edge shadow.

	CXTPPaintManager* m_pPaintManager;		 // Paint manager used for the ribbon.
	CXTPTabPaintManager* m_pTabPaintManager; // Tab paint manager.

	CRect m_rcFileButtonMargin; // Margins of the system button when it is displayed as a tab button
								// (Windows 7 style).

public:
private:
	HICON m_hApplicationIcon;
	BOOL m_bAlphaApplicationIcon;

protected:
	friend class CXTPRibbonBar;
};

AFX_INLINE int CXTPRibbonPaintManager::GetGroupCaptionHeight() const
{
	return m_nGroupCaptionHeight;
}

AFX_INLINE CXTPTabPaintManager* CXTPRibbonPaintManager::GetTabPaintManager() const
{
	return m_pTabPaintManager;
}

AFX_INLINE CXTPFramePaintManager* CXTPRibbonPaintManager::GetFramePaintManager() const
{
	CXTPFramePaintManager* pFramePaintManager = NULL;

	if (NULL != m_pPaintManager)
	{
		pFramePaintManager = m_pPaintManager->GetFramePaintManager();
	}

	return pFramePaintManager;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPRIBBONPAINTMANAGER_H__)