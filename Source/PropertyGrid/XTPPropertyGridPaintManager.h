// XTPPropertyGridPaintManager.h interface for the CXTPPropertyGridPaintManager class.
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
#if !defined(_XTPPROPERTYGRIDPAINTMANAGER_H__)
#	define _XTPPROPERTYGRIDPAINTMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPPropertyGrid;
class CXTPPropertyGridInplaceButton;
class CXTPPropertyGridItem;
class CXTPPropertyGridItemConstraint;
class CXTPPropertyGridItemBool;
class CXTPWinThemeWrapper;

class CXTPButton;
class CXTPComboBox;

class CXTPPropertyGridPaintManager;

//===========================================================================
// Summary:
//     Standalone class used by CXTPPropertyGridPaintManager to store item metrics.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridItemMetrics : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridItemMetrics object.
	// Parameters:
	//     pPaintManager - Pointer to the parent CXTPPropertyGridPaintManager object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemMetrics(CXTPPropertyGridPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Resets all color values to their default values.
	//-----------------------------------------------------------------------
	virtual void SetDefaultValues();

public:
	CXTPFont m_xtpFontNormal;	// Normal font.
	CXTPFont m_xtpFontBold;		 // Bold font.
	CXTPFont m_xtpFontUnderline; // Underline Font.

	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fontNormal, m_xtpFontNormal,
										  GetNormalFontHandle);
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fontBold, m_xtpFontBold, GetBoldFontHandle);
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fontUnderline, m_xtpFontUnderline,
										  GetUnderlineFontHandle);

	int m_nImage; // Image index.

	CXTPPaintManagerColor m_clrHelpBack;	 // Color of the description background.
	CXTPPaintManagerColor m_clrHelpFore;	 // Color of the description text.
	CXTPPaintManagerColor m_clrLine;		 // Line color.
	CXTPPaintManagerColor m_clrFore;		 // Color of the item's text.
	CXTPPaintManagerColor m_clrCategoryFore; // Color of the category text.
	CXTPPaintManagerColor m_clrBack;		 // Background color.
	CXTPPaintManagerColor m_clrReadOnlyFore; // Color of read-only text.
	CXTPPaintManagerColor m_clrVerbFace;	 // Text color of verbs.

	UINT m_uDrawTextFormat; // Text formatting style for the property item.
	int m_nMaxLength; // Maximum number of characters that can be entered into an editable item
					  // (Edit limit).

protected:
	CXTPPropertyGridPaintManager* m_pPaintManager; // Pointer to the paint manager used to draw the
												   // property grid.

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPPropertyGridItemMetrics)

	afx_msg COLORREF OleGetForeColor();
	afx_msg void OleSetForeColor(COLORREF clr);
	afx_msg COLORREF OleGetBackColor();
	afx_msg void OleSetBackColor(COLORREF clr);
	afx_msg void OleSetFont(LPFONTDISP pFontDisp);
	afx_msg LPFONTDISP OleGetFont();

//}}AFX_CODEJOCK_PRIVATE
#	endif
};

//-----------------------------------------------------------------------
// Summary:
//     Window identifier enumeration.
// See Also:
//     CXTPPropertyGridPaintManager::DrawPropertyGridBorder
//-----------------------------------------------------------------------
enum XTPPropertyGridWindow
{
	xtpPropertyGridVerbWindow, // Verb window.
	xtpPropertyGridGridWindow, // Grid window.
	xtpPropertyGridHelpWindow  // Help window.
};

//===========================================================================
// Summary:
//     Standalone class used by CXTPPropertyGrid as the paint manager.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridPaintManager : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridPaintManager object.
	// Parameters:
	//     pGrid - Pointer to a CXTPPropertyGrid object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridPaintManager(CXTPPropertyGrid* pGrid);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridPaintManager object, handles cleanup
	//     and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPPropertyGridPaintManager();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the text color of a specified item.
	// Parameters:
	//     pItem      - Pointer to the item whose text color must be retrieved.
	//     bValuePart - TRUE if the value part color is needed.
	// Returns:
	//     The text color of the specified item.
	//-----------------------------------------------------------------------
	virtual COLORREF GetItemTextColor(CXTPPropertyGridItem* pItem, BOOL bValuePart);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the back color of a specified item.
	// Parameters:
	//     pItem      - Pointer to the item whose back color must be retrieved.
	//     bValuePart - TRUE if the value part color is needed.
	// Returns:
	//     The back color of the specified item.
	//-----------------------------------------------------------------------
	virtual COLORREF GetItemBackColor(CXTPPropertyGridItem* pItem, BOOL bValuePart);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the font of a specified item.
	// Parameters:
	//     pItem      - Pointer to the item whose font must be retrieved.
	//     bValuePart - TRUE if the value part font is needed.
	// Returns:
	//     A pointer to a CFont object containing the font of the specified item.
	//-----------------------------------------------------------------------
	virtual CFont* GetItemFont(CXTPPropertyGridItem* pItem, BOOL bValuePart);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the background of the property grid.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//-----------------------------------------------------------------------
	virtual void FillPropertyGrid(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the background of the view.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//-----------------------------------------------------------------------
	virtual void FillPropertyGridView(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws grid borders.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     rcBorder - Reference to the size of the border to draw.
	//     nWindow  - XTPPropertyGridWindow enumeration value that specifies
	//                the window for which the border is to be drawn; possible
	//                values are as follows: 1 = Grid, 2 = Help, 3 = Verb.
	// See Also:
	//     FillPropertyGridView
	//-----------------------------------------------------------------------
	virtual void DrawPropertyGridBorder(CDC* pDC, RECT& rcBorder, XTPPropertyGridWindow nWindow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adjusts the bounding rectangle of the grid.
	// Parameters:
	//     rc - Reference to the bounding rectangle of the grid.
	//-----------------------------------------------------------------------
	virtual void OnNcCalcSize(RECT& rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a single item of the grid.
	// Parameters:
	//     lpDrawItemStruct - Long pointer to a DRAWITEMSTRUCT structure containing
	//                        information about the type of drawing required.
	//-----------------------------------------------------------------------
	virtual void DrawItem(PDRAWITEMSTRUCT lpDrawItemStruct);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a specified constraint in the in-place list.
	// Parameters:
	//     pDC         - Pointer to a valid device context.
	//     pConstraint - Pointer to the constraint to draw.
	//     rc          - Bounding rectangle of the constraint.
	//     bSelected   - TRUE if the constraint is currently selected.
	//-----------------------------------------------------------------------
	virtual void DrawInplaceListItem(CDC* pDC, CXTPPropertyGridItemConstraint* pConstraint,
									 CRect rc, BOOL bSelected);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adjusts the rectangle of the value part of a specified item.
	// Parameters:
	//     pItem   - Pointer to a property grid item.
	//     rcValue - Reference to the rectangle of the value part of the specified item.
	//-----------------------------------------------------------------------
	virtual void AdjustItemValueRect(CXTPPropertyGridItem* pItem, CRect& rcValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adjusts the rectangle of the caption part of a specified item.
	// Parameters:
	//     pItem   - Pointer to a property grid item.
	//     rcValue - Reference to the rectangle of the caption part of the specified item.
	//-----------------------------------------------------------------------
	virtual void AdjustItemCaptionRect(CXTPPropertyGridItem* pItem, CRect& rcCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Override this method and fill in a specified MEASUREITEMSTRUCT structure
	//     to inform Windows of the list-box dimensions.
	// Parameters:
	//     lpMeasureItemStruct - Long pointer to a MEASUREITEMSTRUCT structure.
	//-----------------------------------------------------------------------
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	void FillInplaceButtonThemed(CDC* pDC, CXTPPropertyGridInplaceButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws an in-place button for the grid.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     pButton - Pointer to a CXTPPropertyGridInplaceButton object.
	//-----------------------------------------------------------------------
	virtual void FillInplaceButton(CDC* pDC, CXTPPropertyGridInplaceButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the paint manager.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the indentifier of the scrollbar theme.
	// Returns:
	//     The identifier of the scrollbar theme; one of the values
	//     defined by the XTPScrollBarTheme enumeration.
	//-----------------------------------------------------------------------
	virtual XTPScrollBarTheme GetScrollBarTheme() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the index of the verb within the verbs collection, if any,
	//     at a specified point.
	// Parameters:
	//     pDC     - Reference to a valid device context.
	//     rcVerbs - Bounding rectangle of all verbs.
	//     pt      - Point to be tested.
	// Returns:
	//     The index of the verb within the verbs collection, if any,
	//     otherwise a value of -1.
	//-----------------------------------------------------------------------
	int HitTestVerbs(CDC* pDC, CRect rcVerbs, CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the default item metrics.
	// Returns:
	//     A pointer to the default item metrics.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemMetrics* GetItemMetrics() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called by the property grid to retrieve the size of
	//     either the help window splitter bar or the verbs splitter bar.
	// Parameters:
	//     rc      - Size of the property grid client area.
	//     bIsHelp - TRUE to retrieve the height of the help window splitter bar,
	//               FALSE to retrieve the height of the verbs splitter bar.
	// Returns:
	//     If bIsHelp is set to TRUE, then the size of the help window splitter bar.
	//     if bIsHelp is set to FALSE, then the size of the verbs splitter bar.
	//-----------------------------------------------------------------------
	CRect GetSplitterRect(CRect rc, BOOL bIsHelp);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the height of the horizontal grid line between property grid items.
	// Returns:
	//     The height, in pixels, of the horizontal grid line between property grid items.
	//-----------------------------------------------------------------------
	virtual int GetItemsBorderHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the height of the horizontal grid line between property grid items.
	// Parameters:
	//     nHeight - Height, in pixels, to be set.
	//-----------------------------------------------------------------------
	virtual void SetItemsBorderHeight(int nHeight);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Draws an expand button for the grid.
	// Parameters:
	//     dc        - Reference to a valid device context.
	//     pItem     - Pointer to a CXTPPropertyGridItem object.
	//     rcCaption - Caption button rectangle.
	//-----------------------------------------------------------------------
	virtual void DrawExpandButton(CDC& dc, CXTPPropertyGridItem* pItem, CRect rcCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the background for the bounding rectangle of a category caption.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     pItem - Pointer to a CXTPPropertyGridItem object.
	//     rc    - Bounding rectangle of the category caption.
	//-----------------------------------------------------------------------
	virtual void DrawCategoryCaptionBackground(CDC* pDC, CXTPPropertyGridItem* pItem, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the value part of a property grid item.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     pItem   - Pointer to a CXTPPropertyGridItem object.
	//     rcValue - Bounding rectangle of the value part.
	//-----------------------------------------------------------------------
	virtual void DrawItemValue(CDC* pDC, CXTPPropertyGridItem* pItem, CRect rcValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the caption part of a property grid item.
	// Parameters:
	//     pDC       - Pointer to a valid device context.
	//     pItem     - Pointer to a CXTPPropertyGridItem object.
	//     rcCaption - Bounding rectangle of the caption part.
	//-----------------------------------------------------------------------
	virtual void DrawItemCaption(CDC* pDC, CXTPPropertyGridItem* pItem, CRect rcCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the in-place buttons of a property grid item.
	// Parameters:
	//     pDC              - Pointer to a valid device context.
	//     lpDrawItemStruct - Long pointer to a DRAWITEMSTRUCT structure containing
	//                        information about the item to be drawn and
	//                        the type of drawing required.
	//     pItem            - Pointer to a CXTPPropertyGridItem object.
	//     rcValue          - Bounding rectangle of the value part.
	//-----------------------------------------------------------------------
	virtual void DrawInplaceButtons(CDC* pDC, PDRAWITEMSTRUCT lpDrawItemStruct,
									CXTPPropertyGridItem* pItem, CRect rcValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a checkbox for a CXTPPropertyGridItemOption item with
	//     with the CheckBox style.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     pButton - Pointer to a CXTPPropertyGridItemBool object.
	//-----------------------------------------------------------------------
	virtual void DrawCheckBox(CDC* pDC, CXTPPropertyGridItemBool* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a check mark.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     rc       - Rectangle in logical units.
	//     bEnabled - TRUE to set to enabled, FALSE to set to disabled.
	//     bChecked - TRUE to set to checked, FALSE to set to unchecked.
	//-----------------------------------------------------------------------
	virtual void DrawCheckMark(CDC* pDC, CRect rc, BOOL bEnabled, BOOL bChecked);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a radio button for a CXTPPropertyGridItemOption item
	//     with the RadioButton (default) style.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     rc       - Rectangle in logical units.
	//     bEnabled - TRUE to set to enabled, FALSE to set to disabled.
	//     bChecked - TRUE to set to checked, FALSE to set to unchecked.
	//-----------------------------------------------------------------------
	virtual void DrawRadioMark(CDC* pDC, CRect rc, BOOL bEnabled, BOOL bChecked);

	virtual void DrawSplitter(CDC* pDC, BOOL bIsHelp);

	//{{AFX_CODEJOCK_PRIVATE
	void DrawInplaceButtons(CDC* pDC, CXTPPropertyGridItem* pItem, CRect rcValue); // Obsolete
	//}}AFX_CODEJOCK_PRIVATE

#	ifdef _XTP_INCLUDE_CONTROLS
	void DrawCheckMarkThemed(CDC* pDC, CRect rc, BOOL bEnabled, BOOL bChecked);
	void DrawRadioMarkThemed(CDC* pDC, CRect rc, BOOL bEnabled, BOOL bChecked);
	void SetControlsTheme(int eTheme); // XTPControlTheme
#	endif

public:
	XTPPropertyGridInplaceButtonsStyle m_buttonsStyle; // TRUE to use WinXP themes.

	CXTPWinThemeWrapper* m_themeTree;   // Themes for tree controls.
	CXTPWinThemeWrapper* m_themeButton; // Themes for button controls
	CXTPWinThemeWrapper* m_themeCombo;  // Themes for combo button.
	BOOL m_bTransparent;				// TRUE to draw background with parent color.

#	ifdef _XTP_INCLUDE_CONTROLS
	CXTPButton* m_pButton;
	CXTPButton* m_pButtonCheck;
	CXTPComboBox* m_pCombo;
	BOOL m_bUseThemedControls;
#	else
private:
	CXTPExcludedMemberPadding<CXTPButton*> m_pButton;
	CXTPExcludedMemberPadding<CXTPButton*> m_pButtonCheck;
	CXTPExcludedMemberPadding<CXTPComboBox*> m_pCombo;
	CXTPExcludedMemberPadding<BOOL> m_bUseThemedControls;

public:
#	endif

public:
	CXTPPaintManagerColor m_clrFace;			  // Face color.
	CXTPPaintManagerColor m_clrShadow;			  // Shadow color.
	CXTPPaintManagerColor m_clrHighlight;		  // Highlight color.
	CXTPPaintManagerColor m_clrHighlightText;	 // Highlight text color.
	CXTPPaintManagerColor m_clrNonHighlightBack;  // Non-highlight background color.
	CXTPPaintManagerColor m_clrNonHighlightText;  // Non-highlight text color.
	CXTPPaintManagerColor m_clrCategorySeparator; // Face color.
	CXTPPaintManagerColor m_clrSplitter;		  // Splitter border color.

protected:
	int m_nItemsBorderHeight;  // Height, in pixels, of the horizontal border between
							   // property grid items.
	int m_nSplitterHeight;	 // Height, in pixels, of the splitter bar.
	int m_nHelpHeight;		   // Height, in pixels, of the description area (help pane).
	int m_nVerbsHeight;		   // Height, in pixels, of the verbs box.
	BOOL m_bThinSplitter;	  // TRUE to draw a single line for the splitter bar.
	CXTPPropertyGrid* m_pGrid; // Parent grid class.

	CXTPPropertyGridItemMetrics* m_pMetrics; // Default metrics of the item.

	friend class CXTPPropertyGrid;

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPPropertyGridPaintManager)
	DECLARE_PROPERTY(HelpBackColor, COLORREF);
	DECLARE_PROPERTY(HelpForeColor, COLORREF);
	DECLARE_PROPERTY(ViewBackColor, COLORREF);
	DECLARE_PROPERTY(ViewForeColor, COLORREF);
	DECLARE_PROPERTY(LineColor, COLORREF);
	DECLARE_PROPERTY(ViewCategoryForeColor, COLORREF);
	DECLARE_PROPERTY(ViewReadOnlyForeColor, COLORREF);

	DECLARE_PROPERTY(BackgroundColor, COLORREF);
	DECLARE_PROPERTY(ShadowColor, COLORREF);
	DECLARE_PROPERTY(HighlightBackColor, COLORREF);
	DECLARE_PROPERTY(HighlightForeColor, COLORREF);
	DECLARE_PROPERTY(HyperlinkColor, COLORREF);
	DECLARE_PROPERTY(ItemsBorderHeight, long);

//}}AFX_CODEJOCK_PRIVATE
#	endif
};

//===========================================================================
// Summary:
//     CXTPPropertyGridOffice2003Theme is a CXTPPropertyGridPaintManager
//     derived class. It represents an Office 2003 style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridOffice2003Theme : public CXTPPropertyGridPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridOffice2003Theme object.
	// Parameters:
	//     pGrid - Pointer to a CXTPPropertyGrid object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridOffice2003Theme(CXTPPropertyGrid* pGrid);

	//-------------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the property grid.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the scrollbar theme identifier.
	// Returns:
	//     The scrollbar theme identifier; one of the values
	//     defined by the XTPScrollBarTheme enumeration.
	//-----------------------------------------------------------------------
	virtual XTPScrollBarTheme GetScrollBarTheme() const;

protected:
	BOOL m_bLunaTheme; // TRUE to use luna colors, FALSE otherwise.
};

//===========================================================================
// Summary:
//     CXTPPropertyGridOffice2007Theme is a CXTPPropertyGridOffice2003Theme
//     derived class. It represents an Office 2007 style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridOffice2007Theme : public CXTPPropertyGridOffice2003Theme
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridOffice2003Theme object.
	// Parameters:
	//     pGrid - Pointer to a CXTPPropertyGrid object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridOffice2007Theme(CXTPPropertyGrid* pGrid);

	//-------------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the property grid.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Draws an in-place button for the grid.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     pButton - Pointer to a CXTPPropertyGridInplaceButton object.
	//-----------------------------------------------------------------------
	virtual void FillInplaceButton(CDC* pDC, CXTPPropertyGridInplaceButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a check mark.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     rc       - Rectangle in logical units.
	//     bEnabled - TRUE to set to enabled, FALSE to set to disabled.
	//     bChecked - TRUE to set to checked, FALSE to set to unchecked.
	//-----------------------------------------------------------------------
	virtual void DrawCheckMark(CDC* pDC, CRect rc, BOOL bEnabled, BOOL bChecked);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a radio button for a CXTPPropertyGridItemOption item
	//     with the RadioButton (default) style.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     rc       - Rectangle in logical units.
	//     bEnabled - TRUE to set to enabled, FALSE to set to disabled.
	//     bChecked - TRUE to set to checked, FALSE to set to unchecked.
	//-----------------------------------------------------------------------
	virtual void DrawRadioMark(CDC* pDC, CRect rc, BOOL bEnabled, BOOL bChecked);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws an expand button for the grid.
	// Parameters:
	//     dc        - Reference to a valid device context.
	//     pItem     - Pointer to a CXTPPropertyGridItem object.
	//     rcCaption - Caption button rectangle.
	//-----------------------------------------------------------------------
	void DrawExpandButton(CDC& dc, CXTPPropertyGridItem* pItem, CRect rcCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the scrollbar theme identifier.
	// Returns:
	//     The scrollbar theme identifier; one of the values
	//     defined by the XTPScrollBarTheme enumeration.
	//-----------------------------------------------------------------------
	virtual XTPScrollBarTheme GetScrollBarTheme() const;
};

//===========================================================================
// Summary:
//     CXTPPropertyGridNativeXPTheme is a CXTPPropertyGridPaintManager
//     derived class. It represents a Native Windows XP style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridNativeXPTheme : public CXTPPropertyGridPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridNativeXPTheme object.
	// Parameters:
	//     pGrid - Pointer to a CXTPPropertyGrid object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridNativeXPTheme(CXTPPropertyGrid* pGrid);

	//-------------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the property grid.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the scrollbar theme identifier.
	// Returns:
	//     The scrollbar theme identifier; one of the values
	//     defined by the XTPScrollBarTheme enumeration.
	//-----------------------------------------------------------------------
	virtual XTPScrollBarTheme GetScrollBarTheme() const;
};

//===========================================================================
// Summary:
//     CXTPPropertyGridOfficeXPTheme is a CXTPPropertyGridPaintManager
//     derived class. It represents an Office XP style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridOfficeXPTheme : public CXTPPropertyGridPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridOfficeXPTheme object.
	// Parameters:
	//     pGrid - Pointer to a CXTPPropertyGrid object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridOfficeXPTheme(CXTPPropertyGrid* pGrid);

	//-------------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the property grid.
	//-------------------------------------------------------------------------
	void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the scrollbar theme identifier.
	// Returns:
	//     The scrollbar theme identifier; one of the values
	//     defined by the XTPScrollBarTheme enumeration.
	//-----------------------------------------------------------------------
	virtual XTPScrollBarTheme GetScrollBarTheme() const;
};

//===========================================================================
// Summary:
//     CXTPPropertyGridCoolTheme is a CXTPPropertyGridPaintManager
//     derived class. It represents a Cool style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridCoolTheme : public CXTPPropertyGridPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridCoolTheme object.
	// Parameters:
	//     pGrid - Pointer to a CXTPPropertyGrid object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridCoolTheme(CXTPPropertyGrid* pGrid);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a single item for the grid.
	// Parameters:
	//     lpDrawItemStruct - Long pointer to a DRAWITEMSTRUCT structure containing
	//                        information about the type of drawing required.
	//-----------------------------------------------------------------------
	virtual void DrawItem(PDRAWITEMSTRUCT lpDrawItemStruct);

	//-------------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the property grid.
	//-------------------------------------------------------------------------
	void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the scrollbar theme identifier.
	// Returns:
	//     The scrollbar theme identifier; one of the values
	//     defined by the XTPScrollBarTheme enumeration.
	//-----------------------------------------------------------------------
	virtual XTPScrollBarTheme GetScrollBarTheme() const;
};

//===========================================================================
// Summary:
//     CXTPPropertyGridSimpleTheme is a CXTPPropertyGridPaintManager
//     derived class. It represents a Simple style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridSimpleTheme : public CXTPPropertyGridPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridSimpleTheme object.
	// Parameters:
	//     pGrid - Pointer to a CXTPPropertyGrid object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridSimpleTheme(CXTPPropertyGrid* pGrid);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a single item for the grid.
	// Parameters:
	//     lpDrawItemStruct - Long pointer to a DRAWITEMSTRUCT structure containing
	//                        information about the type of drawing required.
	//-----------------------------------------------------------------------
	virtual void DrawItem(PDRAWITEMSTRUCT lpDrawItemStruct);

	//-------------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the property grid.
	//-------------------------------------------------------------------------
	void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the scrollbar theme identifier.
	// Returns:
	//     The scrollbar theme identifier; one of the values
	//     defined by the XTPScrollBarTheme enumeration.
	//-----------------------------------------------------------------------
	virtual XTPScrollBarTheme GetScrollBarTheme() const;
};

//===========================================================================
// Summary:
//     CXTPPropertyGridDelphiTheme is a CXTPPropertyGridPaintManager
//     derived class. It represents a Delphi style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridDelphiTheme : public CXTPPropertyGridPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridDelphiTheme object.
	// Parameters:
	//     pGrid - Pointer to a CXTPPropertyGrid object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridDelphiTheme(CXTPPropertyGrid* pGrid);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a single item for the grid.
	// Parameters:
	//     lpDrawItemStruct - Long pointer to a DRAWITEMSTRUCT structure containing
	//                        information about the type of drawing required.
	//-----------------------------------------------------------------------
	virtual void DrawItem(PDRAWITEMSTRUCT lpDrawItemStruct);

	//-------------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the property grid.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the scrollbar theme identifier.
	// Returns:
	//     The scrollbar theme identifier; one of the values
	//     defined by the XTPScrollBarTheme enumeration.
	//-----------------------------------------------------------------------
	virtual XTPScrollBarTheme GetScrollBarTheme() const;
};

//===========================================================================
// Summary:
//     CXTPPropertyGridVisualStudio2005Theme is a CXTPPropertyGridPaintManager
//     derived class. It represents a Visual Studio 2005 style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridVisualStudio2005Theme : public CXTPPropertyGridPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridVisualStudio2005Theme object.
	// Parameters:
	//     pGrid - Pointer to a CXTPPropertyGrid object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridVisualStudio2005Theme(CXTPPropertyGrid* pGrid);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the background for the bounding rectangle of a category caption.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     pItem - Pointer to a CXTPPropertyGridItem object.
	//     rc    - Bounding rectangle of the category caption.
	//-----------------------------------------------------------------------
	void DrawCategoryCaptionBackground(CDC* pDC, CXTPPropertyGridItem* pItem, CRect rc);

	//-------------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the property grid.
	//-------------------------------------------------------------------------
	void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws an expand button for the grid.
	// Parameters:
	//     dc        - Reference to a valid device context.
	//     pItem     - Pointer to a CXTPPropertyGridItem object.
	//     rcCaption - Caption button rectangle.
	//-----------------------------------------------------------------------
	void DrawExpandButton(CDC& dc, CXTPPropertyGridItem* pItem, CRect rcCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the scrollbar theme identifier.
	// Returns:
	//     The scrollbar theme identifier; one of the values
	//     defined by the XTPScrollBarTheme enumeration.
	//-----------------------------------------------------------------------
	virtual XTPScrollBarTheme GetScrollBarTheme() const;
};

//===========================================================================
// Summary:
//     CXTPPropertyGridVisualStudio2010Theme is a CXTPPropertyGridPaintManager
//     derived class. It represents a Visual Studio 2010 style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridVisualStudio2010Theme : public CXTPPropertyGridPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridVisualStudio2010Theme object.
	// Parameters:
	//     pGrid - Pointer to a CXTPPropertyGrid object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridVisualStudio2010Theme(CXTPPropertyGrid* pGrid);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridVisualStudio2010Theme object,
	//     handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	~CXTPPropertyGridVisualStudio2010Theme();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Draws an expand button for the grid.
	// Parameters:
	//     dc        - Reference to a valid device context.
	//     pItem     - Pointer to a CXTPPropertyGridItem object.
	//     rcCaption - Caption button rectangle.
	//-----------------------------------------------------------------------
	virtual void DrawExpandButton(CDC& dc, CXTPPropertyGridItem* pItem, CRect rcCaption);

	//-------------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the property grid.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the scrollbar theme identifier.
	// Returns:
	//     The scrollbar theme identifier; one of the values
	//     defined by the XTPScrollBarTheme enumeration.
	//-----------------------------------------------------------------------
	virtual XTPScrollBarTheme GetScrollBarTheme() const;
};

//===========================================================================
// Summary:
//     CXTPPropertyGridVisualStudio2012Theme is a CXTPPropertyGridVisualStudio2010Theme
//     derived class. It represents a Visual Studio 2012 style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridVisualStudio2012Theme : public CXTPPropertyGridPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridVisualStudio2012Theme object.
	// Parameters:
	//     pGrid - Pointer to a CXTPPropertyGrid object
	//-----------------------------------------------------------------------
	CXTPPropertyGridVisualStudio2012Theme(CXTPPropertyGrid* pGrid, BOOL bLight);

	//-------------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the property grid.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws an expand button for the grid.
	// Parameters:
	//     dc        - Reference to a valid device context.
	//     pItem     - Pointer to a CXTPPropertyGridItem object.
	//     rcCaption - Caption button rectangle.
	//-----------------------------------------------------------------------
	virtual void DrawExpandButton(CDC& dc, CXTPPropertyGridItem* pItem, CRect rcCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the scrollbar theme identifier.
	// Returns:
	//     The scrollbar theme identifier; one of the values
	//     defined by the XTPScrollBarTheme enumeration.
	//-----------------------------------------------------------------------
	virtual XTPScrollBarTheme GetScrollBarTheme() const;

protected:
	BOOL m_bLight;
};

//===========================================================================
// Summary:
//     CXTPPropertyGridOffice2013Theme is a CXTPPropertyGridPaintManager
//     derived class. It represents an Office 2013 style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridOffice2013Theme : public CXTPPropertyGridPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridOffice2013Theme object.
	// Parameters:
	//     pGrid - Pointer to a CXTPPropertyGrid object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridOffice2013Theme(CXTPPropertyGrid* pGrid);

	//-------------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the property grid.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws an expand button for the grid.
	// Parameters:
	//     dc        - Reference to a valid device context.
	//     pItem     - Pointer to a CXTPPropertyGridItem object.
	//     rcCaption - Caption button rectangle.
	//-----------------------------------------------------------------------
	virtual void DrawExpandButton(CDC& dc, CXTPPropertyGridItem* pItem, CRect rcCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the scrollbar theme identifier.
	// Returns:
	//     The scrollbar theme identifier; one of the values
	//     defined by the XTPScrollBarTheme enumeration.
	//-----------------------------------------------------------------------
	virtual XTPScrollBarTheme GetScrollBarTheme() const;

	CXTPPaintManagerColor m_clrGlyphExpand;   // Expand glyph color.
	CXTPPaintManagerColor m_clrGlyphBorder;   // Expand glyph border color.
	CXTPPaintManagerColor m_clrGlyphContract; // Contract glyph color.
};

//===========================================================================
// Summary:
//     CXTPPropertyGridVisualStudio2015Theme is a CXTPPropertyGridVisualStudio2010Theme
//     derived class. It represents a Visual Studio 2015 style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridVisualStudio2015Theme
	: public CXTPPropertyGridVisualStudio2010Theme
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridVisualStudio2015Theme object.
	// Parameters:
	//     pGrid - Pointer to a CXTPPropertyGrid object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridVisualStudio2015Theme(CXTPPropertyGrid* pGrid);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws an expand button for the grid.
	// Parameters:
	//     dc        - Reference to a valid device context.
	//     pItem     - Pointer to a CXTPPropertyGridItem object.
	//     rcCaption - Caption button rectangle.
	//-----------------------------------------------------------------------
	virtual void DrawExpandButton(CDC& dc, CXTPPropertyGridItem* pItem, CRect rcCaption);

	//-------------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the property grid.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the scrollbar theme identifier.
	// Returns:
	//     The scrollbar theme identifier; one of the values
	//     defined by the XTPScrollBarTheme enumeration.
	//-----------------------------------------------------------------------
	virtual XTPScrollBarTheme GetScrollBarTheme() const;
};

//===========================================================================
// Summary:
//     CXTPPropertyGridVisualStudio2017Theme is a CXTPPropertyGridVisualStudio2010Theme
//     derived class. It represents a Visual Studio 2017 style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridVisualStudio2017Theme
	: public CXTPPropertyGridVisualStudio2010Theme
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridVisualStudio2017Theme object.
	// Parameters:
	//     pGrid - Pointer to a CXTPPropertyGrid object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridVisualStudio2017Theme(CXTPPropertyGrid* pGrid);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws an expand button for the grid.
	// Parameters:
	//     dc        - Reference to a valid device context.
	//     pItem     - Pointer to a CXTPPropertyGridItem object.
	//     rcCaption - Caption button rectangle.
	//-----------------------------------------------------------------------
	virtual void DrawExpandButton(CDC& dc, CXTPPropertyGridItem* pItem, CRect rcCaption);

	//-------------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the property grid.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the scrollbar theme identifier.
	// Returns:
	//     The scrollbar theme identifier; one of the values
	//     defined by the XTPScrollBarTheme enumeration.
	//-----------------------------------------------------------------------
	virtual XTPScrollBarTheme GetScrollBarTheme() const;
};

//===========================================================================
// Summary:
//     CXTPPropertyGridVisualStudio2019Theme is a CXTPPropertyGridVisualStudio2010Theme
//     derived class. It represents a Visual Studio 2019 style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridVisualStudio2019Theme
	: public CXTPPropertyGridVisualStudio2010Theme
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridVisualStudio2019Theme object.
	// Parameters:
	//     pGrid - Pointer to a CXTPPropertyGrid object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridVisualStudio2019Theme(CXTPPropertyGrid* pGrid);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws an expand button for the grid.
	// Parameters:
	//     dc        - Reference to a valid device context.
	//     pItem     - Pointer to a CXTPPropertyGridItem object.
	//     rcCaption - Caption button rectangle.
	//-----------------------------------------------------------------------
	virtual void DrawExpandButton(CDC& dc, CXTPPropertyGridItem* pItem, CRect rcCaption);

	//-------------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the property grid.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the scrollbar theme identifier.
	// Returns:
	//     The scrollbar theme identifier; one of the values
	//     defined by the XTPScrollBarTheme enumeration.
	//-----------------------------------------------------------------------
	virtual XTPScrollBarTheme GetScrollBarTheme() const;
};

//===========================================================================
// Summary:
//     CXTPPropertyGridVisualStudio2022Theme is a CXTPPropertyGridVisualStudio2010Theme
//     derived class. It represents a Visual Studio 2022 style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridVisualStudio2022Theme
	: public CXTPPropertyGridVisualStudio2010Theme
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridVisualStudio2022Theme object.
	// Parameters:
	//     pGrid - Pointer to a CXTPPropertyGrid object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridVisualStudio2022Theme(CXTPPropertyGrid* pGrid);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws an expand button for the grid.
	// Parameters:
	//     dc        - Reference to a valid device context.
	//     pItem     - Pointer to a CXTPPropertyGridItem object.
	//     rcCaption - Caption button rectangle.
	//-----------------------------------------------------------------------
	virtual void DrawExpandButton(CDC& dc, CXTPPropertyGridItem* pItem, CRect rcCaption);

	//-------------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the property grid.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the scrollbar theme identifier.
	// Returns:
	//     The scrollbar theme identifier; one of the values
	//     defined by the XTPScrollBarTheme enumeration.
	//-----------------------------------------------------------------------
	virtual XTPScrollBarTheme GetScrollBarTheme() const;
};

//===========================================================================
// Summary:
//     CXTPPropertyGridNativeWindows10Theme is a CXTPPropertyGridVisualStudio2010Theme
//     derived class. It represents a Native Windows 10 style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridNativeWindows10Theme
	: public CXTPPropertyGridVisualStudio2010Theme
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridNativeWindows10Theme object.
	// Parameters:
	//     pGrid - Pointer to a CXTPPropertyGrid object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridNativeWindows10Theme(CXTPPropertyGrid* pGrid);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws an expand button for the grid.
	// Parameters:
	//     dc        - Reference to a valid device context.
	//     pItem     - Pointer to a CXTPPropertyGridItem object.
	//     rcCaption - Caption button rectangle.
	//-----------------------------------------------------------------------
	virtual void DrawExpandButton(CDC& dc, CXTPPropertyGridItem* pItem, CRect rcCaption);

	//-------------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the property grid.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the scrollbar theme identifier.
	// Returns:
	//     The scrollbar theme identifier; one of the values
	//     defined by the XTPScrollBarTheme enumeration.
	//-----------------------------------------------------------------------
	virtual XTPScrollBarTheme GetScrollBarTheme() const;
};

AFX_INLINE CXTPPropertyGridItemMetrics* CXTPPropertyGridPaintManager::GetItemMetrics() const
{
	return m_pMetrics;
}

AFX_INLINE void CXTPPropertyGridPaintManager::DrawInplaceButtons(CDC* pDC,
																 CXTPPropertyGridItem* pItem,
																 CRect rcValue)
{
	DrawInplaceButtons(pDC, NULL, pItem, rcValue);
}

AFX_INLINE int CXTPPropertyGridPaintManager::GetItemsBorderHeight() const
{
	return m_nItemsBorderHeight;
}

AFX_INLINE void CXTPPropertyGridPaintManager::SetItemsBorderHeight(int nHeight)
{
	m_nItemsBorderHeight = nHeight;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(_XTPPROPERTYGRIDPAINTMANAGER_H__)
