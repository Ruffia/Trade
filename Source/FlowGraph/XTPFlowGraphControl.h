// XTPFlowGraphControl.h: interface for the CXTPFlowGraphControl class.
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
#if !defined(__XTPFLOWGRAPHCONTROL_H__)
#	define __XTPFLOWGRAPHCONTROL_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPFlowGraphPaintManager;
class CXTPFlowGraphPage;
class CXTPFlowGraphPages;
class CXTPFlowGraphDrawContext;
class CXTPFlowGraphNode;
class CXTPFlowGraphNodeCustom;
class CXTPPropExchange;
class CXTPFlowGraphSelectedElements;
class CXTPFlowGraphConnection;
class CXTPFlowGraphElement;
class CXTPFlowGraphConnectionPoint;
class CXTPFlowGraphUndoManager;
class CXTPFlowGraphImages;
class CXTPToolTipContext;
class CXTPFlowGraphPageHistory;
class CXTPMarkupContext;

class CXTPMarkupDependencyProperty;

//-------------------------------------------------------------------------
// Summary:
//     Node resizing options (drag/drop size of Nodes).
// Remarks:
//     To resize a Node, you must click the border of the Node and drag it
//     to the desired width/height.
//     Set AllowResizeNodes to TRUE to enable resizable Nodes. You can
//     set the Lock property of a single Node if you don't want it moved
//     and resized.
// See Also:
//     CXTPFlowGraphControl::SetAllowResizeNodes@XTPFlowGraphResize
//-------------------------------------------------------------------------
enum XTPFlowGraphResize
{
	xtpFlowGraphResizeNone		 = 0,
	xtpFlowGraphResizeHorizontal = 1, // Nodes can be resized horizontally only.
	xtpFlowGraphResizeVertical   = 2, // Nodes can be resized vertically only.
	xtpFlowGraphResizeBoth		 = 3  // Nodes can be resized both vertically and horizontally.
};

//-------------------------------------------------------------------------
// Summary:
//     Draw quality options for the flow graph.
// Remarks:
//     Set SmoothingMode to the setting that best matches your systems
//     capabilities.
//
//     These option greatly affect the performance of the control and
//     will in part be affected by the graphics capability of the machine
//     that uses the control. xtpFlowGraphSmoothingModeHighQuality is
//     recommended when you do not intend for the nodes to be dragged by the
//     user. If dragging is enabled, then this setting is most affected by
//     the systems graphic capabilities.
// See Also:
//     CXTPFlowGraphControl::SetSmoothingMode@XTPFlowGraphSmoothingMode
//-------------------------------------------------------------------------
enum XTPFlowGraphSmoothingMode
{
	xtpFlowGraphSmoothingModeHighQuality, // Uses GDI+ to draw the flow graph for the highest
										  // quality picture. This option looks the best, but might
										  // be slow when performing drag operations on lower end
										  // graphic cards.
	xtpFlowGraphSmoothingModeHighSpeed,   // Uses GDI+ to draw the flow graph with slightly lower
										  // quality to improve speed over the High Quality option.
	xtpFlowGraphSmoothingModeGDI,  // Uses GDI Standard to draw the flow graph. Performance will be
								   // good on lower end graphic cards; however, the quality does not
								   // look as good as the GDI+ options.
	xtpFlowGraphSmoothingModeAuto, // Will combine the various quality modes to get the best
								   // performance and quality. It will draw with a higher quality
								   // mode for viewing and then switch to a lower quality mode when
								   // performing drag operations.
};

//-------------------------------------------------------------------------
// Summary:
//     Custom XAML properties used in CXTPFlowGraphNode and CXTPFlowGraphNodeCustom.
//-------------------------------------------------------------------------
enum XTPFlowGraphCustomProperty
{
	xtpFlowGraphPropertyConnectionPoint = 0,	 // Applied to default nodes only. Tells the control
												 // that a given <Rectangle> is a connection point.
	xtpFlowGraphPropertyConnectionPointType = 1, // Applied to default nodes only. Tells the control
												 // that a given XAML element can have a color.
	xtpFlowGraphPropertyCanSetColor = 2, // Applied to default nodes only. Tells the control that a
										 // given XAML element can have a color.
	xtpFlowGraphPropertyConnectionPointIn =
		3, // Applied to custom nodes only. Tells the control of how to place connection points
		   // within the given XAML shape can be applied to canvas on custom markup nodes when the
		   // markup doesn't contain a path.
	xtpFlowGraphPropertyResizable = 4 // Applied to custom Nodes only. Tells the control that a
									  // given shape is resizable or not. A shape is resizable when
									  // this property is omitted.
};

//-------------------------------------------------------------------------
// Summary:
//     Default XAML templates for a default node and its table rows with connection points.
//-------------------------------------------------------------------------
enum XTPFlowGraphDefaultXAMLTemplate
{
	xtpFlowGraphTemplateNode		   = 0, // A Node.
	xtpFlowGraphTemplateNodePointNone  = 1, // Node table row with no connection points.
	xtpFlowGraphTemplateNodePointIn	= 2, // Node table row with input connection point.
	xtpFlowGraphTemplateNodePointOut   = 3, // Node table row with output connection point.
	xtpFlowGraphTemplateNodePointInOut = 4  // Node table row with input & output connection points.
};

//=========================================================================
// Summary:
//     Flow Graph Control. Used to display relationship graphs.
// Remarks:
//     The Flow Graph Control is used to display relationship graphs. This
//     can be thought of as a relationship diagram commonly used to
//     display the relationships for a relational database.
//
//     The control consists of "Nodes" that can be thought of as tables
//     in a database. The Nodes contain "connections" (fields of a
//     table). The connections can have many "connection points" that can
//     be used to link the connections between different nodes (i.e.
//     linking an invoice table to a customer table via a connection).
//
//     Nodes and connection lines can be dragged around the graph
//     allowing the user to fully customize the look of their
//     relationship diagram.
//
//     The control also supports undo/redo edit operations in the case
//     you allow the nodes to be moved, renamed, connections
//     added/removed, etc.
//
//     Several different zoom navigation functions allow the control to
//     zoom to a specific Node, zoom to a range of Nodes, zoom to
//     the selection and fit it to the window.
//
//     Page transitions can be used to transition from one database to
//     another.
//=========================================================================
class _XTP_EXT_CLASS CXTPFlowGraphControl : public CWnd
{
	DECLARE_DYNAMIC(CXTPFlowGraphControl)

public:
	//---------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphControl object.
	//---------------------------------------------------------------------
	CXTPFlowGraphControl();
	//---------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphControl object, handles cleanup and
	//     deallocation.
	//---------------------------------------------------------------------
	virtual ~CXTPFlowGraphControl();

public:
	//---------------------------------------------------------------------
	// Summary:
	//     Sets the active page.
	// Remarks:
	//     The flow graph can contain many pages that each have their own set
	//     of Nodes/connections. GetActivePage will retrieve a reference to the page
	//     currently in view. Note that only one page can be viewed at a time.
	// Parameters:
	//     pPage - Reference to the page to make active.
	//---------------------------------------------------------------------
	void SetActivePage(CXTPFlowGraphPage* pPage);

	//---------------------------------------------------------------------
	// Summary:
	//     Collection of all pages in the flow graph.
	// Remarks:
	//     A page is what contains all the elements in the flow graph. The
	//     flow graph can contain many pages, each of which can contain
	//     Nodes, Node groups, and connections. A page is the first thing that
	//     you must add to the flow graph and then you can add elements
	//     to the pages.
	// Returns:
	//     Reference to the collection of all pages in the flow graph.
	//---------------------------------------------------------------------
	CXTPFlowGraphPages* GetPages() const;

	//---------------------------------------------------------------------
	// Summary:
	//     This method is called whenever the flow graph or any of its
	//     elements have changed.
	// Remarks:
	//     For example, this method is called when the page zoom level, page scroll
	//     offset, window/Node size, window handle, node style, node color,
	//     or connection style is changed.
	//---------------------------------------------------------------------
	virtual void OnGraphChanged();

public:
	//---------------------------------------------------------------------
	// Summary:
	//     Retrieves the FlowGraphPaintManager object used for drawing the
	//     flow graph.
	// Remarks:
	//     The paint manager holds many properties to change the various
	//     colors and other visual properties used in the flow graph.
	// Returns:
	//     The FlowGraphPaintManager object used for drawing the flow graph.
	//---------------------------------------------------------------------
	CXTPFlowGraphPaintManager* GetPaintManager() const;

	//---------------------------------------------------------------------
	// Summary:
	//     Sets the FlowGraphPaintManager object used for drawing the
	//     flow graph.
	// Remarks:
	//     The paint manager holds many properties to change the various
	//     colors and other visual properties used in the flow graph.
	// Parameters:
	//     pPaintManager - Reference to the FlowGraphPaintManager object to
	//                     use for drawing the flow graph.
	//---------------------------------------------------------------------
	void SetPaintManager(CXTPFlowGraphPaintManager* pPaintManager);

	//---------------------------------------------------------------------
	// Summary:
	//     Retrieves a reference to all the images that the flow graph can use.
	// Remarks:
	//     Use this method to manage all images for the flow graph.
	// Returns:
	//     A reference to all the images that the flow graph can use.
	//---------------------------------------------------------------------
	CXTPFlowGraphImages* GetImages() const;

public:
	//---------------------------------------------------------------------
	// Summary:
	//     Exchanges layout and state information to/from a PropExchange
	//     object.
	// Remarks:
	//     The PropExchange object provides a universal solution to
	//     save the layout and state of the flow graph to a single file
	//     or the system registry with support to write/load directly
	//     to/from a file or registry.
	// Parameters:
	//     pPX - PropExhange object to exchange data with.
	//---------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

public:
	//---------------------------------------------------------------------
	// Summary:
	//     Called by the framework to draw the flow graph control.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	// Remarks:
	//     This will redraw the control's background and call OnDraw for the
	//     active page.
	//---------------------------------------------------------------------
	virtual void OnDraw(CXTPFlowGraphDrawContext* pDC);

	//---------------------------------------------------------------------
	// Summary:
	//     Updates the position of elements in the graph.
	// Remarks:
	//     Call this method after applying changes in code that require a
	//     reposition of elements of the flow graph (e.g. adding nodes).
	//---------------------------------------------------------------------
	virtual void Reposition();

	//---------------------------------------------------------------------
	// Summary:
	//     Redraws the control.
	// Remarks:
	//     Call this method after applying changes in code that require a
	//     visual redraw (e.g. color).
	//---------------------------------------------------------------------
	virtual void RedrawControl();

	//---------------------------------------------------------------------
	// Summary:
	//     Retrieves a reference to the currently active page.
	// Remarks:
	//     The flow graph can contain many pages that each have their own set
	//     of Nodes/connections. GetActivePage will retrieve a reference to the page
	//     currently in view. Note that only one page can be viewed at a time.
	// Returns:
	//     A reference to the currently active page.
	//---------------------------------------------------------------------
	CXTPFlowGraphPage* GetActivePage() const;

	//---------------------------------------------------------------------
	// Summary:
	//     Gets an object that keeps track of page history and allows for the
	//     history to be navigated.
	// Returns:
	//     A FlowGraphPageHistory object used to navigate the history of the pages.
	//---------------------------------------------------------------------
	CXTPFlowGraphPageHistory* GetHistory() const;

	//---------------------------------------------------------------------
	// Summary:
	//     Gets the manager used to Undo and Redo edit operations in the flow graph.
	// Remarks:
	//     The FlowGraphUndoManager allows for certain edit operations to be
	//     undone and re-done in the Flow Graph. Supported operations are
	//     Add/Remove/Move/Resize Nodes/Move Connector Lines/Rename Nodes/Grouping.
	//     If any of these operations are performed, then they can easily
	//     be undone/re-done using the methods of the FlowGraphUndoManager.
	//
	//     StartGroup and EndGroup are used to "group" a set of "undoable"
	//     operations together. Grouping the operations together is useful
	//     since the grouped operations can be Undone or Re-done with a single
	//     call to Undo or Redo. If the operations are not grouped, then
	//     you would have to call Undo or Redo for each operation performed.
	//
	//     Any "undoable" operations performed between StartGroup and EndGroup
	//     will be undone/re-done with a single call to Undo or Redo.
	// Returns:
	//     Reference to the FlowGraphUndoManager.
	//---------------------------------------------------------------------
	CXTPFlowGraphUndoManager* GetUndoManager() const;

	//---------------------------------------------------------------------
	// Summary:
	//     Registers the window class if it has not already been registered.
	// Parameters:
	//     hInstance - Instance of the resource where the control is located.
	// Returns:
	//     TRUE if the window class was successfully registered.
	//---------------------------------------------------------------------
	BOOL RegisterWindowClass(HINSTANCE hInstance = NULL);

	//---------------------------------------------------------------------
	// Summary:
	//     Call this method to create a flow graph control.
	// Parameters:
	//     dwStyle -     Styles to use.
	//     rect -        Specifies the size and position of the item.
	//     pParentWnd -  Pointer to the parent window.
	//     nID -         ID to use for this control.
	//---------------------------------------------------------------------
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	using CWnd::Create;

public:
	//---------------------------------------------------------------------
	// Summary:
	//     Gets the current zoom level of the control.
	// Remarks:
	//     0.0 is 0 zoom and 1.0 is 100% zoom.
	// Returns:
	//     The current zoom level of the control.
	//---------------------------------------------------------------------
	double GetZoomLevel() const;

	//---------------------------------------------------------------------
	// Summary:
	//     Sets the current zoom level for the control.
	// Parameters:
	//     dZoomLevel - New zoom level to use.
	// Remarks:
	//     0.0 is 0 zoom and 1.0 is 100% zoom.
	//---------------------------------------------------------------------
	void SetZoomLevel(double dZoomLevel);

	//---------------------------------------------------------------------
	// Summary:
	//     Gets the scroll offset (how much the page has been scrolled). This
	//     retrieves the X/Y coordinates excluding the left/right offset.
	// Returns:
	//     The scroll offset (how much the page has been scrolled). This
	//     retrieves a point that contains the X/Y coordinates excluding
	//     the left/right offset.
	//---------------------------------------------------------------------
	CPoint GetScrollOffset() const;

	//---------------------------------------------------------------------
	// Summary:
	//     Call this method to set the minimum and maximum zoom percentages
	//     that are allowed when using the various zoom methods.
	// Remarks:
	//     By default, there is no zoom restriction. 0.0 is 0 and 1.0 is 100%.
	// Parameters:
	//     dMinZoom - Minimum zoom value (0.0 \- 1.0).
	//     dMaxZoom - Maximum zoom value (0.0 \- 1.0).
	//---------------------------------------------------------------------
	void SetZoomRange(double dMinZoom, double dMaxZoom);

	//---------------------------------------------------------------------
	// Summary:
	//     This method uses the current zoom level and scroll offset to transform
	//     a point in logical coordinates to current drawing coordinates.
	// Parameters:
	//     pt - Point in logical coordinates.
	// Returns:
	//     The transformed current drawing coordinates.
	//---------------------------------------------------------------------
	CPoint TransformPoint(CPoint pt) const;

	//---------------------------------------------------------------------
	// Summary:
	//     Zooms to the currently selected Nodes.
	// Remarks:
	//     This will fill the entire view with the currently selected Nodes.
	//---------------------------------------------------------------------
	virtual void ZoomToSelection();

	//---------------------------------------------------------------------
	// Summary:
	//     Fits all the Nodes in the active page to the window.
	// Remarks:
	//     The contents of the active page will zoom in or out to fit all
	//     content to the full size of the page.
	//---------------------------------------------------------------------
	virtual void ZoomFitToWindow();

	//---------------------------------------------------------------------
	// Summary:
	//     Zooms the view to a specific set of coordinates on the active page.
	// Remarks:
	//     This will zoom the view to the specified coordinates.
	// Parameters:
	//     lpRect -    Specifies the coordinates of the rectangle to zoom to.
	//     bAnimate -  TRUE to use animation wile zooming.
	//---------------------------------------------------------------------
	virtual void ZoomToRect(LPCRECT lpRect, BOOL bAnimate = TRUE);

	//---------------------------------------------------------------------
	// Summary:
	//     Call this method to use animation to scroll to the specified offset
	//     in the control.
	// Parameters:
	//     ptScrollOffset -  A point that represents the offset to scroll to.
	//     dZoomLevel     -  Level of zoom to use.
	// Remarks:
	//     0.0 is 0 zoom and 1.0 is 100% zoom.
	//---------------------------------------------------------------------
	virtual void AnimateTo(CPoint ptScrollOffset, double dZoomLevel);

	//---------------------------------------------------------------------
	// Summary:
	//     This method will transition from a Node on the current page to a Node
	//     on a different page.
	// Parameters:
	//     pNodeFrom - Reference to a Node on the current page to transition from.
	//     pNodeTo   - Reference to a Node on a different page to transition to.
	// Remarks:
	//     AnimatePageTransition will transition from a Node on the current page
	//     to a Node on a different page. This will cause the active page to
	//     change to the page that contains the Node that was transitioned to.
	//---------------------------------------------------------------------
	virtual void AnimatePageTransition(CXTPFlowGraphNode* pNodeFrom, CXTPFlowGraphNode* pNodeTo);

	//---------------------------------------------------------------------
	// Summary:
	//     Sets the delay to use between animation steps when animation is used
	//     (zooming/scrolling).
	// Parameters:
	//     nDelay - New animation delay.
	//---------------------------------------------------------------------
	void SetAnimationDelay(int nDelay);

	//---------------------------------------------------------------------
	// Summary:
	//     Gets the delay used between animation steps when animation is used
	//     (zooming/scrolling).
	// Returns:
	//     The animation delay.
	//---------------------------------------------------------------------
	int GetAnimationDelay() const;

public:
	//---------------------------------------------------------------------
	// Summary:
	//     Specifies whether the user can drag/move Nodes.
	// Parameters:
	//     bAllow - TRUE to allow Nodes to be moved, FALSE to lock them.
	//---------------------------------------------------------------------
	void SetAllowMoveNodes(BOOL bAllow);

	//---------------------------------------------------------------------
	// Summary:
	//     Gets whether the user can drag/move Nodes.
	// Returns:
	//     TRUE if nodes are allowed to be moved, FALSE if they are locked.
	//---------------------------------------------------------------------
	BOOL GetAllowMoveNodes() const;

	//---------------------------------------------------------------------
	// Summary:
	//     Sets Node resizing options (drag/drop size of nodes).
	// Remarks:
	//     To resize a Node, you must click the border of the node and drag it
	//     to the desired width/height.
	// Parameters:
	//     nResizeMode - New resizing options; must be one of the values defined
	//                   by the XTPFlowGraphResize enumeration.
	//---------------------------------------------------------------------
	void SetAllowResizeNodes(XTPFlowGraphResize nResizeMode);

	//---------------------------------------------------------------------
	// Summary:
	//     Gets Node resizing options (drag/drop size of nodes).
	// Remarks:
	//     To resize a Node, you must click the border of the node and drag it
	//     to the desired width/height.
	// Returns:
	//     Current resizing options; returns one of the values defined
	//     by the XTPFlowGraphResize enumeration.
	//---------------------------------------------------------------------
	XTPFlowGraphResize GetAllowResizeNodes() const;

	//---------------------------------------------------------------------
	// Summary:
	//     Specifies whether the user is allowed to drag/move/add/delete connections.
	// Parameters:
	//     bAllowModifyConnections - TRUE to allow the user to
	//                               drag/move/add/delete connections,
	//                               FALSE to lock them.
	//---------------------------------------------------------------------
	void SetAllowModifyConnections(BOOL bAllowModifyConnections);

	//---------------------------------------------------------------------
	// Summary:
	//     Checks whether the user is allowed to drag/move/add/delete connections.
	// Returns:
	//     TRUE if the user is allowed to drag/move/add/delete connections,
	//     FALSE if they are locked.
	//---------------------------------------------------------------------
	BOOL GetAllowModifyConnections() const;

	//---------------------------------------------------------------------
	// Summary:
	//     Sets the draw quality options for the flow graph.
	// Parameters:
	//     nSmoothingMode - Smoothing mode to use; must be an option from
	//                      XTPFlowGraphSmoothingMode.
	// Remarks:
	//     Set SmoothingMode to the setting that best matches your systems
	//     capabilities. Use one of the XTPFlowGraphSmoothingMode options for
	//     the smoothing mode that best suits your needs.
	//
	//     These options greatly affect the performance of the control and
	//     will, in part, be effected by the graphics capability of the machine
	//     that uses the control. xtpFlowGraphSmoothingModeHighQuality is
	//     recommended when you don't intend for the Nodes to be dragged by the
	//     user. If dragging is enabled, then this setting is most affected
	//     by the system's graphic capabilities.
	//---------------------------------------------------------------------
	void SetSmoothingMode(XTPFlowGraphSmoothingMode nSmoothingMode);

	//---------------------------------------------------------------------
	// Summary:
	//     Gets the draw quality options used for the flow graph.
	// Remarks:
	//     These options greatly affect the performance of the control and
	//     will, in part, be effected by the graphics capability of the machine
	//     that uses the control. xtpFlowGraphSmoothingModeHighQuality is
	//     recommended when you don't intend for the Nodes to be dragged by the
	//     user. If dragging is enabled, then this setting is most affected
	//     by the system's graphic capabilities.
	// Returns:
	//     Currently set smoothing mode; returns an option from
	//     XTPFlowGraphSmoothingMode.
	//---------------------------------------------------------------------
	XTPFlowGraphSmoothingMode GetSmoothingMode() const;

public:
	//---------------------------------------------------------------------
	// Summary:
	//     Selects all Nodes in the graph.
	// Remarks:
	//     This will select all Nodes in the graph. Selected Nodes have a
	//     "bluish" color when selected. The current selection is stored in
	//     the active page's selection property.
	//---------------------------------------------------------------------
	virtual void SelectAll();

	//---------------------------------------------------------------------
	// Summary:
	//     Auto-arranges the Nodes in the active page.
	// Remarks:
	//     This will left-align all Nodes in the active page. Nodes with
	//     connections will be placed to the right of the connected Node.
	//---------------------------------------------------------------------
	virtual void Arrange();

	//---------------------------------------------------------------------
	// Summary:
	//     Call this method to place a Node into edit mode so the user can
	//     edit the text.
	// Parameters:
	//     pNode - Reference to the Node to place into edit mode.
	//---------------------------------------------------------------------
	virtual void RenameNode(CXTPFlowGraphNode* pNode);

	//---------------------------------------------------------------------
	// Summary:
	//     Call this method to place a connection point into edit mode so the
	//     user can edit the text.
	// Parameters:
	//     pConnectionPoint - Reference to the connection point to place into
	//                        edit mode.
	//---------------------------------------------------------------------
	virtual void RenameConnectionPoint(CXTPFlowGraphConnectionPoint* pConnectionPoint);

	//---------------------------------------------------------------------
	// Summary:
	//     Helper method to load a custom XAML shape from toolkit using
	//     its resource ID.
	// Parameters:
	//     uResShapeID - Shape resource ID.
	//     pPage       - A pointer to a CXTPFlowGraphPage. If not
	//                   defined, then the active page is used.
	//---------------------------------------------------------------------
	CXTPFlowGraphNodeCustom* AddCustomNodeFromToolkitResource(UINT uResShapeID,
															  CXTPFlowGraphPage* pPage = NULL);

public:
	//---------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if there is a CXTPFlowGraphNode
	//     at the specified point.
	// Parameters:
	//     point - Point to test.
	// Returns:
	//     A reference to the CXTPFlowGraphNode found at the specified
	//     point, or NULL if no CXTPFlowGraphNode is found.
	//---------------------------------------------------------------------
	virtual CXTPFlowGraphNode* HitTestNode(CPoint point) const;

	//---------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if there is a CXTPFlowGraphConnection
	//     at the specified point.
	// Parameters:
	//     point - Point to test.
	// Returns:
	//     A reference to the CXTPFlowGraphConnection found at the specified
	//     point, or NULL if no CXTPFlowGraphConnection is found.
	//---------------------------------------------------------------------
	virtual CXTPFlowGraphConnection* HitTestConnection(CPoint point) const;

	//---------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if there is a CXTPFlowGraphConnectionPoint
	//     at the specified point.
	// Parameters:
	//     point - Point to test.
	// Returns:
	//     A reference to the CXTPFlowGraphConnectionPoint found at the specified
	//     point, or NULL if no CXTPFlowGraphConnectionPoint is found.
	//---------------------------------------------------------------------
	virtual CXTPFlowGraphConnectionPoint* HitTestConnectionArea(CPoint point) const;

public:
	//---------------------------------------------------------------------
	// Summary:
	//     Called after a label has been edited.
	// Parameters:
	//     pItem - Reference to the element that has finished editing.
	//     str   - New label for the element.
	// Remarks:
	//     This will occur after the label is edited, but before it is
	//     changed.
	//---------------------------------------------------------------------
	virtual void OnEndLabelEdit(CXTPFlowGraphElement* pItem, LPCTSTR str);

	//---------------------------------------------------------------------
	// Summary:
	//     Call this method to cancel a label edit.
	// Remarks:
	//     This method is used in the CXTPFlowGraphControl::Reposition
	//     method.
	//---------------------------------------------------------------------
	void CancelLabelEdit();

public:
	//---------------------------------------------------------------------
	// Summary:
	//     Call this method to get a pointer to the tooltip context.
	// Returns:
	//     A pointer to the tooltip context.
	//---------------------------------------------------------------------
	CXTPToolTipContext* GetToolTipContext() const;

	//---------------------------------------------------------------------
	// Summary:
	//     Save the picture to a specified file.
	// Parameters:
	//     szFile - File to save the picture to.
	// Returns:
	//     TRUE if successful.
	//---------------------------------------------------------------------
	BOOL SaveToFile(LPCTSTR szFile);

public:
	//---------------------------------------------------------------------
	// Summary:
	//     Enables/disables markup for the flow graph.
	// Parameters:
	//     bEnable - TRUE to enable markup for the flow graph,
	//               FALSE to disable markup for the flow graph.
	//---------------------------------------------------------------------
	void EnableMarkup(BOOL bEnable = TRUE);

	//---------------------------------------------------------------------
	// Summary:
	//     Enables/disables the scrollbar.
	// Parameters:
	//     nBar    - [in] The scrollbar identifier.
	//     bEnable - [in] TRUE to enable the scrollbar, FALSE to disable.
	// Remarks:
	//     If the window has a sibling scrollbar control, then that scrollbar
	//     is used. Otherwise the window's own scrollbar is used.
	// See also:
	//     void ShowScrollBar(UINT nBar, BOOL bShow = TRUE);
	//---------------------------------------------------------------------
	void EnableScrollBarCtrl(int nBar, BOOL bEnable = TRUE);

	BOOL GetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo, UINT nMask = SIF_ALL);

	BOOL SetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo, BOOL bRedraw = TRUE);

	//---------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the maximum scrolling
	//     position of the scroll bar.
	// Parameters:
	//     nBar - [in] Specifies the type of scroll bar. The parameter can take
	//            one of the following values:
	//            * SB_HORZ: Retrieves the scroll limit of the horizontal
	//                       scrollbar.
	//            * SB_VERT: Retrieves the scroll limit of the vertical
	//                       scrollbar.
	// Returns:
	//     An int value which specified the maximum scrolling position of
	//     the scrollbar if successful, otherwise 0.
	//---------------------------------------------------------------------
	int GetScrollLimit(int nBar);

	//---------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain a pointer to the specified
	//     sibling scroll bar.
	// Parameters:
	//     nBar - [in] Specifies the type of scrollbar. The parameter can take
	//            one of the following values:
	//            * SB_HORZ: Retrieves the position of the horizontal scroll bar.
	//            * SB_VERT: Retrieves the position of the vertical scroll bar.
	// Remarks:
	//      This member function does not operate on scrollbars created when the
	//      WS_HSCROLL or WS_VSCROLL bits are set during the creation of a window.
	//      The CWnd implementation of this function simply returns NULL. Derived
	//      classes, such as CView, implement the described functionality.
	// Returns:
	//      A pointer to the specified sibling scrollbar control if successful,
	//      or NULL if the specified sibling scrollbar does not exist.
	//---------------------------------------------------------------------
	virtual CScrollBar* GetScrollBarCtrl(int nBar) const;

	//---------------------------------------------------------------------
	// Summary:
	//     Retrieves a pointer to the markup context.
	// Returns:
	//     A pointer to a CXTPMarkupContext object.
	//---------------------------------------------------------------------
	CXTPMarkupContext* GetMarkupContext() const;

	//---------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve a custom markup property from the
	//     custom properties map.
	// Parameters:
	//     property - Identifies the property to retrieve.
	// Returns:
	//     A pointer to the custom markup property.
	//---------------------------------------------------------------------
	CXTPMarkupDependencyProperty* GetCustomMarkupProperty(XTPFlowGraphCustomProperty property) const;

	//---------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve a default markup template.
	// Parameters:
	//     tmpl - Identifies the template to retrieve.
	// Returns:
	//     A string containing the default markup template.
	//---------------------------------------------------------------------
	CString GetDefaultMarkupTemplate(XTPFlowGraphDefaultXAMLTemplate tmpl) const;

	//---------------------------------------------------------------------
	// Summary:
	//     Attaches custom properties to a control.
	//---------------------------------------------------------------------
	void RegisterCustomMarkupProperties();

	//---------------------------------------------------------------------
	// Summary:
	//     Sets up default Node's markup templates for a Node and its rows
	//     (used to be names connection points).
	//---------------------------------------------------------------------
	void SetupDefaultMarkupTemplates();

	//---------------------------------------------------------------------
	// Summary:
	//     Loads a markup XAML template from a resource.
	// Parameters:
	//     szTemplate - A string defining a resource of type 'XML'.
	// Returns:
	//     A string containing a markup string (applies to default Nodes).
	//---------------------------------------------------------------------
	static CString AFX_CDECL LoadXamlTemplate(LPCTSTR szTemplate);

public:
	//---------------------------------------------------------------------
	// Summary:
	//     Sends the specified message to the window.
	// Parameters:
	//     nMessage - The message to be sent.
	//     pNMHDR   - Notify header.
	// Returns:
	//     Nonzero if successful, otherwise zero.
	//---------------------------------------------------------------------
	virtual LRESULT SendNotifyMessage(UINT nMessage, NMHDR* pNMHDR = NULL) const;

protected:
	//---------------------------------------------------------------------
	// Summary:
	//     Adjusts scrollbars depending on currently visible elements
	//     (nodes, connections, etc.).
	//---------------------------------------------------------------------
	virtual void AdjustScrollBars();

protected:
	//---------------------------------------------------------------------
	// Summary:
	//     Internal method that is called when Nodes are starting to get
	//     dragged.
	//---------------------------------------------------------------------
	virtual void StartDragNodes();

	//---------------------------------------------------------------------
	// Summary:
	//     Internal method that is called when a Node starts to get resized.
	// Parameters:
	//     pNode   - Pointer to the Node that is starting to get resized.
	//     hitTest - Hit test information; must be one of the values
	//               defined by the SelectionRectHitTest enumeration.
	//---------------------------------------------------------------------
	virtual void StartResizeNode(CXTPFlowGraphNode* pNode, UINT hitTest);

	//---------------------------------------------------------------------
	// Summary:
	//     Internal method that is called when the screen has begun to get
	//     dragged.
	//---------------------------------------------------------------------
	virtual void StartDragScreen();

	//---------------------------------------------------------------------
	// Summary:
	//     Internal method that is called when the selection starts to get
	//     dragged.
	//---------------------------------------------------------------------
	virtual void StartDragSelection();

	//---------------------------------------------------------------------
	// Summary:
	//     Internal method that is called when a connection starts to get
	//     dragged.
	// Parameters:
	//     pConnection - Pointer to the connection that is starting to get
	//                   dragged.
	//---------------------------------------------------------------------
	virtual void StartDragConnection(CXTPFlowGraphConnection* pConnection);

	//---------------------------------------------------------------------
	// Summary:
	//     Internal method that is called when a connection point starts to
	//     get dragged.
	// Parameters:
	//     pPoint - Pointer to the connection point that is starting to get
	//              dragged.
	//---------------------------------------------------------------------
	virtual void StartDragConnectionPoint(CXTPFlowGraphConnectionPoint* pPoint);

	//---------------------------------------------------------------------
	// Summary:
	//     Called when the selection has changed.
	// Parameters:
	//     pt - Point where the selection changed.
	//---------------------------------------------------------------------
	virtual void OnMoveSelection(CPoint pt);

	//---------------------------------------------------------------------
	// Summary:
	//     Calculates the correct ScrollOffset and ZoomLevel needed for a
	//     given rectangle.
	// Parameters:
	//     lpRect -          [in] Rectangle within the client rectangle to
	//                       zoom to.
	//     ptScrollOffset -  [out] Will contain the needed scroll offset for
	//                       the specified rectangle to zoom.
	//     dZoomLevel -      [out] Will contain the needed zoom level for the
	//                       specified rectangle to zoom.
	// Remarks:
	//     This is a helper method that calculates the correct ScrollOffset
	//     and ZoomLevel needed for a given rectangle. You supply the
	//     rectangle and then GetZoomParam will populate ptScrollOffset and
	//     dZoomLevel with the correct values.
	//---------------------------------------------------------------------
	virtual void GetZoomParam(LPCRECT lpRect, CPoint& ptScrollOffset, double& dZoomLevel);

	//---------------------------------------------------------------------
	// Summary:
	//     Creates a CXTPFlowGraphDrawContext object that will use the
	//     specified dc to draw.
	// Parameters:
	//     dc - Pointer to a valid device context.
	// Remarks:
	//     This will retrieve CXTPFlowGraphDrawContext or
	//     CXTPFlowGraphDrawContextGdiPlus depending
	//     on the value of m_nSmoothingMode.
	// Returns:
	//     Pointer to the CXTPFlowGraphDrawContext object that will use the
	//     specified dc to draw.
	//---------------------------------------------------------------------
	virtual CXTPFlowGraphDrawContext* CreateDrawContext(CDC& dc);

	// returns true when at least node was reset
	bool ResetSelectedNodes(CXTPFlowGraphNode* pNodeToSkip = NULL);

	void UpdateActiveNodeAtPoint(CPoint pt, CXTPFlowGraphNode* pNodeToSkip = NULL);

	CXTPFlowGraphNode* IsPointInSizeRectOfSelectedNode(CPoint point, UINT& uHitTest) const;

	//{{AFX_CODEJOCK_PRIVATE
protected:
	DECLARE_MESSAGE_MAP()

	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	//////////////////////////////////////////////////////////////////////////
	// custom scrollbars related:
	//////////////////////////////////////////////////////////////////////////

	void RepositionCustomScrollBars();

	void DeleteCustomScrollBars();

public:
	void GetClientRect(LPRECT lpRect) const;
	void SetScrollbarTheme(XTPScrollBarTheme theme);

	//////////////////////////////////////////////////////////////////////////
	// end
	//////////////////////////////////////////////////////////////////////////

protected:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_CODEJOCK_PRIVATE

protected:
	CBitmap m_bmpCache; // Bitmap Cache.
	BOOL m_bReposition; // Specifies whether the control should reposition itself.
	CXTPFlowGraphPaintManager* m_pPaintManager; // Pointer to the paint manager used to draw the
												// flow graph.
	CXTPFlowGraphPage* m_pActivePage;			// Pointer to the currently active page.
	CXTPFlowGraphPages* m_pPages;				// Pointer to the flow graph's collection of pages.
	BOOL m_bAdjustScrollBars;					// Specifies whether to adjust the scrollbars.

	CPoint m_ptStartDrag; // Point to start dragging.
	CPoint m_ptMouse;	 // Current point the mouse cursor is located.

	CRect m_rcSelectedArea; // Bounding rectangle of the currently selected area.

	CXTPFlowGraphUndoManager* m_pUndoManager; // The pointer to the undo manager object.

	CXTPFlowGraphImages* m_pImages; // Image collection for the flow graph control.

	double m_dMinZoom; // Minimum zoom level allowed for the flow graph control.
	double m_dMaxZoom; // Maximum zoom level allowed for the flow graph control.

	CXTPToolTipContext* m_pToolTipContext; // Tooltip Context.

	int m_nAnimationDelay; // Delay between animation steps used when zooming or scrolling.

	CXTPFlowGraphPageHistory* m_pHistory; // Object that keeps track of page history and allows the
										  // history to be navigated.

	BOOL m_bAllowMoveNodes; // Specifies whether the user can drag/move Nodes. TRUE to allow Nodes
							// to be moved, FALSE to lock them.
	XTPFlowGraphResize m_nAllowResizeNodes; // Node resizing options (drag/drop size of Nodes).
	BOOL m_bAllowModifyConnections;			// Specifies whether the user can drag/move/add/delete
											// connections. TRUE to allow the user to
											// drag/move/add/delete connections, FALSE to lock them.

	BOOL m_bInAction; // Internally used to tell the flow graph it is in animation (scrolling,
					  // zooming) or dragging.

	XTPFlowGraphSmoothingMode m_nSmoothingMode; // Draw quality options for the flow graph.

	CXTPMarkupContext* m_pMarkupContext; // Markup context of flow graph.

	static CMap<XTPFlowGraphCustomProperty, XTPFlowGraphCustomProperty,
				CXTPMarkupDependencyProperty*, CXTPMarkupDependencyProperty*&>
		m_mapCustomProperties;

	// Map of Nodes and its table rows markup templates (default connection points) that depend on a
	// connection point type.
	CMap<XTPFlowGraphDefaultXAMLTemplate, XTPFlowGraphDefaultXAMLTemplate, CString, CString&>
		m_mapDefaultXamlTemplates;

	BOOL m_bCreateScrollbarOnParent; // TRUE if parent window scrollbars are used, otherwise FALSE.

	CScrollBar* m_pCustomScrollBars[2];

private:
	//{{AFX_CODEJOCK_PRIVATE
	struct DRAGNODE
	{
		CPoint ptOrigin;
		CXTPFlowGraphElement* pElement;
		BOOL bNode;
	};
	CArray<DRAGNODE, DRAGNODE&> m_arrDragNodes;
	//}}AFX_CODEJOCK_PRIVATE

	friend class CFlowGraphCtrl;
};

AFX_INLINE CXTPFlowGraphPaintManager* CXTPFlowGraphControl::GetPaintManager() const
{
	return m_pPaintManager;
}
AFX_INLINE CXTPFlowGraphPages* CXTPFlowGraphControl::GetPages() const
{
	return m_pPages;
}
AFX_INLINE CXTPFlowGraphPage* CXTPFlowGraphControl::GetActivePage() const
{
	return m_pActivePage;
}
AFX_INLINE CXTPFlowGraphUndoManager* CXTPFlowGraphControl::GetUndoManager() const
{
	return m_pUndoManager;
}
AFX_INLINE void CXTPFlowGraphControl::SetZoomRange(double dMinZoom, double dMaxZoom)
{
	m_dMinZoom = dMinZoom;
	m_dMaxZoom = dMaxZoom;
}
AFX_INLINE CXTPToolTipContext* CXTPFlowGraphControl::GetToolTipContext() const
{
	return m_pToolTipContext;
}
AFX_INLINE void CXTPFlowGraphControl::SetAnimationDelay(int nDelay)
{
	m_nAnimationDelay = nDelay;
}
AFX_INLINE int CXTPFlowGraphControl::GetAnimationDelay() const
{
	return m_nAnimationDelay;
}
AFX_INLINE CXTPFlowGraphImages* CXTPFlowGraphControl::GetImages() const
{
	return m_pImages;
}
AFX_INLINE CXTPFlowGraphPageHistory* CXTPFlowGraphControl::GetHistory() const
{
	return m_pHistory;
}
AFX_INLINE void CXTPFlowGraphControl::SetAllowMoveNodes(BOOL bAllow)
{
	m_bAllowMoveNodes = bAllow;
}
AFX_INLINE BOOL CXTPFlowGraphControl::GetAllowMoveNodes() const
{
	return m_bAllowMoveNodes;
}
AFX_INLINE void CXTPFlowGraphControl::SetAllowResizeNodes(XTPFlowGraphResize nResizeMode)
{
	m_nAllowResizeNodes = nResizeMode;
}
AFX_INLINE XTPFlowGraphResize CXTPFlowGraphControl::GetAllowResizeNodes() const
{
	return m_nAllowResizeNodes;
}
AFX_INLINE void CXTPFlowGraphControl::SetAllowModifyConnections(BOOL bAllowModifyConnections)
{
	m_bAllowModifyConnections = bAllowModifyConnections;
}
AFX_INLINE BOOL CXTPFlowGraphControl::GetAllowModifyConnections() const
{
	return m_bAllowModifyConnections;
}
AFX_INLINE XTPFlowGraphSmoothingMode CXTPFlowGraphControl::GetSmoothingMode() const
{
	return m_nSmoothingMode;
}
AFX_INLINE CXTPMarkupContext* CXTPFlowGraphControl::GetMarkupContext() const
{
	return m_pMarkupContext;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPFLOWGRAPHCONTROL_H__)
