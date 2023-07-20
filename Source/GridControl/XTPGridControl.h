// XTPGridControl.h: interface for the CXTPGridControl class.
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
#if !defined(__XTPGRIDCONTROL_H__)
#	define __XTPGRIDCONTROL_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPCompatibleDC;
class CXTPPropExchange;
class CXTPToolTipContext;
class CXTPImageManager;
class CXTPMarkupContext;

struct XTP_GRIDRECORDITEM_DRAWARGS;
struct XTP_GRIDRECORDITEM_METRICS;
struct XTP_GRIDRECORDITEM_ARGS;
struct XTP_NM_GRIDTOOLTIPINFO;

class CXTPGridRecords;
class CXTPGridRecordItem;
class CXTPGridColumns;
class CXTPGridGroups;
class CXTPGridColumn;
class CXTPGridRecord;
class CXTPGridRows;
class CXTPGridRow;
class CXTPGridPaintManager;
class CXTPGridColumnOrder;
class CXTPGridInplaceEdit;
class CXTPGridInplaceButtons;
class CXTPGridInplaceList;
class CXTPGridNavigator;
class CXTPGridIconNavigator;
class CXTPGridRecordItemConstraint;
class CXTPGridDataManager;
class CXTPGridHyperlink;
class CXTPGridRecordItemRange;
class CXTPGridSelectedRows;
class CXTPGridBehavior;
class CXTPGridBehaviorRowMouseButton;
class CXTPGridTip;
class CXTPGridSection;
class CXTPGridSections;
class CXTPGridBorder;
class CXTPGridHitTestInfo;

//===========================================================================
// Summary:
//     This structure is sent to the Main window in a WM_NOTIFY message from
//     Item and provides all parameters required by the Main window to process
//     control specific notifications.
// Example:
// <code>
// BEGIN_MESSAGE_MAP(CGridSampleView, CXTPGridView)
//     ON_NOTIFY(XTP_NM_GRID_GETITEMMETRICS, XTP_ID_GRID_CONTROL, OnGridGetItemMetrics)
// END_MESSAGE_MAP()
//
// void CGridSampleView::OnGridGetItemMetrics(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_GRIDITEMMETRICS* pItemNotify = (XTP_NM_GRIDITEMMETRICS*)pNotifyStruct;
//
//     ASSERT(pItemNotify->pDrawArgs);
//     ASSERT(pItemNotify->pDrawArgs->pControl);
//     ASSERT(pItemNotify->pDrawArgs->pRow);
//
//     //     pItemNotify->pDrawArgs->pColumn   - may be NULL (for a group row)
//     //     pItemNotify->pDrawArgs->pItem     - may be NULL (for a group row)
//
//     ASSERT(pItemNotify->pItemMetrics);
//
//     // TODO: customize members of pItemNotify->pItemMetrics.
// }
// </code>
// See Also:
//     CXTPGridControl, CXTPGridControl::GetItemMetrics(),
//     XTP_NM_GRID_GETITEMMETRICS
//===========================================================================
struct XTP_NM_GRIDITEMMETRICS
{
	NMHDR hdr; // Standard structure; contains information about a notification message.
	XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs;   // Pointer to an XTP_GRIDRECORDITEM_DRAWARGS
											  // structure.
	XTP_GRIDRECORDITEM_METRICS* pItemMetrics; // Pointer to an XTP_GRIDRECORDITEM_METRICS
											  // structure to be filled.
};

//===========================================================================
// Summary:
//     This structure is sent to the Main window in a WM_NOTIFY message from
//     Grid Control to determine if records are dragged or dropped.
// Example:
// <code>
// BEGIN_MESSAGE_MAP(CGridSampleView, CXTPGridView)
//     ON_NOTIFY(XTP_NM_GRID_BEGINDRAG, XTP_ID_GRID_CONTROL, OnGridBeginDrag)
// END_MESSAGE_MAP()
//
// void CGridSampleView::OnGridBeginDrag(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_GRIDDRAGDROP* pItemNotify = (XTP_NM_GRIDDRAGDROP*)pNotifyStruct;
//
//     ASSERT(pItemNotify->pRecords);
// }
// </code>
// See Also:
//     XTP_NM_GRID_DROP, XTP_NM_GRID_BEGINDRAG, CXTPGridControl::EnableDragDrop
//===========================================================================
struct XTP_NM_GRIDDRAGDROP
{
	NMHDR hdr; // Standard structure; contains information about a notification message.
	CXTPGridRecords* pRecords;	 // Records will be dragged/dropped.
	CXTPGridRecord* pTargetRecord; // Target record (if any).
	int nTargetRow;				   // Index of target row.
	BOOL bAbove;				   // Where to insert (relative to the target record).
	DROPEFFECT dropEffect;		   // DropEffect flags.
	CPoint pt;					   // Current location of the cursor in client coordinates.
	int nState;					   // Transition state (0 - enter, 1 - leave, 2 - over).
	DWORD dwKeyState;			   // Key state.
	COleDataSource* pDataSource;   // Data Source - issue 22675.
	COleDataObject* pDataObject;   // Data Object - required for drag over and drop.
	BOOL bReturnValue;			   // TRUE to accept dropEffect, FALSE to not accept dropEffect.
};

//===========================================================================
// Summary:
//     This structure is sent to the Main window in a WM_NOTIFY message before
//     processing the OnKeyDown event.
// See Also:
//     CXTPGridControl::OnPreviewKeyDown, XTP_NM_GRID_PREVIEWKEYDOWN,
//     CXTPGridControl::OnKeyDown, CWnd::OnKeyDown.
//===========================================================================
struct XTP_NM_GRIDPREVIEWKEYDOWN
{
	NMHDR hdr; // Standard structure; contains information about a notification message.

	// OnKeyDown parameters
	UINT nChar;   // [in/out] Specifies the virtual key code of the given key.
	UINT nRepCnt; // [in] Repeat count.
	UINT nFlags;  // [in] Specifies the scan code, key-transition code, previous key state, and
				  // context code.

	BOOL bCancel; // [out] TRUE to cancel processing key, FALSE to continue.
};

//===========================================================================
// Summary:
//     Enumeration of operational mouse modes.
// Remarks:
//     GridControl has several mouse states handled by the control.
//     This enumeration helps to clearly identify each of these:
//     * Sends Notifications.
//     * Sends Messages.
// See Also:
//     CXTPGridControl::SetMouseMode, CXTPGridControl::GetMouseMode
//
// <KEYWORDS xtpGridMouseNothing, xtpGridMouseOverColumnDivide, xtpGridMousePrepareDragColumn,
// xtpGridMouseDraggingColumn>
//===========================================================================
enum XTPGridMouseMode
{
	xtpGridMouseNothing,		   // User is just watching to the list.
	xtpGridMouseOverColumnDivide,  // User watching, but mouse is under column divider.
	xtpGridMousePrepareDragColumn, // User holds mouse button pressed on the column header.
	xtpGridMouseDraggingColumn,	// User is dragging column header.
	xtpGridMouseOverRowDivide,	 // User watching, but mouse is under row divider.
};

//-----------------------------------------------------------------------
// Summary:
//     Enumeration of drag and drop flags.
// Remarks:
//     Call CXTPGridControl::EnableDragDrop to allow drag/drop operations.
// Example:
//     <code>m_wndGrid.EnableDragDrop("MyApplication", xtpGridAllowDrop |
//     xtpGridAllowDrag);</code>
// See Also:
//     CXTPGridControl::EnableDragDrop
//-----------------------------------------------------------------------
enum XTPGridDragDrop
{
	xtpGridAllowDrop	  = 1, // Allow drop records to grid.
	xtpGridAllowDragCopy  = 2, // Allow copy records from grid.
	xtpGridAllowDragMove  = 4, // Allow move records from grid.
	xtpGridAllowDrag	  = 6, // Allow copy and move records from grid.
	xtpGridDontDropAsText = 8  // Do not drag record as plain text.
};

//-----------------------------------------------------------------------
// Summary:
//     Enumeration describes the drop marker.
//-----------------------------------------------------------------------
enum XTPGridDropMarker
{
	xtpGridDropBetween = 1, // Drop the row between two rows.
	xtpGridDropSelect  = 2, // Drop the selected rows.
};

//-----------------------------------------------------------------------
// Summary:
//     Enumeration of watermark alignment flags.
// Remarks:
//     Call CXTPGridControl::SetWatermarkAlignment to modify watermark alignment.
// Example:
//     <code>m_wndGrid.SetWatermarkAlignment(xtpWatermarkCenter | xtpWatermarkBottom);</code>
// See Also:
//     CXTPGridControl::SetWatermarkAlignment, GetWatermarkAlignment
//-----------------------------------------------------------------------
enum XTPGridWatermarkAlignment
{
	xtpGridWatermarkUnknown = 0, // Unknown (empty) value.

	xtpGridWatermarkLeft = 0x0001,   // Horizontal alignment: left side of Grid control client
									 // rect.
	xtpGridWatermarkCenter = 0x0002, // Horizontal alignment: center of Grid control client
									 // rect.
	xtpGridWatermarkRight = 0x0004,  // Horizontal alignment: right side of Grid control client
									 // rect.
	xtpGridWatermarkHmask = 0x000F,  // A mask for horizontal alignment flags.

	xtpGridWatermarkTop		= 0x0010, // Vertical alignment: top side of Grid control client rect.
	xtpGridWatermarkVCenter = 0x0020, // Vertical alignment: center of Grid control client rect.
	xtpGridWatermarkBottom  = 0x0040, // Vertical alignment: bottom side of Grid control client
									  // rect.
	xtpGridWatermarkVmask = 0x00F0,   // A mask for vertical alignment flags.

	xtpGridWatermarkStretch		= 0x0100, // Stretch watermark to entire Grid control client rect.
	xtpGridWatermarkEnlargeOnly = 0x0200, // Watermark can be enlarged only, shrinking is
										  // disabled.
	xtpGridWatermarkShrinkOnly = 0x0400,  // Watermark can be shrinked only, enlarging is disabled.
	xtpGridWatermarkPreserveRatio = 0x0800, // Watermark aspect ratio is preserved.
};

//-----------------------------------------------------------------------
// Summary:
//     Enumeration of GetElementRect flags.
// Remarks:
//     Call CXTPGridControl::GetElementRect to get grid element rectangle.
// Example:
//     <code>CRect rcElement = m_wndGrid.GetElementRect(xtpGridRectGroupByArea);</code>
// See Also:
//     CXTPGridControl::GetElementRect
//-----------------------------------------------------------------------
enum XTPGridElementRect
{
	xtpGridElementRectGridArea = 0,				// The area occupied by the records.
	xtpGridElementRectGroupByArea,				// The area occupied by Group By item.
	xtpGridElementRectHeaderArea,				// The area occupied by the header.
	xtpGridElementRectFooterArea,				// The area occupied by the footer.
	xtpGridElementRectHeaderRecordsArea,		// The area occupied by the header records.
	xtpGridElementRectFooterRecordsArea,		// The area occupied by the footer records.
	xtpGridElementRectHeaderRecordsDividerArea, // The area occupied by the header records
												// divider.
	xtpGridElementRectFooterRecordsDividerArea, // The area occupied by the footer records
												// divider.
};

//-----------------------------------------------------------------------
// Summary:
//     Enumeration of options for the SetRecordsTreeFilterMode method.
// Remarks:
//     Call CXTPGridControl::SetRecordsTreeFilterMode to set options for the filtering tree.
// Example:
//     <code>m_wndGrid.SetRecordsTreeFilterMode(xtpGridFilterTreeSimple);</code>
// See Also:
//     CXTPGridControl::SetRecordsTreeFilterMode
//-----------------------------------------------------------------------
enum XTPGridFilterMode
{
	xtpGridFilterTreeSimple				 = 0,
	xtpGridFilterTreeByParentAndChildren = 1,
	xtpGridFilterTreeByEndChildrenOnly   = 2
};

//-----------------------------------------------------------------------
// Summary:
//     Enumeration of scroll update modes.
//-----------------------------------------------------------------------
enum XTPGridScrollUpdateMode
{
	xtpGridScrollUpdateModeImmediate, // Immediately updates the scroll position.
	xtpGridScrollUpdateModeDeferred   // Updates the scroll position deferred.
};

//-----------------------------------------------------------------------
// Summary:
//     Enumeration of scroll modes.
//-----------------------------------------------------------------------
enum XTPGridScrollMode
{
	xtpGridScrollModeNone		= 0, // No scrolling.
	xtpGridScrollModeBlockCount = 1, // Scrolls an entire row or column.
	xtpGridScrollModeBlockSize  = 3, // Scrolls an entire row or column.
	xtpGridScrollModeSmooth		= 2, // Smooth scrolling.

	// Deprecated
	xtpGridScrollModeBlock = xtpGridScrollModeBlockCount,
};

//-----------------------------------------------------------------------
// Summary:
//     Stores the mouse and key states.
//-----------------------------------------------------------------------
struct XTPGridInputState
{
	XTPGridInputState()
		: ptMouse(-1, -1)
		, bKeyControl(FALSE)
		, bKeyShift(FALSE)
		, nRow(-1)
		, bSelected(FALSE)
	{
		// Nothing.
	}

	CPoint ptMouse;   // Mouse position.
	BOOL bKeyControl; // Control key state.
	BOOL bKeyShift;   // Shift key state.
	int nRow;		  // Clicked row index.
	BOOL bSelected;   // Clicked row selection state.
};

//===========================================================================
// Summary:
//     The CXTPGridControl class provides an implementation of
//     the Grid control.
//
// Remarks:
//     A "Grid control" is a window that displays a hierarchical list
//     of items, such as emails in the inbox. Each item is called a CXTPGridRow
//     and consists of its properties and corresponding CXTPGridRecord,
//     which contains all the corresponding data (mostly text).
//     Each Row item (as well as Record) can have a list of sub-items
//     associated with it. By clicking a Row item, the user can expand and
//     collapse the associated list of sub-items.
//
//     The CXTPGridRecords collection holds all the CXTPGridRecord objects
//     that are assigned to the Grid control. It could be accessible via
//     the GetRecords() method. The records in this collection
//     are referred to as the root records. Any record that is subsequently
//     added to a root record is referred to as a child record. Because each
//     CXTPGridRecord can contain a collection of other CXTPGridRecord
//     objects, you might find it difficult to determine your location in the
//     tree structure when you iterate through the collection.
//
//     Record nodes can be expanded to display the next level of child records.
//     The user can expand the CXTPGridRecord by clicking the plus-sign (+)
//     button, if one is displayed, or you can expand the CXTPGridRecord by
//     calling the CXTPGridRecord::SetExpanded method.
//     To expand all the child records, call the ExpandAll method.
//     You can collapse the child CXTPGridRecord level by calling the
//     CXTPGridRecord::SetExpanded(FALSE) method, or the user can press
//     the minus-sign (-) button, if one is displayed. You can also call
//     CollapseAll method to collapse all child records.
//
//     Each record contains an array of record items which are implemented
//     with CXTPGridRecordItem and its descendants. You can create your own
//     types of items simply by inheriting from the base record item class.
//
//     Each record item has an association with a corresponding CXTPGridColumn
//     item. The item will be shown below the corresponding column header
//     depending on its position in Grid control columns array. If a column
//     does not have an associated item in the record, there will be an empty item
//     shown in the corresponding cell.
//
//     Columns array is represented by the CXTPGridColumns collection and could
//     be accessed via the GetColumns() method.
//
//     As a finalization of adding data to the Grid control, which means
//     adding columns and records, the Populate() method should be called. It
//     performs population of control rows with data - creates a rows tree if
//     necessary, rebuilds groups if grouping if enabled, and sorts rows
//     on a specified manner. See Also an example below.
//
//     Handling notification messages sent by the control to the parent
//     window is allowed with the ON_NOTIFY handler. The control uses the
//     SendMessageToParent function to send notifications. See below for
//     the example of how messages could be handled in a parent window:
//
// <code>
// ON_NOTIFY(NM_CLICK, ID_GRID_CONTROL, OnGridItemClick)
// ON_NOTIFY(NM_RCLICK, ID_GRID_CONTROL, OnGridItemRClick)
// ON_NOTIFY(NM_DBLCLK, ID_GRID_CONTROL, OnGridItemDblClick)
// ON_NOTIFY(XTP_NM_SHOWFIELDCHOOSER, ID_GRID_CONTROL, OnShowFieldChooser)
// ON_NOTIFY(XTP_NM_HEADER_RCLICK, ID_GRID_CONTROL, OnGridColumnRClick)
// ON_NOTIFY(NM_KEYDOWN, ID_GRID_CONTROL, OnGridKeyDown)
// </code>
//
//     You can also change the appearance of the CXTPGridControl control
//     by setting some of its display and style properties.
//
//     Also Grid control has an ability to store and restore its
//     settings, which includes all columns with their settings, and some
//     required control's settings. It is implemented via standard MFC and XTP
//     serialization and is available with the member functions
//     SerializeState(CArchive& ar), DoPropExchange(CXTPPropExchange* pPX);
//
//     Grid control supports Copy/Paste clipboard operations.
//     See methods: CanCut(), CanCopy(), CanPaste(), Cut(), Copy(), Paste().
//     There are 2 clipboard formats that are supported:
//          Binary  - contains all record(s) data;
//          Text    - contains visible columns texts.
//
//     To support binary format the XTP serialization is used -
//     DoPropExchange() methods are implemented for CXTPGridRecord,
//     CXTPGridRecordItem and derived CXTPGridRecordItemXXX classes.
//     Also some part of standard MFC serialization is used
//     (see DECLARE_SERIAL macro) to automatically create classes when
//     loading from the data source.
//
//     If you are creating custom records and records items classes, you have
//     to use DECLARE_SERIAL macro and may need to override DoPropExchange()
//     methods to serialize custom data as well as standard records data.
//
//     Storing records is simple: the CXTPGridRecord (or derived class)
//     is stored first, then record items (CXTPGridRecordItemXXX classes)
//     are stored one by one.
//     The class information, which allows for creating object instances when
//     loading, is stored for all classes. See CArchive::WriteClass(),
//     CArchive::ReadClass() and other CArchive members for more details
//     about this idea.
//
//     When Grid control loads records from the data source,
//     the record class is created automatically (using stored
//     class information).
//     Then the items collection is cleared and record items are created
//     automatically and added to the items collection.
//     For example see GridSample project: CMessageRecord class.
//
//     We support text format with '\\t' dividers for record items and
//     "\\r\\n" dividers for records (simple tab-separated text).
//     Such format is also supported by Excel and some other applications.
//
//     There are a few methods and corresponding notifications which allow
//     for the customization of copy/paste operations:
//     OnBeforeCopyToText(); OnBeforePasteFromText(); OnBeforePaste();
//
// See Also:
//     CXTPGridView, CXTPGridHeader, CXTPGridRow, CXTPGridRecord,
//     CXTPGridColumn,
//     CXTPGridRecords, CXTPGridRows, CXTPGridColumns,
//     CXTPGridSubListControl, CXTPGridFilterEditControl
//===========================================================================
class _XTP_EXT_CLASS CXTPGridControl
	: public CWnd
	, public CXTPAccessible
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNCREATE(CXTPGridControl)
	DECLARE_INTERFACE_MAP()
	XTP_DECLARE_CMDTARGETPROVIDER_INTERFACE()

	friend class CXTPGridSubListControl;
	friend class CXTPGridRow;
	friend class CXTPGridGroupRow;
	friend class CXTPGridHeader;
	friend class CXTPGridNavigator;
	friend class CXTPGridInplaceEdit;
	friend class CXTPGridHeaderDragWnd;
	friend class CXTPGridInplaceList;
	friend class CXTPGridColumn;
	friend class CXTPGridView;
	class CGridDropTarget;
	//}}AFX_CODEJOCK_PRIVATE

public:
	//===========================================================================
	// Summary:
	//     Internal grid update helper.
	//===========================================================================
	class _XTP_EXT_CLASS CUpdateContext
	{
	public:
		//-----------------------------------------------------------------------
		// Summary:
		//     Constructs a CUpdateContext object.
		// Parameters:
		//     pControl - Pointer to a Grid Control object.
		//-----------------------------------------------------------------------
		CUpdateContext(CXTPGridControl* pControl)
		{
			m_pControl = pControl;
			pControl->BeginUpdate();
		}

		//-------------------------------------------------------------------------
		// Summary:
		//     Destroys a CUpdateContext object,
		//     handles cleanup and deallocation.
		//-------------------------------------------------------------------------
		~CUpdateContext()
		{
			m_pControl->EndUpdate();
		}

	protected:
		CXTPGridControl* m_pControl; // Updated Grid control pointer.
	};

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to use the custom heap feature.
	// Remarks:
	//     If it is enabled, grid data will be allocated in custom (separate)
	//     heap instead of standard application heap. This optimize memory
	//     using (fragmentation) for big amount of data.
	//     For custom heap used for classes derived from CXTPHeapObjectT template,
	//     like CXTPGridRecord, CXTPGridRecordItem, CXTPGridRow and so others.
	//     This template just overrides operators new and delete.
	//     <p/>It must be called on initialization before any allocations of classes
	//     which use custom heap. OnInitInstance is a fine place for this.
	// Returns:
	//     TRUE if custom heap feature is enabled for all grid allocators,
	//     FALSE otherwise.
	//-----------------------------------------------------------------------
	static BOOL AFX_CDECL UseGridCustomHeap();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to enable the batch allocation feature for grid rows.
	// Remarks:
	//     Batch allocation means that memory allocated not for one object only,
	//     but for many objects at one time (for 1024 objects by default).
	//     Next allocations take memory from this big block. New blocks allocated
	//     when necessary. This increase performance and reduce heap fragmentation.
	//     Batch allocation mechanism responsible for allocation/deallocation
	//     blocks of memory from heap and internally organize free/busy lists of
	//     memory pieces. When object deleted, its memory stored in free list and
	//     used for new objects.
	//     When all memory pieces from block free, it may be deallocated from
	//     heap automatically (this depends on options in _TBatchAllocData)
	//     or by FreeExtraData call,
	//     <p/>It must be called on initialization before any allocations of classes
	//     which use batch allocation. OnInitInstance is a fine place for this.
	// Returns:
	//     TRUE if batch allocation feature is enabled for grid rows,
	//     FALSE otherwise.
	//-----------------------------------------------------------------------
	static BOOL AFX_CDECL UseRowBatchAllocation();

	//-----------------------------------------------------------------------
	// Summary:
	//     This function is used when row batch allocation is enabled to both check
	//     all allocated blocks and deallocate any blocks which are completely free.
	// See Also:
	//     UseRowBatchAllocation, CXTPBatchAllocObjT
	//-----------------------------------------------------------------------
	static void AFX_CDECL FreeRowBatchExtraData();

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPGridControl object.
	// Remarks:
	//     You construct a CXTPGridControl object in two steps.
	//     First, call the constructor CXTPGridControl and then call
	//     the Create method. This initializes the window.
	//
	// Example:
	// <code>
	// // Declare a local CXTPGridControl object.
	// CXTPGridControl myGrid;
	//
	// // Declare a dynamic CXTPGridControl object.
	// CXTPGridControl* pMyGrid = new CXTPGridControl();
	//
	// if (!myGrid.Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE | WM_VSCROLL, CRect(0, 0, 0, 0),
	// this, ID_GRID_CONTROL))
	// {
	//     TRACE(_T("Failed to create view window\n"));
	// }
	// </code>
	// See Also:
	//     Create
	//-----------------------------------------------------------------------
	CXTPGridControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPGridControl object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPGridControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to create a Grid control.
	// Parameters:
	//     dwStyle     - Specifies the window style attributes
	//                   (this could be a combination of standard window styles).
	//     rect        - Size and position of the window,
	//                   in client coordinates of pParentWnd.
	//     pParentWnd  - Specifies the Grid control parent window.
	//     nID         - Specifies the Grid control identifier.
	//     pContext    - The create context of the window.
	// Remarks:
	//     You construct a CXTPGridControl object in two steps.
	//     First, call the constructor CXTPGridControl and then call
	//     the Create method. This initializes the window.
	// Example:
	//     See the example for CXTPGridControl::CXTPGridControl.
	// Returns:
	//     Nonzero if successful, otherwise 0.
	// See Also:
	//     CXTPGridControl::CXTPGridControl
	//-----------------------------------------------------------------------
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,
				CCreateContext* pContext = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     (Re)Stores control configuration to/from the provided archive stream.
	// Parameters:
	//     ar - Archive stream for serializing.
	//-----------------------------------------------------------------------
	virtual void SerializeState(CArchive& ar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads/writes configuration to/from the provided data source.
	// Parameters:
	//     pPX - Pointer to a CXTPPropExchange object to serialize to/from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a specified column to the end of the columns array.
	// Parameters:
	//     pColumn - Pointer to a CXTPGridColumn object to add.
	// Remarks:
	//     Call this member function if you want to add a specified column to the
	//     Grid control. It will be added to the end of the columns array.
	// Returns:
	//     A pointer to the newly added column.
	// Example:
	// <code>
	// // this function adds a column with "Subject" caption and 250 pixels initial width
	// void AddSubjectColumn(CXTPGridControl* pGridCtrl)
	// {
	//     pGridCtrl->AddColumn(new CXTPGridColumn(1, _T("Subject"), 250));
	// }
	// </code>
	// See Also:
	//     CXTPGridColumn overview
	//-----------------------------------------------------------------------
	CXTPGridColumn* AddColumn(CXTPGridColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a specified data record to the records collection.
	// Parameters:
	//     pRecord - Pointer to a CXTPGridRecord object to add.
	// Remarks:
	//     Call this member function if you want to add a specified
	//     data record to the Grid control's internal storage.
	// Returns:
	//     A pointer to the newly added data record.
	// Example:
	// <code>
	// // this function adds 2 empty records to a Grid control
	// void Add2Empties(CXTPGridControl* pGridCtrl)
	// {
	//     pGridCtrl->AddRecord(new CXTPGridRecord());
	//     pGridCtrl->AddRecord(new CXTPGridRecord());
	// }
	// </code>
	// Returns:
	//     Pointer to the recently added record object.
	// See Also:
	//     CXTPGridRecord overview, GetRecords
	//-----------------------------------------------------------------------
	virtual CXTPGridRecord* AddRecord(CXTPGridRecord* pRecord);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a specified data record to the records collection and
	//     associates a row with it.
	// Parameters:
	//     pRecord           - Pointer to a CXTPGridRecord object to add.
	//     pParentRecord     - Pointer to a parent CXTPGridRecord object
	//                         for pRecord to be added to.
	//     nRowChildIndex    - Child index for a row to be inserted at in case the
	//                         index is not defined by other conditions (or -1).
	//     nRecordChildIndex - Child index for a record to be inserted at (or -1).
	// Remarks:
	//     Call this member function if you want to add a data record to the
	//     Grid control's internal storage and associate a row with it.
	// Example:
	// <code>
	// // this function adds 2 empty records to a Grid control
	// void Add2Empties(CXTPGridControl* pGridCtrl)
	// {
	//     pGridCtrl->AddRecordEx(new CXTPGridRecord());
	//     pGridCtrl->AddRecordEx(new CXTPGridRecord());
	// }
	// </code>
	// See Also:
	//     CXTPGridRecord overview, GetRecords, RemoveRecordEx, RemoveRowEx, UpdateRecord
	//-----------------------------------------------------------------------
	virtual void AddRecordEx(CXTPGridRecord* pRecord, CXTPGridRecord* pParentRecord = NULL,
							 int nRowChildIndex = -1, int nRecordChildIndex = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes a specified record, the row associated with the specified record,
	//     and all child records and rows.
	// Parameters:
	//     pRecord           - Pointer to a CXTPGridRecord object to remove.
	//     bAdjustLayout     - TRUE to call the AdjustLayout method.
	//     bRemoveFromParent - TRUE to remove the record from its parent.
	// Remarks:
	//     Call this member function if you want to remove a record and row on the fly
	//     without calling the Populate method.
	// Returns:
	//     TRUE if the operation was successful, otherwise FALSE.
	// See Also:
	//     RemoveRowEx, AddRecordEx, CXTPGridRecords::RemoveRecord, UpdateRecord
	//-----------------------------------------------------------------------
	virtual BOOL RemoveRecordEx(CXTPGridRecord* pRecord, BOOL bAdjustLayout = TRUE,
								BOOL bRemoveFromParent = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes a specified row, the record associated with the specified row,
	//     and all child rows and records.
	// Parameters:
	//     pRow          - Pointer to a CXTPGridRow object to remove.
	//     bAdjustLayout - TRUE to call the AdjustLayout method.
	// Remarks:
	//     Call this member function if you want to remove a row and record on the fly
	//     without calling the Populate method.
	// Returns:
	//     TRUE if the operation was successful, otherwise FALSE.
	// See Also:
	//     RemoveRecordEx, AddRecordEx, CXTPGridRecords::RemoveRecord, UpdateRecord
	//-----------------------------------------------------------------------
	virtual BOOL RemoveRowEx(CXTPGridRow* pRow, BOOL bAdjustLayout = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates a specified record.
	// Parameters:
	//     pRecord         - Pointer to a CXTPGridRecord object to update.
	//     bUpdateChildren - TRUE to update the children of pRecord.
	// Remarks:
	//     Call this member function if you modified a record and want it
	//     to be updated according to the current grouping and sorting.
	// See Also:
	//     CXTPGridRecord overview, GetRecords
	//-----------------------------------------------------------------------
	virtual void UpdateRecord(CXTPGridRecord* pRecord, BOOL bUpdateChildren);

	//-----------------------------------------------------------------------
	// Summary:
	//     Prevents the Grid control from being redrawn until the EndUpdate
	//     method is called.
	// Remarks:
	//     If you want to add items one at a time using the AddRecord method, or
	//     if you want to perform other operations in a single sequence, then
	//     you can use the BeginUpdate method to prevent the control from repainting
	//     the CXTPGridControl each time an item is added. Once you have
	//     completed the task of adding items to the control, call the EndUpdate
	//     method to enable the CXTPGridControl to repaint. This way of
	//     adding items can prevent flickered drawing of the CXTPGridControl when
	//     a large number of items are being added to the control.
	// Example:
	// <code>
	// // This function collapses all rows for the specified Grid control
	// void CollapseAll(CXTPGridControl* pGridCtrl)
	// {
	//     pGridCtrl->BeginUpdate();
	//     for (int i = pGridCtrl->GetRows()->GetCount() - 1; i >= 0; i --)
	//         pGridCtrl->GetRows()->GetAt(i)->SetExpanded(FALSE);
	//
	//     pGridCtrl->EndUpdate();
	// }
	// </code>
	// See Also:
	//     EndUpdate, RedrawControl, AddRecord
	//-----------------------------------------------------------------------
	void BeginUpdate();

	//-----------------------------------------------------------------------
	// Summary:
	//     Deletes all rows and records from the Grid control.
	// Parameters:
	//     bUpdateControl - TRUE to redraw the control, FALSE otherwise.
	//-----------------------------------------------------------------------
	void ResetContent(BOOL bUpdateControl = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Resumes drawing of the Grid control after drawing has been suspended
	//     by the BeginUpdate method.
	// Remarks:
	//     If you want to add items one at a time using the AddRecord method, or
	//     if you want to perform other operations in a single sequence, then
	//     you can use the BeginUpdate method to prevent the control from repainting
	//     the CXTPGridControl each time an item is added. Once you have
	//     completed the task of adding items to the control, call the EndUpdate
	//     method to enable the CXTPGridControl to repaint. This way of
	//     adding items can prevent flickered drawing of the CXTPGridControl when
	//     a large number of items are being added to the control.
	// Example:
	//     See example for CXTPGridControl::BeginUpdate method.
	// See Also:
	//     BeginUpdate, RedrawControl
	//-----------------------------------------------------------------------
	void EndUpdate();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of update locks.
	// Remarks:
	//     The number of update locks.
	//-----------------------------------------------------------------------
	int GetLockUpdateCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Initiates Grid control redrawing.
	// Remarks:
	//     Call this member function if you want to initialize
	//     Grid control redrawing. The control will be redrawn
	//     taking into account its latest state.
	// See Also:
	//     BeginUpdate, EndUpdate
	//-----------------------------------------------------------------------
	void RedrawControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tooltip context.
	// Returns:
	//     A pointer to the tooltip context.
	//-----------------------------------------------------------------------
	CXTPToolTipContext* GetToolTipContext() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to enable built in drag and drop operations
	// Parameters:
	//     lpszClipboardFormat - Name of the clipboard format to be used
	//                           for the Grid control.
	//     dwFlags - Combination of XTPGridDragDrop flags;
	//               can be one or more of following:
	//               * <b>xtpGridAllowDrop</b>: Allow drop records to grid.
	//               * <b>xtpGridAllowDragCopy</b>: Allow copy records from grid.
	//               * <b>xtpGridAllowDragMove</b>: Allow move records from grid.
	//               * <b>xtpGridAllowDrag</b>: Allow copy and move records from grid.
	//     dwDropMarkerFlags - Combination of required bits specified by
	//                         the XTPGridDropMarker enumeration.
	// Returns:
	//     Clipboard format that will be used with Grid Control
	// See Also:
	//     XTPGridDragDrop
	//-----------------------------------------------------------------------
	CLIPFORMAT EnableDragDrop(LPCTSTR lpszClipboardFormat, DWORD dwFlags,
							  DWORD dwDropMarkerFlags = xtpGridDropBetween);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the drop marker flags.
	// Parameter:
	//     dwDropMarkerFlags - Drop marker flags.
	//-----------------------------------------------------------------------
	void SetDropMarkerFlags(DWORD dwDropMarkerFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the drop marker flags.
	// Returns:
	//     The drop marker flags.
	//-----------------------------------------------------------------------
	DWORD GetDropMarkerFlags() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the grid drawing style.
	// Parameters:
	//     orientation - Determines the grid style to set; must be one of the
	//                   values defined by the XTPGridOrientation enumeration;
	//                   xtpGridOrientationVertical to set the vertical grid style,
	//                   xtpGridOrientationHorizontal to set the horizontal grid style.
	//     gridStyle   - Grid style to be set; must be one of the values
	//                   defined by the XTPGridGridStyle enumeration.
	// Remarks:
	//     Call this member function if you want to set the
	//     grid line drawing style for the Grid control.
	//
	//     Possible grid line drawing styles are the following:
	//     * <b>xtpGridGridNoLines</b>: Empty line.
	//     * <b>xtpGridGridSmallDots</b>: Line drawn by small dots.
	//     * <b>xtpGridGridLargeDots</b>: Line drawn by large dots.
	//     * <b>xtpGridGridDashes</b>: Line drawn by dashes.
	//     * <b>xtpGridGridSolid</b>: Draws solid line.
	// See Also:
	//     XTPGridGridStyle overview, GetGridStyle, SetGridColor
	//-----------------------------------------------------------------------
	void SetGridStyle(XTPGridOrientation orientation, XTPGridGridStyle gridStyle);

	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATED_IN_FAVOR(SetGridStyle)
	void SetReportStyle(XTPGridOrientation orientation, XTPGridGridStyle gridStyle);
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the grid drawing style.
	// Parameters:
	//     orientation - Determines the grid style to get; must be one of the
	//                   values defined by the XTPGridOrientation enumeration;
	//                   xtpGridOrientationVertical to get the vertical grid style,
	//                   xtpGridOrientationHorizontal to get the horizontal grid style.
	// Remarks:
	//     Call this member function if you want to get the
	//     grid line drawing style for the Grid control.
	// Returns:
	//     The grid drawing style; one of the values defined by
	//     the XTPGridGridStyle enumeration.
	// See Also:
	//     XTPGridGridStyle overview, SetGridStyle, SetGridColor
	//-----------------------------------------------------------------------
	XTPGridGridStyle GetGridStyle(XTPGridOrientation orientation) const;

	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATED_IN_FAVOR(GetGridStyle)
	XTPGridGridStyle GetReportStyle(XTPGridOrientation orientation) const;
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the grid line color.
	// Parameters:
	//     clrGridLine - Grid line color to be set.
	// Remarks:
	//     Call this member function if you want to change the color of
	//     Grid control grid lines.
	// Returns:
	//     The previous grid line color.
	// See Also:
	//     SetGridStyle, GetGridStyle
	//-----------------------------------------------------------------------
	COLORREF SetGridColor(COLORREF clrGridLine);

	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATED_IN_FAVOR(SetGridColor)
	COLORREF SetReportColor(COLORREF clrGridLine);
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the collection of grid columns.
	// Returns:
	//     A pointer to the collection of grid columns.
	// Example:
	// <code>
	// // this function adds a column with "Subject" caption and 250 pixels initial width
	// void AddSubjectColumn(CXTPGridControl* pGridCtrl)
	// {
	//     pGridCtrl->GetColumns()->Add(new CXTPGridColumn(1, _T("Subject"), 250));
	// }
	// </code>
	// See Also:
	//     CXTPGridColumns overview
	//-----------------------------------------------------------------------
	CXTPGridColumns* GetColumns() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the paint manager object used to draw the Grid control window.
	// Returns:
	//     A pointer to the paint manager object.
	// See Also:
	//     CXTPGridPaintManager overview
	//-----------------------------------------------------------------------
	CXTPGridPaintManager* GetPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the paint manager object used to draw the Grid control window.
	// Parameters:
	//     pPaintManager - Pointer to a CXTPGridPaintManager object.
	//-----------------------------------------------------------------------
	void SetPaintManager(CXTPGridPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the navigator object used for cell navigation by the Grid control.
	// Returns:
	//     A pointer to the navigator object.
	// See Also:
	//     CXTPGridNavigator overview
	//-----------------------------------------------------------------------
	CXTPGridNavigator* GetNavigator() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Performs control population, creating a view from the data.
	// Remarks:
	//     Call this member function to populate control's rows collection
	//     with the data containing in the records collection.
	//     This method automatically creates Tree View references if
	//     necessary (e.g. in Grouping mode).
	//     This method is the main function which should be called for
	//     (re)populating all data changes made in the records collection.
	// See Also:
	//     CXTPGridPaintManager overview
	//-----------------------------------------------------------------------
	virtual void Populate();

	//-----------------------------------------------------------------------
	// Summary:
	//     Performs control's header rows population, creating a view from the data.
	// Remarks:
	//     Call this member function to populate control's header rows collection
	//     with the data contained in the header records collection.
	//     This method is useful when main rows are controlled through the use of
	//     the AddRecordEx/RemoveRecordEx/UpdateRecord methods.
	// See Also:
	//     Populate, PopulateFooterRows, AddRecordEx, RemoveRecordEx, UpdateRecord
	//-----------------------------------------------------------------------
	virtual void PopulateHeaderRows();

	//-----------------------------------------------------------------------
	// Summary:
	//     Performs control's footer rows population, creating a view from the data.
	// Remarks:
	//     Call this member function to populate control's footer rows collection
	//     with the data contained in the footer records collection.
	//     This method is useful when main rows are controlled through the use of
	//     the AddRecordEx/RemoveRecordEx/UpdateRecord methods.
	// See Also:
	//     Populate, PopulateHeaderRows, AddRecordEx, RemoveRecordEx, UpdateRecord
	//-----------------------------------------------------------------------
	virtual void PopulateFooterRows();

	//-----------------------------------------------------------------------
	// Summary:
	//     Ensures that a Grid control row is at least partially visible.
	// Parameters:
	//     pRow - Pointer to the CXTPGridRow object whose
	//            visibility must be checked.
	// Remarks:
	//     Ensures that a grid row item is at least partially visible.
	//     The list view control is scrolled if necessary.
	// See Also:
	//     MoveDown, MoveUp, MovePageDown, MovePageUp, MoveFirst, MoveLast
	//-----------------------------------------------------------------------
	void EnsureVisible(CXTPGridRow* pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Ensures that a Grid control column is at least partially visible.
	// Parameters:
	//     pCheckColumn - Pointer to the CXTPGridColumn object whose
	//                    visibility must be checked.
	// Remarks:
	//     Ensures that a grid column item is at least partially visible.
	//     The list view control is scrolled if necessary.
	// See Also:
	//     MoveRight, MoveLeft, MoveToColumn, MoveFirstColumn, MoveLastColumn
	//-----------------------------------------------------------------------
	void EnsureVisible(CXTPGridColumn* pCheckColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines which grid part is at a specified point.
	// Parameters:
	//     pt    - Point to be tested.
	//     pInfo - Pointer to a CXTPGridHitTestInfo object that will
	//             receive the returned information.
	// Returns:
	//     TRUE if the method was successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL HitTest(CPoint pt, CXTPGridHitTestInfo* pInfo) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines which grid row is at a specified point, if any.
	// Parameters:
	//     pt - Point to be tested.
	// Returns:
	//     A pointer to the CXTPGridRow object at the specified point, if any,
	//     otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPGridRow* HitTest(CPoint pt) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the collection of selected rows.
	// Remarks:
	//     Use this member function to retrieve access to the collection
	//     of currently selected grid rows.
	// Returns:
	//     A pointer to the collection of selected rows.
	// See Also:
	//     CXTPGridSelectedRows overview.
	//-----------------------------------------------------------------------
	CXTPGridSelectedRows* GetSelectedRows() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the currently focused row in the Grid control's view.
	// Returns:
	//     A pointer to the currently focused row if successful, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPGridRow* GetFocusedRow() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the currently focused row in the Grid control's view.
	// Parameters:
	//     pRow        - Pointer to a CXTPGridRow object.
	//     bControlKey - TRUE to select a new focused row.
	// Returns:
	//     TRUE if the specified row has been focused, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL SetFocusedRow(CXTPGridRow* pRow, BOOL bControlKey = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the currently focused row in the Grid control's view.
	// Parameters:
	//     pRow        - Pointer to a CXTPGridRow object.
	//     bControlKey - TRUE to select a new focused row.
	//     bShiftKey   - TRUE to select rows up to the new focused row.
	// Returns:
	//     TRUE if the specified row has been focused, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL SetFocusedRow(CXTPGridRow* pRow, BOOL bShiftKey, BOOL bControlKey);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the currently focused row in the Grid control's view.
	// Parameters:
	//     bEnsureVisible - TRUE to ensure that the row will be visible.
	//     pRow           - Pointer to a CXTPGridRow object.
	//     bControlKey    - TRUE to select a new focused row.
	//     bShiftKey      - TRUE to select rows up to the new focused row.
	// Returns:
	//     TRUE if the specified row has been focused, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL SetFocusedRow(BOOL bEnsureVisible, CXTPGridRow* pRow, BOOL bShiftKey, BOOL bControlKey);

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the number of visible rows between a specified row
	//     and the bottom/top of the current view.
	// Parameters:
	//     nStartRow - Index of the row to begin calculating from.
	//     bMoveDown - TRUE to down, FALSE to move up.
	// Returns:
	//     The number of visible rows between the specified row
	//     and the bottom/top of the current view.
	// See Also:
	//     MovePageDown, MovePageUp
	//-----------------------------------------------------------------------
	int GetGridAreaRows(int nStartRow, BOOL bMoveDown);

	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATED_IN_FAVOR(GetGridAreaRows)
	int GetReportAreaRows(int nStartRow, BOOL bMoveDown);
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the top row of the Grid control view.
	// Parameters:
	//     nIndex - Index of the row to be set.
	// Remarks:
	//     The system scrolls the Grid control view upward until either
	//     the row specified by nIndex is at the top of the view
	//     or the maximum scroll range is reached.
	// See Also:
	//     GetGridAreaRows, EnsureVisible
	//-----------------------------------------------------------------------
	void SetTopRow(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the index of the top row of the Grid control view.
	// Returns:
	//     The index of the top row of the Grid control view.
	// See Also:
	//     SetTopRow
	//-----------------------------------------------------------------------
	int GetTopRowIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the horizontal scroll position.
	// Parameters:
	//     nOffset - Horizontal scroll position to be set.
	// Remarks:
	//     This method only takes effect when auto-column sizing is disabled.
	// See Also:
	//     CXTPGridHeader::SetAutoColumnSizing
	//-----------------------------------------------------------------------
	void SetScrollOffsetH(int nOffset);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the vertical scroll position.
	// Parameters:
	//     nOffset - Vertical scroll position to be set.
	//-----------------------------------------------------------------------
	void SetScrollOffsetV(int nOffset);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets right-to-left (RTL) mode.
	// Parameters:
	//     bRightToLeft - TRUE to set right-to-left (RTL) reading-order properties.
	//-----------------------------------------------------------------------
	void SetLayoutRTL(BOOL bRightToLeft);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if right-to-left (RTL) mode was set.
	// Returns:
	//     TRUE if the layout is right-to-left (RTL), otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsLayoutRTL();

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables preview mode for the control.
	// Parameters:
	//     bIsPreviewMode - TRUE to enable preview mode,
	//                      FALSE to disable preview mode.
	// Remarks:
	//     Preview mode has two states: enabled and disabled. When preview mode is
	//     enabled, the control tries to show additional bands with preview text.
	// See Also:
	//     IsPreviewMode
	//-----------------------------------------------------------------------
	void EnablePreviewMode(BOOL bIsPreviewMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if preview mode is enabled/disabled for the control.
	// Returns:
	//     TRUE if preview mode is enabled, FALSE if preview mode is disabled.
	// See Also:
	//     EnablePreviewMode
	//-----------------------------------------------------------------------
	BOOL IsPreviewMode() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the visible state of the Group By area.
	// Parameters:
	//     bEnable - TRUE to show the Group By area
	//               FALSE to hide the Group By area.
	// Remarks:
	//     Call this member function if you want to show/hide the
	//     Group By area in the grid header.
	// See Also:
	//     IsGroupByVisible
	//-----------------------------------------------------------------------
	void ShowGroupBy(BOOL bEnable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the visible state of the Group By area.
	// Remarks:
	//     Call this member function if you want to determine if the
	//     Group By area in the grid header is shown or not.
	// Returns:
	//     TRUE if the Group By area is shown,
	//     FALSE if the Group By area is hidden.
	// See Also:
	//     ShowGroupBy
	//-----------------------------------------------------------------------
	BOOL IsGroupByVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the visible state of column headers.
	// Parameters:
	//     bShow - TRUE to show column headers,
	//             FALSE to hide column headers.
	// See Also:
	//     IsHeaderVisible, ShowFooter
	//-----------------------------------------------------------------------
	void ShowHeader(BOOL bShow = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the visible state of column headers.
	// Returns:
	//     TRUE if column headers are shown,
	//     FALSE if column headers are hidden.
	// See Also:
	//     ShowHeader
	//-----------------------------------------------------------------------
	BOOL IsHeaderVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the visible state of column footers.
	// Parameters:
	//     bShow - TRUE to show column footers,
	//             FALSE to hide column footers.
	// See Also:
	//     IsFooterVisible, ShowHeader
	//-----------------------------------------------------------------------
	void ShowFooter(BOOL bShow = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the visible state of column footers.
	// Returns:
	//     TRUE if column footers are shown,
	//     FALSE if column footers are hidden.
	// See Also:
	//     ShowHeader
	//-----------------------------------------------------------------------
	BOOL IsFooterVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables group items shading.
	// Parameters:
	//     benable - TRUE to enable group items shading,
	//               FALSE to disable group items shading.
	// Remarks:
	//     Call this member function if you want to show/hide
	//     group rows' headings.
	// See Also:
	//     IsShadeGroupHeadingsEnabled, IsGroupRowsBold, SetGroupRowsBold,
	//     GetItemMetrics
	//-----------------------------------------------------------------------
	void ShadeGroupHeadings(BOOL bEnable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if group items shading is enabled/disabled.
	// Remarks:
	//     Call this member function if you want to determine whether
	//     group rows' headings are enabled or not.
	// Returns:
	//     TRUE if group items shading is enabled,
	//     FALSE if group items shading is disabled.
	// See Also:
	//     ShadeGroupHeadings, IsGroupRowsBold, SetGroupRowsBold,
	//     GetItemMetrics
	//-----------------------------------------------------------------------
	BOOL IsShadeGroupHeadingsEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the group rows text style: bold or normal.
	// Parameters:
	//     bBold - TRUE for bold text style,
	//             FALSE for normal text style.
	// See Also:
	//     IsGroupRowsBold, ShadeGroupHeadings, IsShadeGroupHeadingsEnabled,
	//     GetItemMetrics
	//-----------------------------------------------------------------------
	void SetGroupRowsBold(BOOL bBold = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the group rows text style: bold or normal.
	// Returns:
	//     TRUE if group rows text style is bold,
	//     FALSE if group rows text style is normal.
	// See Also:
	//     SetGroupRowsBold, ShadeGroupHeadings, IsShadeGroupHeadingsEnabled,
	//     GetItemMetrics
	//-----------------------------------------------------------------------
	BOOL IsGroupRowsBold() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of frozen columns starting from the left hand side.
	// Parameters:
	//     nLastIndex - Reference to an int to receive
	//                  the index of the rightmost frozen column.
	// Returns:
	//     The number of the frozen columns.
	//-----------------------------------------------------------------------
	int GetFreezeColumnsCount() const;
	int GetFreezeColumnsCount(
		int& nLastIndex) const; // <combine CXTPGridControl::GetFreezeColumnsCount@const>

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the index of the rightmost frozen column.
	// Returns:
	//     The index of the rightmost frozen column.
	//-----------------------------------------------------------------------
	int GetFreezeColumnsIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the number of frozen columns.
	// Parameters:
	//     nCount    - Number of frozen columns to be set
	//                 (e.g. 1 would make only one leftmost column frozen).
	//     bReserved -
	//-----------------------------------------------------------------------
	void SetFreezeColumnsCount(int nCount, BOOL bReserved = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of columns at the left side where reordering is disabled.
	// Returns:
	//     The number of columns at the left side.
	//-----------------------------------------------------------------------
	int GetDisableReorderColumnsCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the number of columns at the left side where reordering is disabled.
	// Parameters:
	//     nCount - Number of columns to be set.
	//-----------------------------------------------------------------------
	void SetDisableReorderColumnsCount(int nCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the horizontal scrolling mode.
	// Returns:
	//     TRUE if full columns scrolling mode is set,
	//     FALSE if horizontal scrolling by pixels mode is set.
	// See Also:
	//     SetFullColumnScrolling
	//-----------------------------------------------------------------------
	BOOL IsFullColumnScrolling() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the horizontal scrolling mode.
	// Parameters:
	//     bSet - TRUE to set full columns scrolling mode,
	//            FALSE to set horizontal scrolling by pixels mode.
	// See Also:
	//     IsFullColumnScrolling
	//-----------------------------------------------------------------------
	void SetFullColumnScrolling(BOOL bSet);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to remove all items and column in specified position.
	// Parameters:
	//     nIndex - Index of item to remove.
	//-----------------------------------------------------------------------
	void ReleaseItem(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if footer records are pinned to the last body row.
	// Parameters:
	//     bPin - TRUE to pin footer records to the last body row,
	//            FALSE to dock footer records to the footer.
	// Remarks:
	//     By default, footer records are docked to the footer.
	// See Also:
	//     IsFooterRowsVisible, ShowFooterRows
	//-----------------------------------------------------------------------
	void PinFooterRows(BOOL bPin = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if footer records are pinned to the last body row.
	// Returns:
	//     TRUE if footer records are pinned to the last body row,
	//     FALSE if footer records are docked to the footer.
	// See Also:
	//     PinFooterRows
	//-----------------------------------------------------------------------
	BOOL IsFooterRowsPinned() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if footer records are pinned to the last body row.
	//     This method is used only in printing mode.
	// Parameters:
	//     bPin - TRUE to pin footer records to the last body row,
	//            FALSE to dock footer records to the footer.
	// Remarks:
	//     By default, footer records are docked to the footer.
	// See Also:
	//     IsFooterRowsVisible, ShowFooterRows
	//-----------------------------------------------------------------------
	void PinFooterRowsPrinted(BOOL bPin = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if footer records are pinned to the last body row.
	//     This method is used only in printing mode.
	// Returns:
	//     TRUE if footer records are pinned to the last body row,
	//     FALSE if footer records are docked to the footer.
	// See Also:
	//     PinFooterRows
	//-----------------------------------------------------------------------
	BOOL IsFooterRowsPinnedPrinted() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to enable Markup for records.
	// Parameters:
	//     bEnable - TRUE to enable Markup.
	//-------------------------------------------------------------------------
	void EnableMarkup(BOOL bEnable = TRUE);

	//-------------------------------------------------------------------------
	// Summary:
	//     Gets the Markup context.
	// Returns:
	//     A pointer to the Markup context.
	//-------------------------------------------------------------------------
	CXTPMarkupContext* GetMarkupContext() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the image manager of the Grid control.
	// Returns:
	//     A pointer to the image manager of the Grid control.
	// See Also:
	//     SetImageManager
	//-----------------------------------------------------------------------
	CXTPImageManager* GetImageManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the image manager for the Grid control.
	// Parameters:
	//     pImageManager - Image manager to be set
	// Example:
	// <code>
	// CXTPImageManager* pImageManager = new CXTPImageManager();
	// pImageManager->SetIcons(IDR_MAINFRAME);
	// m_wndGrid.SetImageManager(pImageManager);
	// </code>
	// See Also:
	//     GetImageManager
	//-----------------------------------------------------------------------
	void SetImageManager(CXTPImageManager* pImageManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the image list for the paint manager.
	// Parameters:
	//     pImageList - Pointer to a CImageList object.
	// Remarks:
	//     Use this function to set up your own image list
	//     with set bitmaps that represent various states
	//     of rows and depict any other information.
	// Note:
	//     Recommended to use the SetImageManager/GetImageManager methods instead.
	// Example:
	// <code>
	// CImageList listIcons;
	// listIcons.Create(16, 16, ILC_COLOR24 | ILC_MASK, 0, 1));
	// CBitmap bmp;
	// // load bitmap by id
	// bmp.LoadBitmap(IDB_BMGRID);
	// ilIcons.Add(&bmp, RGB(255, 0, 255));
	// m_wndGrid.SetImageList(&lIcons);
	// </code>
	// See Also:
	//     GetImageManager
	//-----------------------------------------------------------------------
	void SetImageList(CImageList* pImageList);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the associated grid header object.
	// Remarks:
	//     Call this member function if you want to retrieve access
	//     to the grid header object properties and methods.
	// Returns:
	//     A pointer to the associated grid header object.
	// See Also:
	//     CXTPGridHeader overview
	//-----------------------------------------------------------------------
	CXTPGridHeader* GetGridHeader() const;

	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATED_IN_FAVOR(GetGridHeader)
	CXTPGridHeader* GetReportHeader() const;
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the associated grid header object.
	// Parameters:
	//     pGridHeader - Pointer to the grid header to be set.
	// Example:
	//     <code>m_wndGrid.SetGridHeader(new CMyGridHeader());</code>
	// See Also:
	//     CXTPGridHeader overview
	//-----------------------------------------------------------------------
	void SetGridHeader(CXTPGridHeader* pGridHeader);

	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATED_IN_FAVOR(SetGridHeader)
	void SetReportHeader(CXTPGridHeader* pGridHeader);
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the indentation, in pixels, for a tree view row
	//     of a specified depth level.
	// Parameters:
	//     nLevel - Tree depth level.
	// Returns:
	//     The indentation, in pixels, for a tree view row
	//     of the specified depth level.
	// See Also:
	//     Populate
	//-----------------------------------------------------------------------
	int GetIndent(int nLevel) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Notifies the parent control about an event that occurred.
	// Parameters:
	//     pRow       - Pointer to the row of the event if used.
	//     pItem      - Pointer to the item of the event if used.
	//     pColumn    - Pointer to the column of the event if used.
	//     nMessage   - Message to be sent to the parent window.
	//     pPoint     - Pointer to the point where the message
	//                  was sent from in client coordinates.
	//     nHyperlink - Hyperlink order number where the message was sent from
	//                  (or -1 if the message was not send from a hyperlink).
	// Remarks:
	//     Sends a message to the parent control in the form of a
	//     WM_NOTIFY message with a specific structure attached.
	// Returns:
	//     The result of the message processing; its value depends
	//     on the message that was sent (see CWnd::SendMessage).
	// See Also:
	//     CXTPGridControl overview, SendNotifyMessage
	//-----------------------------------------------------------------------
	LRESULT SendMessageToParent(CXTPGridRow* pRow, CXTPGridRecordItem* pItem,
								CXTPGridColumn* pColumn, UINT nMessage, CPoint* pPoint,
								int nHyperlink = -1) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sends a specified message to the window.
	// Parameters:
	//     nMessage - Message to be sent.
	//     pNMHDR   - Notify header.
	// Returns:
	//     Nonzero if successful, otherwise zero.
	//-----------------------------------------------------------------------
	LRESULT SendNotifyMessage(UINT nMessage, NMHDR* pNMHDR = NULL) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Collapses all rows.
	// Remarks:
	//     The CollapseAll method collapses all CXTPGridRow objects,
	//     including child rows, that are in the Grid control.
	// See Also:
	//     ExpandAll, CXTPGridRow::SetExpanded
	//-----------------------------------------------------------------------
	void CollapseAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Expands all rows.
	// Parameters:
	//     bRecursive - TRUE to recursively expand all levels of child rows,
	//                  FALSE to only expand one level of child rows.
	// Remarks:
	//     The ExpandAll method expands all CXTPGridRow objects,
	//     including child rows, that are in the Grid control.
	// See Also:
	//     CollapseAll, CXTPGridRow::SetExpanded
	//-----------------------------------------------------------------------
	void ExpandAll(BOOL bRecursive = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if multiple selections is enabled/disabled for the Grid control.
	// Returns:
	//     TRUE if multiple selections is enabled for the Grid control,
	//     FALSE if multiple selections is disabled for the Grid control.
	// See Also:
	//     SetMultipleSelection, GetSelectedRows
	//-----------------------------------------------------------------------
	BOOL IsMultipleSelection() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables multiple selections for the Grid control.
	// Parameters:
	//     bMultipleSelection - TRUE to enable multiple selections,
	//                          FALSE to disable multiple selections.
	// See Also:
	//     IsMultipleSelection, GetSelectedRows
	//-----------------------------------------------------------------------
	void SetMultipleSelection(BOOL bMultipleSelection);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if multi-selection mode is enabled/disabled for the Grid control.
	// Returns:
	//     TRUE if multi-selection mode is enabled for the control,
	//     FALSE if multi-selection mode is disabled for the control.
	// Remarks:
	//     If multi-selection mode is enabled, then VK_CTRL is always ON.
	// See Also:
	//     SetMultiSelectionMode
	//-----------------------------------------------------------------------
	BOOL IsMultiSelectionMode() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables multi-selection mode for the Grid control.
	// Parameters:
	//     bMultiSelectionMode - TRUE to enable multi-selection mode,
	//                           FALSE to disable multi-selection mode.
	// Remarks:
	//     If multi-selection mode is enabled, then VK_CTRL is always ON.
	// See Also:
	//     IsMultiSelectionMode
	//-----------------------------------------------------------------------
	void SetMultiSelectionMode(BOOL bMultiSelectionMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables showing tooltips for the Grid control window.
	// Parameters:
	//     bEnable - TRUE to enable showing tooltips,
	//               FALSE to disable showing tooltips.
	//-----------------------------------------------------------------------
	void EnableToolTips(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if groups are skipped when navigating the Grid control
	//     by using the Up and Down keys.
	// Parameters:
	//     bSkipFocus - TRUE to skip group headings and select the next non-group
	//                  heading row when navigating the Grid control by using
	//                  the Up and Down keys,
	//                  FALSE to cause all rows to be selected (including group
	//                  headings) when navigating the Grid control by using
	//                  the Up and Down keys.
	//-----------------------------------------------------------------------
	void SkipGroupsFocus(BOOL bSkipFocus);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if groups are skipped when navigating the Grid control
	//     by using the Up and Down keys.
	// Returns:
	//     TRUE if group headings are skipped and the next non-group heading row is
	//     selected when navigating the Grid control by using the Up and Down keys,
	//     FALSE if all rows are selected (including group headings) when navigating
	//     the Grid control by using the Up and Down keys.
	//-----------------------------------------------------------------------
	BOOL IsSkipGroupsFocusEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the indentation of the header.
	// Returns:
	//     The indentation of the header.
	//-----------------------------------------------------------------------
	int GetHeaderIndent() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Adjusts scrollbars depending on currently visible rows.
	//-----------------------------------------------------------------------
	virtual void AdjustScrollBars();

	//-----------------------------------------------------------------------
	// Summary:
	//     Adjusts main control areas depending on current control size.
	//-----------------------------------------------------------------------
	virtual void AdjustLayout();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the selected column.
	// Returns:
	//     A pointer to the selected column.
	//-----------------------------------------------------------------------
	CXTPGridColumn* GetFocusedColumn() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the selected column.
	// Parameters:
	//     pColumn - Column to be selected.
	// Returns:
	//     TRUE if the specified column was selected, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL SetFocusedColumn(CXTPGridColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the selection state of a row.
	// Parameters:
	//     index - Index of the row.
	//     state - Integer value denoting the state of the checkbox; must be one
	//             of the values defined by the XTPGridCheckState enumeration.
	//-----------------------------------------------------------------------
	void SetSelectionState(int index, int state);

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the visibility of focused rows should be ensured.
	// Parameters:
	//     bEnsureFocusedRowVisible - TRUE to ensure the visibility of the entire
	//                                row when a record item is focused.
	// See Also:
	//     IsEnsureFocusedRowVisible
	//-----------------------------------------------------------------------
	void EnsureFocusedRowVisible(BOOL bEnsureFocusedRowVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the visibility of focused rows is ensured.
	// Returns:
	//     TRUE if the visibility of focused rows is ensured, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsEnsureFocusedRowVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if each individual record item in a row should receive focus
	//     when an item in the row is clicked.
	// Parameters:
	//     bFocusSubItems - TRUE to highlight the entire row when a record item
	//                      is clicked excluding the item that was clicked,
	//                      FALSE to highlight the entire row when a record item
	//                      is clicked including the item that was clicked.
	// See Also:
	//     IsFocusSubItems
	//-----------------------------------------------------------------------
	void FocusSubItems(BOOL bFocusSubItems);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if each individual record item in a row receives focus
	//     when an item in the row is clicked.
	// Returns:
	//     TRUE if the entire row is highlighted when a record item is clicked
	//     excluding the item that was clicked,
	//     FALSE if the entire row is highlighted when a record item is clicked
	//     including the item that was clicked.
	// See Also:
	//     FocusSubItems
	//-----------------------------------------------------------------------
	BOOL IsFocusSubItems() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if a CXTPGridRecordItem should become editable after
	//     being single-clicked.
	// Parameters:
	//     bEditOnClick - TRUE to allow a CXTPGridRecordItem to become
	//                    editable after being single-clicked.
	// Remarks:
	//     The entire Grid control and/or the specific CXTPGridRecordItem
	//     must also be set to editable in order for a CXTPGridRecordItem
	//     to become editable after being single-clicked.
	// See Also:
	//     AllowEdit, IsAllowEdit, IsEditOnClick
	//-----------------------------------------------------------------------
	void EditOnClick(BOOL bEditOnClick);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a CXTPGridRecordItem becomes editable after
	//     being single-clicked.
	// Returns:
	//     TRUE if a CXTPGridRecordItem becomes editable after
	//     being single-clicked.
	// Remarks:
	//     The entire Grid control and/or the specific CXTPGridRecordItem
	//     must also be set to editable in order for a CXTPGridRecordItem
	//     to become editable after being single-clicked.
	// See Also:
	//     AllowEdit, IsAllowEdit, EditOnClick
	//-----------------------------------------------------------------------
	BOOL IsEditOnClick() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables the Delay Click Edit feature.
	// Parameters:
	//     bEditOnDelayClick - TRUE to enable the Delay Click Edit feature,
	//                         FALSE to disable the Delay Click Edit feature.
	//-----------------------------------------------------------------------
	void EditOnDelayClick(BOOL bEditOnDelayClick);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the Delay Click Edit feature is enabled/disabled.
	// Returns:
	//     TRUE if the Delay Click Edit feature is enabled,
	//     FALSE if the Delay Click Edit feature is disabled.
	//-----------------------------------------------------------------------
	BOOL IsEditOnDelayClick() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if a CXTPGridRecordItem should become editable after
	//     being double-clicked.
	// Parameters:
	//     bEditOnClick - TRUE to allow a CXTPGridRecordItem to become
	//                    editable after being double-clicked.
	// Remarks:
	//     The entire Grid control and/or the specific CXTPGridRecordItem
	//     must also be set to editable in order for a CXTPGridRecordItem
	//     to become editable after being double-clicked.
	// See Also:
	//     AllowEdit, IsAllowEdit, IsEditOnDoubleClick
	//-----------------------------------------------------------------------
	void EditOnDoubleClick(BOOL bEditOnClick);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a CXTPGridRecordItem becomes editable after
	//     being double-clicked.
	// Returns:
	//     TRUE if a CXTPGridRecordItem becomes editable after
	//     being double-clicked.
	// Remarks:
	//     The entire Grid control and/or the specific CXTPGridRecordItem
	//     must also be set to editable in order for a CXTPGridRecordItem
	//     to become editable after being double-clicked.
	// See Also:
	//     AllowEdit, IsAllowEdit, EditOnDoubleClick
	//-----------------------------------------------------------------------
	BOOL IsEditOnDoubleClick() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the index of the most recent row that was requested for editing.
	// Returns:
	//     The index of the most recent row that was requested for editing.
	//-----------------------------------------------------------------------
	int GetLastRqstEditRow() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the index of the most recent column that was requested for editing.
	// Returns:
	//     The index of the most recent column that was requested for editing.
	//-----------------------------------------------------------------------
	int GetLastRqstEditCol() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets an arbitrary index for both the most recent row and most recent column
	//     that was requested for editing.
	// Parameters:
	//     iRqstEditRow - Index to be set for the most recent row.
	//     iRqstEditCol - Index to be set for the most recent column.
	//-----------------------------------------------------------------------
	void SetLastRqstEdit(int iRqstEditRow, int iRqstEditCol);

	//-----------------------------------------------------------------------
	// Summary:
	//     Stops the last request timer used for delay edit.
	//-----------------------------------------------------------------------
	void EnsureStopLastRqstTimer();

	//-----------------------------------------------------------------------
	// Summary:
	//     Starts the last request timer used for delay edit.
	//-----------------------------------------------------------------------
	void StartLastRqstTimer();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the ID of the delay edit timer.
	// Returns:
	//     The ID of the delay edit timer.
	//-----------------------------------------------------------------------
	UINT_PTR GetDelayEditTimer() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Stops the delay edit timer.
	//-----------------------------------------------------------------------
	void EnsureStopDelayEditTimer();

	//-----------------------------------------------------------------------
	// Summary:
	//     Starts the delay edit timer.
	//-----------------------------------------------------------------------
	void StartDelayEditTimer();

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables auto-check mode for checkbox items.
	// Parameters:
	//     bAutoCheck - TRUE to enable auto-check mode, FALSE to disable auto-check mode.
	// Remarks:
	//     If auto-check mode is enabled, then the checkbox will become
	//     checked/unchecked automatically when the user clicks on the checkbox.
	// See Also:
	//     IsAutoCheckItems, CXTPGridRecordItem::OnClick, CXTPGridRecordItem::OnChar
	//-----------------------------------------------------------------------
	void SetAutoCheckItems(BOOL bAutoCheck);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if auto-check mode is enabled/disabled for checkbox items.
	// Remarks:
	//     If auto-check mode is enabled, then the checkbox will become
	//     checked/unchecked automatically when the user clicks on the checkbox.
	// Returns:
	//      TRUE if auto-check mode is enabled, FALSE if auto-check mode is disabled.
	// See Also:
	//     SetAutoCheckItems, CXTPGridRecordItem::OnClick, CXTPGridRecordItem::OnChar
	//-----------------------------------------------------------------------
	BOOL IsAutoCheckItems() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Edits a grid cell by focusing the specified cell and starting
	//     in-place editing for the control specified there.
	// Parameters:
	//     pItemArgs - Pointer to an XTP_GRIDRECORDITEM_ARGS structure
	//                 containing item arguments of the item to be edited.
	// Remarks:
	//     All editing options should be enabled in order for this method
	//     to be executed successfully.
	//     Call this method with NULL as its parameter to stop item edit.
	// See Also:
	//     AllowEdit, CXTPGridColumn::SetEditable
	//-----------------------------------------------------------------------
	void EditItem(XTP_GRIDRECORDITEM_ARGS* pItemArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the in-place edit control.
	// Returns:
	//     A pointer to the in-place edit control.
	//-----------------------------------------------------------------------
	CXTPGridInplaceEdit* GetInplaceEdit() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the collection of in-place buttons of the Grid control.
	// Returns:
	//     A pointer to the collection of in-place buttons of the Grid control.
	//-----------------------------------------------------------------------
	CXTPGridInplaceButtons* GetInplaceButtons() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the in-place list of Grid controls.
	// Returns:
	//     A pointer to the in-place list of Grid controls.
	//-----------------------------------------------------------------------
	CXTPGridInplaceList* GetInplaceList() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the Grid control has focus.
	// Returns:
	//     TRUE if the Grid control has focus, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL HasFocus() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the active item (i.e. the item that has focus in edit mode).
	// Returns:
	//     A pointer to the active item if in edit mode, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPGridRecordItem* GetActiveItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the focused item.
	// Returns:
	//     A pointer to the focused item.
	//-----------------------------------------------------------------------
	CXTPGridRecordItem* GetFocusedRecordItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the indentation before the text of each child node in a
	//     hierarchical tree structure.
	// Parameters:
	//     nIndent - Indentation to be set.
	//-----------------------------------------------------------------------
	void SetTreeIndent(int nIndent);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to enable virtual mode of the control
	// Parameters:
	//     pVirtualRecord - record to be used as virtual for all rows.
	//     nCount         - Count of virtual records.
	//     nFields        - number of fields to assign);
	// Example:
	// <code>
	// class CVirtualRecord : public CXTPGridRecord
	// {
	// public:
	//     CVirtualRecord()
	//     {
	//         AddItem(new CXTPGridRecordItem());
	//         AddItem(new CXTPGridRecordItem());
	//         AddItem(new CXTPGridRecordItem());
	//     }
	//
	//     void GetItemMetrics (XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
	//     XTP_GRIDRECORDITEM_METRICS* pItemMetrics)
	//     {
	//         // Draw virtual record
	//     }
	// }
	// ...
	// m_wndGrid.SetVirtualMode(new CVirtualRecord(), 540);
	// </code>
	// See Also:
	//     IsVirtualMode
	//-----------------------------------------------------------------------
	void SetVirtualMode(CXTPGridRecord* pVirtualRecord, int nCount, int nFields = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if virtual mode is enabled/disabled.
	// Returns;
	//     TRUE if virtual mode is enabled, FALSE if virtual mode is disabled.
	// See Also:
	//     SetVirtualMode
	//-----------------------------------------------------------------------
	BOOL IsVirtualMode() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the filter text of the control.
	// Returns:
	//     The filter text of the control.
	// See Also:
	//     SetFilterText
	//-----------------------------------------------------------------------
	virtual CString GetFilterText();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the filter text for the control.
	// Parameters:
	//     strFilterText - Filter text to be set.
	// Remarks:
	//     You must call the Populate method to update rows.
	// See Also:
	//     GetFilterText, Populate
	//-----------------------------------------------------------------------
	virtual void SetFilterText(LPCTSTR strFilterText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if search filter text is enabled/disabled in hidden columns.
	// Remarks:
	//     This option is disabled by default.
	// Returns;
	//     TRUE if search filter text is enabled in hidden columns,
	//     FALSE if search filter text is disabled in hidden columns.
	// See Also:
	//     SetFilterHiddenColumns, GetFilterText, SetFilterText
	//-----------------------------------------------------------------------
	virtual BOOL IsFilterHiddenColumns() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables search filter text in hidden columns.
	// Parameters:
	//     bFilterHidden - TRUE to enable search filter text in hidden columns,
	//                     FALSE to disable search filter text in hidden columns.
	// Remarks:
	//     This option is disabled by default.
	//     You must call the Populate method to update rows.
	// See Also:
	//     IsFilterHiddenColumns, GetFilterText, SetFilterText, Populate
	//-----------------------------------------------------------------------
	virtual void SetFilterHiddenColumns(BOOL bFilterHidden);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the filtering mode for the record tree.
	// Parameters:
	//     nMode - Filtering mode to be set; must be one of the values
	//             defined by the XTPGridFilterMode enumeration.
	// See Also:
	//     GetRecordsTreeFilterMode, XTPGridFilterMode
	//-----------------------------------------------------------------------
	void SetRecordsTreeFilterMode(int nMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the filtering mode of the record tree.
	// Returns:
	//     The filtering mode of the record tree; one of the values
	//     defined by the XTPGridFilterMode enumeration.
	// See Also:
	//     SetRecordsTreeFilterMode, XTPGridFilterMode
	//-----------------------------------------------------------------------
	int GetRecordsTreeFilterMode() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Registers the window class if it has not already been registered.
	// Parameters:
	//     hInstance - Instance of the resource where the control is located.
	// Returns:
	//     TRUE if the window class was successfully registered.
	//-----------------------------------------------------------------------
	BOOL RegisterWindowClass(HINSTANCE hInstance = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the Cut operation is possible in the current context
	//     (i.e. one or more records are selected).
	// Returns:
	//     TRUE if the Cut operation is possible in the current context, otherwise FALSE.
	// See Also:
	//     CanPaste, Cut, Copy, Paste.
	//-----------------------------------------------------------------------
	virtual BOOL CanCut();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the Copy operation is posssible in the current context
	//     (i.e. one or more records are selected).
	// Returns:
	//     TRUE if the Copy operation is possible in the current context, otherwise FALSE.
	// See Also:
	//     CanPaste, Cut, Copy, Paste.
	//-----------------------------------------------------------------------
	virtual BOOL CanCopy();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the Paste operation is possible in the current context
	//     (i.e. binary and/or text data exists in the clipboard).
	// Returns:
	//     TRUE if the Paste operation is possible in the current context, otherwise FALSE.
	// See Also:
	//     CanCut, CanCopy, Cut, Copy, Paste.
	//-----------------------------------------------------------------------
	virtual BOOL CanPaste();

	//-----------------------------------------------------------------------
	// Summary:
	//     Copies the selected record data (in the binary and/or text data formats)
	//     to the clipboard and deletes the selected record data.
	// See Also:
	//     CanCut, CanCopy, CanPaste, Copy, Paste,
	//     OnBeforeCopyToText, OnBeforePasteFromText, OnBeforePaste.
	//-----------------------------------------------------------------------
	virtual void Cut();

	//-----------------------------------------------------------------------
	// Summary:
	//     Copies the selected record data (in the binary and/or text data formats)
	//     to the clipboard.
	// See Also:
	//     CanCut, CanCopy, CanPaste, Cut, Paste,
	//     OnBeforeCopyToText, OnBeforePasteFromText, OnBeforePaste.
	//-----------------------------------------------------------------------
	virtual void Copy();

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads records from the clipboard and adds them to the records collection
	//     (the binary data format is used rather than the text data format).
	// See Also:
	//     CanCut, CanCopy, CanPaste, Cut, Copy,
	//     OnBeforeCopyToText, OnBeforePasteFromText, OnBeforePaste.
	//-----------------------------------------------------------------------
	virtual void Paste();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the in-place edit control is active (i.e. visible and focused).
	// Returns:
	//     TRUE if the in-place edit control is active, otherwise FALSE.
	// See Also:
	//     GetInplaceEdit.
	//-----------------------------------------------------------------------
	virtual BOOL IsEditMode();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the compare function used to compare (and sort) rows.
	// Parameters:
	//     pCompareFunc - XTPGridRowsCompareFunc function pointer to be used
	//                    to compare rows, or NULL to use the default function.
	// Remarks:
	//     This method uses Visual C++ run-time library (MSVCRT)
	//     implementation of the quick-sort function, qsort, for sorting
	//     stored CXTPGridRow objects.
	//     If pCompareFunc = NULL, then the default compare function is used.
	//     Call the Populate() method to re-sort items.
	// See Also:
	//     SortRows, CXTPGridRows::SortEx, CXTPGridRows::Sort,
	//     CXTPGridRows::XTPGridRowsCompareFunc
	//-----------------------------------------------------------------------
	virtual void SetRowsCompareFunc(XTPGridRowsCompareFunc pCompareFunc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the sort order is applied to record children.
	// Returns:
	//     TRUE if the sort order is applied to record children, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsSortRecordChilds() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the sort order should be applied to record children.
	// Parameters:
	//     bSortRecordChildren - TRUE to apply the sort order to record children,
	//                           FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetSortRecordChilds(BOOL bSortRecordChildren);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the horizontal scrolling step.
	// Returns:
	//     The horizontal scrolling step, in pixels.
	//-----------------------------------------------------------------------
	int GetHScrollStep() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the horizontal scrolling step.
	// Parameters:
	//     nStep - Horizontal scrolling step to be set, in pixels.
	//-----------------------------------------------------------------------
	void SetHScrollStep(int nStep);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the vertical scrolling timer resolution.
	// Returns:
	//     The vertical scrolling timer resolution, in milliseconds.
	//-----------------------------------------------------------------------
	UINT GetAutoVScrollTimerResolution() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the vertical scrolling timer resolution.
	// Parameters:
	//     nNewTimerResoution - Vertical scrolling timer resolution, in milliseconds.
	// See also:
	//     CWnd::SetTimer
	//-----------------------------------------------------------------------
	void SetAutoVScrollTimerResolution(UINT nNewTimerResolution);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if preview editing is enabled/disabled.
	// Returns:
	//     TRUE if preview editing is enabled, FALSE if preview editing is disabled.
	//-----------------------------------------------------------------------
	BOOL IsPreviewAllowEdit() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables preview editing.
	// Parameter:
	//     bAllowEdit - TRUE to enable preview editing, FALSE to disable preview editing.
	//-----------------------------------------------------------------------
	void PreviewAllowEdit(BOOL bAllowEdit);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the user's ability to collapse/expand rows is locked.
	// Returns:
	//     TRUE if the user's ability to collapse/expand rows is locked.
	//-----------------------------------------------------------------------
	BOOL IsLockExpand() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the user's ability to collapse/expand rows should be locked.
	// Parameter:
	//     bLockExpand - TRUE to lock the user's ability to collapse/expand rows.
	//-----------------------------------------------------------------------
	void LockExpand(BOOL bLockExpand);

	//-----------------------------------------------------------------------
	// Summary:
	//     Recalculates and redraws rows.
	// Parameter:
	//     bAll - TRUE to recalculate and redraw all rows,
	//            FALSE to recalculate and redraw only the focused row.
	//-----------------------------------------------------------------------
	void Recalc(BOOL bAll = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables double-buffering.
	// Parameter:
	//     bEnable - TRUE to enable double-buffering, FALSE to disable double-buffering.
	//-----------------------------------------------------------------------
	void EnableDoubleBuffering(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if double-buffering is enabled/disabled.
	// Returns:
	//     TRUE if double buffering is enabled, FALSE if double-buffering is disabled.
	// See Also:
	//     EnableDoubleBuffering
	//-----------------------------------------------------------------------
	BOOL IsDoubleBuffering() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if initial selection is enabled/disabled.
	// Remarks:
	//     If initial selection is enabled, then the first row in the grid
	//     will become selected when the grid is populated.
	// Returns:
	//     TRUE if initial selection is enabled, FALSE if initial selection is disabled.
	// See Also:
	//     SelectionEnable
	//-----------------------------------------------------------------------
	BOOL IsInitialSelectionEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables initial selection.
	// Parameters:
	//     bEnable - TRUE to enable initial selection,
	//               FALSE to disable initial selection.
	// Remarks:
	//     If initial selection is enabled, then the first row in the grid
	//     will become selected when the grid is populated.
	// See Also:
	//     IsSelectionEnabled
	//-----------------------------------------------------------------------
	void InitialSelectionEnable(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if row focus is shown.
	// Returns:
	//     TRUE if row focus is shown, otherwise FALSE.
	// See Also:
	//     ShowRowFocus
	//-----------------------------------------------------------------------
	BOOL IsRowFocusVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if row focus should be shown.
	// Parameters:
	//     bShow - TRUE to show row focus, FALSE otherwise.
	// See Also:
	//     IsRowFocusVisible
	//-----------------------------------------------------------------------
	void ShowRowFocus(BOOL bShow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the watermark alignment.
	// Returns:
	//     The watermark alignment; one of the values defined
	//     by the XTPGridWatermarkAlignment enumeration.
	// See Also:
	//     XTPGridWatermarkAlignment, SetWatermarkAlignment.
	//-----------------------------------------------------------------------
	int GetWatermarkAlignment() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the watermark alignment.
	// Parameters:
	//     nWatermarkAlignment - Watermark alignment to be set; must be one of the values
	//                           defined by the XTPGridWatermarkAlignment enumeration.
	// See Also:
	//     XTPGridWatermarkAlignment, GetWatermarkAlignment.
	//-----------------------------------------------------------------------
	void SetWatermarkAlignment(int nWatermarkAlignment);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a watermark.
	// Parameters:
	//     pDC          - Pointer to the device context.
	//     rcWatermark  - Rectangular bounds of the watermark.
	//     rcClient     - Rectangular bounds of the client area.
	//-----------------------------------------------------------------------
	void DrawWatermark(CDC* pDC, CRect rcWatermark, CRect rcClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves a specified grid element rectangle.
	// Parameters:
	//     nElement - Grid element rectangle to retrieve; must be one of
	//                the values defined by the XTPGridElementRect enumeration.
	// Returns:
	//     The specified grid element rectangle.
	// See Also:
	//     XTPGridElementRect
	//-----------------------------------------------------------------------
	CRect GetElementRect(XTPGridElementRect nElement) const;

	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATED_IN_FAVOR_("GetElementRect with XTPGridElementRect")
	CRect GetElementRect(int nElement) const;
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the grid data manager.
	// Returns:
	//     A pointer to the grid data manager.
	//-----------------------------------------------------------------------
	CXTPGridDataManager* GetDataManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the control mouse mode.
	// Parameters:
	//     nMode - Mouse mode to be set; must be one of the values
	//             defined by the XTPGridMouseMode enumeration.
	// See Also:
	//     XTPGridMouseMode overview, GetMouseMode
	//-----------------------------------------------------------------------
	void SetMouseMode(XTPGridMouseMode nMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the control mouse mode.
	// Returns:
	//     The control mouse mode; one of the values defined
	//     by the XTPGridMouseMode enumeration.
	// See Also:
	//     XTPGridMouseMode overview, SetMouseMode
	//-----------------------------------------------------------------------
	XTPGridMouseMode GetMouseMode() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the watermark bitmap to be shown in the Grid control background.
	// Returns:
	//     The handle of the watermark bitmap.
	//-----------------------------------------------------------------------
	virtual HBITMAP GetWatermarkBitmap() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the watermark bitmap to be shown in the Grid control background.
	// Parameters:
	//     hBitmap      - Handle of the watermark bitmap.
	//     Transparency - Transparency value.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL SetWatermarkBitmap(HBITMAP hBitmap, BYTE Transparency);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the watermark bitmap to be shown in the Grid control background.
	// Parameters:
	//     szPath       - Path to the bitmap file of the watermark bitmap.
	//     Transparency - Transparency value.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL SetWatermarkBitmap(LPCTSTR szPath, BYTE Transparency);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the next focusable column.
	// Parameters:
	//     pRow         - Pointer to a CXTPGridRow object.
	//     nColumnIndex - Column index.
	//     nDirection   - Positive integer to get the next focusable column
	//                    to the right of nColumnIndex,
	//                    Negative integer to get the next focusable column
	//                    to the left of nColumnIndex.
	// Returns:
	//     A pointer to the next focusable column.
	//-----------------------------------------------------------------------
	virtual CXTPGridColumn* GetNextFocusableColumn(CXTPGridRow* pRow, int nColumnIndex,
												   int nDirection);

	CXTPGridBehavior* GetBehavior() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if item icons should be shown while the item is being edited.
	// Parameters:
	//     bShow - TRUE to show item icons, FALSE otherwise.
	//-----------------------------------------------------------------------
	void ShowIconWhenEditing(BOOL bShow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if item icons are shown while the item is being edited.
	// Returns:
	//     TRUE if item icons are shown while the item is being edited, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsShowIconWhenEditing();

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables Fast Deselect mode (like in Windows Explorer).
	// Parameters:
	//     bFastDeselect - TRUE to enable Fast Deselect mode,
	//                     FALSE to disable Fast Deselect mode.
	//-----------------------------------------------------------------------
	void SetFastDeselectMode(BOOL bFastDeselect = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets a column to be an icon view column.
	// Parameters:
	//     pColumn - Pointer to a CXTPGridColumn object.
	//-----------------------------------------------------------------------
	void SetIconColumn(CXTPGridColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a column as an icon view column and specifies if the
	//     column may be reused as a number column in the grid view.
	// Parameters:
	//     bUseColumnForNum - TRUE to reuse the column as a
	//                        number column in the grid view.
	//     nWidth           - Width for the number column.
	//-----------------------------------------------------------------------
	void CreateIconColumn(BOOL bUseColumnForNum = FALSE, int nWidth = 40);

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a column as an icon view column and specifies if the
	//     column may be reused as a number column in the grid view.
	// Parameters:
	//     nCol             - Column index to use as the icon column property.
	//     nIcon            - Icon index to use in icon view.
	//     bUseColumnForNum - TRUE to reuse the column as a
	//                        number column in the grid view.
	//     nWidth           - Width for the number column.
	// Remarks:
	//     This can be used for non-virtual mode.
	//-----------------------------------------------------------------------
	void AssignIconViewPropNumAndIconNum(int nCol = 0, int nIcon = 0, BOOL bUseColumnForNum = FALSE,
										 int nWidth = 20);

	//-----------------------------------------------------------------------
	// Summary:
	//     Toggles between Icon View mode and Grid View mode.
	// Parameters:
	//     bIconView - TRUE for Icon View mode,
	//                 FALSE for Grid View mode.
	//-----------------------------------------------------------------------
	void SetIconView(BOOL bIconView = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if Icon View mode is active.
	// Returns:
	//     TRUE if Icon View mode is active.
	//-----------------------------------------------------------------------
	BOOL IsIconView() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of rows per line.
	// Returns:
	//     The number of rows per line.
	//-----------------------------------------------------------------------
	int GetRowsPerLine() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of rows on a line of abitrary width.
	// Parameters:
	//     iTotalWidth - Width of the line.
	// Returns:
	//     The number of rows on the line.
	//-----------------------------------------------------------------------
	int GetNumRowsOnLine(int iTotalWidth) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the grid navigator.
	// Parameters:
	//     pNagivator - Pointer to the grid navigator to be set.
	//-----------------------------------------------------------------------
	void SetNavigator(CXTPGridNavigator* pNavigator);

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the row number should be shown.
	// Parameters:
	//     bSet - TRUE to show the row number, FALSE otherwise.
	//-----------------------------------------------------------------------
	void ShowRowNumber(BOOL bSet);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the row number is shown.
	// Returns:
	//     TRUE if the row number is shown, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsShowRowNumber();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the text for a specified cell.
	// Parameters:
	//     row   - Row index.
	//     col   - Column index.
	//     sText - Text string to be set.
	// Returns:
	//      TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL SetCellText(int row, int col, CString sText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the formula for a specified cell.
	// Parameters:
	//     row      - Row index.
	//     col      - Column index.
	//     sFormula - Formula to be set.
	// Returns:
	//      TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL SetCellFormula(int row, int col, CString sFormula);

	//-----------------------------------------------------------------------
	// Summary:
	//     Deselects all group rows.
	//-----------------------------------------------------------------------
	void UnselectGroupRows();

	//-----------------------------------------------------------------------
	// Summary:
	//     Releases all sorted items and clears memory.
	//-----------------------------------------------------------------------
	void ReleaseSorted();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the horizontal scroll position.
	// Returns:
	//     The horizontal scroll position.
	//-----------------------------------------------------------------------
	int GetLeftOffset() const;

	// assessors for m_bDragMode
	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if Grid control items are in drag mode.
	// Returns:
	//     TRUE if Grid control items are in drag mode, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsDragMode() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the horizontal sensitivity of Drag'n'Drop operations.
	// Returns:
	//     The horizontal sensitivity of Drag'n'Drop operations, in pixels.
	// See Also:
	//     SetDragSensitivityX, GetDragSensitivityY, SetDragSensitivityY
	//-----------------------------------------------------------------------
	int GetDragSensitivityX() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the horizontal sensitivity of Drag'n'Drop operations.
	// Parameters:
	//      nDragSensitivityX - Horizontal sensitivity for Drag'n'Drop operations,
	//                          in pixels. The value must not be scaled to DPI
	//                          and must be in the range of 1-10000.
	// See Also:
	//     GetDragSensitivityX, GetDragSensitivityY, SetDragSensitivityY
	//-----------------------------------------------------------------------
	void SetDragSensitivityX(int nDragSensitivityX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the vertical sensitivity of Drag'n'Drop operations.
	// Returns:
	//     The vertical sensitivity of Drag'n'Drop operations, in pixels.
	// See Also:
	//     GetDragSensitivityX, SetDragSensitivityX, SetDragSensitivityY
	//-----------------------------------------------------------------------
	int GetDragSensitivityY() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the vertical sensitivity of Drag'n'Drop operation.
	// Parameters:
	//      nDragSensitivityY - Vertical sensitivity for Drag'n'Drop operations,
	//                          in pixels. The value must not be scaled to DPI
	//                          and must be in the range of 1-10000.
	// See Also:
	//     GetDragSensitivityX, SetDragSensitivityX, GetDragSensitivityY
	//-----------------------------------------------------------------------
	void SetDragSensitivityY(int nDragSensitivityY);

	//-----------------------------------------------------------------------
	// Summary:
	//     Resets the icon view column to its default values.
	// Remarks:
	//     This method should be called in the event that the
	//     icon view column is destroyed.
	//-----------------------------------------------------------------------
	void SetIconViewToDefaults();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the scroll mode.
	// Parameters:
	//     orientation - Orientation of the scroll mode to get; must be one of the
	//                   values defined by the XTPGridOrientation enumeration.
	// Returns:
	//     The scroll mode; one of the values defined by the
	//     XTPGridScrollMode enumeration.
	//-----------------------------------------------------------------------
	XTPGridScrollMode GetScrollMode(XTPGridOrientation orientation) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the scroll mode.
	// Parameters:
	//     orientation - Orientation of the scroll mode to set; must be one of the
	//                   values defined by the XTPGridOrientation enumeration.
	//     scrollMode  - Scroll mode to be set; must be one of the
	//                   values defined by the XTPGridScrollMode enumeration.
	//-----------------------------------------------------------------------
	void SetScrollMode(XTPGridOrientation orientation, XTPGridScrollMode scrollMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the sections collection.
	// Returns:
	//     A pointer to the sections collection.
	//-----------------------------------------------------------------------
	CXTPGridSections* GetSections() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if cell merging is allowed.
	// Returns:
	//     TRUE if cell merging is allowed, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsCellMergingAllowed() const;

#	if XTP_GRID_DEPRECATED()

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the grid drawing style.
	// Parameters:
	//     bvertical - Determines the grid drawing style to get;
	//                 TRUE to get the vertical grid drawing style,
	//                 FALSE to get the horizontal grid drawing style.
	// Returns:
	//     The grid drawing style; one of the values defined by the
	//     XTPGridLineStyle enumeration.
	// See Also:
	//     XTPGridGridStyle overview, SetGridStyle, SetGridColor
	//-----------------------------------------------------------------------
	XTPGridGridStyle GetGridStyle(BOOL bVertical) const;

	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATED_IN_FAVOR(GetGridStyle)
	XTPGridGridStyle GetReportStyle(BOOL bVertical) const;
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the grid drawing style.
	// Parameters:
	//     bvertical - Determines the grid drawing style to set;
	//                 TRUE to get the vertical grid drawing style,
	//                 FALSE to get the horizontal grid drawing style.
	//     gridStyle - Grid style to be set; can be
	//                 any of the values listed in the Remarks section.
	// Remarks:
	//     Grid drawing style can be one of the following:
	//          * <b>xtpGridGridNoLines</b>: Empty line.
	//          * <b>xtpGridGridSmallDots</b>: Line drawn by small dots.
	//          * <b>xtpGridGridLargeDots</b>: Line drawn by large dots.
	//          * <b>xtpGridGridDashes</b>: Line drawn by dashes.
	//          * <b>xtpGridGridSolid</b>: Draws solid line.
	//
	// See Also:
	//     XTPGridGridStyle overview, GetGridStyle, SetGridColor
	//-----------------------------------------------------------------------
	void SetGridStyle(BOOL bVertical, XTPGridGridStyle gridStyle);

	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATED_IN_FAVOR(SetGridStyle)
	void SetReportStyle(BOOL bVertical, XTPGridGridStyle gridStyle);
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the collection of data records.
	// Returns:
	//     A pointer to the collection of data records.
	// Remarks:
	//     Use this member function to retrieve access to the collection
	//     of grid data records. You may then perform standard operations
	//     on the collection (e.g. adding, removing, etc.).
	// See Also:
	//     CXTPGridRecords overview, AddRecord
	//-----------------------------------------------------------------------
	CXTPGridRecords* GetRecords() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the collection of header records.
	// Returns:
	//     A pointer to the collection of header records.
	// Remarks:
	//     Use this member function to retrieve access to the collection
	//     of grid header records. You may then perform standard operations
	//     on the collection (e.g. adding, removing, etc.).
	// See Also:
	//     CXTPGridRecords overview, AddRecord
	//-----------------------------------------------------------------------
	CXTPGridRecords* GetHeaderRecords() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the collection of footer records.
	// Returns:
	//     A pointer to the collection of footer records.
	// Remarks:
	//     Use this member function to retrieve access to the collection
	//     of grid footer records. You may then perform standard operations
	//     on the collection (e.g. adding, removing, etc.).
	// See Also:
	//     CXTPGridRecords overview, AddRecord
	//-----------------------------------------------------------------------
	CXTPGridRecords* GetFooterRecords() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the collection of grid rows.
	// Remarks:
	//     Use this member function to retrieve access to the collection
	//     of grid rows, representing the current control view.
	//     Note that the rows collection could be rebuilt automatically
	//     by executing the Populate method.
	// Returns:
	//     A pointer to the collection of grid rows.
	// Example:
	//     See example for the CXTPGridControl::BeginUpdate method.
	// See Also:
	//     CXTPGridRows overview, Populate
	//-----------------------------------------------------------------------
	CXTPGridRows* GetRows() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the collection of grid header rows.
	// Remarks:
	//     Use this member function to retrieve access to the collection
	//     of grid header rows, representing the current control view.
	//     Note that the rows collection could be rebuilt automatically
	//     by executing the Populate method.
	// Returns:
	//     A pointer to the collection of grid header rows.
	// See Also:
	//     CXTPGridRows overview, Populate
	//-----------------------------------------------------------------------
	CXTPGridRows* GetHeaderRows() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the collection of grid footer rows.
	// Remarks:
	//     Use this member function to retrieve access to the collection
	//     of grid footer rows, representing the current control view.
	//     Note that the rows collection could be rebuilt automatically
	//     by executing the Populate method.
	// Returns:
	//     A pointer to the collection of grid footer rows.
	// See Also:
	//     CXTPGridRows overview, Populate
	//-----------------------------------------------------------------------
	CXTPGridRows* GetFooterRows() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the grid bounding rectangle.
	// Returns:
	//     The grid bounding rectangle.
	//-----------------------------------------------------------------------
	CRect GetGridRectangle() const;

	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATED_IN_FAVOR(GetGridRectangle)
	CRect GetReportRectangle() const;
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the header records bounding rectangle.
	// Returns:
	//     The header records bounding rectangle.
	//-----------------------------------------------------------------------
	CRect GetHeaderRowsRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the footer records bounding rectangle.
	// Returns:
	//     The footer records bounding rectangle.
	//-----------------------------------------------------------------------
	CRect GetFooterRowsRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the user is allowed to move the selection to header rows.
	// Parameters:
	//     bAllowAccess - TRUE to allow the user to move the selection to header rows,
	//                    FALSE otherwise.
	// See Also:
	//     IsHeaderRowsAllowAccess
	//-----------------------------------------------------------------------
	void HeaderRowsAllowAccess(BOOL bAllowAccess);

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the user is allowed to move the selection to footer rows.
	// Parameters:
	//     bAllowAccess - TRUE to allow the user to move the selection to footer rows,
	//                    FALSE otherwise.
	// See Also:
	//     IsFooterRowsAllowAccess
	//-----------------------------------------------------------------------
	void FooterRowsAllowAccess(BOOL bAllowAccess);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the user is allowed to move the selection to header rows.
	// Returns:
	//     TRUE if the user is allowed to move the selection to header rows,
	//     otherwise FALSE.
	// See Also:
	//     HeaderRowsAllowAccess.
	//-----------------------------------------------------------------------
	BOOL IsHeaderRowsAllowAccess() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the user is allowed to move the selection to footer rows.
	// Returns:
	//     TRUE if the user is allowed to move the selection to footer rows,
	//     otherwise FALSE.
	// See Also:
	//     FooterRowsAllowAccess.
	//-----------------------------------------------------------------------
	BOOL IsFooterRowsAllowAccess() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if rows selection is enabled/disabled.
	// Returns:
	//     TRUE if rows selection is enabled, FALSE if rows selection is disabled.
	// See Also:
	//     SelectionEnable
	//-----------------------------------------------------------------------
	BOOL IsSelectionEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables rows selection.
	// Parameters:
	//     bEnable - TRUE to enable rows selection, FALSE to disable rows selection.
	// See Also:
	//     IsSelectionEnabled
	//-----------------------------------------------------------------------
	void SelectionEnable(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables header rows selection.
	// Parameters:
	//     bEnable - TRUE to enable header rows selection,
	//               FALSE to disable header rows selection.
	// See Also:
	//     IsHeaderRowsSelectionEnabled
	//-----------------------------------------------------------------------
	void HeaderRowsEnableSelection(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if header rows selection is enabled/disabled.
	// Returns:
	//     TRUE if header rows selection is enabled,
	//     FALSE if header rows selection is disabled.
	// See Also:
	//     HeaderRowsEnableSelection.
	//-----------------------------------------------------------------------
	BOOL IsHeaderRowsSelectionEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables footer rows selection.
	// Parameters:
	//     bEnable - TRUE to enable footer rows selection,
	//               FALSE to disable footer rows selection.
	// See Also:
	//     IsFooterRowsSelectionEnabled
	//-----------------------------------------------------------------------
	void FooterRowsEnableSelection(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if footer rows selection is enabled/disabled.
	// Returns:
	//     TRUE if footer rows selection is enabled,
	//     FALSE if footer rows selection is disabled.
	// See Also:
	//     FooterRowsEnableSelection.
	//-----------------------------------------------------------------------
	BOOL IsFooterRowsSelectionEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the text in all CXTPGridRecordItem(s) can be edited.
	// Parameters:
	//     bAllowEdit - TRUE to allow the text in CXTPGridRecordItem(s)
	//                  to be edited, FALSE otherwise.
	// Remarks:
	//     If bAllowEdit is set to TRUE, then an edit box will be added that
	//     allows for CXTPGridRecordItem(s) to be editable.
	//     If bAllowEdit is set to FALSE, then the edit box that allows for
	//     CXTPGridRecordItem(s) to be editable will be removed.
	// See Also:
	//     IsAllowEdit, EditOnClick, IsEditOnClick
	//-----------------------------------------------------------------------
	void AllowEdit(BOOL bAllowEdit);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the text in all CXTPGridRecordItem(s) can be edited.
	// Returns:
	//     TRUE if the text in all CXTPGridRecordItem(s) can be edited,
	//     otherwise FALSE.
	// See Also:
	//     AllowEdit, EditOnClick, IsEditOnClick
	//-----------------------------------------------------------------------
	BOOL IsAllowEdit() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the text in all CXTPGridRecordItem(s) can be edited
	//     (for header records only).
	// Parameters:
	//     bAllowEdit - TRUE to allow the text in CXTPGridRecordItem(s)
	//                  to be edited, FALSE otherwise.
	// Remarks:
	//     If bAllowEdit is set to TRUE, then an edit box will be added that
	//     allows for CXTPGridRecordItem(s) to be editable.
	//     If bAllowEdit is set to FALSE, then the edit box that allows for
	//     CXTPGridRecordItem(s) to be editable will be removed.
	// See Also:
	//     IsHeaderRowsAllowEdit, EditOnClick, IsEditOnClick
	//-----------------------------------------------------------------------
	void HeaderRowsAllowEdit(BOOL bAllowEdit);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the text in all CXTPGridRecordItem(s) can be edited
	//     (for header records only).
	// Returns:
	//     TRUE if the text in all CXTPGridRecordItem(s) can be edited,
	//     otherwise FALSE.
	// See Also:
	//     HeaderRowsAllowEdit, EditOnClick, IsEditOnClick
	//-----------------------------------------------------------------------
	BOOL IsHeaderRowsAllowEdit() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the text in all CXTPGridRecordItem(s) can be edited
	//     (for footer records only).
	// Parameters:
	//     bAllowEdit - TRUE to allow the text in CXTPGridRecordItem(s)
	//                  to be edited, FALSE otherwise.
	// Remarks:
	//     If bAllowEdit is set to TRUE, then an edit box will be added that
	//     allows for CXTPGridRecordItem(s) to be editable.
	//     If bAllowEdit is set to FALSE, then the edit box that allows for
	//     CXTPGridRecordItem(s) to be editable will be removed.
	// See Also:
	//     IsFooterRowsAllowEdit, EditOnClick, IsEditOnClick
	//-----------------------------------------------------------------------
	void FooterRowsAllowEdit(BOOL bAllowEdit);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the text in all CXTPGridRecordItem(s) can be edited
	//     (for footer records only).
	// Returns:
	//     TRUE if the text in all CXTPGridRecordItem(s) can be edited,
	//     otherwise FALSE.
	// See Also:
	//     FooterRowsAllowEdit, EditOnClick, IsEditOnClick
	//-----------------------------------------------------------------------
	BOOL IsFooterRowsAllowEdit() const;

	//////////////////////////////////////////////////////////////////////////
	// Allow Group
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables row grouping.
	// Parameters:
	//     bAllowGroup - TRUE to enable row grouping,
	//                   FALSE to disable row grouping.
	// See Also:
	//     IsAllowGroup
	//-----------------------------------------------------------------------
	void AllowGroup(BOOL bAllowGroup);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if row grouping is enabled/disabled.
	// Returns:
	//     TRUE if row grouping is enabled, otherwise FALSE.
	// See Also:
	//     AllowGroup
	//-----------------------------------------------------------------------
	BOOL IsAllowGroup() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables header row grouping.
	// Parameters:
	//     bAllowGroup - TRUE to enable header row grouping,
	//                   FALSE to disable header row grouping.
	// See Also:
	//     IsHeaderRowsAllowGroup
	//-----------------------------------------------------------------------
	void HeaderRowsAllowGroup(BOOL bAllowGroup);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if header row grouping is enabled/disabled.
	// Returns:
	//     TRUE if header row grouping is enabled, otherwise FALSE.
	// See Also:
	//     HeaderRowsAllowGroup
	//-----------------------------------------------------------------------
	BOOL IsHeaderRowsAllowGroup() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables footer row grouping.
	// Parameters:
	//     bAllowGroup - TRUE to enable footer row grouping,
	//                   FALSE to disable footer row grouping.
	// See Also:
	//     IsFooterRowsAllowGroup
	//-----------------------------------------------------------------------
	void FooterRowsAllowGroup(BOOL bAllowGroup);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if footer row grouping is enabled/disabled.
	// Returns:
	//     TRUE if footer row grouping is enabled, otherwise FALSE.
	// See Also:
	//     FooterRowsAllowGroup
	//-----------------------------------------------------------------------
	BOOL IsFooterRowsAllowGroup() const;

	//////////////////////////////////////////////////////////////////////////
	// Allow Sort
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables row sorting.
	// Parameters:
	//     bAllowSort - TRUE to enable row sorting,
	//                  FALSE to disable row sorting.
	// See Also:
	//     IsAllowSort
	//-----------------------------------------------------------------------
	void AllowSort(BOOL bAllowSort);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if row sorting is enabled/disabled.
	// Returns:
	//     TRUE if row sorting is enabled, otherwise FALSE.
	// See Also:
	//     AllowSort
	//-----------------------------------------------------------------------
	BOOL IsAllowSort() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables header row sorting.
	// Parameters:
	//     bAllowSort - TRUE to enable header row sorting,
	//                  FALSE to disable header row sorting.
	// See Also:
	//     IsHeaderRowsAllowSort
	//-----------------------------------------------------------------------
	void HeaderRowsAllowSort(BOOL bAllowSort);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if header row sorting is enabled/disabled.
	// Returns:
	//     TRUE if header row sorting is enabled, otherwise FALSE.
	// See Also:
	//     HeaderRowsAllowSort
	//-----------------------------------------------------------------------
	BOOL IsHeaderRowsAllowSort() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables footer row sorting.
	// Parameters:
	//     bAllowSort - TRUE to enable footer row sorting,
	//                  FALSE to disable footer row sorting.
	// See Also:
	//     IsFooterRowsAllowSort
	//-----------------------------------------------------------------------
	void FooterRowsAllowSort(BOOL bAllowSort);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if footer row sorting is enabled/disabled.
	// Returns:
	//     TRUE if footer row sorting is enabled, otherwise FALSE.
	// See Also:
	//     FooterRowsAllowSort
	//-----------------------------------------------------------------------
	BOOL IsFooterRowsAllowSort() const;

	//////////////////////////////////////////////////////////////////////////
	// Show Rows
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies the visible state of header records.
	// Parameters:
	//     bShow - TRUE to show header records,
	//             FALSE to hide header records.
	// See Also:
	//     IsHeaderRowsVisible, ShowFooterRows
	//-----------------------------------------------------------------------
	void ShowHeaderRows(BOOL bShow = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the visible state of header records.
	// Returns:
	//     TRUE if header records are shown, FALSE if header records are hidden.
	// See Also:
	//     ShowHeaderRows
	//-----------------------------------------------------------------------
	BOOL IsHeaderRowsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies the visible state of footer records.
	// Parameters:
	//     bShow - TRUE to show footer records,
	//             FALSE to hide footer records.
	// See Also:
	//     IsFooterRowsVisible, ShowHeaderRows
	//-----------------------------------------------------------------------
	void ShowFooterRows(BOOL bShow = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the visible state of footer records.
	// Returns:
	//     TRUE if footer records are shown, FALSE if footer records are hidden.
	// See Also:
	//     ShowFooterRows
	//-----------------------------------------------------------------------
	BOOL IsFooterRowsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the Grid control theme.
	// Parameters:
	//     paintTheme     - Grid control theme; must be one of the values
	//                      listed in the Remarks section.
	//     bEnableMetrics - TRUE to allow the theme to override control metrics.
	// Remarks:
	//     paintTheme can be one of the following values:
	//      * <b>xtpGridThemeDefault</b>: Enables default theme.
	//      * <b>xtpGridThemeOfficeXP</b>: Enables Office XP style theme.
	//      * <b>xtpGridThemeOffice2003</b>: Enables Office 2003 style theme.
	//      * <b>xtpGridThemeOffice2007</b>: Enables Office 2007 style theme.
	//      * <b>xtpGridThemeOffice2010</b>: Enables Office 2010 style theme.
	//      * <b>xtpGridThemeOffice2013</b>: Enables Office 2013 style theme.
	//      * <b>xtpGridThemeVisualStudio2005</b>: Enables Visual Studio 2005 style theme.
	//      * <b>xtpGridThemeVisualStudio2010</b>: Enables Visual Studio 2010 style theme.
	//      * <b>xtpGridThemeVisualStudio2012Light</b>: Enables VS 2012 Light style
	//                                                    Property Grid theme.
	//      * <b>xtpGridThemeVisualStudio2012Dark</b>: Enables VS 2012 Dark style
	//                                                   Property Grid theme.
	//      * <b>xtpGridThemeResource</b>: Enables visual style theme.
	//-----------------------------------------------------------------------
	void SetTheme(XTPGridPaintTheme paintTheme, BOOL bEnableMetrics = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the Grid control theme.
	// Returns:
	//     The Grid control theme; one of the values defined
	//     by the XTPGridPaintTheme enumeration.
	//-----------------------------------------------------------------------
	XTPGridPaintTheme GetCurrentTheme() const;

#	endif // XTP_GRID_DEPRECATED()

protected:
	int GetPageRowCount(CDC* pDC) const;

	void AdjustScrollBarH(BOOL bUpdate, UINT nMask); // Adjusts the horizontal scrollbar.
	void AdjustScrollBarV(BOOL bUpdate, UINT nMask); // Adjusts the vertical scrollbar.
	void UpdateScrollBarV();

	void Paint(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Performs all drawing logic.
	// Parameters:
	//     pDC - Provided device context to draw control image with.
	//-----------------------------------------------------------------------
	virtual void OnDraw(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws all rows in the provided device context.
	// Parameters:
	//     pDC         - Provided device context to draw rows image with.
	//     rcClient    - Reference to a rectangle to draw rows image into.
	//     pRows       - Rows collection to draw.
	//     nTopRow     - Index of the topmost visible row of the rows collection.
	//     nColumnFrom - Index of the first column to draw from.
	//     nColumnTo   - Index of the last column to draw to.
	//     pnHeight    - Pointer to an integer to receive the total height
	//                   of the drawn rows; can be NULL.
	// Returns:
	//     The index of the last printed row.
	//-----------------------------------------------------------------------
	virtual int DrawRows(CDC* pDC, CRect& rcClient, int y, CXTPGridRows* pRows, int nTopRow,
						 int nColumnFrom, int nColumnTo, int* pnHeight = NULL);

	virtual void DrawIconView(CDC* pDC, CRect& rcClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the 'NoItems' text in the provided device conext.
	// Parameters:
	//     pDC      - Provided device context to draw text with.
	//     rcClient - Rectangle to draw text into.
	//-----------------------------------------------------------------------
	virtual void DrawNoItems(CDC* pDC, const CRect& rcClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the internal control state was changed since the last drawing.
	// Remarks:
	//     This method is used for caching control image drawing.
	// Returns:
	//     TRUE if the internal control state was changed since last drawing,
	//     otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsChanged() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the internal control state was changed since the last drawing.
	// Parameters:
	//     bChanged - TRUE if the internal control state was changed since
	//                the last drawing and the control must be republished.
	// Remarks:
	//     This method is used for caching control image drawing.
	//-----------------------------------------------------------------------
	void SetChanged(BOOL bChanged = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Re-sorts the rows tree according to the sort order for columns.
	// Remarks:
	//     This method works faster than Populate.
	//-----------------------------------------------------------------------
	virtual void ReSortRows();

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates the field chooser control with its content.
	//-----------------------------------------------------------------------
	void UpdateSubList();

	//-----------------------------------------------------------------------
	// Summary:
	//     Inserts a specified row at a specified index in the rows array.
	// Parameters:
	//     nIndex - Integer index to insert the row at.
	//     pRow   - Row to be inserted.
	// Remarks:
	//     This method inserts the specified row to the rows array at
	//     the specified index with all its children expanded.
	//     Use _DoExpand() to expand all child items.
	// Returns:
	//     The number of inserted rows.
	//-----------------------------------------------------------------------
	virtual int InsertRow(int nIndex, CXTPGridRow* pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Checks a record for filter text.
	// Parameters:
	//     pRecord         - A record to apply filter to.
	//     strFilterText   - Filter string text.
	//     bIncludePreview - Include preview item in filtering or not.
	// Remarks:
	//     This member function represents filter functionality. First, it parses
	//     the input text of a filter string by tokens; second it enumerates all visible
	//     columns to find text matching to the filter string. Returns TRUE if
	//     matching found, FALSE otherwise
	// Returns:
	//     TRUE if record is filtered with the specified filter,
	//     FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL ApplyFilter(CXTPGridRecord* pRecord, CString strFilterText, BOOL bIncludePreview);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called after a user selects a row or a column.
	// Remarks:
	//     After the user selects a row or column, the Grid control sends a
	//     notification to the parent window by calling OnSelectionChanged().
	// See Also:
	//     XTP_NM_GRID_SELCHANGED, SendNotifyMessage()
	//-----------------------------------------------------------------------
	virtual void OnSelectionChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called before the focus changes.
	// Parameters:
	//     pNewRow  - Pointer to the newly focused row object.
	//     pNewCol  - Pointer to the newly focused column object.
	// Remarks:
	//     Before the user changes the focused row or column, the Grid control
	//     sends a notification to the parent window by calling OnFocusChanging().
	// Returns:
	//     TRUE if the operation will continue, FALSE if the operation will be canceled.
	// See Also:
	//     XTP_NM_GRID_FOCUS_CHANGING, SendNotifyMessage(), OnFocusChanged
	//-----------------------------------------------------------------------
	virtual BOOL OnFocusChanging(CXTPGridRow* pNewRow, CXTPGridColumn* pNewCol);

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates metrics for a row.
	// Parameters:
	//     pDrawArgs    - Pointer to an XTP_GRIDRECORDITEM_DRAWARGS
	//                    structure used to calculate metrics.
	//     pItemMetrics - Pointer to an XTP_GRIDRECORDITEM_METRICS
	//                    structure to fill with values.
	//-----------------------------------------------------------------------
	virtual void GetItemMetrics(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
								XTP_GRIDRECORDITEM_METRICS* pItemMetrics);

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a CXTPGridRow object.
	//     Overwrite it to use the derived CXTPGridRow class.
	// Returns:
	//     A pointer to the newly created CXTPGridRow object.
	//-----------------------------------------------------------------------
	virtual CXTPGridRow* CreateRow();

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a CXTPGridGroupRow object.
	//     Overwrite it to use the derived CXTPGridGroupRow class.
	// Returns:
	//     A pointer to the newly created CXTPGridGroupRow object.
	//-----------------------------------------------------------------------
	virtual CXTPGridGroupRow* CreateGroupRow();

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a CXTPGridRow object for header/footer rows.
	//     Overwrite it to use the derived CXTPGridRow class.
	// Returns:
	//     A pointer to the newly created CXTPGridRow object.
	//-----------------------------------------------------------------------
	virtual CXTPGridRow* CreateHeaderFooterRow();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user begins dragging a row.
	// Parameters:
	//     point  - Drag point.
	//     nFlags - Flags for various mouse button states.
	// Remarks:
	//     You can override this method or catch a LVN_BEGINDRAG message to
	//     proceed with drag operations.
	//     OnBeginDrag will not know which mouse button is used for the
	//     drag unless you pass nFlags.
	//-----------------------------------------------------------------------
	virtual void OnBeginDrag(CPoint point);
	virtual void OnBeginDrag(CPoint point,
							 UINT nFlags); // <COMBINE CXTPGridControl::OnBeginDrag@CPoint>

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before a record is copied to the clipboard in text format.
	// Parameters:
	//     pRecord      - [in] Pointer to the source record.
	//     rarStrings   - [in/out] Reference to an array of strings with record item
	//                    values. By default, this array contains visible item captions.
	// Remarks:
	//     If you would like to customize text data for the clipboard, then
	//     you can change rarStrings as needed.
	//
	//     Default implementation sends an XTP_NM_GRID_BEFORE_COPY_TOTEXT
	//     notification.
	// Returns:
	//     TRUE if the operation will be canceled, FALSE if the operation will continue.
	// See Also:
	//     OnBeforePasteFromText, OnBeforePaste, Cut, Copy, Paste.
	//-----------------------------------------------------------------------
	virtual BOOL OnBeforeCopyToText(CXTPGridRecord* pRecord, CStringArray& rarStrings);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before creating a new record using text data
	//     from the clipboard.
	// Parameters:
	//     arStrings    - [in] Array of strings with record items values.
	//     ppRecord     - [out] Double pointer to the new record.
	// Remarks:
	//     If you would like to customize creating a new record from text data, then
	//     you must create a new record object with record items and fill them
	//     using the default values and strings provided by the arStrings parameter.
	//
	//     If the new record will not be set to the ppRecord parameter, then the
	//     control will create a CXTPGridRecord object with
	//     CXTPGridRecordItemVariant items and fill the visible items using
	//     the strings provided by the arStrings parameter.
	//
	//     Default implementation sends an XTP_NM_GRID_BEFORE_PASTE_FROMTEXT
	//     notification.
	// Returns:
	//     TRUE if the operation will be canceled, FALSE if the operation will continue.
	// See Also:
	//     OnBeforeCopyToText, OnBeforePaste, Cut, Copy, Paste.
	//-----------------------------------------------------------------------
	virtual BOOL OnBeforePasteFromText(CStringArray& arStrings, CXTPGridRecord** ppRecord);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called after creating a new record using binary data
	//     from the clipboard, but before adding it to records collection.
	// Parameters:
	//     ppRecord     - [in/out] Double pointer to the new record.
	// Remarks:
	//     If you would like to customize the new record created from binary data,
	//     then you may either create a new record or change the provided record
	//     by using the ppRecord parameter.
	//
	//     Default implementation sends an XTP_NM_GRID_BEFORE_PASTE
	//     notification.
	// Returns:
	//     TRUE if the operation will be canceled, FALSE if the operation will continue.
	// See Also:
	//     OnBeforeCopyToText, OnBeforePasteFromText, Cut, Copy, Paste.
	//-----------------------------------------------------------------------
	virtual BOOL OnBeforePaste(CXTPGridRecord** ppRecord);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before processing the OnKeyDown event.
	// Parameters:
	//     rnChar     - [in/out] Reference to a variable that specifies
	//                  the virtual key code of the given key. For a list
	//                  of standard virtual key codes, see Winuser.h.
	//     nRepCnt    - [in] Repeat count (the number of times that the keystroke
	//                  is repeated as a result of the user holding down the key).
	//     nFlags     - [in] Specifies the scan code, key-transition code,
	//                  previous key state, and context code.
	// Remarks:
	//     If you would like to customize keyboard behavior, then you can either
	//     change the rnChar parameter value or perform your logic and then
	//     return FALSE to disable default processing.
	//
	//     Default implementation sends an XTP_NM_GRID_PREVIEWKEYDOWN
	//     notification.
	// Returns:
	//     TRUE if the operation will continue, FALSE if the operation will be canceled.
	// See Also:
	//     OnKeyDown, CWnd::OnKeyDown, XTP_NM_GRID_PREVIEWKEYDOWN.
	//-----------------------------------------------------------------------
	virtual BOOL OnPreviewKeyDown(UINT& rnChar, UINT nRepCnt, UINT nFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is used by the Grid control to calculate
	//     the best-fit column width.
	// Parameters:
	//     pColumn - Pointer to a column for width calculation.
	// Returns:
	//     The best-fit column width if successful, otherwise a value of 0.
	//-----------------------------------------------------------------------
	virtual int OnGetColumnDataBestFitWidth(CXTPGridColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is used by the Grid control to calculate
	//     the items captions maximum width.
	// Parameters:
	//     pDC        - Pointer to a valid device context.
	//     pRows      - Pointer to a rows collection.
	//     pColumn    - Pointer to a column for width calculation.
	//     nStartRow  - Index of the row to begin calculating from.
	//     nRowsCount - Number of rows to include within the calculation
	//                  (i.e. the rows specified by this parameter includes both
	//                  nStartRow and the rows that immediately follow nStartRow).
	// Returns:
	//     The items captions maximum width.
	//-----------------------------------------------------------------------
	virtual int OnGetItemsCaptionMaxWidth(CDC* pDC, CXTPGridRows* pRows, CXTPGridColumn* pColumn,
										  int nStartRow = 0, int nRowsCount = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is used to start monitoring mouse position for
	//     automatic vertical scrolling.
	// Remarks:
	//     Used when dragging records.
	// See Also:
	//     EnsureStopAutoVertScroll, DoAutoVertScrollIfNeed
	//-----------------------------------------------------------------------
	virtual void EnsureStartAutoVertScroll();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is used to stop monitoring mouse position for
	//     automatic vertical scrolling.
	// Remarks:
	//     Used when dragging records.
	// See Also:
	//     EnsureStartAutoVertScroll, DoAutoVertScrollIfNeed
	//-----------------------------------------------------------------------
	virtual void EnsureStopAutoVertScroll();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is used to monitor mouse position. If the mouse
	//     is moved to 20 pixels before either the top or bottom
	//     grid borders, then the control will be scrolled vertically.
	// Parameters:
	//     ptClick - Coordinates of the initial click (or mouse down).
	//     pt      - Current mouse position.
	// Remarks:
	//     Used when dragging records.
	// See Also:
	//     EnsureStartAutoVertScroll, EnsureStopAutoVertScroll
	//-----------------------------------------------------------------------
	virtual void DoAutoVertScrollIfNeed(CPoint ptClick, CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the combined height of all rows within a specified rows collection.
	// Parameters:
	//     pRows       - Pointer to the rows collection to calculate
	//                   the combined height of all rows within.
	//     nTotalWidth - Width of the row.
	//     nMaxHeight  - Should the calculated height exceed the value of this
	//                   parameter, then the calculation will end. Set this
	//                   parameter to -1 to calculate the height of all rows.
	// Returns:
	//     The combined height of all rows within the specified rows collection.
	//-----------------------------------------------------------------------
	virtual int GetRowsHeight(CXTPGridRows* pRows, int nTotalWidth, int nMaxHeight = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is used to notify about changes to the constraint selection.
	// Parameters:
	//     pRow        - Pointer to the current row object.
	//     pItem       - Pointer to the current record item object.
	//     pColumn     - Pointer to the current column object.
	//     pConstraint - Pointer to the hot selected constraint.
	// Remarks:
	//     Sent for every selection change before the new selection is applied or canceled.
	// Returns:
	//     TRUE if the notification was sent successfully, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL OnConstraintSelecting(CXTPGridRow* pRow, CXTPGridRecordItem* pItem,
									   CXTPGridColumn* pColumn,
									   CXTPGridRecordItemConstraint* pConstraint);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get the tooltip text for the current record item.
	// Parameters:
	//     pRow            - Pointer to the current row object.
	//     pItem           - Pointer to the current record item object.
	//     rstrToolTipText - Reference to a CString object to customize the
	//                       tooltip text. It contains default tooltip text.
	// Remarks:
	//     If rstrToolTipText is set to empty, then the default tooltip will be shown
	//     when there is not enough space to draw item text.
	//     If rstrToolTipText is set to a single space (" "), then no tooltip will be shown.
	// Returns:
	//     A reference to an XTP_NM_GRIDTOOLTIPINFO structure.
	//-----------------------------------------------------------------------
	virtual const XTP_NM_GRIDTOOLTIPINFO& OnGetToolTipInfo(CXTPGridRow* pRow,
														   CXTPGridRecordItem* pItem,
														   CString& rstrToolTipText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adjusts control indentation properties depending on the current tree depth.
	// See Also:
	//     GetIndent
	//-----------------------------------------------------------------------
	void AdjustIndentation();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when an item is dragged over the Grid control.
	// Parameters:
	//     pDataObject - Pointer to the COleDataObject being dragged over the drop target.
	//     dwKeyState  - State of keys on the keyboard (including modifier keys).
	//                   This is a combination of any number of the following: MK_CONTROL,
	//                   MK_SHIFT, MK_ALT, MK_LBUTTON, MK_MBUTTON, and MK_RBUTTON.
	//     point       - Current mouse position relative to the Grid control.
	//     nState      - Transition state (0 - enter, 1 - leave, 2 - over).
	// Returns:
	//     A value from the DROPEFFECT enumerated type which indicates the type
	//     of drop that would occur if the user dropped the object at this position.
	//     The type of drop often depends on the current key state as indicated
	//     by dwKeyState. A standard mapping of key states to DROPEFFECT values is:
	//     * <b>DROPEFFECT_NONE</b>: The data object cannot be dropped in this window.
	//     * <b>DROPEFFECT_COPY</b> for <b>MK_CONTROL</b>: Creates a copy of
	//                                                     the dropped object.
	//     * <b>DROPEFFECT_MOVE</b> for <b>MK_ALT</b>: Creates a copy of the dropped
	//                                                 object and deletes the original
	//                                                 object. This is typically the
	//                                                 default drop effect when the
	//                                                 view can accept the data object.
	//-----------------------------------------------------------------------
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point,
								  int nState);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when an item has been dropped into the Grid control.
	// Parameters:
	//     pDataObject - Pointer to the COleDataObject dropped into the drop target.
	//     dropEffect  - Drop effect requested by the user; can be any
	//                   of the values listed in the Remarks section.
	//     point       - Current mouse position relative to the Grid control.
	// Remarks:
	//     The <i>dropEffect</i> parameter can be one of the following values:
	//     * <b>DROPEFFECT_COPY</b>: Creates a copy of the data object being dropped.
	//     * <b>DROPEFFECT_MOVE</b>: Moves the data object to the current mouse location.
	// Returns:
	//     TRUE if the drop was successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     Relays a tooltip event.
	// Parameter:
	//     message - Windows message.
	//-----------------------------------------------------------------------
	virtual void RelayToolTipEvent(UINT message);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of rows per mouse scroll.
	// Returns:
	//     The number of rows per mouse scroll.
	//-----------------------------------------------------------------------
	static UINT AFX_CDECL GetMouseScrollLines();

	//{{AFX_CODEJOCK_PRIVATE
	virtual CString _GetSelectedRowsVisibleColsText();
	BOOL _GetSelectedRows(CXTPGridRecords* pRecords,
						  CXTPInternalCollectionT<CXTPGridRow>* pRows = NULL);

	void _SelectChilds(CXTPGridRecords* pRecords);
	void _SelectRows(CXTPGridRecords* pRecords);

	virtual BOOL _ReadRecordsFromText(LPCTSTR pcszText, CXTPGridRecords& rarRecords);
	virtual CXTPGridRecord* _CreateRecodFromText(LPCTSTR pcszRecord);

	virtual BOOL _WriteSelectedRowsData(CXTPPropExchange* pPX);
	virtual BOOL _ReadRecordsFromData(CXTPPropExchange* pPX, CXTPGridRecords& rarRecords);
	BOOL _WriteRecordsData(CXTPPropExchange* pPX, CXTPGridRecords* pRecords);
	virtual void DrawDropMarker(CDC* pDC);
	virtual void DrawExtDropMarker(CDC* pDC, int y);
	virtual BOOL _ApplyFilter(CXTPGridRecord* pRecord, CString strFilterText, BOOL bIncludePreview);
	//}}AFX_CODEJOCK_PRIVATE

	//{{AFX_CODEJOCK_PRIVATE
	// System accessibility support.
	virtual HRESULT GetAccessibleParent(IDispatch** ppdispParent);
	virtual HRESULT GetAccessibleChildCount(long* pcountChildren);
	virtual HRESULT GetAccessibleChild(VARIANT varChild, IDispatch** ppdispChild);
	virtual HRESULT GetAccessibleName(VARIANT varChild, BSTR* pszName);
	virtual HRESULT GetAccessibleRole(VARIANT varChild, VARIANT* pvarRole);
	virtual HRESULT AccessibleLocation(long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight,
									   VARIANT varChild);
	virtual HRESULT AccessibleHitTest(long xLeft, long yTop, VARIANT* pvarChild);
	virtual HRESULT GetAccessibleState(VARIANT varChild, VARIANT* pvarState);
	virtual CCmdTarget* GetAccessible();
	//}}AFX_CODEJOCK_PRIVATE

	//{{AFX_VIRTUAL(CXTPGridControl)
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID,
						CCreateContext* pContext = NULL);
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	virtual CScrollBar* GetScrollBarCtrl(int nBar) const;
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPGridControl)
	afx_msg LRESULT OnPrintClient(WPARAM wParam, LPARAM lParam);

	// Window events
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR uTimerID);
	afx_msg void OnNcPaint();

	// Focus
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);

	// Scrolling
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	// Mouse events
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint point);
	afx_msg void OnMouseLeave();

	// Key events
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint pos);

	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnCaptureChanged(CWnd* pWnd);
	afx_msg void OnSysColorChange();
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	afx_msg LRESULT OnGetObject(WPARAM wParam, LPARAM lParam);

#	ifdef _XTP_ACTIVEX
	// Dispatch and event IDs
public:
	enum
	{
		//{{AFX_DISP_ID(CXTPGridControl)
		eventidDrawItem			  = 11L,
		eventidMeasureRow		  = 12L,
		eventidDrawPreviewItem	= 24L,
		eventidMeasurePreviewItem = 33L,
		//}}AFX_DISP_ID
	};
#	endif

#	ifdef _XTP_ACTIVEX

public:
	void SetCustomDraw(long dwFlags);

	// Event maps
	//{{AFX_EVENT(CXTPGridControl)
	void FireMeasureRow(LPDISPATCH Row, OLE_HANDLE hDC, int nWidth, int* pnHeight)
	{
		((COleControl*)GetParent())
			->FireEvent(eventidMeasureRow, EVENT_PARAM(VTS_DISPATCH VTS_HANDLE VTS_I4 VTS_PI4), Row,
						hDC, nWidth, pnHeight);
	}

	void FireMeasureRowV(LPDISPATCH Row, OLE_HANDLE hDC, int nWidth, VARIANT* pHeight)
	{
		((COleControl*)GetParent())
			->FireEvent(eventidMeasureRow + 100,
						EVENT_PARAM(VTS_DISPATCH VTS_HANDLE VTS_I4 VTS_PVARIANT), Row, hDC, nWidth,
						pHeight);
	}

	void FireDrawItem(LPDISPATCH Row, LPDISPATCH Column, LPDISPATCH Item, OLE_HANDLE hDC, CRect rc,
					  BOOL* pbDoDefault)
	{
		((COleControl*)GetParent())
			->FireEvent(eventidDrawItem,
						EVENT_PARAM(VTS_DISPATCH VTS_DISPATCH VTS_DISPATCH VTS_HANDLE VTS_I4 VTS_I4
										VTS_I4 VTS_I4 VTS_PBOOL),
						Row, Column, Item, hDC, rc.left, rc.top, rc.right, rc.bottom, pbDoDefault);
	}

	void FireDrawItemV(LPDISPATCH Row, LPDISPATCH Column, LPDISPATCH Item, OLE_HANDLE hDC, CRect rc,
					   VARIANT* pbDoDefault)
	{
		((COleControl*)GetParent())
			->FireEvent(eventidDrawItem + 100,
						EVENT_PARAM(VTS_DISPATCH VTS_DISPATCH VTS_DISPATCH VTS_HANDLE VTS_I4 VTS_I4
										VTS_I4 VTS_I4 VTS_PVARIANT),
						Row, Column, Item, hDC, rc.left, rc.top, rc.right, rc.bottom, pbDoDefault);
	}

	void FireDrawPreviewItem(LPDISPATCH Row, LPDISPATCH Item, OLE_HANDLE hDC, CRect rc,
							 BOOL* pbDoDefault)
	{
		((COleControl*)GetParent())
			->FireEvent(eventidDrawPreviewItem,
						EVENT_PARAM(VTS_DISPATCH VTS_DISPATCH VTS_HANDLE VTS_I4 VTS_I4 VTS_I4 VTS_I4
										VTS_PBOOL),
						Row, Item, hDC, rc.left, rc.top, rc.right, rc.bottom, pbDoDefault);
	}

	void FireDrawPreviewItemV(LPDISPATCH Row, LPDISPATCH Item, OLE_HANDLE hDC, CRect rc,
							  VARIANT* pbDoDefault)
	{
		((COleControl*)GetParent())
			->FireEvent(eventidDrawPreviewItem + 100,
						EVENT_PARAM(VTS_DISPATCH VTS_DISPATCH VTS_HANDLE VTS_I4 VTS_I4 VTS_I4 VTS_I4
										VTS_PVARIANT),
						Row, Item, hDC, rc.left, rc.top, rc.right, rc.bottom, pbDoDefault);
	}

	void FireMeasurePreviewItem(LPDISPATCH Row, OLE_HANDLE hDC, int nWidth, int* pnHeight)
	{
		((COleControl*)GetParent())
			->FireEvent(eventidMeasurePreviewItem,
						EVENT_PARAM(VTS_DISPATCH VTS_HANDLE VTS_I4 VTS_PI4), Row, hDC, nWidth,
						pnHeight);
	}

	void FireMeasurePreviewItemV(LPDISPATCH Row, OLE_HANDLE hDC, int nWidth, VARIANT* pHeight)
	{
		((COleControl*)GetParent())
			->FireEvent(eventidMeasurePreviewItem + 100,
						EVENT_PARAM(VTS_DISPATCH VTS_HANDLE VTS_I4 VTS_PVARIANT), Row, hDC, nWidth,
						pHeight);
	}
	//}}AFX_EVENT
#	endif
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void OnButton(UINT nFlags, CPoint point, CXTPGridBehaviorRowMouseButton* behavior);

	int _GetFocusedRowType() const;

	int _GetFocusedRowIndex() const;

	BOOL _SetFocusedRow(CXTPGridRow* pRow);

	BOOL _SetSelectedRow(CXTPGridRow* pRow, int nFocusedRowIndex, BOOL bShiftKey, BOOL bControlKey);

	void DrawDefaultGrid(CDC* pDC, CRect rcClient, int nRowHeight, int nLeftOffset);
	void RedrawScrollBar(int nBar);

	void GetColumnTotalSize(int& nTotalCount, int& nTotalWidth) const;
	void GetColumnOutOfViewSize(int& nOutOfViewCount, int& nOutOfViewWidth) const;
	void GetColumnScrollSize(int nOutOfViewWidth, int& nScrollCount, int& nScrollWidth) const;
	void GetParametersOfHScrollingBlockSize(int& nTotalWidth, int& nScrollPage) const;
	void GetParametersOfHScrollingBlockCount(int& nTotalCount, int& nScrollPage) const;

	void UpdateScrollBarVisibility(int nBar);

public:
	BOOL m_bThemeMetrics; // TRUE to allow themes to override control metrics.
	BOOL m_bMovePivot; // Tells whether the freeze column is displayed or not when columns are added
					   // to the group by box.
	BOOL m_bStrictFiltering; // Tells whether strict filtering is enabled or not.

	BOOL m_bForcePagination; // Specifies whether to force the grid to be split up into "pages"
							 // while in print preview mode.
	BOOL m_bSelectionExcludeGroupRows; // TRUE if the selection excludes group rows.

	XTPGridInputState m_mouseDownState; // MouseDown state.

	DWORD m_dwLastMouseMessage;

	CXTPGridRow* m_pHotExpandButtonRow; // Row with hot expand button.
	CXTPGridHyperlink* m_pHotHyperlink; // Hot hyperlink.

protected:
	CXTPGridBehavior* m_pBehavior; //

	BOOL m_bDoubleBuffering; // Enables double-buffering.
	BOOL m_bChanged;		 // Internal member for storing changed flag.
	BOOL m_bInternalMove;	// Internal member for storing DD run-time flag.

	int m_nLockUpdateCount; // A counter of the update locks. An image will be redrawn only when the
							// lock counter is equal to zero.

	CRect m_rcGroupByArea; // The area occupied by Group By item.
	CRect m_rcHeaderArea;  // The area occupied by the header.
	CRect m_rcFooterArea;  // The area occupied by the footer.

	CXTPGridColumns* m_pColumns; // List columns container.

	CXTPGridRows* m_pRows; // Virtual list rows container. Used for changing order, etc.

	CXTPGridHeader* m_pGridHeader; // List header member.

	CXTPGridSections* m_pSections;
	CXTPGridSection* m_pSectionHeader;
	CXTPGridSection* m_pSectionBody;
	CXTPGridSection* m_pSectionFooter;
	CXTPGridSection* m_pSectionScroll;

	CXTPMarkupContext* m_pMarkupContext;   // Markup context.
	CXTPGridPaintManager* m_pPaintManager; // Paint manager.
	CXTPGridNavigator* m_pNavigator;	   // Navigator.

	CXTPGridNavigator* m_pNavigatorGrid;
	CXTPGridIconNavigator* m_pNavigatorIcon;

	//////////////////////////////////////////////////////////////////////////
	// Scrolling
	//////////////////////////////////////////////////////////////////////////

	XTPGridScrollUpdateMode m_scrollUpdateMode; // Scroll update mode.
	XTPGridScrollMode m_scrollModeH;			// Horizontal scroll mode.
	XTPGridScrollMode m_scrollModeV;			// Vertical scroll mode.

	int m_nScrollOffsetH; // Horizontal scroll position.

	int m_nScrollStepH; // Horizontal scroll step (in pixels).
	int m_nScrollStepV; // Vertical scroll step (in pixels).

	int m_nDisableReorderColumnsCount; // Count of columns at the left side where reordering is
									   // disabled.

	CXTPGridSelectedRows* m_pSelectedRows; // Container for the currently selected rows.

	CXTPGridRow* m_pHotRow; // Hot row.

	int m_nRowsPerWheel; // Amount of rows to scroll by mouse wheel.

	CXTPGridTip* m_pTip; // Tip window.
	CBitmap m_bmpCache;  // Cached window bitmap.

	BOOL m_bGroupByEnabled;   // TRUE if Group By box is enabled.
	BOOL m_bSortRecordChilds; // TRUE to apply sort order for record children.

	XTPGridMouseMode m_mouseMode; // Current mouse operation mode
	BOOL m_bBlockSelection;		  // TRUE if multiple selection is enabled.
	BOOL m_bMultiSelectionMode;   // TRUE if multi-selection mode is enabled (i.e. VK_CTRL is always
								  // on).
	BOOL m_bShowTooltips;		  // TRUE if show tooltips is enabled.
	BOOL m_bAutoCheckItems; // TRUE to enable auto-check mode, FALSE to disable auto-check mode.

	CXTPImageManager* m_pImageManager; // Image list for the Grid control.
	BOOL m_bSkipGroupsFocus; // TRUE if group rows are skipped when navigating rows with the Up and
							 // Down arrow keys.

	BOOL m_bLockExpand;				 // TRUE if collapsing/expanding rows is locked.
	BOOL m_bEnsureFocusedRowVisible; // TRUE if focused rows will be forced visible.
	BOOL m_bFocusSubItems;			 // TRUE if sub-items can receive focus.
	BOOL m_bEditOnClick;			 // TRUE if sub-items become editable on a single-click
	BOOL m_bEditOnDelayClick;		 // TRUE if sub-items become editable on a delay-click
	BOOL m_bEditOnDoubleClick;		 // TRUE if sub-items become editable on a double-click

	BOOL m_bPreviewAllowEdit;	 // TRUE if the preview of the row can be edited.
	BOOL m_bHeaderVisible;		  // TRUE if column headers are visible.
	BOOL m_bFooterVisible;		  // TRUE if column footers are visible.
	BOOL m_bPinFooterRows;		  // TRUE if footer rows are drawn immediately after the body rows.
	BOOL m_bPinFooterRowsPrinted; // TRUE if footer rows are printed immediately after the body
								  // rows.
	BOOL m_bInitialSelectionEnable; // TRUE if Initial (in Populate() call) selection is enabled.
	BOOL m_bRowFocusVisible;		// TRUE if showing focused row rectangle is enabled.

	CXTPGridColumn* m_pFocusedColumn; // Pointer to the currently focused CXTPGridColumn.

	CXTPToolTipContext* m_pToolTipContext; // Tooltip context.
	CXTPGridRecordItem* m_pActiveItem;	 // Pointer to the currently focused CXTPGridRecordItem.
	CXTPGridInplaceEdit* m_pInplaceEdit;   // In-place edit pointer.
	CXTPGridInplaceButtons* m_pInplaceButtons; // In-place buttons pointer.
	CXTPGridInplaceList* m_pInplaceList;	   // In-place list pointer.
	BOOL m_bVScrollBarVisible;				   // TRUE if vertical scrollbar is visible.
	BOOL m_bHScrollBarVisible;				   // TRUE if horizontal scrollbar is visible.
	UINT m_nAutoVScrollTimerResolution; // Vertical scrolling timer resolution (in milliseconds).
	CPoint m_ptDrag;					// Drag position.
	int m_nDragSensitivityX; // Horizontal sensitivity of Drag'n'Drop operation (in pixels). Default
							 // value is 3. DPI aware.
	int m_nDragSensitivityY; // Vertical sensitivity of Drag'n'Drop operation (in pixels). Default
							 // value is 3. DPI aware.
	BOOL m_bPrepareDrag;	 // TRUE if the user clicks the Grid control and does not release the
							 // button.
	CString m_strFilterText; // Filter text.
	BOOL m_bFilterHiddenColumns;  // Search filter text in hidden columns too.
	int m_nRecordsTreeFilterMode; // Tree Filter mode.

	CLIPFORMAT m_cfGrid; // Grid clipboard format for drag/drop operations.

	CGridDropTarget* m_pDropTarget; // Internal drag/drop helper.
	BOOL m_bDragMode;				// TRUE if records are currently being dragged.
	BOOL m_bInternalDrag;			// TRUE if records begin dragging from this control.
	DWORD m_dwDragDropFlags;		// Drag/drop flags.
	DWORD m_dwDropMarkerFlags;		// The drop marker flags.
	CXTPGridSelectedRows* m_pSelectedRowsBeforeDrag; // The selected rows before a dragging
													 // action.
	int m_nDropPos;									 // Position of records to be dropped.
	CXTPGridRecords* m_pDropRecords;				 // Drop records.

	BOOL m_bAdjustLayoutRunning;   // TRUE if AdjustLayout handler is entered, FALSE otherwise. Used
								   // to prevent OnSize reenter and stack overflow in Win95/98/ME.
	UINT_PTR m_uAutoScrollTimerID; // Auto scroll timer ID or 0.

	//{{AFX_CODEJOCK_PRIVATE
	long m_nOLEDropMode;  // Store OLE drop mode.
	BOOL m_nOLEDropAbove; // Drop above record?
	//}}AFX_CODEJOCK_PRIVATE

	CXTPGridRow* m_ptrVirtualEditingRow; // Currently editing row in virtual mode.

	XTPGridRowsCompareFunc m_pRowsCompareFunc; // Pointer to the rows compare function.

	HBITMAP m_hbmpWatermark;						// Watermark bitmap handle.
	BYTE m_WatermarkTransparency;					// Watermark bitmap transparency value.
	BITMAP m_bmWatermark;							// Watermark bitmap info.
	XTPGridWatermarkAlignment m_WatermarkAlignment; // Watermark alignment flags.
	int m_nEnsureVisibleRowIdx;						// Ensure visible row index.
	int m_nEnsureVisibleColumnIdx;					// Ensure visible column index.

	CXTPGridDataManager* m_pDataManager; // Data manager.

	BOOL m_bShowIconWhenEditing; // TRUE to show item icons while the item is being edited.

#	ifdef _XTP_ACTIVEX

public:
	BOOL m_bCustomDrawItem;
	BOOL m_bCustomDrawPreviewItem;
	BOOL m_bCustomMeasureRow;
	BOOL m_bCustomBeforeDrawRow;
	BOOL m_bCustomMeasurePreviewItem;

#	endif

private:
	XTP_NM_GRIDTOOLTIPINFO* m_pCachedToolTipInfo;

public:
	int m_iIconWidth;		  // Icon geometry settings.
	int m_iIconHeight;		  // Icon geometry settings.
	int m_iIconWidthSpacing;  // Icon geometry settings.
	int m_iIconHeightSpacing; // Icon geometry settings.

	int m_iIconPropNum; // Icon view setting.
	int m_iIconNum;		// Icon view setting.

	BOOL m_bMarkupEnabled;			 // Flag to check markup settings.
	int m_iIconViewColumn;			 // Icon view setting.
	BOOL m_bIconColumnIndexNotValid; // Used to indicate icon column is created, but does not yet
									 // have a valid index.

	BOOL m_bStrictBestFit;		// TRUE to use BestFit only for non-autosize mode.
	BOOL m_bWasShiftKey;		// Flag set on MouseUp or KeyUp if Shift key was On.
	BOOL m_bSortedDragDrop;		// Flag to set Drag Drop mode like
								// Vista Windows Explorer or XP Windows Explorer.
	BOOL m_bTrapTabKey;			// Flag to set Trap Tab key in m_bEditOnClick && m_bAllowEdit case.
	BOOL m_bDesktopTrackerMode; // Flag to set Vista Tracker Mode On or Off.
	BOOL m_bUnrestrictedDragDrop;   // TRUE to allow a child to be dropped to any pos; default =
									// FALSE.
	BOOL m_bFreeHeightMode;			// Flag to set RC Free Height Mode.
	int m_nDefaultRowFreeHeight;	// Row height for initialization.
	CString m_sCustomTitle;			// Used for a unique title (e.g. for PrintJob name).
	BOOL m_bRClickDrag;				// TRUE to allow drag on right click.
	BOOL m_bKeepSelectionAfterSort; // TRUE to keep the selection after sorting items.

	int m_iColumnForNum; // Index of the row number column used.

protected:
	BOOL m_bCreated;  // TRUE if created using the Create() method.
	BOOL m_bIconView; // TRUE if icon view, FALSE otherwise.
	// Height of a grid cell for items in large icon view, in pixels. Each item fits into a
	// rectangle of size SM_CXICONSPACING by SM_CYICONSPACING when arranged. This value is always
	// greater than or equal to SM_CYICON.
	int m_iColumnForNumPrev;
	int m_iIconViewRowsPerLine; // The icon view rows per line.
	BOOL m_bUseIconColumnForNum;
	CXTPGridColumnOrder* m_pPrevVisible;	 // The previous visible column order.
	CXTPGridColumnOrder* m_pPrevGroupsOrder; // The previous group column order.
	// Visible columns before setting icon view
	BOOL m_bPrevFocusSubItems;  // TRUE if there are previous focused sub-items, FALSE otherwise.
	BOOL m_bPrevHeaderAutoSize; // TRUE if the previous header is auto-size, FALSE otherwise.

	BOOL m_bPrevHeaderRows;  // TRUE if there are previous header rows, FALSE otherwise.
	BOOL m_bPrevFooterRows;  // TRUE if there are previous footer rows, FALSE otherwise.
	int m_nPrevTreeIndent;   // Tree indentation.
	BOOL m_bPrevHeaderShow;  // Tells whether the previous header is visible or not.
	BOOL m_bPrevFooterShow;  // Tells whether the previous footer is visible or not.
	BOOL m_bPrevPreviewMode; // Tells whether the preview mode is enabled or not previously.

	BOOL m_bNoNeedSortedDragDrop; // Dynamic flag during Sorted DragDrop operation
	//(set during OnDragOver state - used during OnDrop state)

	UINT_PTR m_uiDelayEditTimer; // The delay edit timer.
	UINT m_uiDelayEditMaxTime;   // The delay edit time interval.

	int m_iLastRqstEditRow; // The last row which requested a delay edit.
	int m_iLastRqstEditCol; // The last column which requested a delay edit.

	// ICON_VIEW_MODE RELATED <<
	UINT_PTR m_uRqstEditTimer;		  // The edit timer ID.
	BOOL m_bFastDeselectMode;		  // Tells whether the fast deselect mode is enabled or not.
	XTPGridPaintTheme m_themeCurrent; // Currently set theme.

	BOOL m_bRelayWndMsg; // Used for relaying window messages.

	friend class CGridControlCtrl;
	friend class CGridDropTarget;
	friend class CXTPGridPaintManager;
	friend class CXTPGridSections;
	friend class CXTPGridSection;
};

//===========================================================================
// Summary:
//      Helper class with static member functions used to control active locale
//      for Grid control and perform some locale dependent tasks.
//===========================================================================
class _XTP_EXT_CLASS CXTPGridControlLocale
{
private:
	CXTPGridControlLocale(){};

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Determines which locale is the used active locale;
	//     the resource file locale or the current user locale.
	// Returns:
	//     TRUE if the resource file locale is the used active locale,
	//     FALSE if the current user locale is the used active locale.
	// See Also:
	//     SetUseResourceFileLocale, CXTPResourceManager::GetResourcesLangID,
	//     GetActiveLCID, LOCALE_USER_DEFAULT
	//-----------------------------------------------------------------------
	static BOOL AFX_CDECL IsUseResourceFileLocale();

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies which locale is the used active locale;
	//     the resource file locale or the current user locale.
	// Parameters:
	//     bUseResourceFileLocale - TRUE to use the resource file locale,
	//                              FALSE to use the current user locale.
	// See Also:
	//     IsUseResourceFileLocale, CXTPResourceManager::GetResourcesLangID,
	//     GetActiveLCID, LOCALE_USER_DEFAULT
	//-----------------------------------------------------------------------
	static void AFX_CDECL SetUseResourceFileLocale(BOOL bUseResourceFileLocale);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the active locale ID (resource file locale or current user locale).
	// Returns:
	//     If the used active locale is the resource file locale, then the
	//     resource file locale ID is returned.
	//     If the used active locale is the current user locale, then
	//     LOCALE_USER_DEFAULT is returned.
	// See Also:
	//     IsUseResourceFileLocale, SetUseResourceFileLocale,
	//     CXTPResourceManager::GetResourcesLangID, LOCALE_USER_DEFAULT
	//-----------------------------------------------------------------------
	static LCID AFX_CDECL GetActiveLCID();

	//-----------------------------------------------------------------------
	// Summary:
	//     Changes the type of a VARIANT value using the active locale ID.
	// Parameters:
	//     rVarValue   - [in, out] Reference to a VARIANT value to change the type of.
	//     vartype     - [in] Type to change the VARIANT value to.
	//     bThrowError - [in] TRUE to make the function throw an exception in
	//                        the event that an error occurs,
	//                        FALSE to make the function return TRUE or FALSE.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     GetActiveLCID, ::VariantChangeTypeEx API function.
	//-----------------------------------------------------------------------
	static BOOL AFX_CDECL VariantChangeTypeEx(VARIANT& rVarValue, VARTYPE vartype,
											  BOOL bThrowError = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//      Formats a string using strftime() function format specifiers.
	//      The active locale ID is used.
	// Parameters:
	//      dt                - [in] Reference to a COleDateTime object
	//                               containing a date-time value to format.
	//      lpcszFormatString - [in] Format-control string.
	// Returns:
	//      A string containing a date-time formatted using the active locale ID.
	// See Also:
	//      GetActiveLCID, strftime
	//-----------------------------------------------------------------------
	static CString AFX_CDECL FormatDateTime(const COleDateTime& dt, LPCTSTR lpcszFormatString);

private:
	static BOOL s_bUseResourceFileLocale;

private:
	static CString AFX_CDECL _FormatDateTime(const COleDateTime& dt, LPCTSTR lpcszFormatString,
											 LCID lcLocaleID);

	static void AFX_CDECL _InitMappedSpecs();

	static void AFX_CDECL _ProcessMappedSpecs(CString& rstrFormat, const SYSTEMTIME* pST,
											  LCID lcLocaleID);
	static void AFX_CDECL _ProcessDateTimeSpecs(CString& rstrFormat, const SYSTEMTIME* pST,
												LCID lcLocaleID);
	static void AFX_CDECL __ProcessDate_x(CString& rstrFormat, const SYSTEMTIME* pST,
										  LCID lcLocaleID);
	static void AFX_CDECL __ProcessTime_X(CString& rstrFormat, const SYSTEMTIME* pST,
										  LCID lcLocaleID);
	static void AFX_CDECL _ProcessOtherSpecs(CString& rstrFormat, const COleDateTime& dt);

private:
	struct XTP_TIMESPEC
	{
		LPCTSTR pcszSpec;
		LPCTSTR pcszFormat;
		BOOL bTime;
	};

	static CArray<XTP_TIMESPEC, XTP_TIMESPEC&> s_arMappedSpecs;

	static void AFX_CDECL _AddsMappedSpec(LPCTSTR pcszSpec, LPCTSTR pcszFormat, BOOL bTime);
};

//////////////////////////////////////////////////////////////////////////

AFX_INLINE CXTPGridPaintManager* CXTPGridControl::GetPaintManager() const
{
	return m_pPaintManager;
}
AFX_INLINE CXTPGridNavigator* CXTPGridControl::GetNavigator() const
{
	return m_pNavigator;
}
AFX_INLINE CXTPGridSelectedRows* CXTPGridControl::GetSelectedRows() const
{
	return m_pSelectedRows;
}

AFX_INLINE CXTPGridColumns* CXTPGridControl::GetColumns() const
{
	return m_pColumns;
}
AFX_INLINE XTPGridMouseMode CXTPGridControl::GetMouseMode() const
{
	return m_mouseMode;
}
AFX_INLINE BOOL CXTPGridControl::IsChanged() const
{
	return m_bChanged;
}
AFX_INLINE void CXTPGridControl::SetChanged(BOOL bChanged)
{
	m_bChanged = bChanged;
}
AFX_INLINE void CXTPGridControl::ShowGroupBy(BOOL bEnable)
{
	//  ASSERT(!IsVirtualMode());
	m_bGroupByEnabled = bEnable;
	AdjustLayout();
	AdjustScrollBars();
	RedrawControl();
}
AFX_INLINE BOOL CXTPGridControl::IsGroupByVisible() const
{
	return m_bGroupByEnabled;
}
AFX_INLINE void CXTPGridControl::ShowHeader(BOOL bShow /*= TRUE*/)
{
	m_bHeaderVisible = bShow;
	AdjustLayout();
	AdjustScrollBars();
}
AFX_INLINE BOOL CXTPGridControl::IsHeaderVisible() const
{
	return m_bHeaderVisible;
}
AFX_INLINE void CXTPGridControl::ShowFooter(BOOL bShow /*= TRUE*/)
{
	m_bFooterVisible = bShow;
	AdjustLayout();
	AdjustScrollBars();
}
AFX_INLINE BOOL CXTPGridControl::IsFooterVisible() const
{
	return m_bFooterVisible;
}
AFX_INLINE BOOL CXTPGridControl::IsMultipleSelection() const
{
	return m_bBlockSelection;
}
AFX_INLINE void CXTPGridControl::SetMultipleSelection(BOOL bSet)
{
	m_bBlockSelection = bSet;
}

AFX_INLINE BOOL CXTPGridControl::IsMultiSelectionMode() const
{
	return m_bMultiSelectionMode;
}

AFX_INLINE void CXTPGridControl::SetMultiSelectionMode(BOOL bSet)
{
	m_bMultiSelectionMode = bSet;
}

AFX_INLINE void CXTPGridControl::EnableToolTips(BOOL bEnable)
{
	m_bShowTooltips = bEnable;
}
AFX_INLINE void CXTPGridControl::SkipGroupsFocus(BOOL bSkipFocus)
{
	m_bSkipGroupsFocus = bSkipFocus;
}
AFX_INLINE BOOL CXTPGridControl::IsSkipGroupsFocusEnabled() const
{
	return m_bSkipGroupsFocus;
}
AFX_INLINE CXTPImageManager* CXTPGridControl::GetImageManager() const
{
	return m_pImageManager;
}
AFX_INLINE CXTPGridColumn* CXTPGridControl::GetFocusedColumn() const
{
	return m_pFocusedColumn;
}
AFX_INLINE void CXTPGridControl::FocusSubItems(BOOL bFocusSubItems)
{
	m_bFocusSubItems = bFocusSubItems;
	m_pFocusedColumn = NULL;
}
AFX_INLINE CXTPGridInplaceEdit* CXTPGridControl::GetInplaceEdit() const
{
	return m_pInplaceEdit;
}
AFX_INLINE CXTPGridInplaceButtons* CXTPGridControl::GetInplaceButtons() const
{
	return m_pInplaceButtons;
}
AFX_INLINE CXTPGridInplaceList* CXTPGridControl::GetInplaceList() const
{
	return m_pInplaceList;
}
AFX_INLINE CXTPGridRecordItem* CXTPGridControl::GetActiveItem() const
{
	return m_pActiveItem;
}
AFX_INLINE void CXTPGridControl::EnsureFocusedRowVisible(BOOL bEnsureFocusedRowVisible)
{
	m_bEnsureFocusedRowVisible = bEnsureFocusedRowVisible;
}
AFX_INLINE BOOL CXTPGridControl::IsEnsureFocusedRowVisible() const
{
	return m_bEnsureFocusedRowVisible;
}

AFX_INLINE BOOL CXTPGridControl::IsFocusSubItems() const
{
	return m_bFocusSubItems;
}

AFX_INLINE BOOL CXTPGridControl::IsDoubleBuffering() const
{
	return m_bDoubleBuffering;
}

AFX_INLINE void CXTPGridControl::EditOnClick(BOOL bEditOnClick)
{
	if (bEditOnClick)
	{
		m_bEditOnDelayClick = FALSE;
	}
	m_bEditOnClick = bEditOnClick;
}

AFX_INLINE BOOL CXTPGridControl::IsEditOnClick() const
{
	return m_bEditOnClick;
}

AFX_INLINE void CXTPGridControl::EditOnDelayClick(BOOL bEditOnDelayClick)
{
	if (bEditOnDelayClick)
	{
		m_bEditOnClick = FALSE;
	}
	m_bEditOnDelayClick = bEditOnDelayClick;
}

AFX_INLINE BOOL CXTPGridControl::IsEditOnDelayClick() const
{
	return m_bEditOnDelayClick;
}

AFX_INLINE void CXTPGridControl::EditOnDoubleClick(BOOL bEditOnClick)
{
	m_bEditOnDoubleClick = bEditOnClick;
}

AFX_INLINE BOOL CXTPGridControl::IsEditOnDoubleClick() const
{
	return m_bEditOnDoubleClick;
}

AFX_INLINE int CXTPGridControl::GetLastRqstEditRow() const
{
	return m_iLastRqstEditRow;
}

AFX_INLINE int CXTPGridControl::GetLastRqstEditCol() const
{
	return m_iLastRqstEditCol;
}

// Information for delay editing.
AFX_INLINE UINT_PTR CXTPGridControl::GetDelayEditTimer() const
{
	return m_uiDelayEditTimer;
}

AFX_INLINE void CXTPGridControl::SetLastRqstEdit(int iLastRqstEditRow, int iLastRqstEditCol)
{
	m_iLastRqstEditRow = iLastRqstEditRow;
	m_iLastRqstEditCol = iLastRqstEditCol;
}

AFX_INLINE CString CXTPGridControl::GetFilterText()
{
	return m_strFilterText;
}

AFX_INLINE void CXTPGridControl::SetFilterText(LPCTSTR strFilterText)
{
	m_strFilterText = strFilterText;
}

AFX_INLINE CXTPGridHeader* CXTPGridControl::GetGridHeader() const
{
	return m_pGridHeader;
}

AFX_INLINE void CXTPGridControl::SetAutoCheckItems(BOOL bAutoCheck)
{
	m_bAutoCheckItems = bAutoCheck;
}

AFX_INLINE BOOL CXTPGridControl::IsAutoCheckItems() const
{
	return m_bAutoCheckItems;
}

AFX_INLINE int CXTPGridControl::GetDisableReorderColumnsCount() const
{
	return m_nDisableReorderColumnsCount;
}

AFX_INLINE int CXTPGridControl::GetLockUpdateCount() const
{
	return m_nLockUpdateCount;
}

AFX_INLINE void CXTPGridControl::SetRowsCompareFunc(XTPGridRowsCompareFunc pCompareFunc)
{
	m_pRowsCompareFunc = pCompareFunc;
}

AFX_INLINE int CXTPGridControl::GetHScrollStep() const
{
	return m_nScrollStepH;
}

AFX_INLINE void CXTPGridControl::SetHScrollStep(int nStep)
{
	ASSERT(nStep > 0);
	m_nScrollStepH = max(1, nStep);
}

AFX_INLINE UINT CXTPGridControl::GetAutoVScrollTimerResolution() const
{
	return m_nAutoVScrollTimerResolution;
}

AFX_INLINE BOOL CXTPGridControl::IsFooterRowsPinned() const
{
	return m_bPinFooterRows;
}

AFX_INLINE void CXTPGridControl::PinFooterRowsPrinted(BOOL bPin)
{
	m_bPinFooterRowsPrinted = bPin;
}

AFX_INLINE BOOL CXTPGridControl::IsFooterRowsPinnedPrinted() const
{
	return m_bPinFooterRowsPrinted;
}

AFX_INLINE BOOL CXTPGridControl::IsPreviewAllowEdit() const
{
	return m_bPreviewAllowEdit;
}

AFX_INLINE void CXTPGridControl::PreviewAllowEdit(BOOL bAllowEdit)
{
	m_bPreviewAllowEdit = bAllowEdit;
}

AFX_INLINE void CXTPGridControl::SetSortRecordChilds(BOOL bSortRecordChildren)
{
	m_bSortRecordChilds = bSortRecordChildren;
}

AFX_INLINE BOOL CXTPGridControl::IsSortRecordChilds() const
{
	return m_bSortRecordChilds;
}

AFX_INLINE BOOL CXTPGridControl::IsFilterHiddenColumns() const
{
	return m_bFilterHiddenColumns;
}

AFX_INLINE void CXTPGridControl::SetFilterHiddenColumns(BOOL bFilterHidden)
{
	m_bFilterHiddenColumns = bFilterHidden;
}

AFX_INLINE int CXTPGridControl::GetRecordsTreeFilterMode() const
{
	return m_nRecordsTreeFilterMode;
}

AFX_INLINE void CXTPGridControl::SetRecordsTreeFilterMode(int nMode)
{
	m_nRecordsTreeFilterMode = nMode;
}

AFX_INLINE BOOL CXTPGridControl::IsInitialSelectionEnabled() const
{
	return m_bInitialSelectionEnable;
}

AFX_INLINE BOOL CXTPGridControl::IsRowFocusVisible() const
{
	return m_bRowFocusVisible;
}

AFX_INLINE void CXTPGridControl::ShowRowFocus(BOOL bShow)
{
	m_bRowFocusVisible = bShow;
}

AFX_INLINE int CXTPGridControl::GetWatermarkAlignment() const
{
	return m_WatermarkAlignment;
}

AFX_INLINE void CXTPGridControl::SetWatermarkAlignment(int nWatermarkAlignment)
{
	m_WatermarkAlignment = (XTPGridWatermarkAlignment)nWatermarkAlignment;
}

AFX_INLINE BOOL CXTPGridControl::IsIconView() const
{
	return m_bIconView;
}

AFX_INLINE int CXTPGridControl::GetRowsPerLine() const
{
	return m_iIconViewRowsPerLine;
}

AFX_INLINE int CXTPGridControl::GetLeftOffset() const
{
	return m_nScrollOffsetH;
}

AFX_INLINE void CXTPGridControl::SetDropMarkerFlags(DWORD dwDropMarkerFlags)
{
	m_dwDropMarkerFlags = dwDropMarkerFlags;
}

AFX_INLINE DWORD CXTPGridControl::GetDropMarkerFlags() const
{
	return m_dwDropMarkerFlags;
}

AFX_INLINE BOOL CXTPGridControl::IsDragMode() const
{
	return m_bDragMode;
}

AFX_INLINE int CXTPGridControl::GetDragSensitivityX() const
{
	return m_nDragSensitivityX;
}

AFX_INLINE int CXTPGridControl::GetDragSensitivityY() const
{
	return m_nDragSensitivityY;
}

AFX_INLINE void CXTPGridControl::SetFastDeselectMode(BOOL bFastDeselect)
{
	m_bFastDeselectMode = bFastDeselect;
}

AFX_INLINE CXTPGridBehavior* CXTPGridControl::GetBehavior() const
{
	return m_pBehavior;
}

AFX_INLINE void CXTPGridControl::LockExpand(BOOL bLockExpand)
{
	m_bLockExpand = bLockExpand;
}

AFX_INLINE BOOL CXTPGridControl::IsLockExpand() const
{
	return m_bLockExpand;
}

AFX_INLINE XTPGridPaintTheme CXTPGridControl::GetCurrentTheme() const
{
	return m_themeCurrent;
}

AFX_INLINE void CXTPGridControl::SetReportStyle(XTPGridOrientation orientation,
												XTPGridGridStyle gridStyle)
{
	SetGridStyle(orientation, gridStyle);
}

AFX_INLINE COLORREF CXTPGridControl::SetReportColor(COLORREF clrGridLine)
{
	return SetGridColor(clrGridLine);
}

AFX_INLINE void CXTPGridControl::SetReportHeader(CXTPGridHeader* pGridHeader)
{
	SetGridHeader(pGridHeader);
}

AFX_INLINE void CXTPGridControl::SetReportStyle(BOOL bVertical, XTPGridGridStyle gridStyle)
{
	SetGridStyle(bVertical, gridStyle);
}

AFX_INLINE XTPGridGridStyle CXTPGridControl::GetReportStyle(XTPGridOrientation orientation) const
{
	return GetGridStyle(orientation);
}

AFX_INLINE int CXTPGridControl::GetReportAreaRows(int nStartRow, BOOL bMoveDown)
{
	return GetGridAreaRows(nStartRow, bMoveDown);
}

AFX_INLINE CXTPGridHeader* CXTPGridControl::GetReportHeader() const
{
	return GetGridHeader();
}

AFX_INLINE XTPGridGridStyle CXTPGridControl::GetReportStyle(BOOL bVertical) const
{
	return GetGridStyle(bVertical);
}

AFX_INLINE CRect CXTPGridControl::GetReportRectangle() const
{
	return GetGridRectangle();
}

//----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPPromptDlg : public CDialog
{
	DECLARE_DYNAMIC(CXTPPromptDlg)
public:
	CXTPPromptDlg(CWnd* pParent = NULL);
	virtual ~CXTPPromptDlg();
	virtual void OnOK();
	virtual INT_PTR DoModal();
	virtual BOOL OnInitDialog();
	CString m_sName;
};

extern UINT XTP_WM_GRID_SETTHEME;

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPGRIDCONTROL_H__)
