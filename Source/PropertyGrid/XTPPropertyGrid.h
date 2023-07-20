// XTPPropertyGrid.h interface for the CXTPPropertyGrid class.
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
#if !defined(__XTPPROPERTYGRID_H__)
#	define __XTPPROPERTYGRID_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

// Starting from version 19.2, PropertyGrid supports scrollbar themes.
// However, due to technical challenges, having scrollbar themes enabled has
// also introduced a number of issues for various use cases.
// If you are affected by any of those issues and would prefer to fallback to
// pre-19.2 behavior at the price of disabling scrollbar themes, then you can
// either uncomment the XTP_PROPERTY_GRID_DISABLE_SCROLLBAR_THEMES macro at
// the beginning of the XTPPropertyGrid.h file or specify that macro as defined
// in either the C++ compiler properties of the ToolkitPro or PropertyGrid
// library project for selected configurations and then re-build either the
// ToolkitPro or PropertyGrid project in order for the change to take effect.
#	ifndef XTP_PROPERTY_GRID_DISABLE_SCROLLBAR_THEMES
#		define XTP_PROPERTY_GRID_DISABLE_SCROLLBAR_THEMES
#	endif

class CXTPToolTipContext;
class CXTPImageManager;
class CXTPMarkupContext;
class CXTPPropertyGridPaintManager;

class CXTPPropertyGridView;
class CXTPPropertyGridPaintManager;
class CXTPPropertyGrid;

//===========================================================================
// Summary:
//     CXTPPropertyGridUpdateContext is a CXTPCmdTarget derived class used
//     to save the state of the property grid such as the currently
//     selected item, XTPPropertyGridSortOrder preference, and the expanding state
//     of each item in the grid. The property grid state information is
//     stored in the CXTPPropertyGridUpdateContext object when
//     CXTPPropertyGrid::BeginUpdate is called. After CXTPPropertyGrid::EndUpdate
//     is called, the expanding state of each item, selected item, and
//     sort preference will be restored.
// Remarks:
//     Each time you add a new item or category to the grid, the state
//     of the grid is updated and the grid is redrawn.
//
//     If you want to delete all items in the grid and then add a lot of
//     new items, then it will be quite slow. To do this, you would call
//     BeginUpdate, then delete all the items. The property grid will be
//     locked until you call EndUpdate.
//
//     It might be useful to call BeginWaitCursor to indicate that the
//     grid is "busy" updating.
// See Also:
//     CXTPPropertyGrid::BeginUpdate, CXTPPropertyGrid::EndUpdate
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridUpdateContext : public CXTPCmdTarget
{
	DECLARE_DYNAMIC(CXTPPropertyGridUpdateContext);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridUpdateContext object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridUpdateContext();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if any items in the grid were expanded before
	//     CXTPPropertyGrid::BeginUpdate was called.
	// Remarks:
	//     Only items with children can be expanded.
	// Returns:
	//     TRUE if no items were expanded before BeginUpdate,
	//     FALSE if at least one item was expanded.
	//-----------------------------------------------------------------------
	BOOL IsEmpty() const;

protected:
	CMap<UINT, UINT, BOOL, BOOL&> m_mapState; // Stores the expanded state of each item in the grid
											  // before CXTPPropertyGrid::BeginUpdate.
	UINT m_nSelected;						  // Stores the ID of the currently selected item before
											  // CXTPPropertyGrid::BeginUpdate.
	UINT m_nTopIndex; // Stores the index of the top-most visible item in the property grid.
	XTPPropertyGridSortOrder m_propertySort; // Stores the currently used sort method of the grid
											 // before CXTPPropertyGrid::BeginUpdate.

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPPropertyGridUpdateContext)
//}}AFX_CODEJOCK_PRIVATE
#	endif

	friend class CXTPPropertyGrid;
};

class CXTPPropertyGridVerbs;

//===========================================================================
// Summary:
//     CXTPPropertyGridVerb is used to create a verb link similar to what is seen
//     in Visual Studio .NET. For example, when the DataGrid control is selected at
//     design time in VS .NET, an AutoFormat link button appears in the Properties window
//     just below the property grid and above the help panel.
//     Verbs will be displayed in a separate panel just below the property grid. Verbs
//     are links that can be used to perform actions in your application. When a verb is
//     clicked, CXTPPropertyGrid::OnVerbClick is called by the framework.
//     If both the verb panel and help panel are visible at the same time, then the
//     verb panel is displayed above the help panel.
// See Also:
//     CXTPPropertyGrid::GetVerbs, CXTPPropertyGridVerbs
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridVerb : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridVerb object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridVerb();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the text caption of this verb link.
	// Returns:
	//     The text caption of this verb link.
	//-----------------------------------------------------------------------
	CString GetCaption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the ID of this verb link.
	// Returns:
	//     The ID of this verb link.
	//-----------------------------------------------------------------------
	int GetID() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the index of this verb link within the collection of verb links.
	// Returns:
	//     The index of this verb link within the collection of verb links.
	//-----------------------------------------------------------------------
	int GetIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if this verb link is focused.
	// Remarks:
	//     If a verb link is focused, then a focus rectangle is placed
	//     around the text caption of the verb link.
	// Returns:
	//     TRUE if this verb link is focused, FALSE if this verb link is not focused.
	//-----------------------------------------------------------------------
	BOOL IsFocused() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is used internally by PaintManager and PropertyGrid to set
	//     the bounding rectangle for verbs. You do not need to call this member.
	// Parameters:
	//     rc - Bounding rectangle of the verb.
	//-----------------------------------------------------------------------
	void SetPart(CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is used internally by PaintManager and PropertyGrid to get
	//     the bounding rectangle of verbs. You do not need to call this member.
	//     this member.
	// Returns:
	//     The bounding rectangle of the verb.
	//-----------------------------------------------------------------------
	CRect GetPart() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the last position that the user clicked.
	// Returns:
	//     The last position that the user clicked. If it was "clicked"
	//     by pressing the Enter/Return key while a verb has focus, then
	//     the bottom left position of the "clicked" verb is returned.
	//-----------------------------------------------------------------------
	CPoint GetClickPoint() const;

protected:
	CString m_strCaption;			 // Text caption of this verb link.
	int m_nID;						 // ID assigned to this verb link.
	int m_nIndex;					 // Index of this verb link in the collection of verbs.
	CRect m_rcPart;					 // Bounding rectangle of the verb.
	CPoint m_ptClick;				 // The last position that the user clicked.
	CXTPPropertyGridVerbs* m_pVerbs; // Collection of verb links.

	friend class CXTPPropertyGridVerbs;
	friend class CXTPPropertyGrid;

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPPropertyGridVerb)
//}}AFX_CODEJOCK_PRIVATE
#	endif
};

//===========================================================================
// Summary:
//     A collection of CXTPPropertyGridVerb objects that are displayed in the
//     verb panel. See CXTPPropertyGridVerb for a description of verbs.
// See Also:
//     CXTPPropertyGridVerb
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridVerbs : public CXTPCmdTarget
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridVerbs object.
	//-------------------------------------------------------------------------
	CXTPPropertyGridVerbs();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridVerbs object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	~CXTPPropertyGridVerbs();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if any verbs have been added to the collection of verbs.
	// Returns:
	//     TRUE if there are no verbs in the collection,
	//     FALSE if there is at least one verb in the collection.
	//-----------------------------------------------------------------------
	BOOL IsEmpty() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all verbs from the collection of verbs.
	//-----------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the verb at a specified index within the collection of verbs.
	// Parameters:
	//     nIndex - Index of the verb to retrieve.
	// Returns:
	//     A pointer to the verb at the specified index within the collection of verbs.
	//-----------------------------------------------------------------------
	CXTPPropertyGridVerb* GetAt(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the total number of verbs in the collection of verbs.
	// Returns:
	//     The total number of verbs in the collection of verbs.
	//-----------------------------------------------------------------------
	int GetCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a verb link to the verb panel.
	// Parameters:
	//     lpszCaption - Text caption of the verb link.
	//     nID         - ID to assign to the verb link.
	// Remarks:
	//     Call this method to add a CXTPPropertyGridVerb object to the
	//     collection of verbs. Verbs are displayed in the verb panel
	//     located just below the property grid and above the help panel.
	//-----------------------------------------------------------------------
	void Add(LPCTSTR lpszCaption, UINT nID);

protected:
	CArray<CXTPPropertyGridVerb*, CXTPPropertyGridVerb*> m_arrVerbs; // Collection of verb links.
	CXTPPropertyGrid* m_pGrid; // Pointer to the property grid that the verbs are drawn on.

	friend class CXTPPropertyGrid;
	friend class CXTPPropertyGridVerb;

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_ENUM_VARIANT(CXTPPropertyGridVerbs)

	LPDISPATCH OleItem(long nIndex);
	LPDISPATCH OleGetItem(long nIndex);
	int OleGetItemCount();

	DECLARE_OLETYPELIB_EX(CXTPPropertyGridVerbs)
//}}AFX_CODEJOCK_PRIVATE
#	endif
};

//-----------------------------------------------------------------------
// Summary:
//     XTPPropertyGridUI is an enumeration used by CXTPPropertyGrid
//     during navigation of the Property Grid, items, and verbs
//     that specifies which object should receive focus.
// Remarks:
//     After the Tab key or Shift+Tab key combinations are pressed,
//     focus will be given to the object specified in the XTPPropertyGridUI
//     enumeration (i.e. if xtpPropertyGridUIViewNext is passed into OnNavigate
//     when an item in the grid has focus, then the next item will receive focus.
// See Also:
//     CXTPPropertyGrid::OnNavigate, CXTPPropertyGrid::NavigateItems
//
// <KEYWORDS xtpPropertyGridUIParentPrev, xtpPropertyGridUIViewPrev, xtpPropertyGridUIView,
// xtpPropertyGridUIInplaceEdit, xtpPropertyGridUIInplaceButton, xtpPropertyGridUIViewNext,
// xtpPropertyGridUIVerb, xtpPropertyGridUIParentNext>
//-----------------------------------------------------------------------
enum XTPPropertyGridUI
{
	xtpPropertyGridUIParentPrev = -2, // Used to indicate that focus should be given to the previous
									  // object in the tab order.
	xtpPropertyGridUIViewPrev = -1, // If m_bTabItems is TRUE, used to indicate that focus should be
									// given to the previous item in the grid. This occurs when an
									// item has focus and Shift+Tab is pressed.
	xtpPropertyGridUIView = 0, // Used to indicate that focus should be given to the Property Grid.
	xtpPropertyGridUIInplaceEdit = 1,	// Used to indicate that the Tab key is pressed while an
										 // in-place edit box has focus.
	xtpPropertyGridUIInplaceControl = 2, // Used to indicate that the Tab key is pressed while an
										 // in-place edit box has focus.
	xtpPropertyGridUIInplaceButton = 3,  // Used to indicate that the Tab key is pressed while an
										 // in-place button has focus.
	xtpPropertyGridUIViewNext = 4,  // If m_bTabItems is TRUE, used to indicate that focus should be
									// given to the next item in the grid. This occurs when an item
									// has focus and the Tab key is pressed.
	xtpPropertyGridUIVerb = 5,		// Used to indicate that the Tab key is pressed while a verb has
									// focus.
	xtpPropertyGridUIParentNext = 6 // Used to indicate that focus should be given to the next
									// object in the tab order.
};

//-----------------------------------------------------------------------
// Summary:
//     XTPPropertyGridHitCode is an enumeration used by CXTPPropertyGrid
//     to determine which part of the property grid is located at a
//     specific point. CXTPPropertyGrid::HitTest is used to test
//     a point when the user clicks on the property grid.
// See Also:
//     CXTPPropertyGrid::HitTest
// Example:
//     See CXTPPropertyGrid::HitTest.
//
// <KEYWORDS xtpPropertyGridHitError, xtpPropertyGridHitHelpSplitter,
// xtpPropertyGridHitVerbsSplitter, xtpPropertyGridHitFirstVerb>
//-----------------------------------------------------------------------
enum XTPPropertyGridHitCode
{
	xtpPropertyGridHitError = -1,		 // Indicates that neither the help splitter,
										 // the verb splitter, or a verb was clicked.
	xtpPropertyGridHitHelpSplitter  = 1, // Indicates that the help splitter was clicked.
	xtpPropertyGridHitVerbsSplitter = 2, // Indicates that the verb splitter was clicked.
	xtpPropertyGridHitFirstVerb		= 3  // Indicates that a verb was clicked.
};

//===========================================================================
// Summary:
//     CXTPPropertyGrid is a CWnd derived class. It is used to create a property grid
//     control similar to the property window in VS.NET.
//     See the "PropertyGrid" demo for an example of its usage.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGrid : public CWnd
{
	DECLARE_DYNAMIC(CXTPPropertyGrid)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGrid object.
	//-----------------------------------------------------------------------
	CXTPPropertyGrid();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGrid object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPPropertyGrid();

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates the property grid control.
	// Parameters:
	//     rect        - Reference to the size of the control.
	//     pParentWnd  - Pointer to the parent of the control.
	//     nID         - Identifier of the property grid control.
	//     dwListStyle - List style of the items in the property grid.
	// Remarks:
	//     Available styles are listed below:
	//     * <p><b>LBS_OWNERDRAWFIXED</b>: The owner of the grid is responsible for
	//                                     drawing its contents; the items in
	//                                     the list box are the same height. Should be
	//                                     used with LBS_NOINTEGRALHEIGHT.
	//     * <p><b>LBS_OWNERDRAWVARIABLE</b>: The owner of the grid is responsible for
	//                                        drawing its contents; the items in
	//                                        the grid are variable in height. Should be
	//                                        used with LBS_NOINTEGRALHEIGHT.
	//     * <p><b>XTP_PGS_OWNERDRAW</b>: Apply this style to the property grid to use
	//                                    the owner draw feature. Catch XTP_PGN_DRAWITEM
	//                                    to draw an item. Recommended to use a custom
	//                                    paint manager instead (SetCustomTheme).
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     SetCustomTheme, SetTheme
	//-----------------------------------------------------------------------
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID, DWORD dwListStyle = 0);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a new category to the property grid control.
	// Parameters:
	//     lpszCaption - Name of the category to be added.
	//     nID         - Identifier of the new category.
	//     pCategory   - Allows for custom classses to be categories. If it is NULL, then
	//                   the property grid creates a default CXTPPropertyGridItem as a
	//                   category. If it is not NULL, then the specified item will be used.
	// Returns:
	//     A pointer to the CXTPPropertyGridItem object of the newly added category.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItem* AddCategory(LPCTSTR lpszCaption, CXTPPropertyGridItem* pCategory = NULL);
	CXTPPropertyGridItem* AddCategory(
		int nID, CXTPPropertyGridItem* pCategory =
					 NULL); //<COMBINE CXTPPropertyGrid::AddCategory@LPCTSTR@CXTPPropertyGridItem*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Inserts a new category to the property grid control.
	// Parameters:
	//     lpszCaption - Name of the category to be inserted.
	//     nID         - Identifier of the new category.
	//     nIndex      - Index of the category to be inserted.
	//     pCategory   - Allows for custom classses to be categories. If it is NULL, then
	//                   the property grid creates a default CXTPPropertyGridItem as a
	//                   category. If it is not NULL, then the specified item will be used.
	// Returns:
	//     A pointer to the CXTPPropertyGridItem object of the newly inserted category.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItem* InsertCategory(int nIndex, LPCTSTR lpszCaption,
										 CXTPPropertyGridItem* pCategory = NULL);
	CXTPPropertyGridItem* InsertCategory(
		int nIndex, int nID,
		CXTPPropertyGridItem* pCategory =
			NULL); //<COMBINE CXTPPropertyGrid::InsertCategory@int@LPCTSTR@CXTPPropertyGridItem*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the grouping style for the property grid control.
	// Parameters:
	//     sort - Grouping style to be set; can be any of the
	//            values listed in the Remarks section.
	// Remarks:
	//     sort parameter can be one of the following:
	//     * <b>xtpPropertyGridSortCategorized</b>: Group items by category.
	//     * <b>xtpPropertyGridSortAlphabetical</b>: Sort items alphabetically.
	//     * <b>xtpPropertyGridSortNoSort</b>: Disable sorting.
	// See Also:
	//     GetPropertySort
	//-----------------------------------------------------------------------
	void SetPropertySort(XTPPropertyGridSortOrder sort);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the grouping style of the property grid control.
	// Returns:
	//     The grouping style of the property grid control; one of the
	//     values defined by the XTPPropertyGridSortOrder enumeration.
	// See Also:
	//     SetPropertySort
	//-----------------------------------------------------------------------
	XTPPropertyGridSortOrder GetPropertySort() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Shows/hides the toolbar control in the property grid window.
	// Parameters:
	//     bShow - TRUE to show the toolbar control, FALSE to hide.
	//-----------------------------------------------------------------------
	void ShowToolBar(BOOL bShow = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Shows/hides the description pane in the property grid window.
	// Parameters:
	//     bShow - TRUE to show the description pane, FALSE to hide.
	//-----------------------------------------------------------------------
	void ShowHelp(BOOL bShow = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Shows/hides the view pane.
	// Parameters:
	//     bShow - TRUE to show the view pane, FALSE to hide.
	//-----------------------------------------------------------------------
	void ShowView(BOOL bShow = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables tooltips under items.
	// Parameters:
	//     bEnable - TRUE to enable tooltips, FALSE to disable.
	//-----------------------------------------------------------------------
	void EnableToolTips(BOOL bEnable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the custom colors for the property grid control.
	// Parameters:
	//     clrHelpBack     - An RGB value that represents the background color of
	//                       the description pane.
	//     clrHelpFore     - An RGB value that represents the text color of
	//                       the description pane.
	//     clrViewLine     - An RGB value that represents the color of separating lines
	//                       in the property grid control.
	//     clrViewBack     - An RGB value that represents the background color
	//                       of the property grid control.
	//     clrViewFore     - An RGB value that represents the text color of
	//                       the property grid control.
	//     clrCategoryFore - An RGB value that represents the text color of
	//                       the category item text.
	//-----------------------------------------------------------------------
	void SetCustomColors(COLORREF clrHelpBack, COLORREF clrHelpFore, COLORREF clrViewLine,
						 COLORREF clrViewBack, COLORREF clrViewFore,
						 COLORREF clrCategoryFore = ::GetSysColor(COLOR_GRAYTEXT));

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the default colors for the property grid control.
	//-----------------------------------------------------------------------
	void SetStandardColors();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves an item by its caption/identifier.
	// Parameters:
	//     strCaption - Caption of the item to retrieve.
	//     nID        - Indentifier of the item to retrieve.
	// Returns:
	//     A pointer to the CXTPPropertyGridItem object if found, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItem* FindItem(LPCTSTR strCaption) const;
	CXTPPropertyGridItem* FindItem(UINT nID) const; // <COMBINE
													// CXTPPropertyGrid::FindItem@LPCTSTR@const>

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all items from the list box of the property grid control.
	//-----------------------------------------------------------------------
	void ResetContent();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the visible state of the help pane.
	// Returns:
	//     TRUE if the help pane is visible, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsHelpVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the visible state of the toolbar.
	// Returns:
	//     TRUE if the toolbar is visible, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsBarVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the height of the help pane when it is visible.
	// Returns:
	//     The height, in pixels, of the help pane when it is visible.
	//-----------------------------------------------------------------------
	int GetHelpHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the background color of the help pane.
	// Returns:
	//     The background color of the help pane.
	//-----------------------------------------------------------------------
	COLORREF GetHelpBackColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the foreground color of the help pane.
	// Returns:
	//     The foreground color of the help pane.
	//-----------------------------------------------------------------------
	COLORREF GetHelpForeColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the collection of categories of the property grid.
	// Returns:
	//     A pointer to the collection of categories of the property grid.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItems* GetCategories() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the collection of verbs displayed in the verb panel.
	// Returns:
	//     A pointer to the collection of verbs displayed in the verb panel.
	//-----------------------------------------------------------------------
	CXTPPropertyGridVerbs* GetVerbs() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the view of the property grid.
	// Returns:
	//     A reference to the view of the property grid.
	//-----------------------------------------------------------------------
	CXTPPropertyGridView& GetGridView() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the toolbar of the property grid.
	// Returns:
	//     A reference to the toolbar of the property grid.
	//-----------------------------------------------------------------------
	CToolBar& GetToolBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the currently selected item.
	// Returns:
	//     A pointer to the currently selected item, if any, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItem* GetSelectedItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the item at a specified index in the list.
	// Parameters:
	//     nIndex - Zero-based index of the item to retrieve.
	// Returns:
	//     A pointer to the item at the specified index in the list.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItem* GetItem(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the height for the help pane when it is visible.
	// Parameters:
	//     nHeight - Height, in pixels, to be set.
	//-----------------------------------------------------------------------
	void SetHelpHeight(int nHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the divider position for the grid view.
	// Parameters:
	//     dDivider - Double value between 0 and 1 that represents the location
	//                of the divider as a percentage from the left side of the
	//                view. For example, set this parameter to 0.25 in order to
	//                set the divider to divide at 25%.
	//-----------------------------------------------------------------------
	void SetViewDivider(double dDivider);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the divider position for the grid view.
	// Parameters:
	//     nDivider     - Divider position, in pixels, to be set.
	//     bLockDivider - TRUE to prevent the divider from scaling with the
	//                    property grid when the property grid is resized.
	//-----------------------------------------------------------------------
	void SetViewDividerPos(int nDivider, BOOL bLockDivider = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the divider position of the grid view.
	// Returns:
	//     A double value between 0 and 1 that represents the location of the
	//     divider as a percentage from the left side of the view. For example,
	//     0.25 would be returned if the divider is set to divide at 25%.
	//-----------------------------------------------------------------------
	double GetViewDivider() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the font of the property grid.
	// Parameters:
	//     pFont - Pointer to the font to be set.
	//-----------------------------------------------------------------------
	void SetFont(CFont* pFont);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the theme for the property grid.
	// Parameters:
	//     paintTheme - Theme to be set defined by the XTPPropertyGridPaintTheme enumeration.
	//-----------------------------------------------------------------------
	void SetTheme(XTPPropertyGridPaintTheme paintTheme);
	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATE("One of XTPPropertyGridPaintTheme constants must be used. Rename xtpGrid* to "
				   "xtpPropertyGrid* constant being used.")
	void SetTheme(XTPGridPaintTheme paintTheme);
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets a custom theme for the property grid.
	// Parameters:
	//     pPaintManager - Pointer to a CXTPPropertyGridPaintManager object.
	//-----------------------------------------------------------------------
	void SetCustomTheme(CXTPPropertyGridPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the theme of the property grid.
	// Returns:
	//     The theme of the property grid; one of the values
	//     defined by the XTPPropertyGridPaintTheme enumeration.
	//-----------------------------------------------------------------------
	XTPPropertyGridPaintTheme GetCurrentTheme() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Refreshes the items in the list.
	//-----------------------------------------------------------------------
	void Refresh();

	//-----------------------------------------------------------------------
	// Summary:
	//     "Locks" the property grid. This method should be called
	//     when adding or deleting a large number of items.
	// Parameters:
	//     context       - Reference to a CXTPPropertyGridUpdateContext object.
	//     bResetContent - TRUE to delete all items.
	// Remarks:
	//     Each time you add a new item or category to the grid, the state
	//     of the grid is updated and the grid is redrawn.
	//
	//     If you want to delete all items in the grid and then add a lot of
	//     new items, then it will be quite slow. To do this, you would call
	//     BeginUpdate, then delete all the items. The property grid will be
	//     locked until you call EndUpdate.
	//
	//     It might be useful to call BeginWaitCursor to indicate that the
	//     grid is "busy" updating.
	// See Also:
	//     EndUpdate, CXTPPropertyGridUpdateContext
	//-----------------------------------------------------------------------
	void BeginUpdate(CXTPPropertyGridUpdateContext& context, BOOL bResetContent = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     "Unlocks" the property grid. This method should be called
	//     after BeginUpdate.
	// Parameters:
	//     context - Reference to a CXTPPropertyGridUpdateContext object.
	// See Also:
	//     BeginUpdate, CXTPPropertyGridUpdateContext
	//-----------------------------------------------------------------------
	void EndUpdate(CXTPPropertyGridUpdateContext& context);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the visible state of the verb panel.
	// Returns:
	//     TRUE if the verb panel is visible, otherwise FALSE.
	// See Also:
	//     CXTPPropertyGridVerb, CXTPPropertyGridVerbs
	//-----------------------------------------------------------------------
	BOOL IsVerbsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the paint manager associated with the property grid
	//     (i.e. the paint manager used to draw the property grid control window).
	// Returns:
	//     A pointer to the paint manager associated with the property grid.
	//-----------------------------------------------------------------------
	CXTPPropertyGridPaintManager* GetPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a property grid view.
	//     Override this method to create a custom property grid view.
	// Remarks:
	//     Starting from version 19.2, PropertyGrid supports scrollbar themes.
	//     However, due to technical challenges, having scrollbar themes enabled has
	//     also introduced a number of issues for various use cases.
	//     If you are affected by any of those issues and would prefer to fallback to
	//     pre-19.2 behavior at the price of disabling scrollbar themes, then you can
	//     either uncomment the XTP_PROPERTY_GRID_DISABLE_SCROLLBAR_THEMES macro at
	//     the beginning of the XTPPropertyGrid.h file or specify that macro as defined
	//     in either the C++ compiler properties of the ToolkitPro or PropertyGrid
	//     library project for selected configurations and then re-build either the
	//     ToolkitPro or PropertyGrid project in order for the change to take effect.
	// Returns:
	//     A pointer to the newly created property grid view.
	//-----------------------------------------------------------------------
	virtual CXTPPropertyGridView* CreateView() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called either when a user clicks on a verb or
	//     when a verb has focus and the Enter/Return key is pressed.
	//     Override this method to perform custom actions such as displaying a dialog.
	// Parameters:
	//     nIndex - Index of the verb within the collection of verbs.
	//     pt     - Point on screen that was clicked by the user (i.e. the position
	//              of the mouse cursor over the verb). This point is stored in the
	//              CXTPPropertyGridVerb::m_ptClick member which is a CPoint object.
	//              Coordinates are relative to the screen and not to the client.
	//-----------------------------------------------------------------------
	virtual void OnVerbClick(int nIndex, CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when items in the property grid are navigated
	//     by using keys on the keyboard. This includes navigating the built-in
	//     toolbar, verbs, and items in the property grid.
	// Parameters:
	//     nUIElement - Indicates which object should receive focus; must be one of
	//                  the values defined by the XTPPropertyGridUI enumeration.
	//     bForward   - TRUE if either the Tab key is pressed and items are being
	//                  navigated in the forward direction or if either
	//                  the Right or Down arrows are being used to navigate the items.
	//                  FALSE if Shift+Tab is pressed and items are being
	//                  navigated in the backward direction or if either
	//                  the Up or Left arrows are being used to navigate the items.
	//     pItem      - Pointer to the currently selected item.
	// See Also:
	//     XTPPropertyGridUI
	//-----------------------------------------------------------------------
	virtual void OnNavigate(XTPPropertyGridUI nUIElement, BOOL bForward,
							CXTPPropertyGridItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables item navigation by using the Tab key.
	// Parameters:
	//     bTabItems    - TRUE to enable item navigation by using the Tab key,
	//                    FALSE to disable item navigation by using the Tab key.
	//     bTabCaptions - TRUE to allow caption items (i.e. items which do not
	//                    have in-place controls) to function as Tab stops (i.e.
	//                    can receive focus when the Tab key is pressed), FALSE
	//                    to prevent caption items from functioning as Tab stops.
	// Remarks:
	//     If bTabItems is set to TRUE, then focus will move in the following order:
	//     caption, item value, item in-place button (if used), next item caption.
	//-----------------------------------------------------------------------
	void NavigateItems(BOOL bTabItems, BOOL bTabCaptions = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if in-place buttons should always be shown.
	// Parameters:
	//     bShow - TRUE to always show in-place buttons,
	//             FALSE to only show in-place buttons when their items are selected.
	// See Also:
	//     GetShowInplaceButtonsAlways
	//-----------------------------------------------------------------------
	void SetShowInplaceButtonsAlways(BOOL bShow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if in-place buttons are always shown.
	// Returns:
	//     TRUE if in-place buttons are always shown,
	//     FALSE if in-place buttons are only shown when their items are selected.
	// See Also:
	//     SetShowInplaceButtonsAlways
	//-----------------------------------------------------------------------
	BOOL GetShowInplaceButtonsAlways() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the height of the verb vox when it is visible.
	// Parameters:
	//     nHeight - Height, in pixels, to be set.
	// See Also:
	//     CXTPPropertyGridVerb, CXTPPropertyGridVerbs, IsVerbsVisible, GetVerbsHeight
	//-----------------------------------------------------------------------
	void SetVerbsHeight(int nHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the height of the verb box when it is visible.
	// Returns:
	//     The height, in pixels, of the verb box.
	// See Also:
	//     CXTPPropertyGridVerb, CXTPPropertyGridVerbs, IsVerbsVisible, SetVerbsHeight
	//-----------------------------------------------------------------------
	int GetVerbsHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if changed item highlighting is enabled/disabled.
	// Returns:
	//     TRUE if changed item highlighting is enabled,
	//     FALSE if changed item highlighting is disabled.
	// See Also:
	//     HighlightChangedItems, CXTPPropertyGridItem::IsValueChanged
	//-----------------------------------------------------------------------
	BOOL IsHighlightChangedItems() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables changed item highlighting.
	// Parameters:
	//     bHighlightChanged - TRUE to enable changed item highlighting,
	//                         FALSE to disable changed item highlighting.
	// See Also:
	//     IsHighlightChangedItems, CXTPPropertyGridItem::IsValueChanged
	//-----------------------------------------------------------------------
	void HighlightChangedItems(BOOL bHighlightChanged);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if highlighting is enabled/disabled for in-place button text
	//     when the item value is changed.
	// Returns:
	//     TRUE if highlighting is enabled for in-place button text
	//     when the item value is changed,
	//     FALSE if highlighting is disabled for in-place button text
	//     when the item value is changed.
	// See Also:
	//     HighlightChangedItems, CXTPPropertyGridItem::IsValueChanged
	//-----------------------------------------------------------------------
	BOOL IsHighlightChangedButtonItems() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables highlighting for in-place button text
	//     when the item value is changed.
	// Parameters:
	//     bHighlightChanged - TRUE to enable highlighting for in-place button text
	//                         when the item value is changed,
	//                         FALSE to disable highlighting for in-place button text
	//                         when the item value is changed.
	// See Also:
	//     IsHighlightChangedItems, CXTPPropertyGridItem::IsValueChanged
	//-----------------------------------------------------------------------
	void HighlightChangedButtonItems(BOOL bHighlightChanged);

	//-------------------------------------------------------------------------
	// Summary:
	//     Redraws the control.
	//-------------------------------------------------------------------------
	void RedrawControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the image manager associated with the property grid
	//     (i.e. the image manager that stores images in the property grid).
	// Returns:
	//     A pointer to the image manager associated with the property grid.
	//-----------------------------------------------------------------------
	CXTPImageManager* GetImageManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the image manager associated with the property grid
	//     (i.e. the image manager that stores images in the property grid).
	// Parameters:
	//     pImageManager - Pointer to the image manager to be set.
	//-----------------------------------------------------------------------
	void SetImageManager(CXTPImageManager* pImageManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if variable items height should be allowed.
	// Parameters:
	//     bVariable - TRUE to allow variable items height.
	// See Also:
	//     CXTPPropertyGridItem::SetHeight, CXTPPropertyGridItem::SetMultiLinesCount
	//-----------------------------------------------------------------------
	void SetVariableItemsHeight(BOOL bVariable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the in-place edit control.
	// Parameters:
	//     pInplaceEdit - Pointer to the in-place edit control to be set.
	// Example:
	//     <code>m_wndPropertyGrid.SetInplaceEdit(new CMyInplaceEdit());</code>
	// See Also:
	//     SetInplaceList, CXTPPropertyGridItem::GetInplaceEdit
	//-----------------------------------------------------------------------
	void SetInplaceEdit(CXTPPropertyGridInplaceEdit* pInplaceEdit);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the in-place list box control.
	// Parameters:
	//     pInplaceList - Pointer to the in-place list box control to be set.
	// Example:
	//     <code>m_wndPropertyGrid.SetInplaceList(new CMyInplaceList());</code>
	// See Also:
	//     SetInplaceEdit, CXTPPropertyGridItem::GetInplaceList
	//-----------------------------------------------------------------------
	void SetInplaceList(CXTPPropertyGridInplaceList* pInplaceList);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sends a specified message to the owner window.
	// Parameters:
	//     wParam - Specifies additional message-dependent information.
	//     lParam - Specifies additional message-dependent information.
	// Returns:
	//     The result of the message processing; this value depends on the message sent.
	//-----------------------------------------------------------------------
	virtual LRESULT SendNotifyMessage(WPARAM wParam = 0, LPARAM lParam = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the zero-based index of the first visible item (i.e. the top item)
	//     in the property grid.
	// Remarks:
	//     The item with the zero-based index of 0 will be the first visible item
	//     in the property grid initially. The first visible item in the property grid
	//     may change if the list box is scrolled.
	// Returns:
	//     The zero-based index of the first visible item (i.e. the top item)
	//     in the property grid if successful, otherwise a value of -1.
	//-----------------------------------------------------------------------
	int GetTopIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the zero-based index for the first visible item (i.e. the top item)
	//     in the property grid.
	// Parameters:
	//     nIndex - Zero-based index of the item to be set.
	// Remarks:
	//     This method ensures that the item with the zero-based index specified by
	//     nIndex is visible. This is accomplished by scrolling the property grid until
	//     either the item with the zero-based index specified by nIndex appears at
	//     the top of the property grid or until the maximum scroll range is reached.
	//-----------------------------------------------------------------------
	void SetTopIndex(int nIndex);

	//-------------------------------------------------------------------------
	// Summary:
	//     Gets the number of items in the property grid.
	// Remarks:
	//     Since indices are zero-based, the value returned by this method will be
	//     one greater than the zero-based index of the last item in the property grid.
	// Returns:
	//     The number of items in the property grid if successful,
	//     otherwise LB_ERR if an error occurs.
	//-------------------------------------------------------------------------
	int GetCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Registers the window class if it has not already been registered.
	// Parameters:
	//     hInstance - Instance of the resource where the property grid is located.
	// Returns:
	//     TRUE if the window class was successfully registered.
	//-----------------------------------------------------------------------
	BOOL RegisterWindowClass(HINSTANCE hInstance = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the user can resize the property grid splitter.
	// Parameters:
	//     bVariable - TRUE to allow the user to resize the property grid splitter.
	// Remarks:
	//     SetVariableSplitterPos allows the user to resize the
	//     property grid splitter by moving the mouse cursor over the
	//     splitter and dragging the mouse. A resize mouse cursor
	//     will appear when the mouse is positioned over the splitter.
	// See Also:
	//     GetVariableSplitterPos, SetViewDividerPos
	//-----------------------------------------------------------------------
	void SetVariableSplitterPos(BOOL bVariable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the user can resize the property grid splitter.
	// Remarks:
	//     SetVariableSplitterPos allows the user to resize the
	//     property grid splitter by moving the mouse cursor over the
	//     splitter and dragging the mouse. A resize mouse cursor
	//     will appear when the mouse is positioned over the splitter.
	// Returns:
	//     TRUE if the user can resize the property grid splitter.
	// See Also:
	//     SetVariableSplitterPos, SetViewDividerPos
	//-----------------------------------------------------------------------
	BOOL GetVariableSplitterPos() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the user can resize the help box with the mouse.
	// Parameters:
	//     bVariable - TRUE to allow the user to resize the help box with the mouse.
	// Remarks:
	//     SetVariableHelpHeight allows the user to resize the help box
	//     by moving the mouse cursor over the help splitter that
	//     appears between the bottom of the property grid and the top
	//     of the help box. A resize mouse cursor will appear when
	//     the mouse if positioned over the splitter.
	// See Also:
	//     SetHelpHeight, GetVariableHelpHeight
	//-----------------------------------------------------------------------
	void SetVariableHelpHeight(BOOL bVariable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the user can resize the help box with the mouse.
	// Remarks:
	//     SetVariableHelpHeight allows the user to resize the help box
	//     by moving the mouse cursor over the help splitter that
	//     appears between the bottom of the property grid and the top
	//     of the help box. A resize mouse cursor will appear when
	//     the mouse if positioned over the splitter.
	// Returns:
	//     TRUE if the user can resize the help box with the mouse.
	// See Also:
	//     SetHelpHeight, SetVariableHelpHeight
	//-----------------------------------------------------------------------
	BOOL GetVariableHelpHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the border style for the property grid.
	// Parameters:
	//     borderStyle - Border style to be set; must be one of the values
	//                   defined by the XTPPropertyGridBorderStyle enumeration.
	// See Also:
	//     GetBorderStyle, XTPPropertyGridBorderStyle
	//-----------------------------------------------------------------------
	void SetBorderStyle(XTPPropertyGridBorderStyle borderStyle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the border style of the property grid.
	// Returns:
	//     The border style of the property grid; one of the values
	//     defined by the XTPPropertyGridBorderStyle enumeration.
	// See Also:
	//     SetBorderStyle, XTPPropertyGridBorderStyle
	//-----------------------------------------------------------------------
	XTPPropertyGridBorderStyle GetBorderStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tooltip context.
	// Returns:
	//     A pointer to the tooltip context.
	//-----------------------------------------------------------------------
	CXTPToolTipContext* GetToolTipContext() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables multi-select mode.
	// Parameters:
	//     bMultiSelect - TRUE to enable multi-select mode,
	//                    FALSE to disable multi-select mode.
	// See Also:
	//     GetSelectedItems
	//-----------------------------------------------------------------------
	void EnableMultiSelect(BOOL bMultiSelect = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of selected items if multi-select mode is enabled.
	// Parameters:
	//     pItems - Pointer to a CArray to receive the indices of the selected items.
	// Returns:
	//     The number of selected items.
	// See Also:
	//     EnableMultiSelect
	//-----------------------------------------------------------------------
	int GetSelectedItems(CArray<int, int>* pItems) const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if right-to-left (RTL) mode is set.
	// Returns:
	//     TRUE if text is displayed using right-to-left (RTL)
	//     reading-order properties.
	//-----------------------------------------------------------------------
	BOOL IsLayoutRTL() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if right-to-left (RTL) mode should be set.
	// Parameters:
	//     bRightToLeft - TRUE to display text using right-to-left (RTL)
	//                    reading-order properties.
	//-----------------------------------------------------------------------
	void SetLayoutRTL(BOOL bRightToLeft);

	//-------------------------------------------------------------------------
	// Summary:
	//     Repositions the view and help pane inside the property grid.
	//-------------------------------------------------------------------------
	void Reposition();

	//-------------------------------------------------------------------------
	// Summary:
	//     Enables/disables edit operations for the property grid.
	// Parameters:
	//     bAllowEdit - TRUE to enable edit opertations for the property grid,
	//                  FALSE to disable edit operations for the property grid.
	//-------------------------------------------------------------------------
	void AllowEdit(BOOL bAllowEdit);

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Enables/disables markup for the property grid.
	// Parameters:
	//     bEnable - TRUE to enable markup for the property grid,
	//               FALSE to disable markup for the property grid.
	//-------------------------------------------------------------------------
	void EnableMarkup(BOOL bEnable = TRUE);

	//-------------------------------------------------------------------------
	// Summary:
	//     Gets the Markup context.
	// Returns:
	//     A pointer to the Markup context.
	//-------------------------------------------------------------------------
	CXTPMarkupContext* GetMarkupContext() const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called by the framework during the resizing of splitters.
	// Parameters:
	//     rc - Tracking rectangle.
	//-----------------------------------------------------------------------
	void OnInvertTracker(CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the help splitter, the verb splitter, or a verb was clicked.
	// Parameters:
	//     pt - Point to be tested.
	// Returns:
	//     The HitTest method returns one of the following values which specifies which part
	//     of the property grid is located under the coordinates specified by <i>pt</i>.
	//
	//     * <b>xtpPropertyGridHitError</b>: Indicates that something other than the help splitter,
	//                               the verb splitter, or a verb was clicked.
	//     * <b>xtpPropertyGridHitHelpSplitter</b>: Indicates that the help splitter was clicked.
	//     * <b>xtpPropertyGridHitVerbsSplitter</b>: Indicates that the verb splitter was clicked.
	//     * <b>xtpPropertyGridHitFirstVerb</b>: If the return value is greater than or equal
	//                                   to xtpPropertyGridHitFirstVerb, then the item that was
	//                                   clicked is a verb. You can retrieve the verb
	//                                   index by subtracting xtpPropertyGridHitFirstVerb from
	//                                   the return value. For example:
	// <code>
	// int iHit = m_wndPropertyGrid.HitTest(pt);
	// if (iHit >= xtpPropertyGridHitFirstVerb)
	// {
	//     // Get the index of the verb.
	//     m_iVerbIndex = (iHit-xtpPropertyGridHitFirstVerb);
	// }
	// </code>
	// See Also:
	//     XTPPropertyGridHitCode
	//-----------------------------------------------------------------------
	virtual int HitTest(CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called either when a new item is selected in the
	//     property grid or when focus is switched from one item to another.
	// Parameters:
	//     pItem - Pointer to the newly selected/focused item.
	// See Also:
	//     GetSelectedItem
	//-----------------------------------------------------------------------
	virtual void OnSelectionChanged(CXTPPropertyGridItem* pItem);

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called when the sort order is changed.
	// See Also:
	//     SetPropertySort
	//-------------------------------------------------------------------------
	virtual void OnSortChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     Recalculates both the position and size of the property grid and
	//     then resizes the property grid to a specified size.
	// Remarks:
	//     The new size for the property grid is 0, 0, cx, cy including
	//     the area that contains the toolbar and grid, but not including
	//     the area that contains the verb and help panel.
	// Parameters:
	//     cx - Bottom-right x- coordinate to be set for
	//          the property grid's bounding rectangle.
	//     cy - Bottom-right y- coordinate to be set for
	//          the property grid's bounding rectangle.
	//-----------------------------------------------------------------------
	virtual void Reposition(int cx, int cy);

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called when the "built-in" toolbar is created.
	//-------------------------------------------------------------------------
	virtual void CreateToolbar();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called when the toolbar buttons must be redrawn.
	// Remarks:
	//     The toolbar buttons must be redrawn when the sort order is changed
	//     while the toolbar is shown. The sort order can be changed either
	//     by code or by clicking on a button in the toolbar.
	//-------------------------------------------------------------------------
	void RefreshToolBarButtons();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called either when a verb receives focus or
	//     when focus is switched from one verb to another.
	//-------------------------------------------------------------------------
	void OnVerbsChanged();

	//-------------------------------------------------------------------------
	// Summary:
	//     Creates the property grid window.
	// Parameters:
	//     dwListStyle - Default list style for the view.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-------------------------------------------------------------------------
	BOOL CreateGridView(DWORD dwListStyle);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to refresh all binding in pItem tree. pItem can be NULL to refresh all
	//     items
	// Parameters:
	//     pItem - Root item to refresh
	//-------------------------------------------------------------------------
	void RefeshBindings(CXTPPropertyGridItem* pItem = NULL);

private:
	CWnd* GetNextGridTabItem(BOOL bForward);
	void RestoreExpandingState(CXTPPropertyGridItems* pItems,
							   CXTPPropertyGridUpdateContext& context);
	void SaveExpandingState(CXTPPropertyGridItems* pItems, CXTPPropertyGridUpdateContext& context);
	void RecreateView();

public:
	BOOL m_bHideSelection; // TRUE to hide the selection when the control does not have focus.

protected:
	BOOL m_bHelpVisible;				  // TRUE if the help panel is visible.
	BOOL m_bViewVisible;				  // TRUE if the view panel is visible.
	BOOL m_bEnableTooltips;				  // TRUE if tooltips are enabled.
	BOOL m_bVariableHelpHeight;			  // TRUE to allow the user to resize the help pane.
	BOOL m_bToolBarVisible;				  // TRUE if the built in toolbar is visible.
	CXTPPropertyGridToolBar m_wndToolbar; // "built-in" property grid toolbar.
	HCURSOR m_hCursorSplit;				  // Handle to the cursor when positioned over a splitter.
	HCURSOR m_hCursorHand;				  // Handle of the cursor.
	BOOL m_bTabItems;					  // TRUE to navigate items with the tab key.
	BOOL m_bTabCaptions;				  // TRUE to navigate captions with the tab key.

	CXTPPropertyGridVerbs* m_pVerbs; // Collection of verbs (links) displayed in the verb panel when
									 // the panel is visible.
	BOOL m_bVerbsVisible;			 // TRUE if the verb panel is visible.

	XTPPropertyGridPaintTheme m_themeCurrent;	  // Current theme.
	CXTPPropertyGridPaintManager* m_pPaintManager; // Current paint manager.
	int m_nFocusedVerb; // Index of the currently focused verb within the collection of verbs.
	mutable CXTPPropertyGridView* m_pView; // View pointer.
	BOOL m_bHighlightChanged;			   // TRUE to highlight changed values.
	BOOL m_bHighlightChangedButton;		   // TRUE to highlight changed in-place button value text.
	CXTPImageManager* m_pImageManager;	 // Image manager of the property grid.
	BOOL m_bVariableItemsHeight;		   // TRUE to allow variable items height.
	BOOL m_bPreSubclassWindow;			   // 'true' when initialized from PreSubclassWindow.

	CXTPPropertyGridInplaceEdit* m_pInplaceEdit;	// In-place edit control.
	CXTPPropertyGridInplaceList* m_pInplaceListBox; // In-place list box control.
	BOOL m_bShowInplaceButtonsAlways;				// TRUE to show all in-place buttons.
	CXTPToolTipContext* m_pToolTipContext;			// Tooltip context.
	BOOL m_bMultiSelect;							// TRUE for multi-select.
	CXTPMarkupContext* m_pMarkupContext;			// Markup context of the property grid.
	BOOL m_bAllowEdit;								// FALSE to prevent all edit operations.

private:
	CRect m_rectTracker;
	BOOL m_bVerbActivate;
	XTPPropertyGridBorderStyle m_borderStyle;

protected:
//{{AFX_CODEJOCK_PRIVATE
#	ifdef _XTP_ACTIVEX
	BOOL m_bFocusNext;
#	endif
	//{{AFX_VIRTUAL(CXTPPropertyGrid)
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPPropertyGrid)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg LRESULT OnPrintClient(WPARAM wParam, LPARAM /*lParam*/);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSortAlphabetic();
	afx_msg void OnSortCategorized();
	afx_msg void OnSysColorChange();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG
	afx_msg BOOL OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

	//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect,
				CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	void SetLayoutRTL(CWnd* pWnd, BOOL bRTLLayout);

	friend class CXTPPropertyGridView;
	friend class CPropertyGridCtrl;
	friend class CXTPPropertyGridVerb;
	friend class CXTPPropertyGridVerbs;
	friend class CXTPPropertyGridItem;
};

//////////////////////////////////////////////////////////////////////
AFX_INLINE BOOL CXTPPropertyGridUpdateContext::IsEmpty() const
{
	return m_mapState.IsEmpty();
}

AFX_INLINE CString CXTPPropertyGridVerb::GetCaption() const
{
	return m_strCaption;
}
AFX_INLINE int CXTPPropertyGridVerb::GetID() const
{
	return m_nID;
}
AFX_INLINE int CXTPPropertyGridVerb::GetIndex() const
{
	return m_nIndex;
}
AFX_INLINE void CXTPPropertyGridVerb::SetPart(CRect rc)
{
	m_rcPart = rc;
}
AFX_INLINE CRect CXTPPropertyGridVerb::GetPart() const
{
	return m_rcPart;
}
AFX_INLINE CPoint CXTPPropertyGridVerb::GetClickPoint() const
{
	return m_ptClick;
}
AFX_INLINE BOOL CXTPPropertyGridVerb::IsFocused() const
{
	return (m_nIndex == m_pVerbs->m_pGrid->m_nFocusedVerb)
		   && (::GetFocus() == m_pVerbs->m_pGrid->GetSafeHwnd());
}
AFX_INLINE CXTPPropertyGridVerb* CXTPPropertyGridVerbs::GetAt(int nIndex) const
{
	return m_arrVerbs.GetAt(nIndex);
}
AFX_INLINE BOOL CXTPPropertyGridVerbs::IsEmpty() const
{
	return m_arrVerbs.GetSize() == 0;
}
AFX_INLINE BOOL CXTPPropertyGrid::IsHelpVisible() const
{
	return m_bHelpVisible;
}
AFX_INLINE BOOL CXTPPropertyGrid::IsBarVisible() const
{
	return m_bToolBarVisible;
}
AFX_INLINE void CXTPPropertyGrid::EnableToolTips(BOOL bEnable)
{
	m_bEnableTooltips = bEnable;
}
AFX_INLINE CToolBar& CXTPPropertyGrid::GetToolBar()
{
	return m_wndToolbar;
}
AFX_INLINE XTPPropertyGridPaintTheme CXTPPropertyGrid::GetCurrentTheme() const
{
	return m_themeCurrent;
}
AFX_INLINE CXTPPropertyGridVerbs* CXTPPropertyGrid::GetVerbs() const
{
	return m_pVerbs;
}
AFX_INLINE BOOL CXTPPropertyGrid::IsVerbsVisible() const
{
	return !m_pVerbs->IsEmpty();
}
AFX_INLINE CXTPPropertyGridPaintManager* CXTPPropertyGrid::GetPaintManager() const
{
	return m_pPaintManager;
}
AFX_INLINE BOOL CXTPPropertyGrid::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
										 DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
										 UINT nID, CCreateContext* pContext)
{
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE void CXTPPropertyGrid::NavigateItems(BOOL bTabItems, BOOL bTabCaptions)
{
	m_bTabItems	= bTabItems;
	m_bTabCaptions = bTabCaptions;
}
AFX_INLINE BOOL CXTPPropertyGrid::IsHighlightChangedItems() const
{
	return m_bHighlightChanged;
}
AFX_INLINE void CXTPPropertyGrid::HighlightChangedItems(BOOL bHighlightChanged)
{
	m_bHighlightChanged = bHighlightChanged;
	RedrawControl();
}
AFX_INLINE BOOL CXTPPropertyGrid::IsHighlightChangedButtonItems() const
{
	return m_bHighlightChangedButton;
}
AFX_INLINE void CXTPPropertyGrid::HighlightChangedButtonItems(BOOL bHighlightChanged)
{
	m_bHighlightChangedButton = bHighlightChanged;
	RedrawControl();
}
AFX_INLINE void CXTPPropertyGrid::SetVariableHelpHeight(BOOL bVariable)
{
	m_bVariableHelpHeight = bVariable;
}
AFX_INLINE BOOL CXTPPropertyGrid::GetVariableHelpHeight() const
{
	return m_bVariableHelpHeight;
}
AFX_INLINE BOOL CXTPPropertyGrid::GetShowInplaceButtonsAlways() const
{
	return m_bShowInplaceButtonsAlways;
}
AFX_INLINE void CXTPPropertyGrid::SetShowInplaceButtonsAlways(BOOL bShow)
{
	m_bShowInplaceButtonsAlways = bShow;
	Reposition();
}
AFX_INLINE CXTPToolTipContext* CXTPPropertyGrid::GetToolTipContext() const
{
	return m_pToolTipContext;
}
AFX_INLINE CXTPMarkupContext* CXTPPropertyGrid::GetMarkupContext() const
{
	return m_pMarkupContext;
}
AFX_INLINE void CXTPPropertyGrid::SetTheme(XTPGridPaintTheme paintTheme)
{
	SetTheme(static_cast<XTPPropertyGridPaintTheme>(paintTheme));
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPPROPERTYGRID_H__)
