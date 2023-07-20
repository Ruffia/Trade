// XTPGridBorder.h: Interface of the CXTPGridBorder class.
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
#if !defined(__XTPGRIDBORDER_H__)
#	define __XTPGRIDBORDER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPGridThickness;

enum XTPGridBorderEdge
{
	xtpGridBorderEdgeLeft,
	xtpGridBorderEdgeTop,
	xtpGridBorderEdgeRight,
	xtpGridBorderEdgeBottom,
	xtpGridBorderEdgeCount
};

class _XTP_EXT_CLASS CXTPGridBorderEdge : public CXTPCmdTarget
{
public:
	CXTPGridBorderEdge();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the line style of the border edge.
	// Returns:
	//     The line style of the border edge.
	//-----------------------------------------------------------------------
	XTPGridLineStyle GetLineStyle() const;

	void SetLineStyle(XTPGridLineStyle lineStyle);

protected:
	XTPGridLineStyle m_lineStyle;
};

class _XTP_EXT_CLASS CXTPGridBorder : public CXTPCmdTarget
{
public:
	CXTPGridBorder();

	virtual ~CXTPGridBorder();

	virtual void Draw(CDC* pDC, CRect rcBorder);
	virtual void DrawH(CDC* pDC, CRect rcBorder);
	virtual void DrawV(CDC* pDC, CRect rcBorder);

	CSize GetSize() const;

	//////////////////////////////////////////////////////////////////////////
	// Properties
	//////////////////////////////////////////////////////////////////////////
public:
	CXTPGridBorderEdge* GetEdge(const XTPGridBorderEdge edge) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the border color.
	// Returns:
	//     The border color.
	//-----------------------------------------------------------------------
	COLORREF GetBorderColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the border color.
	// Parameters:
	//     crBorderColor - New border color.
	//-----------------------------------------------------------------------
	void SetBorderColor(COLORREF crBorder);

	CXTPGridThickness* GetMargin() const;

	CXTPGridThickness* GetBorderThickness() const;

	CXTPGridThickness* GetPadding() const;

	static CRect AFX_CDECL DeflateRect(CRect rcDeflate, const CXTPGridBorder* pBorder);

	//////////////////////////////////////////////////////////////////////////
	// Members
	//////////////////////////////////////////////////////////////////////////
protected:
	COLORREF m_crBackground; // Background color.
	COLORREF m_crBorder;	 // Border color.

	CXTPGridBorderEdge* m_pEdgeLeft;   // Left edge.
	CXTPGridBorderEdge* m_pEdgeTop;	// Top edge.
	CXTPGridBorderEdge* m_pEdgeRight;  // Right edge.
	CXTPGridBorderEdge* m_pEdgeBottom; // Bottom edge.

	CXTPGridThickness* m_pMargin;		   // Outer margin.
	CXTPGridThickness* m_pBorderThickness; // Border thickness.
	CXTPGridThickness* m_pPadding;		   // Inner padding.
};

//{{AFX_CODEJOCK_PRIVATE

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPGRIDBORDER_H__)
//}}AFX_CODEJOCK_PRIVATE
