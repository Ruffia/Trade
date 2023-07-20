// XTPGridDragDrop.h: interface for the CXTPGridHeaderDragWnd and
//                      CXTPGridHeaderDropWnd classes.
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
#if !defined(__XTPGRIDDRAGDROP_H__)
#	define __XTPGRIDDRAGDROP_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPGridHeader;
class CXTPGridPaintManager;
class CXTPGridColumn;

//===========================================================================
// Summary:
//     CXTPGridHeaderDragWnd is a CWnd derived class. It is used
//     internally to represent a dragging column window.
//===========================================================================
class _XTP_EXT_CLASS CXTPGridHeaderDragWnd : public CWnd
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a CXTPGridHeaderDragWnd object.
	//-----------------------------------------------------------------------
	CXTPGridHeaderDragWnd();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPGridHeaderDragWnd object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPGridHeaderDragWnd();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a dragging window with the specified parameters.
	// Parameters:
	//     rect          - Drawing rectangle position.
	//     pHeader       - Pointer to the parent grid header object.
	//     pPaintManager - Paint manager for performing draw operations.
	//     pColumn       - Pointer to the associated column.
	// Returns:
	//     TRUE if the dragging window was created successfully, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL Create(CRect rect, CXTPGridHeader* pHeader, CXTPGridPaintManager* pPaintManager,
						CXTPGridColumn* pColumn);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the framework to draw the contents of the window using
	//     the specified device context.
	// Parameters:
	//     pDC      - Pointer to the device context in which the drawing occurs.
	//     rcClient - Coordinates of the client window position.
	//-----------------------------------------------------------------------
	virtual void OnDraw(CDC* pDC, CRect rcClient);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_MSG(CXTPGridHeaderDragWnd)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG

	//{{AFX_VIRTUAL(CXTPGridHeaderDragWnd)
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
	//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect,
				CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

	DECLARE_MESSAGE_MAP()
protected:
	CXTPGridHeader* m_pHeader;			   // Pointer to the associated grid header object.
	CXTPGridPaintManager* m_pPaintManager; // Pointer to the associated paint manager object.
	CXTPGridColumn* m_pColumn;			   // Pointer to the associated grid column object.
};

//===========================================================================
// Summary:
//     CXTPGridHeaderDropWnd is a CWnd derived class. It is used
//     internally to represent drop target arrows.
//===========================================================================
class _XTP_EXT_CLASS CXTPGridHeaderDropWnd : public CWnd
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a CXTPGridHeaderDropWnd object.
	// Parameters:
	//     crColor - Drop window color.
	//-----------------------------------------------------------------------
	CXTPGridHeaderDropWnd(COLORREF crColor);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPGridHeaderDropWnd object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPGridHeaderDropWnd();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Changes the window position.
	// Parameters:
	//     xPos - New horizontal position.
	//     yPos - New vertical position.
	//-----------------------------------------------------------------------
	void SetWindowPos(int xPos, int yPos);

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a drop window with a specified gap inside.
	// Parameters:
	//     pParentWnd - Pointer to the parent window.
	//     nHeight    - Height of the gap between two arrows, in pixels.
	// Returns:
	//     TRUE if the drop window was created successfully, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL Create(CWnd* pParentWnd, int nHeight);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_VIRTUAL(CXTPGridHeaderDropWnd)
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPGridHeaderDropWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect,
				CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	DECLARE_MESSAGE_MAP()

protected:
	COLORREF m_clr; // Drawing brush.
	CRgn m_rgn;		// Arrows region.
	int m_nHeight;  // Height of the gap between two arrows.
};

//////////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTPGridHeaderDragWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
											  DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
											  UINT nID, CCreateContext* pContext)
{
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE BOOL CXTPGridHeaderDropWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
											  DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
											  UINT nID, CCreateContext* pContext)
{
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPGRIDDRAGDROP_H__)
