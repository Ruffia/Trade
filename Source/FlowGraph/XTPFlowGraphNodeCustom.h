// XTPFlowGraphNodeCustom.h: interface for the CXTPFlowGraphNodeCustom class.
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
#if !defined(__XTPFLOWGRAPHNODECUSTOM_H__)
#	define __XTPFLOWGRAPHNODECUSTOM_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPMarkupPath;
class CXTPMarkupPathData;

#	define XAML_MARKUP_TAG_NODEPATH L"nodePath"
#	define XAML_MARKUP_TAG_NODECANVAS L"nodeCanvas"
#	define XAML_MARKUP_TAG_NODEBORDER L"nodeBorder"

#	define XAML_MARKUP_PROPERTY_CONNECTIONPOINTS_AS_RHOMBUS L"Rhombus"
#	define XAML_MARKUP_PROPERTY_CONNECTIONPOINTS_IN_CENTER L"Center"

//------------------------------------------------------------------------------
// Summary:
//     Defines the position for a caption of a custom markup node.
// See Also:
//     CXTPFlowGraphNodeCustom::SetCaptionPosition
//------------------------------------------------------------------------------
enum XTPFlowGraphCustomNodeCaptionPosition
{
	xtpFlowGraphCustomNodeCaptionCenter = 0, // Center of the shape; default behavior.
	xtpFlowGraphCustomNodeCaptionBelow		 // Below the shape.
};

//------------------------------------------------------------------------------
// Summary:
//     Path (figure) fill type.
//------------------------------------------------------------------------------
enum XTPFlowGraphCustomNodeFillType
{
	xtpFlowGraphCustomNodeFillSolid = 0,
	xtpFlowGraphCustomNodeFillGradientHorizontal,
	xtpFlowGraphCustomNodeFillGradientVertical,
	xtpFlowGraphCustomNodeFillGradientDiagonalLeft,
	xtpFlowGraphCustomNodeFillGradientDiagonalRight,
	xtpFlowGraphCustomNodeFillGradientCenterHorizontal,
	xtpFlowGraphCustomNodeFillGradientCenterVertical
};

//==============================================================================
// Summary:
//     Represents a Node object defined by custom XAML markup.
// Remarks:
//     XAML markup of a custom Node must have
//       <Canvas x:Name='nodeCanvas'>
//     as a root element.
//     A child element of Canvas must be a shape (defined by Path, Image, etc.).
//     A Path must have a Stretch='Fill' property for correct resizing of a Node.
//     Path should have a certain name:
//       <Path x:Name='nodePath' Stretch="Fill"/>
//     If you want a Node to have a caption, you should specify
//       <TextBlock x:Name='nodeCaption'/>
//     as the second child element of Canvas.
//     Connection points of a Node are supporting points of a shape if the shape
//     is defined by a Path. Otherwise, when a custom Node is a shape with no Path (like
//     ellipses or a custom image), connection points' disposition is defined by the
//     ConnectionPointsIn property (see ellipse.xaml and image.xaml samples for details).
//==============================================================================
class _XTP_EXT_CLASS CXTPFlowGraphNodeCustom : public CXTPFlowGraphNode
{
	DECLARE_SERIAL(CXTPFlowGraphNodeCustom);

protected:
	//--------------------------------------------------------------------------
	// Summary:
	//     Retrieves a stretched path from a specified path based on the render size
	//     the Node.
	// Parameters:
	//     pNodePath - The specified path.
	// Returns:
	//     The new stretched path (that must be deleted with a delete operator).
	//--------------------------------------------------------------------------
	CXTPMarkupPathData* CreateStretchedPathData(const CXTPMarkupPath* pNodePath) const;

	//--------------------------------------------------------------------------
	// Summary:
	//     Retrieves a rectangle with a specified point in the center of it.
	// Parameters:
	//     point - The specified point.
	// Returns:
	//     The rectangle with the specified point in the center of it.
	//--------------------------------------------------------------------------
	CRect GetPointRect(CPoint point) const;

public:
	//--------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphNodeCustom object.
	//--------------------------------------------------------------------------
	CXTPFlowGraphNodeCustom();

	//--------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphNodeCustom object, handles cleanup and
	//     deallocation.
	//--------------------------------------------------------------------------
	~CXTPFlowGraphNodeCustom();

	//--------------------------------------------------------------------------
	// Summary:
	//     Sets the Node's caption position.
	// Parameters:
	//     pos - New caption position.
	//--------------------------------------------------------------------------
	void SetCaptionPosition(XTPFlowGraphCustomNodeCaptionPosition pos);

	//--------------------------------------------------------------------------
	// Summary:
	//     Gets the Node's caption position.
	// Returns:
	//     The Node's caption position.
	//--------------------------------------------------------------------------
	XTPFlowGraphCustomNodeCaptionPosition GetCaptionPosition() const;

	//--------------------------------------------------------------------------
	// Summary:
	//     Retrieves the Node's selection rectangle (visible only when the Node is selected).
	// Returns:
	//     The Node's selection rectangle.
	//--------------------------------------------------------------------------
	CRect GetSelectionRect();

public:
	//--------------------------------------------------------------------------
	// virtual methods of CXTPFlowGraphNode
	//--------------------------------------------------------------------------

	// custom controls cannot be placed inside custom nodes. Virtual methods below must be empty
	virtual void SetWindowHandle(HWND)
	{
	}
	virtual void SetWindowSize(CSize)
	{
	}
	virtual void UpdateWindowPosition()
	{
	}
	virtual void DrawWindowContent(CXTPFlowGraphDrawContext*)
	{
	}

	virtual void UpdateMarkupPointColor(CXTPFlowGraphConnectionPoint*)
	{
	}

	virtual void SetDefaultMarkup(){}; // no default markup for a custom node

	virtual void SetColor(COLORREF clr, BYTE bAlpha = 255);

	virtual BOOL IsCustomMarkup() const
	{
		return TRUE;
	}

	virtual void SetSize(CSize sz);

	virtual void AddNamedConnectionPoints(
		LPCTSTR /*szName*/, XTPFlowGraphConnectionPointType /*type = xtpFlowGraphPointNone*/,
		LPCTSTR /*szCaption = NULL*/)
	{
	} // you cannot add connection points manually

	virtual void UpdateConnectionPoints(CXTPMarkupObject* pRootElement);

	virtual void UpdateColorRecursive(CXTPMarkupObject* pRootElement, COLORREF clrColor);

	virtual CRect GetBoundingRectangle(BOOL bIncludeConnectionPointsEllipses = TRUE) const;

	virtual CXTPMarkupTextBlock* UpdateMarkupCaption();

	virtual void UpdateCaptionColor();

	virtual void CalcMinSize();

	virtual void DoPropExchange(CXTPPropExchange* pPX);

	virtual CXTPFlowGraphConnectionPoint* HitTestConnectionPoint(CPoint point);

public:
	// alpha for gradients is not yet supported by the markup engine
	// http://msdn.microsoft.com/en-us/library/ms743320.aspx
	void SetColor(COLORREF clrOne, COLORREF clrTwo, XTPFlowGraphCustomNodeFillType fType,
				  BYTE bAlpha = 255);

	COLORREF GetSecondColor() const;

	void SetSecondColor(COLORREF clrSecondColor);

	XTPFlowGraphCustomNodeFillType GetFillType() const;

	void SetFillType(XTPFlowGraphCustomNodeFillType newType);

protected:
	XTPFlowGraphCustomNodeCaptionPosition m_CaptionPosition;

	COLORREF m_clrSecondColor;

	XTPFlowGraphCustomNodeFillType m_FillType;

public:
#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPFlowGraphNodeCustom)

	afx_msg void OleSetCaptionPosition(int nStyle);
	afx_msg int OleGetCaptionPosition();

	afx_msg void OleSetMarkupText(LPCTSTR lpszMarkupText);
	afx_msg BSTR OleGetMarkupText();

	afx_msg void OleSetSecondColor(OLE_COLOR clr);
	afx_msg OLE_COLOR OleGetSecondColor();

	afx_msg void OleSetFillType(int nType);
	afx_msg int OleGetFillType();

	//}}AFX_CODEJOCK_PRIVATE
#	endif
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPFLOWGRAPHNODECUSTOM_H__)
