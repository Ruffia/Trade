// XTPColorPageStandard.h : header file
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
#if !defined(__XTPCOLORSTANDARD_H__)
#	define __XTPCOLORSTANDARD_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPColorHex is a CStatic derived class. It displays a color selection
//     grid used by CXTPColorPageStandard.
//===========================================================================
class _XTP_EXT_CLASS CXTPColorHex : public CStatic
{
	DECLARE_DYNAMIC(CXTPColorHex)

protected:
	//===========================================================================
	// Summary:
	//     CRgnCell is a CRgn derived class used by the CXTPColorHex to create
	//     individual color cells.
	//===========================================================================
	class CRgnCell : public CRgn
	{
	public:
		//===========================================================================
		// Summary:
		//     Contains pre-computed hexagon data for both small and big hexagons.
		//===========================================================================
		struct CTX_DATA
		{
			//===========================================================================
			// Summary:
			//     Describes pre-computed hexagon data.
			//===========================================================================
			struct HEXAGON_DATA
			{
				float width;
				float half;
				float height;
				float roof;
				float side;
			};
			HEXAGON_DATA lg;
			HEXAGON_DATA sm;
		};

		//-----------------------------------------------------------------------
		// Summary:
		//     Constructs a CRgnCell object.
		// Parameters:
		//     data   - Pre-computed data.
		//     rect   - Returns a reference to a CRect object that represents the
		//              size and position of the color cell.
		//     color  - An RGB value that represents the color of the cell to draw.
		//     u      - Index of the cell to be selected when VK_UP is pressed.
		//     d      - Index of the cell to be selected when VK_DOWN is pressed.
		//     x      - Specifies the left position of CRgnCell.
		//     y      - Specifies the top of CRgnCell.
		//     bLarge - TRUE if the cell size is large.
		//-----------------------------------------------------------------------
		CRgnCell(const CTX_DATA& data, CXTPRectF& rect, COLORREF color, float u, float d);
		CRgnCell(const CTX_DATA& data, CXTPRectF& rect, COLORREF color, float u, float d, float x,
				 float y, BOOL bLarge = FALSE); // <combine CXTPColorHex::CRgnCell::CRgnCell@const
												// CTX_DATA&@CXTPRectF&@COLORREF@float@float>

	protected:
		//-----------------------------------------------------------------------
		// Summary:
		//     Used by the CRgnCell to render each color cell.
		// Parameters:
		//     pDC - A CDC pointer that represents the current device context.
		// Returns:
		//     TRUE if the color cell was successfully drawn, otherwise FALSE.
		//-----------------------------------------------------------------------
		virtual BOOL DrawCell(CDC* pDC);

		//-----------------------------------------------------------------------
		// Summary:
		//     Used by the CRgnCell to display a selection around the active
		//     color cell.
		// Parameters:
		//     pDC - A CDC pointer that represents the current device context.
		// Returns:
		//     TRUE if the color cell was successfully drawn, otherwise FALSE.
		//-----------------------------------------------------------------------
		virtual BOOL DrawSelection(CDC* pDC);

		int m_iCell[2];   // Index array that indicates which cell should be selected when the up or
						  // down arrow key is pressed.
		COLORREF m_color; // An RGB value that represents the color of the cell.
		CXTPRectF m_rect; // Represents the size and position of the color cell.
		CXTPPoint2f m_pts[6]; // CPoint array that represents the x- and y- coordinates for each
							  // vertex of the cell's polygon.

		friend class CXTPColorHex;

	private:
		static void AFX_CDECL ConvertFloatToIntPoints(const CXTPPoint2f (&from)[6],
													  CPoint (&to)[6]);

		static void AFX_CDECL ComputeHexagon(const CTX_DATA::HEXAGON_DATA& data, float left,
											 float top, CXTPPoint2f (&points)[6]);
	};

	// ----------------------------------------------------------------------
	// Summary:
	//     List for maintaining CRgnCell objects.
	// Remarks:
	//     CList definition used by the CXTPColorHex control to maintain
	//     a list of CRgnCell objects representing each color cell in
	//     the hex color control.
	// See Also:
	//     CXTPColorHex, CRgnCell
	// ----------------------------------------------------------------------
	typedef CList<CRgnCell*, CRgnCell*> CRgnCellList;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPColorHex object.
	//-----------------------------------------------------------------------
	CXTPColorHex();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPColorHex object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPColorHex();

public:
	// ----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve RGB information for the
	//     color found at a location specified by 'point'.
	// Parameters:
	//     point - Represents the x- and y- coordinates of the cell to
	//             retrieve RGB color information for.
	// Returns:
	//     A COLORREF value.
	// ----------------------------------------------------------------------
	COLORREF ColorFromPoint(CPoint point);

	// ----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve a pointer to a CRgnCell object
	//     that represents information for a color cell specified by either
	//     the list 'iIndex' or RGB 'color'.
	// Parameters:
	//     iIndex - Index into m_rgnList list.
	//     color  - An RGB value that represents the color.
	// Returns:
	//     A pointer to a CRgnCell object.
	// ----------------------------------------------------------------------
	CRgnCell* GetColorCell(int iIndex);
	CRgnCell* GetColorCell(COLORREF color); // <combine CXTPColorHex::GetColorCell@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the selected color using either
	//     the list 'iIndex' or RGB 'color'.
	// Parameters:
	//     iIndex  - Index into m_rgnList list.
	//     color   - An RGB value that represents the color.
	//     bNotify - TRUE to send a WM_XTP_UPDATECOLOR notification to the
	//               owner window indicating that the color has changed.
	// Returns:
	//     TRUE if the color cell was selected, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL SetSelectedColor(int iIndex, BOOL bNotify = TRUE);
	BOOL SetSelectedColor(COLORREF color,
						  BOOL bNotify = TRUE); // <combine CXTPColorHex::SetSelectedColor@int@BOOL>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the currently selected color.
	// Returns:
	//     An RGB color value that represents the currently selected color.
	//-----------------------------------------------------------------------
	COLORREF GetSelectedColor() const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the CXTPColorHex class to
	//     perform initialization when the window is created or sub-classed.
	// Returns:
	//     TRUE if the window was successfully initialized, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual bool Init();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function updates the color selection based on the
	//     x- and y- coordinates specified by 'point'.
	// Parameters:
	//     point   - Current x- and y- coordinates of the color to select.
	//     pCell   - Pointer a CRgnCell object to select.
	//     bNotify - TRUE to send a WM_XTP_UPDATECOLOR notification to the
	//               owner window indicating that the color has changed.
	//-----------------------------------------------------------------------
	BOOL UpdateSelection(CPoint point, BOOL bNotify = TRUE);
	BOOL UpdateSelection(CRgnCell* pCell,
						 BOOL bNotify = TRUE); // <combine
											   // CXTPColorHex::UpdateSelection@CPoint@BOOL>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to determine if a specified
	//     color is defined in the color array.
	// Parameters:
	//     color - A COLORREF value to check.
	// Returns:
	//     true if the specified color is defined in the color array,
	//     otherwise false.
	//-----------------------------------------------------------------------
	virtual bool IsValidColor(COLORREF color) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the CXTPColorHex class to build
	//     the array of color cells used to render the hex color control.
	//-----------------------------------------------------------------------
	void CreateCellArray();

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPColorHex)
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPColorHex)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

	CBitmap m_bmpPicker;	 // Background device context.
	bool m_bPreSubclassInit; // true when initializing from PreSubclassWindow.
	COLORREF m_clrColor;	 // A COLORREF value that contains the RGB information for the current
							 // color.
	CRgnCellList m_rgnList;  // Array of CRgnCellList objects that represent displayed color cells.
	CRgnCell* m_pCell; // Pointer to a CRgnCell object that represents the currently selected color.
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE COLORREF CXTPColorHex::GetSelectedColor() const
{
	return m_clrColor;
}

//===========================================================================
// Summary:
//     CXTPColorPageStandard is derived from CPropertyPage. It is
//     used to create a CXTPColorPageStandard dialog.
//===========================================================================
class _XTP_EXT_CLASS CXTPColorPageStandard : public CPropertyPage
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPColorPageStandard object.
	// Parameters:
	//     pParentSheet - Pointer to the parent property sheet.
	//-----------------------------------------------------------------------
	CXTPColorPageStandard(CXTPColorDialog* pParentSheet = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPColorPageStandard object, handles cleanup and
	//     deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPColorPageStandard();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to set the selected color for
	//     the page.
	// Parameters:
	//     clr - An RGB value that represents the color.
	//-----------------------------------------------------------------------
	void SetColor(COLORREF clr);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPColorPageStandard)
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPColorPageStandard)
	afx_msg LRESULT OnUpdateColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColorDblClick(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	//{{AFX_DATA(CXTPColorPageStandard)
	enum
	{
		IDD = XTP_IDD_COLORSTANDARD
	};
	//}}AFX_DATA
	//}}AFX_CODEJOCK_PRIVATE

protected:
	CXTPColorHex m_colorHex;		 // ColorHex control.
	CXTPColorDialog* m_pParentSheet; // Pointer to the parent property sheet.

	friend class CXTPColorHex;

public:
	virtual BOOL OnSetActive();
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTPColorPageStandard::SetColor(COLORREF clr)
{
	m_colorHex.SetSelectedColor(clr, FALSE);
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPCOLORSTANDARD_H__)
