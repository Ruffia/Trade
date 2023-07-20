// XTPFlowGraphNode.h: interface for the CXTPFlowGraphNode class.
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
#if !defined(__XTPFLOWGRAPHNODE_H__)
#	define __XTPFLOWGRAPHNODE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPFlowGraphPage;
class CXTPFlowGraphDrawContext;
class CXTPFlowGraphControl;
class CXTPFlowGraphConnectionPoints;
class CXTPFlowGraphConnectionPoint;
class CXTPFlowGraphNodeGroup;
class CXTPPropExchange;
class CXTPFlowGraphImage;
class CXTPFlowGraphSelectionRect;

class CXTPMarkupUIElement;
class CXTPMarkupObject;
class CXTPMarkupUIElementCollection;
class CXTPMarkupVisual;
class CXTPMarkupTextBlock;

#	define XAML_MARKUP_PROPERTY_CPNAME L"CPName"
#	define XAML_MARKUP_PROPERTY_CPTYPE L"CPType"
#	define XAML_MARKUP_PROPERTY_CANSETCOLOR L"CanSetColor"
#	define XAML_MARKUP_PROPERTY_CPOINTS_IN L"ConnectionPointsIn"
#	define XAML_MARKUP_PROPERTY_RESIZABLE L"Resizable"

#	define XAML_MARKUP_TAG_ITEMSCOLLECTION L"nodeItemsCollection"
#	define XAML_MARKUP_TAG_CUSTOMCONTROL L"nodeCustomControl"
#	define XAML_MARKUP_TAG_NODECAPTION L"nodeCaption"
#	define XAML_MARKUP_TAG_NODECAPTIONBORDER L"nodeCaptionBorder"
#	define XAML_MARKUP_TAG_NODE_ITEM_CAPTION L"nodeItemCaption"
#	define XAML_MARKUP_TAG_NODE_ITEM_IMAGE L"nodeItemImage"
#	define XAML_MARKUP_TAG_NODEIMAGE L"nodeImage"
#	define XAML_MARKUP_TAG_NODECONTENT L"nodeContent"

#	define XAML_MARKUP_CPNAME_PLACEHOLDER _T("%CPNAME%")
#	define XAML_MARKUP_CPCAPTION_PLACEHOLDER _T("%CPCAPTION%")

#	define XAML_MARKUP_RESOURCENAME_NODE _T("FLOWGRAPH_XAML_NODE")
#	define XAML_MARKUP_RESOURCENAME_NODE_ITEM _T("FLOWGRAPH_XAML_NODE_ITEM")
#	define XAML_MARKUP_RESOURCENAME_NODE_ITEM_IN _T("FLOWGRAPH_XAML_NODE_ITEM_IN")
#	define XAML_MARKUP_RESOURCENAME_NODE_ITEM_OUT _T("FLOWGRAPH_XAML_NODE_ITEM_OUT")
#	define XAML_MARKUP_RESOURCENAME_NODE_ITEM_INOUT _T("FLOWGRAPH_XAML_NODE_ITEM_INOUT")

//----------------------------------------------------------------------------
// Summary:
//     Node connection point types (input/output).
// Remarks:
//     The xtpFlowGraphConnectionPointType is used to specify whether a
//     CXTPFlowGraphConnection will have any connection points.
//     Connection points are used to make connections between connections
//     in different Nodes.
// See Also:
//     CXTPFlowGraphConnectionPoint::SetType@XTPFlowGraphConnectionPointType
//----------------------------------------------------------------------------
enum XTPFlowGraphConnectionPointType
{
	xtpFlowGraphPointNone   = 0, // Connection has no connection points.
	xtpFlowGraphPointInput  = 1, // Connection only has an input point.
	xtpFlowGraphPointOutput = 2, // Connection only has an output point.

	// Connection has both input and output points. For the default node type.
	// When iterating connection points, you will never find a connection point of
	// this type. It is used to add a table row with actual 2 connection points:
	// xtpFlowGraphPointInput and xtpFlowGraphPointOutput.
	xtpFlowGraphPointInputAndOutput = 3,

	// A new connection point type for other shapes.
	xtpFlowGraphPointBoth = 4

};

//============================================================================
// Summary:
//     Represents a Node object.
// Remarks:
//     Nodes are the main objects that are displayed in the flow graph.
//     In terms of a database, they can be thought of as tables. Nodes contain
//     many connection points, which are like the fields in a table. Finally,
//     there are connections that are used to draw a relationship between the
//     fields of two tables.
//============================================================================
class _XTP_EXT_CLASS CXTPFlowGraphNode : public CXTPFlowGraphElement
{
	DECLARE_SERIAL(CXTPFlowGraphNode);

public:
	//------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphNode object.
	//------------------------------------------------------------------------
	CXTPFlowGraphNode();
	//------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphNode object, handles cleanup and
	//     deallocation.
	//------------------------------------------------------------------------
	virtual ~CXTPFlowGraphNode();

public:
	//------------------------------------------------------------------------
	// Summary:
	//     Sets the caption that is displayed for this Node.
	// Parameters:
	//     lpszCaption - Caption to displayed for this Node.
	// Remarks:
	//     The Node's caption will be displayed at the top of the Node and
	//     its background color is the same color as the Node, which is, by
	//     default, a different color than the connection points that get
	//     added to the node.
	//------------------------------------------------------------------------
	void SetCaption(LPCTSTR lpszCaption);
	//------------------------------------------------------------------------
	// Summary:
	//     Gets the caption that is displayed for this Node.
	// Remarks:
	//     The Node's caption will be displayed at the top of the Node and
	//     its background color is the same color as the Node, which is, by
	//     default, a different color than the connection points that get
	//     added to the node.
	// Returns:
	//     The caption that is displayed for this Node.
	//------------------------------------------------------------------------
	CString GetCaption() const;

	//------------------------------------------------------------------------
	// Summary:
	//     Sets the tooltip for this Node.
	// Parameters:
	//     lpszTooltip - New tooltip for this Node.
	// Remarks:
	//     This is the text that is displayed when the mouse is hovered over
	//     the Node.
	//------------------------------------------------------------------------
	void SetTooltip(LPCTSTR lpszTooltip);
	//------------------------------------------------------------------------
	// Summary:
	//     Gets the tooltip for this Node.
	// Remarks:
	//     This is the text that is displayed when the mouse is hovered over
	//     the Node.
	// Returns:
	//     The tooltip for this Node.
	//------------------------------------------------------------------------
	CString GetTooltip() const;

	//------------------------------------------------------------------------
	// Summary:
	//     Sets the color for the Node.
	// Parameters:
	//     clr - New color for the Node.
	//------------------------------------------------------------------------
	virtual void SetColor(COLORREF clr, BYTE bAlpha = 255);
	//------------------------------------------------------------------------
	// Summary:
	//     Gets the color of the node.
	// Returns:
	//     The color of the node.
	//------------------------------------------------------------------------
	COLORREF GetColor() const;

	//------------------------------------------------------------------------
	// Summary:
	//     Reserved for future use. Currently can be used as a "tag" property
	//     if needed.
	//------------------------------------------------------------------------
	void SetStyle(int nStyle);
	//------------------------------------------------------------------------
	// Summary:
	//     Reserved for future use. Currently can be used as a "tag" property
	//     if needed.
	//------------------------------------------------------------------------
	int GetStyle() const;

	//------------------------------------------------------------------------
	// Summary:
	//     This method sets the 32-bit value associated with the Node.
	// Parameters:
	//     dwTag - New 32-bit value to associate with the Node.
	// Remarks:
	//     The tag is used to store any arbitrary value that must be
	//     associated with the Node.
	//------------------------------------------------------------------------
	void SetTag(DWORD_PTR dwTag);
	//------------------------------------------------------------------------
	// Summary:
	//     This method retrieves the application-supplied, 32-bit value
	//     associated with the Node.
	// Remarks:
	//     The tag is used to store any arbitrary value that must be
	//     associated with the Node.
	// Returns:
	//     The 32-bit value associated with the Node.
	//------------------------------------------------------------------------
	DWORD_PTR GetTag() const;

	//------------------------------------------------------------------------
	// Summary:
	//     Sets the ID used to identify this Node.
	// Parameters:
	//     nId - New ID to be used to identify this Node.
	//------------------------------------------------------------------------
	void SetID(int nId);
	//------------------------------------------------------------------------
	// Summary:
	//     Gets the ID used to identify this Node.
	// Returns:
	//     The ID used to identify this Node.
	//------------------------------------------------------------------------
	int GetID() const;

	//------------------------------------------------------------------------
	// Summary:
	//     Call this member to associate an object with the Node. This will
	//     tell the Node what to display (i.e. the HWND of a dialog).
	// Parameters:
	//     hWnd - HWND of the object that should be displayed in the Node.
	//------------------------------------------------------------------------
	virtual void SetWindowHandle(HWND hWnd);
	//------------------------------------------------------------------------
	// Summary:
	//     Call this member to get the HWND of the object associated with the
	//     Node. This is the HWND of the object that is displayed in the Node.
	// Returns:
	//     The HWND of the object associated with the Node.
	//------------------------------------------------------------------------
	HWND GetWindowHandle() const;

	//------------------------------------------------------------------------
	// Summary:
	//     Sets the size of the window for the external control that is
	//     attached to the Node (if applicable).
	// Parameters:
	//     sz - New size of the window.
	// Remarks:
	//     This will set the size for the attached external control (i.e. if
	//     a tree control is attached to the node). You would attach the
	//     external control using the SetWindowHandle method.
	//------------------------------------------------------------------------
	virtual void SetWindowSize(CSize sz);
	//------------------------------------------------------------------------
	// Summary:
	//     Gets the size of the window for the external control that is
	//     attached to the Node (if applicable).
	// Remarks:
	//     This will get the size for the attached external control (i.e. if
	//     a tree control is attached to the node). This is the size of the
	//     window of the control that was attached using the SetWindowHandle
	//     method.
	// Returns:
	//     The size of the window for the external control that is attached
	//     to the Node.
	//------------------------------------------------------------------------
	CSize GetWindowSize() const;

	//------------------------------------------------------------------------
	// Summary:
	//     Sets the ID of the image to display next to the caption text of
	//     the Node.
	// Parameters:
	//     nImageIndex - The ID of the image to display next to the caption
	//                   text of the Node.
	// Remarks:
	//     Use the image index to specify the index of the previously loaded
	//     image to display next to the caption text of the node.
	//------------------------------------------------------------------------
	void SetImageIndex(int nImageIndex);
	//------------------------------------------------------------------------
	// Summary:
	//     Gets the ID of the image to display next to the caption text of
	//     the Node.
	// Returns:
	//     The ID of the image to display next to the caption text of the Node.
	//------------------------------------------------------------------------
	int GetImageIndex() const;

	//------------------------------------------------------------------------
	// Summary:
	//     Sets the locked state of the Node.
	// Parameters:
	//     bLocked - TRUE to set the Node to locked, FALSE otherwise.
	// Remarks:
	//     If the Node is locked, then its connection points
	//     cannot accept connection lines and any connection
	//     lines that were added cannot be removed.
	//------------------------------------------------------------------------
	void SetLocked(BOOL bLocked = TRUE);

	//------------------------------------------------------------------------
	// Summary:
	//     Gets the locked state of the Node.
	// Remarks:
	//     If the Node is locked, then its connection points
	//     cannot accept connection lines and any connection
	//     lines that were added cannot be removed.
	// Returns:
	//     TRUE if the Node is locked, otherwise FALSE.
	//------------------------------------------------------------------------
	BOOL IsLocked() const;

	//------------------------------------------------------------------------
	// Summary:
	//     Sets the visibility of the Node and its connections.
	// Parameters:
	//     bVisible - TRUE to set the Node and its connections to visible,
	//                FALSE to set the Node and its connections to invisible.
	//------------------------------------------------------------------------
	void SetVisible(BOOL bVisible = TRUE);

	//------------------------------------------------------------------------
	// Summary:
	//     Gets the visibility of the Node and its connections.
	// Returns:
	//     TRUE if the Node and its connections are visible, otherwise FALSE.
	//------------------------------------------------------------------------
	BOOL IsVisible() const;

public:
	//------------------------------------------------------------------------
	// Summary:
	//     Sets the location of the Node.
	// Parameters:
	//     ptLocation - Point containing the x- and y- coordinates for the
	//                  location of the Node.
	//------------------------------------------------------------------------
	void SetLocation(CPoint ptLocation);
	//------------------------------------------------------------------------
	// Summary:
	//     Gets the location of the Node.
	// Returns:
	//     A point containing the x- and y- coordinates of the location of
	//     the Node.
	//------------------------------------------------------------------------
	CPoint GetLocation() const;

	//------------------------------------------------------------------------
	// Summary:
	//     Sets the size for the Node (Node's Markup!).
	// Parameters:
	//     sz - New size for the Node.
	//------------------------------------------------------------------------
	virtual void SetSize(CSize sz);

	//------------------------------------------------------------------------
	// Summary:
	//     Sets the size for the Node.
	// Parameters:
	//     sz - New size for the Node.
	//------------------------------------------------------------------------
	void SetSizeCore(CSize sz);

	//------------------------------------------------------------------------
	// Summary:
	//     Gets the size of the Node.
	// Returns:
	//     A CSize object containing the size of the Node.
	//------------------------------------------------------------------------
	CSize GetSize() const;

	//------------------------------------------------------------------------
	// Summary:
	//     Gets the size of the Node (including user resizing).
	// Returns:
	//     The size of the Node.
	//------------------------------------------------------------------------
	CSize GetUserSize() const;

	//------------------------------------------------------------------------
	// Summary:
	//     Checks if the Node is resizable.
	// Returns:
	//     TRUE if the Node is resizable.
	//------------------------------------------------------------------------
	BOOL IsResizable() const;

	//------------------------------------------------------------------------
	// Summary:
	//     Gets the minimum size of the Node; calculated in the CalcMinSize method.
	// Returns:
	//     The minimum size of the Node
	//------------------------------------------------------------------------
	CSize GetMinSize() const;

	//------------------------------------------------------------------------
	// Summary:
	//     Sets the minimum size for the Node.
	// Parameters:
	//     sz - New minimum size for the Node.
	//------------------------------------------------------------------------
	void SetMinSize(CSize sz);

	//------------------------------------------------------------------------
	// Summary:
	//     Gets the screen coordinates for the Node.
	// Returns:
	//     The screen coordinates for the Node.
	//------------------------------------------------------------------------
	CRect GetScreenRect(BOOL bIncludeConnectionPointsEllipses = TRUE) const;

	virtual CRect GetBoundingRectangle(BOOL bIncludeConnectionPointsEllipses = TRUE) const;

public:
	//------------------------------------------------------------------------
	// Summary:
	//     Gets a reference to the page that this Node belongs to.
	// Returns:
	//     A reference to the page that this Node belongs to.
	//------------------------------------------------------------------------
	CXTPFlowGraphPage* GetPage() const;
	//------------------------------------------------------------------------
	// Summary:
	//     Retrieves the group that this Node is a part of (if any).
	// Remarks:
	//     A group is just a "box" that is used to visually group Nodes
	//     together. By default, the group will only be shown when a Node in
	//     the group is selected, but you can choose to always show the
	//     groups with ShowNodeGroupsAlways.
	// Returns:
	//     A reference to the group that this Node is a part of (if any),
	//     otherwise NULL.
	//------------------------------------------------------------------------
	CXTPFlowGraphNodeGroup* GetGroup() const;
	//------------------------------------------------------------------------
	// Summary:
	//     Gets the image to display next to the caption text of the Node.
	// Returns:
	//     A pointer to the image to display next to the caption text of the Node.
	//------------------------------------------------------------------------
	CXTPFlowGraphImage* GetImage() const;
	//------------------------------------------------------------------------
	// Summary:
	//     Gets the connection points collection for this Node.
	// Returns:
	//     A pointer to the connection points collection for this Node.
	//------------------------------------------------------------------------
	CXTPFlowGraphConnectionPoints* GetConnectionPoints() const;
	//------------------------------------------------------------------------
	// Summary:
	//     Gets a reference to the parent FlowGraph control.
	// Returns:
	//     A reference to the parent FlowGraph control.
	//------------------------------------------------------------------------
	CXTPFlowGraphControl* GetControl() const;

public:
	//------------------------------------------------------------------------
	// Summary:
	//     Removes this node from the flow graph.
	//------------------------------------------------------------------------
	void Remove();

public:
	//------------------------------------------------------------------------
	// Summary:
	//     Determines which connection point, if any, is at a specified point.
	// Parameters:
	//     point - Point to be tested.
	// Returns:
	//     A pointer to the connection point found at the specified point, if any,
	//     or NULL if no connection point is found.
	//------------------------------------------------------------------------
	virtual CXTPFlowGraphConnectionPoint* HitTestConnectionPoint(CPoint point);

	//------------------------------------------------------------------------
	// Summary:
	//     Updates the window position of the attached external control.
	// Remarks:
	//     This method is also called internally when the Node is drawn.
	//------------------------------------------------------------------------
	virtual void UpdateWindowPosition();

	//------------------------------------------------------------------------
	// Summary:
	//     Draws the contents of the Node.
	// Parameters:
	//     pDC - Pointer to a CXTPFlowGraphDrawContext object.
	//------------------------------------------------------------------------
	virtual void DrawWindowContent(CXTPFlowGraphDrawContext* pDC);

	//------------------------------------------------------------------------
	// Summary:
	//     Specifies the selected state of the Node.
	// Parameters:
	//     bSelect - TRUE to set the Node to selected, FALSE otherwise.
	//------------------------------------------------------------------------
	virtual void Select(BOOL bSelect = TRUE);

public:
	//------------------------------------------------------------------------
	// Summary:
	//     Call this member function to store/load a Node using the specified data object.
	// Parameters:
	//     pPX - Source/destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store/load Node data to/from storage.
	//------------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//------------------------------------------------------------------------
	// Summary:
	//     Stores/loads Node common properties (for both default and derived Nodes).
	// Parameters:
	//     pPX - Source/destination CXTPPropExchange data object reference.
	// Remarks:
	//     This function is called automatically in both default and custom Nodes.
	//------------------------------------------------------------------------
	void DoCommonPropExchange(CXTPPropExchange* pPX);

public:
	//------------------------------------------------------------------------
	// Remarks:
	//     Recalculates the layout of the Node.
	// Remarks:
	//     Call this functions to correctly reposition the connections within
	//     the Node. This also will readjust the layout when the size or
	//     caption has changed.
	//------------------------------------------------------------------------
	virtual void RecalLayout();
	//------------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the Node.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//------------------------------------------------------------------------
	virtual void Draw(CXTPFlowGraphDrawContext* pDC);

	//--------------------------------------------------------------------------
	// Summary:
	//     Retrieves the Node's selection rectangle (visible only when the Node is selected).
	// Returns:
	//     The Node's selection rectangle.
	//--------------------------------------------------------------------------
	CRect GetSelectionRect() const;

	//------------------------------------------------------------------------
	// Summary:
	//     This virtual method is called to draw the Node's selection rectangle.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//------------------------------------------------------------------------
	virtual void DrawSelectionRect(CXTPFlowGraphDrawContext* pDC);

	//------------------------------------------------------------------------
	// Summary:
	//     A virtual method that draws the Node's connection points (circles)
	//     in their placeholders.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//------------------------------------------------------------------------
	virtual void DrawConnectionPoints(CXTPFlowGraphDrawContext* pDC);

	//------------------------------------------------------------------------
	// Summary:
	//     A virtual method that calculates the minimum size of the Node
	//     (used in resizing mechanism).
	//------------------------------------------------------------------------
	virtual void CalcMinSize();

	// Default Markup support:

public:
	//------------------------------------------------------------------------
	// Summary:
	//     Adds a table row to the Node (of default type), with none, one, or
	//     two real connection points.
	//------------------------------------------------------------------------
	// Parameters:
	//     szName - Unique name.
	//     szCaption - Caption of the table row. Uses szName if not specified.
	//     type - Type.
	// Remarks:
	//     xtpFlowGraphPointNone           - No actual connection points.
	//     xtpFlowGraphPointInput          - One connection point (in the left of the row).
	//     xtpFlowGraphPointOutput         - One connection point (in the right of the row).
	//     xtpFlowGraphPointInputAndOutput - Two connection points (in the left and right of the
	//     row).
	//------------------------------------------------------------------------
	virtual void AddNamedConnectionPoints(
		LPCTSTR szName, XTPFlowGraphConnectionPointType type = xtpFlowGraphPointNone,
		LPCTSTR szCaption = NULL);

	//------------------------------------------------------------------------
	// Summary:
	//     An important common method used to get a rectangle of any visual
	//     Markup element.
	// Remarks:
	//     Rectangle coordinates are relative to the Markup root element
	//     (<Border> for example).
	// Parameters:
	//     pElementVisual - Pointer to a valid visual markup element.
	// Returns:
	//     The rectangle of the visual markup element.
	//------------------------------------------------------------------------
	CRect GetMarkupVisualElementRect(const CXTPMarkupVisual* pElementVisual) const;

	//------------------------------------------------------------------------
	// Summary:
	//     Retrieves the index of a table row in the list (to insert a new one).
	// Parameters:
	//     pPoint    - Pointer to a connection point object located in this row.
	//     pChildren - Pointer to a collection of markup elements from which
	//                 the index of the element associated with the specified
	//                 connection point object is to be returned.
	// Returns:
	//     The index of the collection element associated with the
	//     specified connection point object.
	//------------------------------------------------------------------------
	int GetMarkupTableRowIndex(CXTPFlowGraphConnectionPoint* pPoint,
							   CXTPMarkupUIElementCollection* pChildren);

	//------------------------------------------------------------------------
	// Summary:
	//     Removes a table row (with its connection points) from a default Node.
	// Parameters:
	//     pPoint - Pointer to a connection point that is located in the table row.
	// Returns:
	//     The index of the removed table row in the table rows list.
	//------------------------------------------------------------------------
	int RemoveMarkupTableRow(CXTPFlowGraphConnectionPoint* pPoint);

	//------------------------------------------------------------------------
	// Summary:
	//     Retrieves the XAML TextBlock element for a table row of a default Node.
	// Parameters:
	//     strConnectionPointName - The name of a connection point located within
	//                              the table row with the XAML TextBlock element
	//                              to be retrieved.
	// Returns:
	//     A pointer to a CXTPMarkupTextBlock element.
	//------------------------------------------------------------------------
	CXTPMarkupTextBlock* GetTableRowTextBlock(const CString& strConnectionPointName) const;

	//------------------------------------------------------------------------
	// Summary:
	//     Retrieves the rectangle of a XAML TextBlock element for a table row of
	//     a default Node.
	// Parameters:
	//     strConnectionPointName - The name of a connection point located within
	//                              the table row with the XAML TextBlock element
	//                              with the rectangle to be retrieved.
	// Returns:
	//     The rectangle of the XAML TextBlock element. Coordinates are relative
	//     to the Node's location.
	//------------------------------------------------------------------------
	CRect GetTableRowCaptionRect(CXTPFlowGraphConnectionPoint* pPoint) const;

	//------------------------------------------------------------------------
	// Summary:
	//     Sets the default XAML Markup for a default Node with zero table rows.
	//------------------------------------------------------------------------
	virtual void SetDefaultMarkup();

	//------------------------------------------------------------------------
	// Summary:
	//     Updates the Markup point caption (when changed or restored using DoPropExchange).
	// Parameters:
	//     pPoint - Pointer to a CXTPFlowGraphConnectionPoint object.
	//------------------------------------------------------------------------
	void UpdateMarkupPointCaption(CXTPFlowGraphConnectionPoint* pPoint);

	//------------------------------------------------------------------------
	// Summary:
	//     Updates the Markup point name (when changed or restored using DoPropExchange).
	// Parameters:
	//     pPoint - Pointer to a CXTPFlowGraphConnectionPoint object.
	//------------------------------------------------------------------------
	void UpdateMarkupPointName(CXTPFlowGraphConnectionPoint* pPoint);

	//------------------------------------------------------------------------
	// Summary:
	//     Updates the Markup point color (when changed or restored using DoPropExchange).
	// Parameters:
	//     pPoint - Pointer to a CXTPFlowGraphConnectionPoint object.
	//------------------------------------------------------------------------
	virtual void UpdateMarkupPointColor(CXTPFlowGraphConnectionPoint* pPoint);

	//------------------------------------------------------------------------
	// Summary:
	//     Updates the markup point image (when changed or restored using DoPropExchange).
	// Parameters:
	//     pPoint - Pointer to a CXTPFlowGraphConnectionPoint object.
	//------------------------------------------------------------------------
	void UpdateMarkupPointImage(CXTPFlowGraphConnectionPoint* pPoint);

	//------------------------------------------------------------------------
	// Summary:
	//     Sets Markup text for the Node (for a custom Node).
	// Parameters:
	//     szMarkupText - Markup text to set.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//------------------------------------------------------------------------
	BOOL SetMarkupText(LPCTSTR szMarkupText);

	//------------------------------------------------------------------------
	// Summary:
	//     A virtual function that determines if the Node is a custom Node
	//     (a Node with custom XAML markup).
	// Returns:
	//     TRUE if the Node is a custom Node, otherwise FALSE.
	//------------------------------------------------------------------------
	virtual BOOL IsCustomMarkup() const
	{
		return FALSE;
	}

	//------------------------------------------------------------------------
	// Summary:
	//     A virtual method that finds and updates real connection points in the Markup.
	// Parameters:
	//     pRootElement - An XAML element used to search for connection points in
	//                    its children.
	// Remarks:
	//     Implementation of this method for default Nodes is recursive.
	//     A rectangle for a connection point is defined by the <Rectangle> element
	//     in XAML for default Nodes.
	//     Implementation of this method for custom Nodes uses an absolutely different
	//     approach (see CXTPFlowGraphNodeCustom::UpdateConnectionPoints).
	//------------------------------------------------------------------------
	virtual void UpdateConnectionPoints(CXTPMarkupObject* pRootElement);

	//------------------------------------------------------------------------
	// Summary:
	//     A recursive method to update/set the color of a Node.
	// Parameters:
	//     pRootElement - An XAML element used to search for XAML elements that can have
	//                    a color property (see XAML_MARKUP_PROPERTY_CANSETCOLOR).
	//     clrColor     - Color to be set.
	//------------------------------------------------------------------------
	virtual void UpdateColorRecursive(CXTPMarkupObject* pRootElement, COLORREF clrColor);

	//------------------------------------------------------------------------
	// Summary:
	//     Updates the foreground color of the CXTPMarkupTextBlock element
	//     of the Node's caption.
	//------------------------------------------------------------------------
	virtual void UpdateCaptionColor();

	//------------------------------------------------------------------------
	// Summary:
	//     Updates the Node's image.
	//------------------------------------------------------------------------
	virtual void UpdateMarkupImage();

	//------------------------------------------------------------------------
	// Summary:
	//     Updates Node's caption <TextBlock> element with Node's caption text
	//     (m_strCaption).
	// Returns:
	//     A pointer to a CXTPMarkupTextBlock element.
	//------------------------------------------------------------------------
	virtual CXTPMarkupTextBlock* UpdateMarkupCaption();

	//------------------------------------------------------------------------
	// Summary:
	//     Get the Node's caption <TextBlock> element.
	// Returns:
	//     A pointer to a CXTPMarkupTextBlock element.
	//------------------------------------------------------------------------
	CXTPMarkupTextBlock* GetCaptionTextBlock() const;

	//------------------------------------------------------------------------
	// Summary:
	//     Get the Node's caption <TextBlock> rectangle.
	// Returns:
	//     The Node's caption <TextBlock> rectangle. Coordinates are relative
	//     to the Node's location.
	//------------------------------------------------------------------------
	CRect GetCaptionRect() const;

	//------------------------------------------------------------------------
	// Summary:
	//     Updates the default font defined in the paint manager; used in Markup drawing.
	//------------------------------------------------------------------------
	virtual void UpdateMarkupFont();

protected:
	//------------------------------------------------------------------------
	// Summary:
	//     This method is called when the Node is removed.
	//------------------------------------------------------------------------
	virtual void OnRemoved();

	static LRESULT CALLBACK ControlWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam,
											  LPARAM lParam); // New window procedure.

	void SetCustomWndProc();
	void ResetCustomWndProc();

protected:
	CXTPFlowGraphPage* m_pPage; // Pointer to the page this Node is displayed on.

	CXTPFlowGraphNode* m_pNextVisibleNode; // Pointer to the next visible node.

	CString m_strCaption; // Caption displayed for this node.

	CString m_strMarkup;

	CXTPMarkupUIElement* m_pMarkup; // Markup that is responsible for drawing this Node.

	CPoint m_ptLocation; // Point that contains the coordinates of the Node.

	CSize m_szUserSize;   // Current size of the Node including user resizing.
	CSize m_szActualSize; // Real size of the Node before the user might have resized it.
	CSize m_szMinSize;
	CSize m_szUserMinSize; // Manually set minimum size.

	BOOL m_bResizable;

	CString m_strTooltip; // Tooltip of the item.
	COLORREF m_clrNode;   // Color of the Node.

	BYTE m_bAlpha; // Alpha channel 0-255

	int m_nStyle; // Reserved for future use. Currently can be used as a "tag" property if needed.
	DWORD_PTR m_dwTag; // The 32-bit value associated with the item.
	int m_nId;		   // ID of the node.

	HWND m_hWnd;				   // Handle of the window inside the Node.
	WNDPROC m_pOriginalWindowProc; // Window procedure pointer.
	CSize m_szWindow; // Size of the window of the attached control (if applicable). For example,
					  // if a tree control was attached to the Node, this would be it's window size.

	static CMap<HWND, HWND, CXTPFlowGraphNode*, CXTPFlowGraphNode*>
		m_WindowToNodeHash; // Used in the custom window 'ControlWindowProc'.

	CXTPFlowGraphSelectionRect* m_pSelectionRect;

	int m_nGroupId; // ID of the group this Node belongs to (if any).

	int m_nImageIndex; // Index of an image to display next to the caption text of the Node.

	BOOL m_bLocked; // Specifies whether the Node is locked. TRUE if the node if locked, FALSE
					// otherwise.

	BOOL m_bVisible; // Specifies whether the Node is visible. TRUE if the node if visible, FALSE
					 // otherwise.

	CXTPFlowGraphConnectionPoints* m_pConnectionPoints; // Collection of connection points that are
														// displayed for this Node.
	CXTPFlowGraphNodeGroup* m_pGroup; // Pointer to the group this Node belongs to (if any).

	friend class CXTPFlowGraphNodes;
	friend class CXTPFlowGraphNodeGroup;
	friend class CXTPFlowGraphPaintManager;
	friend class CXTPFlowGraphPage;
	friend class CXTPFlowGraphControl;

	//{{AFX_CODEJOCK_PRIVATE
private:
	int m_nArrangeIndex;
	int m_nArrangeLevel;
	int m_nArrangeComponent;
	//}}AFX_CODEJOCK_PRIVATE

	//{{AFX_CODEJOCK_PRIVATE
	// Paint Manager Properties:
public:
	CRect m_rcWindow;
	//}}AFX_CODEJOCK_PRIVATE

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPFlowGraphNode)

	afx_msg void OleSetCaption(LPCTSTR lpszCaption);
	afx_msg BSTR OleGetCaption();
	afx_msg void OleSetTooltip(LPCTSTR lpszTooltip);
	afx_msg BSTR OleGetTooltip();
	afx_msg void OleSetColor(OLE_COLOR clr);
	afx_msg OLE_COLOR OleGetColor();
	afx_msg void OleSetStyle(int nStyle);
	afx_msg int OleGetStyle();
	afx_msg LPDISPATCH OleGetConnectionPoints();

	afx_msg int OleGetLeft();
	afx_msg void OleSetLeft(int nLeft);
	afx_msg int OleGetTop();
	afx_msg void OleSetTop(int nTop);
	afx_msg int OleGetWidth();
	afx_msg int OleGetHeight();

	afx_msg LPDISPATCH OleGetPage();
	afx_msg BOOL OleGetSelected();
	afx_msg void OleSetSelected(BOOL bSelected);

	afx_msg LPDISPATCH OleGetGroup();
	afx_msg LPDISPATCH OleHitTestConnectionPoint(long x, long y);

	afx_msg void OleAddNamedConnectionPoints(LPCTSTR lpszName, long type);
	afx_msg void OleAddNamedConnectionPointsWithCaption(LPCTSTR lpszName, long type,
														LPCTSTR lpszCaption);

	afx_msg void OleSetWindowHandle(HWND hWnd);
	afx_msg HWND OleGetWindowHandle();

	afx_msg int OleGetWindowWidth();
	afx_msg void OleSetWindowWidth(int nWidth);

	afx_msg int OleGetWindowHeight();
	afx_msg void OleSetWindowHeight(int nHeight);

	afx_msg void OleSetWidth(int nWidth);
	afx_msg void OleSetHeight(int nHeight);

//}}AFX_CODEJOCK_PRIVATE
#	endif
};

AFX_INLINE CString CXTPFlowGraphNode::GetCaption() const
{
	return m_strCaption;
}
AFX_INLINE CPoint CXTPFlowGraphNode::GetLocation() const
{
	return m_ptLocation;
}
AFX_INLINE CXTPFlowGraphConnectionPoints* CXTPFlowGraphNode::GetConnectionPoints() const
{
	return m_pConnectionPoints;
}
AFX_INLINE void CXTPFlowGraphNode::SetTooltip(LPCTSTR lpszTooltip)
{
	m_strTooltip = lpszTooltip;
}
AFX_INLINE CString CXTPFlowGraphNode::GetTooltip() const
{
	return m_strTooltip;
}
AFX_INLINE COLORREF CXTPFlowGraphNode::GetColor() const
{
	return m_clrNode;
}
AFX_INLINE void CXTPFlowGraphNode::SetStyle(int nStyle)
{
	m_nStyle = nStyle;
	OnGraphChanged();
}
AFX_INLINE int CXTPFlowGraphNode::GetStyle() const
{
	return m_nStyle;
}
AFX_INLINE CXTPFlowGraphPage* CXTPFlowGraphNode::GetPage() const
{
	return m_pPage;
}
AFX_INLINE void CXTPFlowGraphNode::SetTag(DWORD_PTR dwTag)
{
	m_dwTag = dwTag;
}
AFX_INLINE DWORD_PTR CXTPFlowGraphNode::GetTag() const
{
	return m_dwTag;
}
AFX_INLINE void CXTPFlowGraphNode::SetID(int nId)
{
	m_nId = nId;
}
AFX_INLINE int CXTPFlowGraphNode::GetID() const
{
	return m_nId;
}
AFX_INLINE HWND CXTPFlowGraphNode::GetWindowHandle() const
{
	return m_hWnd;
}
AFX_INLINE CSize CXTPFlowGraphNode::GetWindowSize() const
{
	return m_szWindow;
}
AFX_INLINE CXTPFlowGraphNodeGroup* CXTPFlowGraphNode::GetGroup() const
{
	return m_pGroup;
}
AFX_INLINE int CXTPFlowGraphNode::GetImageIndex() const
{
	return m_nImageIndex;
}
AFX_INLINE void CXTPFlowGraphNode::SetLocked(BOOL bLocked)
{
	m_bLocked = bLocked;
}
AFX_INLINE BOOL CXTPFlowGraphNode::IsLocked() const
{
	return m_bLocked;
}
AFX_INLINE BOOL CXTPFlowGraphNode::IsVisible() const
{
	return m_bVisible;
}
AFX_INLINE BOOL CXTPFlowGraphNode::IsResizable() const
{
	return m_bResizable;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPFLOWGRAPHNODE_H__)
