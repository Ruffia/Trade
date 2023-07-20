// XTPGridRecordItem.h: interface for the CXTPGridRecordItem class.
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
#if !defined(__XTPGRIDRECORDITEM_H__)
#	define __XTPGRIDRECORDITEM_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPMarkupUIElement;
class CXTPMarkupContext;
class CXTPGridControl;
class CXTPGridRow;
class CXTPGridRecordItem;
class CXTPGridHyperlink;
class CXTPGridHyperlinks;
class CXTPGridColumn;
class CXTPGridPaintManager;
class CXTPGridRecord;
class CXTPGridInplaceButton;
class CXTPPropExchange;
class CXTPGridRecordItemConstraint;
class CXTPGridRecordItemConstraints;
class CXTPGridRecordItemControlHookWnd;
class CXTPGridRecordItemControl;
class CXTPGridRecordItemControls;
class CXTPGridBorder;
class CXTPGridGroupRow;

//===========================================================================
// Summary:
//     Basic set of parameters transferred to item handlers. This structure
//     groups together essential parameters used in item processing activities.
//     It is the parent for all other structures:
//     XTP_GRIDRECORDITEM_DRAWARGS, XTP_GRIDRECORDITEM_CLICKARGS, etc.
// Example:
// <code>
// XTP_GRIDRECORDITEM_ARGS itemArgs(pControl, pRow, pColumn);
// pItem->OnBeginEdit(&itemArgs);
// </code>
// See Also:
//     CXTPGridRecordItem::OnBeginEdit
//===========================================================================
struct _XTP_EXT_CLASS XTP_GRIDRECORDITEM_ARGS
{
	//{{AFX_CODEJOCK_PRIVATE
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a XTP_GRIDRECORDITEM_ARGS object
	// Parameters:
	//     pControl - Parent Grid control pointer
	//     pRow     - Item's row pointer
	//     pColumn  - Item's column pointer
	//-----------------------------------------------------------------------
	XTP_GRIDRECORDITEM_ARGS();
	XTP_GRIDRECORDITEM_ARGS(CXTPGridControl* pControl, CXTPGridRow* pRow, CXTPGridColumn* pColumn);
	// <COMBINE XTP_GRIDRECORDITEM_ARGS::XTP_GRIDRECORDITEM_ARGS>

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGrid object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~XTP_GRIDRECORDITEM_ARGS();

	void AddRef();
	void Release();

	const XTP_GRIDRECORDITEM_ARGS& operator=(const XTP_GRIDRECORDITEM_ARGS& src);
	//}}AFX_CODEJOCK_PRIVATE

	CXTPGridControl* pControl; // Pointer to the main Grid control.
	CXTPGridRow* pRow;		   // Pointer to the associated row.
	CXTPGridColumn* pColumn;   // Grid column at click position, if any, NULL otherwise.
	CXTPGridRecordItem* pItem; // Pointer to the associated item.
	CRect rcItem;			   // Item position in control client coordinates.
};

//===========================================================================
// Summary:
//     A set of parameters transferred to the Draw item handler.
//     It inherits essential parameters from XTP_GRIDRECORDITEM_ARGS.
// Remarks:
//     This structure is used in almost all functions concerning drawing activities.
// Example:
// <code>
// // fill structure
// XTP_GRIDRECORDITEM_DRAWARGS drawArgs;
// drawArgs.pDC = pDC;
// drawArgs.pControl = m_pControl;
// drawArgs.pRow = pRow;
// // call function
// pItem->Draw(&drawArgs);
// </code>
// See Also:
//     CXTPGridPaintManager
//===========================================================================
struct _XTP_EXT_CLASS XTP_GRIDRECORDITEM_DRAWARGS : public XTP_GRIDRECORDITEM_ARGS
{
	//{{AFX_CODEJOCK_PRIVATE
	XTP_GRIDRECORDITEM_DRAWARGS()
	{
		nTextAlign = DT_LEFT;
		pDC		   = NULL;
	}
	//}}AFX_CODEJOCK_PRIVATE

	CDC* pDC;		// Pointer to control drawing context.
	int nTextAlign; // Text alignment mode (DT_LEFT || DT_RIGHT || DT_CENTER).
};

//===========================================================================
// Summary:
//     A set of parameters transferred to mouse click item handlers.
//     It inherits essential parameters from XTP_GRIDRECORDITEM_ARGS.
// Remarks:
//     XTP_GRIDRECORDITEM_CLICKARGS is used in most functions involved
//     in mouse event processing.
// See Also:
//     CXTPGridControl, CXTPGridRecordItem::OnClick
//===========================================================================
struct _XTP_EXT_CLASS XTP_GRIDRECORDITEM_CLICKARGS : public XTP_GRIDRECORDITEM_ARGS
{
	CPoint ptClient; // Coordinates of the mouse click point.
};

//===========================================================================
// Summary:
//     This structure is sent to the Main window in a WM_NOTIFY message from
//     Item and provides all parameters required by the Main window to process
//     control specific notifications.
// Example:
// <code>
// BEGIN_MESSAGE_MAP(CPropertiesView, CView)
//     ON_NOTIFY(XTP_NM_GRID_VALUECHANGED, ID_GRID_CONTROL, OnPropertyChanged)
// END_MESSAGE_MAP()
//
// void CPropertiesView::OnPropertyChanged(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_GRIDRECORDITEM* pItemNotify = (XTP_NM_GRIDRECORDITEM*) pNotifyStruct;
//
//     switch (pItemNotify->pItem->GetItemData())
//     {
//         // Some code.
//     }
// }
// </code>
// See Also:
//     XTP_NM_GRIDINPLACEBUTTON
//===========================================================================
struct XTP_NM_GRIDRECORDITEM
{
	NMHDR hdr;		   // Standard structure; contains information about a notification message.
	CXTPGridRow* pRow; // Pointer to the row associated with the notification.
	CXTPGridRecordItem* pItem; // Pointer to the record item associated with the notification.
	CXTPGridColumn* pColumn;   // Pointer to the column associated with the notification.
	int nHyperlink;			   // Index of clicked Hyperlink, if any, or -1 otherwise.
	POINT pt;				   // Point where the message ocurred.
};

//===========================================================================
// Summary:
//     A set of parameters transferred with an XTP_NM_GRID_REQUESTEDIT message.
// Remarks:
//     XTP_NM_GRIDREQUESTEDIT is used to cancel edit operations.
// See Also:
//     XTP_NM_GRID_REQUESTEDIT
//===========================================================================
struct XTP_NM_GRIDREQUESTEDIT : public XTP_NM_GRIDRECORDITEM
{
	BOOL bCancel; // TRUE to cancel operation.
};

#	ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
struct XTP_NM_GRIDVALUECHANGING : public XTP_NM_GRIDRECORDITEM
{
	BOOL bCancel;
	LPVARIANT lpNewValue;
};
//}}AFX_CODEJOCK_PRIVATE
#	endif

//-----------------------------------------------------------------------
// Summary:
//     This structure is sent to the Main window in a WM_NOTIFY message from
//     Item and provides all parameters required by the Main window to process
//     control specific notifications.
// Remarks:
//     Use this structure to get the in-place button which produces the
//     XTP_NM_GRID_INPLACEBUTTONDOWN message.
// See Also:
//     XTP_NM_GRID_INPLACEBUTTONDOWN
//-----------------------------------------------------------------------
struct XTP_NM_GRIDINPLACEBUTTON : public XTP_NM_GRIDRECORDITEM
{
	CXTPGridInplaceButton* pButton; // Pointer to in-place button.
};

//-----------------------------------------------------------------------
// Summary:
//     This structure is sent to the Main window in a WM_NOTIFY message from
//     Item and provides all parameters required by the Main window to process
//     control specific notifications.
// Remarks:
//     It is sent before clipboard Copy/Paste operations.
// See Also:
//      XTP_NM_GRID_BEFORE_COPY_TOTEXT, XTP_NM_GRID_BEFORE_PASTE_FROMTEXT,
//      XTP_NM_GRID_BEFORE_PASTE, CXTPGridControl::OnBeforeCopyToText(),
//      CXTPGridControl::OnBeforePasteFromText(),
//      CXTPGridControl::OnBeforePaste().
//-----------------------------------------------------------------------
struct XTP_NM_GRID_BEFORE_COPYPASTE
{
	NMHDR hdr; // Standard structure; contains information about a notification message.

	CXTPGridRecord** ppRecord; // [in/out] A pointer to a record pointer.
	CStringArray* parStrings;  // [in/out] A pointer to a strings array with record items values.
};

//-----------------------------------------------------------------------
// Summary:
//     This structure is sent to the Main window in a WM_NOTIFY message from
//     Item and provides all parameters required by the Main window to process
//     control specific notifications.
// Remarks:
//     Use this structure to notify about constraint changing.
// See Also:
//     XTP_NM_GRIDCONSTRAINTSELECTING
//-----------------------------------------------------------------------
struct XTP_NM_GRIDCONSTRAINTSELECTING : public XTP_NM_GRIDRECORDITEM
{
	CXTPGridRecordItemConstraint* pConstraint; // Pointer to the constraint associated with the
											   // notification.
};

//-----------------------------------------------------------------------
// Summary:
//     This structure is sent to the Main window in a WM_NOTIFY message from
//     Item and provides all parameters required by the Main window to process
//     control specific notifications.
// Remarks:
//     Use this structure to notify about tooltip showing.
// See Also:
//     XTP_NM_GRIDTOOLTIPTEXT
//-----------------------------------------------------------------------
struct XTP_NM_GRIDTOOLTIPINFO : public XTP_NM_GRIDRECORDITEM
{
	CString* pstrText; // Pointer to a CString object with Tooltip text.
};

//-----------------------------------------------------------------------
// Summary:
//      This enum defines the alignment style for the column icon and text.
//-----------------------------------------------------------------------
enum XTPGridColumnIconAlignment
{
	xtpColumnTextLeft	  = DT_LEFT,	  // Aligns text to the left.
	xtpColumnTextCenter	= DT_CENTER,	// Centers text horizontally in the column.
	xtpColumnTextRight	 = DT_RIGHT,	 // Aligns text to the right.
	xtpColumnTextVCenter   = DT_VCENTER,   // Centers text vertically.
	xtpColumnTextWordBreak = DT_WORDBREAK, // Breaks words. Lines are automatically broken between
										   // words if a word would extend past the edge of the
										   // rectangle. A carriage return-line feed sequence also
										   // breaks the line.

	xtpColumnTextMask = 0xFF, // A mask for text alignment styles.

	xtpColumnIconLeft   = 0x00100000, // Aligns icon to the left.
	xtpColumnIconCenter = 0x00200000, // Centers icon horizontally in the column.
	xtpColumnIconRight  = 0x00400000, // Aligns icon to the right.

	xtpColumnIconTop	 = 0x01000000, // Aligns icon to the top.
	xtpColumnIconVCenter = 0x02000000, // Aligns icon to the center.
	xtpColumnIconBottom  = 0x04000000, // Aligns icon to the bottom.

	xtpColumnIconMask = 0x0FF00000 // A mask for icon alignment styles.
};

//-----------------------------------------------------------------------
// Summary:
//      This enum defines alignment style for the group row custom icon.
// See Also:
//      XTP_GRIDRECORDITEM_METRICS::nGroupRowIconAlignment
//-----------------------------------------------------------------------
enum XTPGridGroupRowIconAlignment
{
	xtpGroupRowIconUnknown = 0, // Unknown (empty) value.

	xtpGroupRowIconLeft		  = 0x001, // Draw icon at the left side of group row rect.
	xtpGroupRowIconBeforeText = 0x002, // Draw icon before caption text (between Expand/Collapse
									   // icon and text).
	xtpGroupRowIconAfterText = 0x004,  // Draw icon after caption text.
	xtpGroupRowIconRight	 = 0x008,  // Draw icon at the right side of group row rect.

	xtpGroupRowIconHmask = 0x00F, // A mask for horizontal alignment flags.

	xtpGroupRowIconVTop			 = 0x100, // Vertical alignment: top of group row rect.
	xtpGroupRowIconVCenter		 = 0x200, // Vertical alignment: center of group row rect.
	xtpGroupRowIconVCenterToText = 0x400, // Vertical alignment: center of caption text rect.
	xtpGroupRowIconVBottom		 = 0x800, // Vertical alignment: bottom of group row rect.

	xtpGroupRowIconVmask = 0xF00, // A mask for vertical alignment flags.
};

//-----------------------------------------------------------------------
// Summary:
//      This enum defines flags for virtual row metrics.
// See Also:
//      XTP_GRIDRECORDITEM_METRICS::nVirtRowFlags
//-----------------------------------------------------------------------
enum XTPGridVirtualRowFlags
{
	xtpVirtRowUnknown	 = 0,	 // Unknown (empty) value.
	xtpVirtRowHasChildren = 0x001, // A row has children.
	xtpVirtRowLastChild   = 0x002  // A row is the last child of its parent.
};

//-----------------------------------------------------------------------
// Summary:
//     Record item drawing metrics.
//     Its helper structures group together parameters to store general
//     drawing metrics.
//-----------------------------------------------------------------------
struct XTP_GRIDRECORDITEM_METRICS : public CXTPCmdTarget
{
	CFont* pFont;			// Drawing font.
	COLORREF clrForeground; // Item foreground color.
	COLORREF clrBackground; // Item background color.
	CString strText;		// Item text.

	int nGroupRowIcon;			// Group row icon ID. See CXTPGridControl::GetImageManager().
	int nGroupRowIconAlignment; // Group row icon alignment. See XTPEnumGroupRowIconAlignment.
	int nColumnAlignment;		// Column alignment. See XTPGridColumnIconAlignment.
	int nItemIcon;				// Item icon ID. See CXTPGridControl::GetImageManager().
	int nVirtRowLevel;			// Virtual row level.
	int nVirtRowFlags;			// Virtual row flags.

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	CXTPFont m_xtpFontMetrics;
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fntMetrics, m_xtpFontMetrics,
										  GetMetricsFontHandle);
	//}}AFX_CODEJOCK_PRIVATE
#	endif

	//{{AFX_CODEJOCK_PRIVATE
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a XTP_GRIDRECORDITEM_METRICS struct
	//-------------------------------------------------------------------------
	XTP_GRIDRECORDITEM_METRICS()
	{
		Reset();

#	ifdef _XTP_ACTIVEX
		EnableAutomation();
#	endif
	}

	void Reset()
	{
		pFont		  = NULL;
		clrForeground = XTP_GRID_COLOR_DEFAULT;
		clrBackground = XTP_GRID_COLOR_DEFAULT;

		nGroupRowIcon		   = XTP_GRID_NOICON;
		nGroupRowIconAlignment = xtpGroupRowIconRight | xtpGroupRowIconVCenter;
		nColumnAlignment	   = xtpColumnTextLeft;
		nItemIcon			   = XTP_GRID_NOICON;
		nVirtRowLevel		   = 0;
		nVirtRowFlags		   = 0;
	}
	//}}AFX_CODEJOCK_PRIVATE

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	~XTP_GRIDRECORDITEM_METRICS()
	{
	}

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	LPFONTDISP OleGetFont();
	void OleSetFont(LPFONTDISP pFontDisp);

//}}AFX_CODEJOCK_PRIVATE
#	endif
};

//===========================================================================
// Summary:
//     This class represents a collection of in-place buttons
//     for a single item of the Grid control.
//===========================================================================
class _XTP_EXT_CLASS CXTPGridInplaceButtons
	: public CArray<CXTPGridInplaceButton*, CXTPGridInplaceButton*>
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPGridInplaceButtons object.
	//-----------------------------------------------------------------------
	CXTPGridInplaceButtons();
};

//===========================================================================
// Summary:
//     This class represents edit options for a single item or column.
//===========================================================================
class _XTP_EXT_CLASS CXTPGridRecordItemEditOptions : public CXTPCmdTarget
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPGridRecordItemEditOptions object.
	//-------------------------------------------------------------------------
	CXTPGridRecordItemEditOptions();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPGridRecordItemEditOptions object, handles
	//     cleanup and deallocation.
	//-------------------------------------------------------------------------
	~CXTPGridRecordItemEditOptions();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a new constraint to the constraint list.
	// Parameters:
	//     lpszConstraint - Caption of the constraint to be added.
	//     dwData         - The 32-bit value associated with the constraint.
	// Returns:
	//     A pointer to the newly added CXTPGridRecordItemConstraint object.
	//-----------------------------------------------------------------------
	CXTPGridRecordItemConstraint* AddConstraint(LPCTSTR lpszConstraint, DWORD_PTR dwData = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Finds a constraint by its value.
	// Parameters:
	//     dwData - The 32-bit value associated with the constraint.
	// Returns:
	//     A pointer to the CXTPGridRecordItemConstraint object if found, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPGridRecordItemConstraint* FindConstraint(DWORD_PTR dwData);

	//-----------------------------------------------------------------------
	// Summary:
	//     Finds a constraint by its caption.
	// Parameters:
	//     lpszConstraint - Caption of the constraint.
	// Returns:
	//     A pointer to the CXTPGridRecordItemConstraint object if found, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPGridRecordItemConstraint* FindConstraint(LPCTSTR lpszConstraint);

	//-------------------------------------------------------------------------
	// Summary:
	//     Gets the constraints list.
	// Returns:
	//     A pointer to the CXTPGridRecordItemConstraints object.
	//-------------------------------------------------------------------------
	CXTPGridRecordItemConstraints* GetConstraints()
	{
		return m_pConstraints;
	}

	//-------------------------------------------------------------------------
	// Summary:
	//     Adds a drop-down combo button to the CXTPGridRecordItem.
	// Parameters:
	//     bInside - TRUE to keep the drop-down combo button inside
	//               the parent control rectangle.
	// Returns:
	//     A pointer to the newly added CXTPGridInplaceButton object.
	// Remarks:
	//     This method adds a drop-down combo button to the GridRecordItem.
	//     When the button is pressed, it will display all of the data items
	//     that were added as CXTPGridRecordItemConstraints.
	//-------------------------------------------------------------------------
	CXTPGridInplaceButton* AddComboButton(BOOL bInside = FALSE);

	//-------------------------------------------------------------------------
	// Summary:
	//     Adds an expand button to the CXTPGridRecordItem.
	// Parameters:
	//     bInside - TRUE to keep the expand button inside
	//               the parent control rectangle.
	// Returns:
	//     A pointer to the newly added CXTPGridInplaceButton object.
	// Remarks:
	//     A small button with three ellipses is displayed and you can use this
	//     button to display your own custom dialog.
	//-------------------------------------------------------------------------
	CXTPGridInplaceButton* AddExpandButton(BOOL bInside = FALSE);

	//-------------------------------------------------------------------------
	// Summary:
	//     Adds a spin button to the CXTPGridRecordItem.
	// Parameters:
	//     bInside - TRUE to keep the spin button inside
	//               the parent control rectangle.
	// Returns:
	//     A pointer to the newly added CXTPGridInplaceButton object.
	// Remarks:
	//     A spin control with two buttons is displayed and you can use this
	//     button to change the cell numeric value.
	//-------------------------------------------------------------------------
	CXTPGridInplaceButton* AddSpinButton(BOOL bInside = FALSE);

	//-------------------------------------------------------------------------
	// Summary:
	//     Retrieves a button by its index.
	// Parameters:
	//     nIndex - Index of the button to retrieve.
	// Returns:
	//     A pointer to a CXTPGridInplaceButton object.
	//-------------------------------------------------------------------------
	CXTPGridInplaceButton* GetButton(int nIndex);

	//-------------------------------------------------------------------------
	// Summary:
	//     Removes all buttons of the CXTPGridRecordItem.
	//-------------------------------------------------------------------------
	void RemoveButtons();

public:
	BOOL m_bAllowEdit; // TRUE to enable changing the value of the field, FALSE to enable read-mode.
					   // If FALSE, then m_bConstraintEdit is ignored.
	BOOL m_bConstraintEdit; // TRUE to prevent possibility to type in a custom value in the edit box
							// if applicable.
	CXTPGridRecordItemConstraints* m_pConstraints; // Constraint list.
	CXTPGridInplaceButtons arrInplaceButtons;	  // Array of in-place buttons.
	BOOL m_bSelectTextOnEdit;					   // Select all text on edit.
	BOOL m_bScrollTextOnEdit;					   // Scroll text on edit.
	BOOL m_bExpandOnSelect;						   // TRUE to expand expandable contents on edit.
	DWORD m_dwEditStyle;						   // Edit Style (ES_MULTILINE, ES_NUMBER...).
	int m_nMaxLength; // Maximum number of characters that can be entered into an editable item
					  // (Edit limit).

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPGridRecordItemEditOptions);

	afx_msg LPDISPATCH OleGetConstraints();
	afx_msg LPDISPATCH OleGetInplaceButton(long nIndex);
	afx_msg long OleInplaceButtonsCount();

	afx_msg LPDISPATCH OleAddComboButton(const VARIANT& bInside);
	afx_msg LPDISPATCH OleAddExpandButton(const VARIANT& bInside);
	afx_msg LPDISPATCH OleAddSpinButton(const VARIANT& bInside);

//}}AFX_CODEJOCK_PRIVATE
#	endif
};

//===========================================================================
// Summary:
//     Class for working with single list cell. Base class for extended list cells.
//     It determines the behavior of all specific record items.
//     You don't use its class directly in the control.
//     If you wish to add your own Record Item to the control, you must inherit it
//     from CXTPGridRecordItem. Most member functions in this class are virtual.
// See Also:
//     CXTPGridRecordItemDateTime, CXTPGridRecordItemNumber,
//     CXTPGridRecordItemPreview, CXTPGridRecordItemText,
//     CXTPGridRecordItemVariant
//===========================================================================
class _XTP_EXT_CLASS CXTPGridRecordItem
	: public CXTPHeapObjectT<CXTPCmdTarget, CXTPGridDataAllocator>
{
	DECLARE_SERIAL(CXTPGridRecordItem)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPGridRecordItem default constructor.
	//-----------------------------------------------------------------------
	CXTPGridRecordItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPGridRecordItem default destructor.
	//-----------------------------------------------------------------------
	virtual ~CXTPGridRecordItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     Should be overridden by descendants for drawing itself.
	// Parameters:
	//     pDrawArgs - Pointer to an XTP_GRIDRECORDITEM_DRAWARGS
	//                 structure containing drawing arguments.
	// Returns:
	//     The text width of the record item.
	// Remarks:
	//     This function simply prepares and calls the necessary drawing
	//     functions from PaintManager to draw a grid item. As such,
	//     you must provide your own implementation of PaintManager in
	//     order to change the appearance of your grid item.
	// Example:
	// <code>
	// // fill structure
	// XTP_GRIDRECORDITEM_DRAWARGS drawArgs;
	// drawArgs.pDC = pDC;
	// drawArgs.pControl = m_pControl;
	// drawArgs.pRow = this;
	// // call function
	// Draw(&drawArgs);
	// </code>
	//
	// See Also:
	//     XTP_GRIDRECORDITEM_DRAWARGS
	//-----------------------------------------------------------------------
	virtual int Draw(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Should be overridden by descendants for drawing itself.
	// Parameters:
	//     pDrawArgs - Pointer to an XTP_GRIDRECORDITEM_DRAWARGS
	//                 structure containing drawing arguments.
	//     pMetrics  - Pointer to an XTP_GRIDRECORDITEM_METRICS
	//                 structure containing item metrics.
	//-----------------------------------------------------------------------
	virtual void OnDrawCaption(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
							   XTP_GRIDRECORDITEM_METRICS* pMetrics);

	//-----------------------------------------------------------------------
	// Summary:
	//     Should be overridden by descendants for drawing itself.
	// Parameters:
	//     pDrawArgs - Pointer to an XTP_GRIDRECORDITEM_DRAWARGS
	//                 structure containing drawing arguments.
	//     rcItem    - Reference to an item rectangle.
	//-----------------------------------------------------------------------
	virtual void OnDrawControls(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs, CRect& rcItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines which grid record item control, if any, is at a specified point.
	// Parameters:
	//     ptPoint - Point to test.
	// Remarks:
	//     Call this member function to test the item contents at the
	//     specified point.
	// Returns:
	//     A pointer to the grid record item control at the specified point, if any,
	//     otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPGridRecordItemControl* HitTest(CPoint ptPoint);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a mouse click occurred on a hyperlink.
	// Parameters:
	//     ptClick - Point of the mouse click.
	// Returns:
	//     Zero-based index of the clicked link, if any, otherwise a value of -1.
	//-----------------------------------------------------------------------
	virtual int HitTestHyperlink(CPoint ptClick);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes mouse left button down clicks.
	// Parameters:
	//     pClickArgs - Pointer to an XTP_GRIDRECORDITEM_CLICKARGS
	//                  structure containing click arguments.
	// Returns:
	//     TRUE if the implementation handles the mouse click event and no
	//     further handling is required, otherwise FALSE.
	// Remarks:
	//     Usually this function is called by GridRow's OnLButtonDown function.
	// See Also:
	//     XTP_GRIDRECORDITEM_CLICKARGS
	//-----------------------------------------------------------------------
	virtual BOOL OnLButtonDown(XTP_GRIDRECORDITEM_CLICKARGS* pClickArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes mouse left button up clicks.
	// Parameters:
	//     pClickArgs - Pointer to an XTP_GRIDRECORDITEM_CLICKARGS
	//                  structure containing click arguments.
	// Returns:
	//     TRUE if the implementation handles the mouse click event and no
	//     further handling is required, otherwise FALSE.
	// Remarks:
	//     Usually this function is called by GridRow's OnLButtonUp function.
	// See Also:
	//     XTP_GRIDRECORDITEM_CLICKARGS
	//-----------------------------------------------------------------------
	virtual BOOL OnLButtonUp(XTP_GRIDRECORDITEM_CLICKARGS* pClickArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes single mouse clicks.
	// Parameters:
	//     pClickArgs - Pointer to an XTP_GRIDRECORDITEM_CLICKARGS
	//                  structure containing click arguments.
	// Remarks:
	//     Usually this function is called by GridRow's OnClick function.
	// See Also:
	//     XTP_GRIDRECORDITEM_CLICKARGS
	//-----------------------------------------------------------------------
	virtual void OnClick(XTP_GRIDRECORDITEM_CLICKARGS* pClickArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes double mouse clicks.
	// Parameters:
	//     pClickArgs - Pointer to an XTP_GRIDRECORDITEM_CLICKARGS
	//                  structure containing click arguments.
	// Remarks:
	//     Usually this function is called by GridRow's OnDblClick function.
	// See Also:
	//     XTP_GRIDRECORDITEM_CLICKARGS
	//-----------------------------------------------------------------------
	virtual void OnDblClick(XTP_GRIDRECORDITEM_CLICKARGS* pClickArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes the move mouse event.
	// Parameters:
	//     point    - Specifies the x- and y- coordinates of the cursor.
	//     nFlags   - Additional flags.
	//     pControl - Pointer to the Grid control.
	// Remarks:
	//     Usually this function is called by GridRow's OnMouseMove function.
	//-----------------------------------------------------------------------
	virtual void OnMouseMove(UINT nFlags, CPoint point, CXTPGridControl* pControl = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the text caption of the item.
	// Remarks:
	//     Can be overridden by descendants.
	// Returns:
	//     The text caption of the item, or an empty string for a base record item class.
	//-----------------------------------------------------------------------
	virtual CString GetCaption();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the caption prefix of a specified column.
	// Parameters:
	//     pColumn - Pointer to a CXTPGridColumn object.
	// Returns:
	//     The caption prefix of the specified column.
	//-----------------------------------------------------------------------
	CString GetColumnCaptionPrefix(CXTPGridColumn* pColumn) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the text caption of the record item.
	// Parameters:
	//     pColumn - Pointer to the corresponding column of the record item.
	// Remarks:
	//     Can be overridden by descendants.
	// Returns:
	//     The text caption of the record item,
	//     or an empty string for a base record item class.
	//-----------------------------------------------------------------------
	virtual CString GetCaption(CXTPGridColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the caption text bounding rectangle.
	// Parameters:
	//     pDrawArgs - Pointer to an XTP_GRIDRECORDITEM_DRAWARGS
	//                 structure containing drawing arguments.
	//     rcItem    - Reference to a CRect object that receives
	//                 the caption text bounding rectangle.
	//-----------------------------------------------------------------------
	virtual void GetCaptionRect(XTP_GRIDRECORDITEM_ARGS* pDrawArgs, CRect& rcItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the group caption text.
	// Parameters:
	//     pColumn - Pointer to a CXTPGridColumn object.
	// Returns:
	//     The group caption text.
	//-----------------------------------------------------------------------
	virtual CString GetGroupCaption(CXTPGridColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the group caption native value.
	// Parameters:
	//     pColumn - Pointer to a CXTPGridColumn object.
	// Returns:
	//     The group caption native value.
	//-----------------------------------------------------------------------
	virtual COleVariant GetGroupCaptionValue(CXTPGridColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the group caption ID.
	// Parameters:
	//     pColumn - Pointer to a CXTPGridColumn object.
	// Returns:
	//     The group caption ID.
	//-----------------------------------------------------------------------
	virtual int GetGroupCaptionID(CXTPGridColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Compares the group caption of the current record item with
	//     the group caption of a provided record item/group row.
	// Parameters:
	//     pColumn   - Pointer to the corresponding column of the current record item.
	//     pItem     - Pointer to the record item with the group caption to
	//                 compare with the group caption of the current record item.
	//     pGroupRow - Pointer to the group row with the group caption to
	//                 compare with the group caption of the current record item.
	// Returns:
	//     Zero if the group caption of the current record item is
	//     identical to the group caption of pItem/pGroupRow;
	//     Less than zero if the group caption of the current record item is
	//     less than the group caption of pItem/pGroupRow;
	//     Greater than zero if the group caption of the current record item is
	//     greater than the group caption of pItem/pGroupRow.
	//-----------------------------------------------------------------------
	virtual int CompareGroupCaption(CXTPGridColumn* pColumn, CXTPGridRecordItem* pItem);
	virtual int CompareGroupCaption(
		CXTPGridColumn* pColumn,
		CXTPGridGroupRow*
			pGroupRow); // <combine
						// CXTPGridRecordItem::CompareGroupCaption@CXTPGridColumn*@CXTPGridRecordItem*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Compares the current record item with a provided record item.
	//     Provides default behavior for descendants.
	// Parameters:
	//     pColumn - Pointer to the corresponding column of the current record item.
	//     pItem   - Pointer to the record item to compare with the current record item.
	// Returns:
	//     Zero if the current record item is equal to pItem;
	//     Less than zero if the current record item is less than pItem;
	//     Greater than zero if the current record item is greater than pItem.
	//-----------------------------------------------------------------------
	virtual int Compare(CXTPGridColumn* pColumn, CXTPGridRecordItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the record item text font.
	// Parameters:
	//     pFont - New font to be set.
	//-----------------------------------------------------------------------
	virtual void SetFont(CFont* pFont);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the record item text font.
	// Returns:
	//     A pointer to the record item text font.
	//-----------------------------------------------------------------------
	virtual CFont* GetFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tooltip text of the record item.
	// Returns:
	//     The tooltip text of the record item
	//-----------------------------------------------------------------------
	virtual CString GetTooltip() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the tooltip text for the record item.
	// Parameters:
	//     lpszTooltip - Tooltip text to be set.
	//-----------------------------------------------------------------------
	virtual void SetTooltip(LPCTSTR lpszTooltip);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the record item text color.
	// Parameters:
	//     clrText - New color to be set.
	//-----------------------------------------------------------------------
	virtual void SetTextColor(COLORREF clrText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the record item text color.
	// Returns:
	//     The record item text color.
	//-----------------------------------------------------------------------
	virtual COLORREF GetTextColor();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the bold state of the record item font.
	// Parameters:
	//     bBold - TRUE to set the record item font to bold,
	//             FALSE to set the record item font to normal.
	//-----------------------------------------------------------------------
	virtual void SetBold(BOOL bBold = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the bold state of the record item font.
	// Returns:
	//     TRUE if the record item font is bold, FALSE if the record item font is normal.
	//-----------------------------------------------------------------------
	virtual BOOL IsBold() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the record item text background color.
	// Parameters:
	//     clrBackground - Record item text background color to be set.
	//-----------------------------------------------------------------------
	virtual void SetBackgroundColor(COLORREF clrBackground);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the record item text background color.
	// Returns:
	//     The record item text background color.
	//-----------------------------------------------------------------------
	virtual COLORREF GetBackgroundColor();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the record item formula.
	// Returns:
	//     The record item formula.
	//-----------------------------------------------------------------------
	virtual CString GetFormula() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the record item formula.
	// Parameters:
	//     sFormula - Record item formula to be set.
	//-----------------------------------------------------------------------
	virtual void SetFormula(LPCTSTR sFormula);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the record item caption format string.
	// Returns:
	//     The record item caption format string.
	//-----------------------------------------------------------------------
	virtual CString GetFormatString() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the record item caption format string.
	// Parameters:
	//     strFormat - Record item caption format string to be set.
	// Remarks:
	//     If the record item caption format string is set, then the caption text will
	//     be formatted according to this format string before it is drawn. This is
	//     convenient in many cases (e.g. when drawing dates). The format string is C like
	//     style; see the sprintf() C function or the CString.Format() member function.
	//-----------------------------------------------------------------------
	virtual void SetFormatString(LPCTSTR strFormat);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the editable state of the record item.
	// Returns:
	//     The editable state of the record item.
	//-----------------------------------------------------------------------
	virtual BOOL IsEditable() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the editable state for the record item.
	// Parameters:
	//     bEditable - TRUE to set the record item to editable, FALSE otherwise.
	// Returns:
	//     The previous editable state of the record item.
	//-----------------------------------------------------------------------
	virtual BOOL SetEditable(BOOL bEditable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a new hyperlink to the record item.
	// Parameters:
	//     pHyperlink - Pointer to the hyperlink to add.
	// Remarks:
	//     Each record item can have a number of hyperlinks.
	//     To add a new hyperlink to the record item, you must
	//     first create the new instance of hyperlink class.
	// Returns:
	//     The zero-based ID of the newly added hyperlink.
	// Example:
	//     <code>AddHyperlink(new CXTPGridHyperlink(27, 7);</code>
	// See Also:
	//     CXTPGridHyperlink.
	//-----------------------------------------------------------------------
	virtual int AddHyperlink(CXTPGridHyperlink* pHyperlink);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of hyperlinks in the record item.
	// Remarks:
	//     Each record item can have a number of hyperlinks. To process all of
	//     the hyperlinks of a record item, you must get the number of hyperlinks
	//     in the record item by using the GetHyperlinksCount() member function.
	// Returns:
	//     The number of hyperlinks in the record item.
	//-----------------------------------------------------------------------
	virtual int GetHyperlinksCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the hyperlink at a specified index.
	// Parameters:
	//     nHyperlink - Zero-based index of the hyperlink to retrieve.
	// Remarks:
	//     To process a hyperlink, you get a pointer to it by calling GetHyperlinkAt();
	// Returns:
	//     A pointer to the hyperlink at the specified index.
	// Example:
	// <code>
	// int nHyperlinks = GetHyperlinksCount();
	// CXTPGridHyperlink* pHyperlink;
	// for(int nHyperlink = 0; nHyperlink < nHyperlinks; nHyperlink++)
	// {
	//     pHyperlink = GetHyperlinkAt(nHyperlink);
	//     // YOUR PROCESSING HERE
	//     // ...
	//     //
	// }
	// </code>
	// See Also:
	//     CXTPGridHyperlink.
	//-----------------------------------------------------------------------
	virtual CXTPGridHyperlink* GetHyperlinkAt(int nHyperlink) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes the hyperlink at a specified index.
	// Parameters:
	//     nHyperlink - Zero-based index of the hyperlink to remove.
	//-----------------------------------------------------------------------
	virtual void RemoveHyperlinkAt(int nHyperlink);

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates preview item metrics based on specified draw arguments.
	//     Can be overridden by descendants.
	// Parameters:
	//     pDrawArgs    - Pointer to an XTP_GRIDRECORDITEM_DRAWARGS
	//                    structure used to calculate item metrics.
	//     pItemMetrics - Pointer to an XTP_GRIDRECORDITEM_METRICS
	//                    structure to fill with values.
	// See Also:
	//     XTP_GRIDRECORDITEM_DRAWARGS, XTP_GRIDRECORDITEM_METRICS
	//-----------------------------------------------------------------------
	virtual void GetItemMetrics(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
								XTP_GRIDRECORDITEM_METRICS* pItemMetrics);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the record item is a preview item.
	// Returns:
	//     TRUE if the record item is a preview item, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsPreviewItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the index of the record item.
	// Returns:
	//     The index of the record item.
	//-----------------------------------------------------------------------
	int GetIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the ID of the associated resource bitmap.
	// Returns:
	//     The ID of the associated resource bitmap.
	//-----------------------------------------------------------------------
	virtual int GetIconIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the ID for the associated resource bitmap.
	// Parameters:
	//     nIconIndex - ID of the resource bitmap to set.
	// Returns:
	//     The ID of the previous associated resource bitmap.
	//-----------------------------------------------------------------------
	virtual int SetIconIndex(int nIconIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the alignment value of the record item.
	// Returns:
	//     The alignment value of the record item; one of the values
	//     defined by the XTPGridColumnIconAlignment enumeration.
	//-----------------------------------------------------------------------
	int GetAlignment() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the alignment value for the record item.
	// Parameters:
	//     nAlignment - Alignment value to be set; must be one of the values
	//                  defined by the XTPGridColumnIconAlignment enumeration.
	// Returns:
	//     The previous alignment value of the record item; one of the values
	//     defined by the XTPGridColumnIconAlignment enumeration.
	//-----------------------------------------------------------------------
	int SetAlignment(int nAlignment);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the sort priority of the record item.
	// Returns:
	//     The sort priority of the record item.
	// See Also:
	//     GetSortPriority, SetSortPriority, SetGroupPriority, GetGroupPriority
	//-----------------------------------------------------------------------
	virtual int GetSortPriority() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the sort priority for the record item.
	// Parameters:
	//     nSortPriority - Sort priority to be set.
	// See Also:
	//     GetSortPriority, SetSortPriority, SetGroupPriority, GetGroupPriority
	//-----------------------------------------------------------------------
	virtual void SetSortPriority(int nSortPriority);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the group priority of the record item.
	// Returns:
	//     The group priority of the record item.
	// See Also:
	//     GetSortPriority, SetSortPriority, SetGroupPriority, GetGroupPriority
	//-----------------------------------------------------------------------
	virtual int GetGroupPriority() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the group priority for the record item.
	// Parameters:
	//     nGroupPriority - Group priority to be set.
	// See Also:
	//     GetSortPriority, SetSortPriority, SetGroupPriority, GetGroupPriority
	//-----------------------------------------------------------------------
	virtual void SetGroupPriority(int nGroupPriority);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the group caption for the record item.
	// Parameters:
	//     strCaption - Group caption to be set.
	//-----------------------------------------------------------------------
	virtual void SetGroupCaption(LPCTSTR strCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the caption for the record item.
	// Parameters:
	//     strCaption - Caption to be set.
	//-----------------------------------------------------------------------
	virtual void SetCaption(LPCTSTR strCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//      Causes formula computed value to be updated.
	//-----------------------------------------------------------------------
	void UpdateValue();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets if the record item is checked.
	// Parameters:
	//     bChecked - TRUE if the record item is checked.
	// See Also:
	//     IsChecked, HasCheckbox
	//-----------------------------------------------------------------------
	virtual void SetChecked(BOOL bChecked);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the record item is checked.
	// Returns:
	//     TRUE if the record item is checked, otherwise FALSE.
	// See Also:
	//     SetChecked, HasCheckbox
	//-----------------------------------------------------------------------
	virtual BOOL IsChecked() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the checked state for the record item.
	// Parameter:
	//     iState - Integer value denoting the state of the check box; must be one
	//              of the values defined by the XTPGridCheckState enumeration.
	// See Also:
	//     SetChecked, HasCheckbox
	//-----------------------------------------------------------------------
	void SetCheckedState(int iState);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the checked state of the record item.
	// Returns:
	//     An integer value denoting the state of the check box; one
	//     of the values defined by the XTPGridCheckState enumeration.
	// See Also:
	//     SetChecked, HasCheckbox
	//-----------------------------------------------------------------------
	int GetCheckedState() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds check box area for item.
	// Parameters:
	//     bHasCheckbox - TRUE if item has check box area.
	//     bTristate    - TRUE is tri-state checkbox, FALSE - if usual
	// See Also:
	//     SetChecked, IsChecked
	//-----------------------------------------------------------------------
	virtual void HasCheckbox(BOOL bHasCheckbox, BOOL bTristate = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if item has check box area
	// Returns:
	//     TRUE if item has check box; otherwise FALSE
	// See Also:
	//     SetChecked, IsChecked
	//-----------------------------------------------------------------------
	virtual BOOL GetHasCheckbox() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns whether items with a check box will go into edit mode on click.
	// Returns:
	//     TRUE if items with a check box will go into edit mode on click.
	//-----------------------------------------------------------------------
	BOOL GetCanEditCheckboxItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies whether items with a check box will go into edit mode on click.
	// Parameters:
	//     bSet - TRUE for items with a check box will go into edit mode on click.
	//-----------------------------------------------------------------------
	void SetCanEditCheckboxItem(BOOL bSet);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the parent record object.
	// Parameters:
	//     pGridRecord - Pointer to a CXTPGridRecord object.
	//-----------------------------------------------------------------------
	void SetRecord(CXTPGridRecord* pGridRecord);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the parent record object.
	// Returns:
	//     A pointer to the parent record object.
	//-----------------------------------------------------------------------
	CXTPGridRecord* GetRecord() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the application-supplied, 32-bit value associated
	//     with the record item.
	// Returns:
	//     The 32-bit value associated with the record item.
	//-----------------------------------------------------------------------
	virtual DWORD_PTR GetItemData() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the 32-bit value associated with the record item.
	// Parameters:
	//     dwData - New value to be set.
	//-----------------------------------------------------------------------
	virtual void SetItemData(DWORD_PTR dwData);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the record item can receive focus.
	// Returns:
	//     TRUE if the record item can receive focus, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsFocusable() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the record item can receive focus.
	// Parameters:
	//     bFocusable - TRUE to allow the record item to receive focus,
	//                  FALSE to disallow the record item from receiving focus.
	// Remarks:
	//     This method specifies if the grid item can receive focus and overrides
	//     the CXTPGridControl::FocusSubItems setting which only allows for
	//     individual items to receive focus. If FALSE, then the record item cannot
	//     be edited and combo/expand buttons will not be displayed.
	//-----------------------------------------------------------------------
	virtual void SetFocusable(BOOL bFocusable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to store/load a grid record item
	//     using the specified data object.
	// Parameters:
	//     pPX - Source/destination CXTPPropExchange data object reference.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the Markup element that renders the item.
	// Returns:
	//     A pointer to the Markup element that renders the item.
	//-----------------------------------------------------------------------
	CXTPMarkupUIElement* GetMarkupUIElement() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Resets the Markup element.
	// See Also:
	//      GetMarkupUIElement, CXTPGridControl::EnableMarkup
	//-----------------------------------------------------------------------
	virtual void ResetMarkupUIElement();

	//-----------------------------------------------------------------------
	// Summary:
	//     The framework calls this member function to determine whether a
	//     point is in the bounding rectangle of the specified tool.
	// Parameters:
	//     point - Specifies the x- and y-coordinate of the cursor. These
	//             coordinates are always relative to the upper-left corner of the window
	//     pTI   - Pointer to a TOOLINFO structure.
	// Returns:
	//     If the tooltip control was found, the window control ID. If
	//     the tooltip control was not found, -1.
	//-----------------------------------------------------------------------
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI);
	// GRID
	void Merge(CXTPGridRecordItem* pMergeItem);

	BOOL IsMerged() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the current item is in a merge group and, if so,
	//     if it is not the left-top (main) item of the group.
	// Remarks:
	//     If the current item is in a merge group and is not the
	//     left-top (main) item of the group, then the current item
	//     receives properties from the main item of the merge group.
	// Returns:
	//     TRUE if the current item is in a merge group and is not the
	//     left-top (main) item of the group, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsChildOfMerge() const;

	CXTPGridRecordItem* GetMergeItem() const;

	// Pointer to the top-left item in the merged group; may point to itself if it is the
	// top-left item in the merge group. NULL if the item does not belong to merged cells.
	CXTPGridRecordItem* m_pMergeItem;
	// GRID
protected:
	virtual void ParseBBCode(CString& strText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the check box bitmap.
	// Parameters:
	//     pDrawArgs - Pointer to an XTP_GRIDRECORDITEM_DRAWARGS
	//                 structure containing drawing arguments.
	//     rcItem    - Item area rectangle coordinates.
	//-----------------------------------------------------------------------
	virtual void DrawCheckBox(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs, CRect& rcItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the hyperlinks collection.
	// Returns:
	//     A pointer to a CXTPGridHyperlinks object.
	//-----------------------------------------------------------------------
	CXTPGridHyperlinks* GetHyperlinks();

protected:
	virtual BOOL IsAllowEdit(XTP_GRIDRECORDITEM_ARGS* pItemArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an editable item enters edit mode.
	// Parameters:
	//     pItemArgs - Pointer to an XTP_GRIDRECORDITEM_ARGS structure.
	// Remarks:
	//     An item can enter edit mode when the user clicks on it and starts typing,
	//     or when a check box item is checked/unchecked.
	// Returns:
	//     TRUE if the edit request was successful, FALSE if the edit request was canceled.
	//-----------------------------------------------------------------------
	virtual BOOL OnRequestEdit(XTP_GRIDRECORDITEM_ARGS* pItemArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user begins to edit an item.
	// Parameters:
	//     pItemArgs - Pointer to an XTP_GRIDRECORDITEM_ARGS
	//                 structure containing item arguments.
	//-----------------------------------------------------------------------
	virtual void OnBeginEdit(XTP_GRIDRECORDITEM_ARGS* pItemArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user presses a key.
	// Parameters:
	//     pItemArgs - Pointer to an XTP_GRIDRECORDITEM_ARGS
	//                 structure containing item arguments.
	//     nChar     - Pressed character code.
	// Returns:
	//     TRUE if the item processed the key.
	//-----------------------------------------------------------------------
	virtual BOOL OnChar(XTP_GRIDRECORDITEM_ARGS* pItemArgs, UINT nChar);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user cancels editing an item.
	// Parameters:
	//     pControl - Pointer to the parent Grid control.
	//     bApply   - TRUE to save the value of the in-place edit.
	//-----------------------------------------------------------------------
	virtual void OnCancelEdit(CXTPGridControl* pControl, BOOL bApply);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to save the value from an in-place edit.
	// Parameters:
	//     pItemArgs - Pointer to an XTP_GRIDRECORDITEM_ARGS
	//                 structure containing item arguments.
	//-----------------------------------------------------------------------
	virtual void OnValidateEdit(XTP_GRIDRECORDITEM_ARGS* pItemArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the value of the in-place edit control is changed.
	// Parameters:
	//     pItemArgs - Pointer to an XTP_GRIDRECORDITEM_ARGS
	//                 structure containing item arguments.
	//     szText    - New in-place edit text.
	//-----------------------------------------------------------------------
	// virtual void OnEditChanged(XTP_GRIDRECORDITEM_ARGS* pItemArgs, LPCTSTR szText)
	//{
	//  UNREFERENCED_PARAMETER(pItemArgs);
	//  UNREFERENCED_PARAMETER(szText);
	//}
	virtual void OnEditChanged(XTP_GRIDRECORDITEM_ARGS* pItemArgs, LPCTSTR szText);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before the value of the in-place editor is
	//     changed by the user.
	// Parameters:
	//     pItemArgs   - Pointer to an XTP_GRIDRECORDITEM_ARGS
	//                   structure containing item arguments.
	//     rstrNewText - New string to be set in the editor.
	// Returns:
	//     FALSE to cancel the edit operation.
	//-----------------------------------------------------------------------
	// virtual BOOL OnEditChanging(XTP_GRIDRECORDITEM_ARGS* pItemArgs, CString& rstrNewText)
	//{
	//  UNREFERENCED_PARAMETER(pItemArgs);
	//  UNREFERENCED_PARAMETER(rstrNewText);
	//  return TRUE;
	//}
	virtual BOOL OnEditChanging(XTP_GRIDRECORDITEM_ARGS* pItemArgs, CString& rstrNewText);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when in-place editing has been canceled and the
	//     value has not changed.
	// Parameters:
	//     pItemArgs   - Pointer to an XTP_GRIDRECORDITEM_ARGS
	//                   structure containing item arguments.
	//-----------------------------------------------------------------------
	virtual void OnEditCanceled(XTP_GRIDRECORDITEM_ARGS* pItemArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the value of the selected contraint of the
	//     in-place list is changed by the user.
	// Parameters:
	//     pItemArgs   - Pointer to an XTP_GRIDRECORDITEM_ARGS
	//                   structure containing item arguments.
	//     pConstraint - Pointer to the selected constraint.
	//-----------------------------------------------------------------------
	virtual void OnConstraintChanged(XTP_GRIDRECORDITEM_ARGS* pItemArgs,
									 CXTPGridRecordItemConstraint* pConstraint);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the value associated with a selected constraint.
	// Parameters:
	//     pItemArgs - Pointer to an XTP_GRIDRECORDITEM_ARGS
	//                 structure containing item arguments.
	// Returns:
	//     The value associated with the selected constraint.
	//-----------------------------------------------------------------------
	virtual DWORD GetSelectedConstraintData(XTP_GRIDRECORDITEM_ARGS* pItemArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user presses the in-place button of the item.
	// Parameters:
	//     pButton - Pointer to the in-place button of the item.
	//-----------------------------------------------------------------------
	virtual void OnInplaceButtonDown(CXTPGridInplaceButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the edit options of the item.
	// Parameters:
	//     pColumn - Pointer to the corresponding column of the item.
	// Returns:
	//     A pointer to the CXTPGridRecordItemEditOptions object.
	//-----------------------------------------------------------------------
	CXTPGridRecordItemEditOptions* GetEditOptions(CXTPGridColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the item controls list.
	// Returns:
	//     A pointer to the CXTPGridRecordItemControls object.
	//-----------------------------------------------------------------------
	CXTPGridRecordItemControls* GetItemControls();

	double StringToDouble(CString strText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a border for this item.
	//-----------------------------------------------------------------------
	BOOL CreateBorder();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys the border of this item.
	//-----------------------------------------------------------------------
	BOOL DestroyBorder();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the border of this item.
	// Returns:
	//     A pointer to the border of this item.
	//-----------------------------------------------------------------------
	CXTPGridBorder* GetBorder() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the parent grid object.
	// Parameters:
	//     pControl - Pointer to a CXTPGridControl object.
	//-----------------------------------------------------------------------
	void SetGridControl(CXTPGridControl* pControl);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	//-----------------------------------------------------------------------
	// Summary:
	//     If possible, returns the column associated with this Record Item.
	//     Otherwise, returns NULL
	//-----------------------------------------------------------------------
	virtual CXTPGridColumn* GetColumn();
	virtual void DoMouseButtonClick();
	//}}AFX_CODEJOCK_PRIVATE

protected:
	CXTPGridControl* m_pControl; // Parent Grid control.
	CXTPGridRecord* m_pRecord;   // Pointer to the associated record.
	CXTPGridBorder* m_pBorder;   // Item border.

	CFont* m_pFontCaption; // Storage for item caption font.

	COLORREF m_clrText;		  // Storage for item text color.
	COLORREF m_clrBackground; // Storage for item background color.
	BOOL m_bBoldText;		  // TRUE if bold text.

	CString m_strFormatString; // Stores string for extra formatting of item caption.
	CString m_strFormula;	  // Stores Formula with simple algorithm to calculate.

	BOOL m_bEditable;				   // Stores editable flag.
	CXTPGridHyperlinks* m_pHyperlinks; // Array of the Item's hyperlinks.

	int m_nIconIndex; // ID of the bitmap from the application resources.

	int m_nSortPriority;  // Sort priority.
	int m_nGroupPriority; // Group priority.

	CString m_strGroupCaption; // Caption of the group.
	CString m_strCaption;	  // Caption of the item.
	BOOL m_bFocusable;		   // TRUE if item accept focus.

	int m_checkState;			 // TRUE if item checked.
	BOOL m_bHasCheckbox;		 // TRUE if item has check box.
	BOOL m_bCanEditCheckboxItem; // TRUE if an item with a check box will get an edit control (only
								 // useful if column is wide)
	BOOL m_bTristateCheckbox;	// TRUE if the check box is tri-state.
	DWORD_PTR m_dwData;			 // The 32-bit value associated with the item.
	CString m_strTooltip;		 // Tooltip of the item.

	CXTPGridRecordItemEditOptions* m_pEditOptions; // Edit options of the item.

	XTPGridColumnIconAlignment m_Alignment; // Alignment style for item.

	CXTPGridRecordItemControls* m_pItemControls;	  // Record item control list.
	CXTPGridRecordItemControl* m_pFocusedItemControl; // Focused record item control.
	BOOL m_bItemControlUnderMouse; // TRUE if an item control is under the mouse pointer.
	CXTPGridRecordItemControlHookWnd* m_pItemControlHookWnd; // Item control hook window.
	CRect m_rcGlyph;										 // Coordinates of drawn glyph.

	CXTPMarkupUIElement* m_pMarkupUIElement; // Markup element.

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPGridRecordItem);

	afx_msg LPDISPATCH OleGetRecord();
	afx_msg LPDISPATCH OleGetEditOptions();
	afx_msg void OleCreateEditOptions();
	afx_msg LPDISPATCH OleGetHyperlinks();

	afx_msg BSTR OleGetFormat();
	afx_msg void OleSetFormat(LPCTSTR pcszFormat);

	afx_msg BSTR OleGetFormula();
	afx_msg void OleSetFormula(LPCTSTR pcszFormula);

	afx_msg BSTR OleGetCaption();
	afx_msg void OleSetCaption(LPCTSTR pcszCaption);

	afx_msg BSTR OleGetGroupCaption();
	afx_msg void OleSetGroupCaption(LPCTSTR pcszGroupCaption);

	afx_msg BSTR OleGetTooltip();
	afx_msg void OleSetTooltip(LPCTSTR pcszTooltip);

	afx_msg LPDISPATCH OleGetItemControls();
	afx_msg LPDISPATCH OleGetMarkupUIElement();

	COleVariant m_oleTag;

//}}AFX_CODEJOCK_PRIVATE
#	endif

	friend class CXTPGridRecord;
	friend class CXTPGridControl;
	friend class CXTPGridNavigator;
	friend class CXTPGridIconNavigator;
	friend class CXTPGridRecordItemControls;
};

AFX_INLINE CString CXTPGridRecordItem::GetFormula() const
{
	return m_strFormula;
}

AFX_INLINE void CXTPGridRecordItem::SetFormula(LPCTSTR sFormula)
{
	m_strFormula = sFormula;
}

AFX_INLINE CString CXTPGridRecordItem::GetFormatString() const
{
	return m_strFormatString;
}

AFX_INLINE void CXTPGridRecordItem::SetFormatString(LPCTSTR strFormat)
{
	m_strFormatString = strFormat;
}

AFX_INLINE BOOL CXTPGridRecordItem::SetEditable(BOOL bEditable)
{
	BOOL bOldEditable = m_bEditable;
	m_bEditable		  = bEditable;
	return bOldEditable;
}

AFX_INLINE BOOL CXTPGridRecordItem::IsPreviewItem() const
{
	return FALSE;
}

AFX_INLINE int CXTPGridRecordItem::GetIconIndex() const
{
	return m_nIconIndex;
}

AFX_INLINE int CXTPGridRecordItem::SetIconIndex(int nIconIndex)
{
	int nOldID   = GetIconIndex();
	m_nIconIndex = nIconIndex;
	return nOldID;
}

AFX_INLINE int CXTPGridRecordItem::GetAlignment() const
{
	return m_Alignment;
}

AFX_INLINE int CXTPGridRecordItem::SetAlignment(int nAlignment)
{
	int nOld	= m_Alignment;
	m_Alignment = (XTPGridColumnIconAlignment)nAlignment;
	return nOld;
}

AFX_INLINE int CXTPGridRecordItem::GetSortPriority() const
{
	return m_nSortPriority;
}

AFX_INLINE void CXTPGridRecordItem::SetSortPriority(int nSortPriority)
{
	m_nSortPriority = nSortPriority;
}

AFX_INLINE int CXTPGridRecordItem::GetGroupPriority() const
{
	return m_nGroupPriority;
}

AFX_INLINE void CXTPGridRecordItem::SetGroupPriority(int nGroupPriority)
{
	m_nGroupPriority = nGroupPriority;
}

AFX_INLINE void CXTPGridRecordItem::SetGroupCaption(LPCTSTR strCaption)
{
	m_strGroupCaption = strCaption;
}

AFX_INLINE void CXTPGridRecordItem::SetChecked(BOOL bChecked)
{
	if (bChecked)
	{
		m_checkState = xtpGridCheckStateChecked;
	}
	else
	{
		m_checkState = xtpGridCheckStateUnchecked;
	}
}

AFX_INLINE BOOL CXTPGridRecordItem::IsChecked() const
{
	return xtpGridCheckStateChecked == m_checkState;
}

AFX_INLINE void CXTPGridRecordItem::SetCheckedState(int iState)
{
	if (m_bTristateCheckbox)
	{
		m_checkState = XTPGridCheckState(iState);
	}
	else
	{
		m_checkState = iState ? xtpGridCheckStateChecked : xtpGridCheckStateUnchecked;
	}
}

AFX_INLINE int CXTPGridRecordItem::GetCheckedState() const
{
	if (m_bTristateCheckbox)
	{
		return m_checkState;
	}
	else
	{
		return m_checkState ? xtpGridCheckStateChecked : xtpGridCheckStateUnchecked;
	}
}

AFX_INLINE void CXTPGridRecordItem::HasCheckbox(BOOL bHasCheckbox, BOOL bTristate)
{
	m_bHasCheckbox		= bHasCheckbox;
	m_bTristateCheckbox = bTristate;
}

AFX_INLINE BOOL CXTPGridRecordItem::GetHasCheckbox() const
{
	return m_bHasCheckbox;
}

AFX_INLINE void CXTPGridRecordItem::SetRecord(CXTPGridRecord* pGridRecord)
{
	m_pRecord = pGridRecord;
	ResetMarkupUIElement();
}

AFX_INLINE CXTPGridRecord* CXTPGridRecordItem::GetRecord() const
{
	return m_pRecord;
}

AFX_INLINE void CXTPGridRecordItem::SetFocusable(BOOL bFocusable)
{
	m_bFocusable = bFocusable;
}

AFX_INLINE DWORD_PTR CXTPGridRecordItem::GetItemData() const
{
	return m_dwData;
}

AFX_INLINE void CXTPGridRecordItem::SetItemData(DWORD_PTR dwData)
{
	m_dwData = dwData;
}

AFX_INLINE DWORD
	CXTPGridRecordItem::GetSelectedConstraintData(XTP_GRIDRECORDITEM_ARGS* /*pItemArgs*/)
{
	return DWORD(-1);
}

AFX_INLINE CString CXTPGridRecordItem::GetTooltip() const
{
	if (IsChildOfMerge())
	{
		return GetMergeItem()->m_strTooltip;
	}
	return m_strTooltip;
}

AFX_INLINE void CXTPGridRecordItem::SetTooltip(LPCTSTR lpszTooltip)
{
	m_strTooltip = lpszTooltip;
}

AFX_INLINE BOOL CXTPGridRecordItem::IsBold() const
{
	return m_bBoldText;
}

AFX_INLINE CXTPMarkupUIElement* CXTPGridRecordItem::GetMarkupUIElement() const
{
	return m_pMarkupUIElement;
}

AFX_INLINE void CXTPGridRecordItem::OnEditCanceled(XTP_GRIDRECORDITEM_ARGS* pItemArgs)
{
	UNREFERENCED_PARAMETER(pItemArgs);
}

AFX_INLINE void CXTPGridRecordItem::SetGridControl(CXTPGridControl* pControl)
{
	m_pControl = pControl;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPGRIDRECORDITEM_H__)
