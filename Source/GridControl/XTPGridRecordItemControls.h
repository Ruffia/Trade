// XTPGridRecordItemControls.h
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
#if !defined(__XTPGRIDRECORDITEMCONTROLS_H__)
#	define __XTPGRIDRECORDITEMCONTROLS_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

struct XTP_NM_GRIDRECORDITEM;
struct XTP_GRIDRECORDITEM_CLICKARGS;
class CXTPGridRecordItemControl;

//-----------------------------------------------------------------------
// Summary:
//      This enum defines types for the item control.
//-----------------------------------------------------------------------
enum XTPGridItemControlType
{
	xtpItemControlTypeUndefined = 0, // Undefined type.

	xtpItemControlTypeButton = 1, // Button.
};

//-----------------------------------------------------------------------
// Summary:
//      This enum defines alignment styles for the item control.
//-----------------------------------------------------------------------
enum XTPGridItemControlAlignment
{
	xtpItemControlUnknown = 0, // Unknown (empty) value.

	xtpItemControlLeft  = 0x001, // Aligns control to the left.
	xtpItemControlRight = 0x002, // Aligns control to the right.
};

//-----------------------------------------------------------------------
// Summary:
//     This structure is sent to the Main window in a WM_NOTIFY message from
//     the item control and provides all parameters needed to process
//     control specific notifications by the main window.
// Remarks:
//     Use this structure to get the item control that produced the message.
// See Also:
//     XTP_NM_GRID_ITEMBUTTONCLICK
//-----------------------------------------------------------------------
struct XTP_NM_GRIDITEMCONTROL : public XTP_NM_GRIDRECORDITEM
{
	CXTPGridRecordItemControl* pItemControl; // Pointer to the item control.
};

//===========================================================================
// Summary:
//     This class represents a generic item control and handles control's
//     behaviour, common for all derived item controls.
//     You cannot use this class directly in the control. If you wish to add your
//     own item control, you must inherit it from CXTPGridRecordItemControl.
//     Some member functions in this class are virtual.
// See Also:
//     CXTPGridRecordItemButton
//===========================================================================
class _XTP_EXT_CLASS CXTPGridRecordItemControl : public CXTPCmdTarget
{
	DECLARE_SERIAL(CXTPGridRecordItemControl)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a CXTPGridRecordItemControl object.
	// Parameters:
	//     szCaption - Item control caption.
	//-----------------------------------------------------------------------
	CXTPGridRecordItemControl(LPCTSTR szCaption = _T(""));

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPGridRecordItemControl, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPGridRecordItemControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to store/load a grid item control
	//     using the specified data object.
	// Parameters:
	//     pPX - Source/destination CXTPPropExchange data object reference.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the item control caption.
	// Returns:
	//     The item control caption.
	// See Also:
	//     SetCaption
	//-----------------------------------------------------------------------
	CString GetCaption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the item control caption.
	// Parameters:
	//     szCaption - Item control caption to be set.
	// See Also:
	//     GetCaption
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR szCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the item control type.
	// Returns:
	//     The item control type.
	// See Also:
	//     XTPGridItemControlType
	//-----------------------------------------------------------------------
	int GetType() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the item control index.
	// Returns:
	//     The item control index.
	//-----------------------------------------------------------------------
	int GetIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the font of the item control caption.
	// Returns:
	//     A pointer to the font of the item control caption.
	// See Also:
	//     SetFont
	//-----------------------------------------------------------------------
	CFont* GetFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the font for the item control caption.
	// Parameters:
	//     pFont - New font to be set.
	// See Also:
	//     GetFont
	//-----------------------------------------------------------------------
	void SetFont(CFont* pFont);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the color of the item control caption.
	// Returns:
	//     The color of the item control caption.
	// See Also:
	//     SetCaptionColor
	//-----------------------------------------------------------------------
	COLORREF GetCaptionColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the color of the item control caption.
	// Parameters:
	//     clrCaption - New color to be set.
	// See Also:
	//     GetCaptionColor
	//-----------------------------------------------------------------------
	void SetCaptionColor(COLORREF clrCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the item control size.
	// Returns:
	//     The item control size.
	// See Also:
	//     SetSize, GetWidth, SetWidth, GetHeight, SetHeight
	//-----------------------------------------------------------------------
	CSize GetSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the item control size.
	// Parameters:
	//     sizeControl - Item control size to be set.
	// Remarks:
	//     If the item control size is set to (<=0, <=0), then the control will
	//     occupy the rest of the cell.
	// See Also:
	//     GetSize, GetWidth, SetWidth, GetHeight, SetHeight
	//-----------------------------------------------------------------------
	void SetSize(CSize sizeControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the item control width.
	// Returns:
	//     The item control width.
	// See Also:
	//     GetSize, SetSize, SetWidth, GetHeight, SetHeight
	//-----------------------------------------------------------------------
	int GetWidth() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the item control width.
	// Parameters:
	//     nWidth - Item control width to be set.
	// Remarks:
	//     If the width is set to less than 0, then the control will
	//     occupy the rest of the cell width.
	// See Also:
	//     GetSize, SetSize, GetWidth, GetHeight, SetHeight
	//-----------------------------------------------------------------------
	void SetWidth(int nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the item control height.
	// Returns:
	//     The item control height.
	// See Also:
	//     GetSize, SetSize, GetWidth, SetWidth, SetHeight
	//-----------------------------------------------------------------------
	int GetHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the item control height.
	// Parameters:
	//     nHeight - Item control height to be set.
	// Remarks:
	//     If the height is set to less than 0, then the control height will
	//     be the same as the cell height.
	// See Also:
	//     GetSize, SetSize, GetWidth, SetWidth, GetHeight
	//-----------------------------------------------------------------------
	void SetHeight(int nHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the item control rectangle
	//     (i.e. the rectangle occupied by the item control).
	// Returns:
	//     The item control rectangle.
	//-----------------------------------------------------------------------
	CRect GetRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the item control flags.
	// Returns:
	//     The item control flags.
	// See Also:
	//     SetFlags
	//-----------------------------------------------------------------------
	UINT GetFlags() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the item control flags.
	// Parameters:
	//     unFlags - Item control flags to be set.
	// See Also:
	//     GetFlags
	//-----------------------------------------------------------------------
	void SetFlags(UINT unFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the item control alignment.
	// Returns:
	//     The item control alignment.
	// See Also:
	//     XTPGridItemControlAlignment, SetAlignment
	//-----------------------------------------------------------------------
	int GetAlignment() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the item control alignment.
	// Parameters:
	//     nAlignment - Item control alignment to be set.
	// See Also:
	//     XTPGridItemControlAlignment, GetAlignment
	//-----------------------------------------------------------------------
	void SetAlignment(int nAlignment);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the item control is enabled/disabled.
	// Returns:
	//     TRUE if the item control is enabled,
	//     FALSE if the item control is disabled.
	// See Also:
	//     SetEnable
	//-----------------------------------------------------------------------
	BOOL GetEnable() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables the item control.
	// Parameters:
	//     bEnable - TRUE to enable the item control,
	//               FALSE to disable the item control.
	// Remarks:
	//     If the control is disabled, then it is drawn as disabled and
	//     will not react to user input.
	// See Also:
	//     GetEnable
	//-----------------------------------------------------------------------
	void SetEnable(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the item control state.
	// Remarks:
	//     The item control state is control specific.
	// Returns:
	//     The item control state.
	//-----------------------------------------------------------------------
	int GetState() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies the themed state of the item control.
	// Parameters:
	//     bThemed - TRUE to set the item control to themed, FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetThemed(BOOL bThemed);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the themed state of the item control.
	// Returns:
	//     TRUE if item control is themed, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL GetThemed() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the icon index for a specified control state.
	// Parameters:
	//     nState     - Item control state.
	//     nIconIndex - Icon index for the specified state.
	// Remarks:
	//     The icon specified for the specified control state is drawn during
	//     control drawing.
	//     If no icon is specified for a certain state, then the icon for
	//     state = 0 is drawn.
	//     If no icon specified for any state, then no icon is drawn.
	// See Also:
	//     GetIconIndex
	//-----------------------------------------------------------------------
	void SetIconIndex(int nState, int nIconIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the icon index of a specified item control state.
	// Parameters:
	//     nState - Item control state whose icon index must be retrieved.
	// Returns:
	//     The icon index of the specified item control state.
	// See Also:
	//     SetIconIndex
	//-----------------------------------------------------------------------
	int GetIconIndex(int nState) const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the item control rectangle
	//     (i.e. the rectangle occupied by the item control).
	// Parameters:
	//     rcControl - Item control rectangle to be set.
	// See Also:
	//     GetRect
	//-----------------------------------------------------------------------
	void SetRect(CRect rcControl);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Processes mouse left button down clicks.
	// Parameters:
	//     pClickArgs - Pointer to an XTP_GRIDRECORDITEM_CLICKARGS
	//                  structure containing mouse click arguments.
	// Remarks:
	//     Usually this function is called by GridRecordItem's OnLButtonDown function.
	// See Also:
	//     XTP_GRIDRECORDITEM_CLICKARGS
	//-----------------------------------------------------------------------
	virtual void OnLButtonDown(XTP_GRIDRECORDITEM_CLICKARGS* pClickArgs)
	{
		UNREFERENCED_PARAMETER(pClickArgs);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes mouse left button up clicks.
	// Parameters:
	//     pClickArgs - Pointer to an XTP_GRIDRECORDITEM_CLICKARGS
	//                  structure containing mouse click arguments.
	// Remarks:
	//     Usually this function is called by GridRecordItem's OnLButtonUp function.
	// See Also:
	//     XTP_GRIDRECORDITEM_CLICKARGS
	//-----------------------------------------------------------------------
	virtual void OnLButtonUp(XTP_GRIDRECORDITEM_CLICKARGS* pClickArgs)
	{
		UNREFERENCED_PARAMETER(pClickArgs);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Notifies the item control that the mouse cursor has entered
	//     the item control's area.
	// Parameters:
	//     nFlags - Additional flags.
	//     point  - Mouse cursor position.
	// Remarks:
	//     Usually this function is called by GridRecordItem's OnMouseMove function.
	//-----------------------------------------------------------------------
	virtual void OnMouseEnter(UINT nFlags, CPoint point)
	{
		UNREFERENCED_PARAMETER(nFlags);
		UNREFERENCED_PARAMETER(point);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Notifies the item control that the mouse cursor has left
	//     the item control's area.
	// Parameters:
	//     nFlags - Additional flags.
	//     point  - Mouse cursor position.
	// Remarks:
	//     Usually this function is called by GridRecordItem's OnMouseMove function.
	//-----------------------------------------------------------------------
	virtual void OnMouseLeave(UINT nFlags, CPoint point)
	{
		UNREFERENCED_PARAMETER(nFlags);
		UNREFERENCED_PARAMETER(point);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes the move mouse event.
	// Parameters:
	//     nFlags - Additional flags.
	//     point  - Mouse cursor position.
	// Remarks:
	//     Usually this function is called by GridRecordItem's OnMouseMove function.
	//-----------------------------------------------------------------------
	virtual void OnMouseMove(UINT nFlags, CPoint point)
	{
		UNREFERENCED_PARAMETER(nFlags);
		UNREFERENCED_PARAMETER(point);
	}

protected:
	int m_nType;		  // Control type. See XTPGridItemControlType for details.
	int m_nIndex;		  // Control index.
	CString m_strCaption; // Control caption.

	CXTPFont m_xtpFontCaption; // Control caption font.
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fntCaption, m_xtpFontCaption,
										  GetCaptionFontHandle);

	COLORREF m_clrCaption; // Control caption color.
	CSize m_sizeControl;   // Control size. If size is (<=0, <=0), then the control occupies the
						   // entire cell.
	CRect m_rcControl;	 // Control rectangle.
	UINT m_unFlags;		   // Control flags.
	XTPGridItemControlAlignment m_Alignment; // Control alignment. See
											 // XTPGridItemControlAlignment for details.
	BOOL m_bEnabled;						 // Control enable flag.
	int m_nState;							 // Control state. Depends on control type.
	BOOL m_bThemed;							 // TRUE if control is themed, FALSE otherwise.
	CMap<int, int, int, int> m_mapIcon;		 // Control icon map.

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPGridRecordItemControl);

	afx_msg BSTR OleGetCaption();
	afx_msg void OleSetCaption(LPCTSTR pcszCaption);

	LPFONTDISP OleGetFont();
	void OleSetFont(LPFONTDISP pFontDisp);

	afx_msg void OleSetSize(long cx, long cy);
	afx_msg void OleGetRect(long* pnLeft, long* pnTop, long* pnRight, long* pnBottom);
//}}AFX_CODEJOCK_PRIVATE
#	endif

	friend class CXTPGridRecordItemControls;
	friend class CXTPGridRecordItemButton;
	friend class CXTPGridPaintManager;
};

//===========================================================================
// Summary:
//     This class represents an item button and inherits basic functionality
//     from the CXTPGridRecordItemControl class.
//     You create an item button simply by calling its constructor with one
//     parameter - a text string.
// See Also:
//     CXTPGridRecordItemControl
//===========================================================================
class _XTP_EXT_CLASS CXTPGridRecordItemButton : public CXTPGridRecordItemControl
{
	DECLARE_SERIAL(CXTPGridRecordItemButton)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates an item button.
	// Parameters:
	//     szCaption - Button caption.
	//-----------------------------------------------------------------------
	CXTPGridRecordItemButton(LPCTSTR szCaption = _T(""));

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to store/load a grid item button
	//     using the specified data object.
	// Parameters:
	//     pPX - Source/destination CXTPPropExchange data object reference.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes mouse left button down clicks.
	// Parameters:
	//     pClickArgs - Pointer to an XTP_GRIDRECORDITEM_CLICKARGS
	//                  structure containing mouse click arguments.
	// Remarks:
	//     Usually this function is called by GridRecordItem's OnLButtonDown function.
	// See Also:
	//     XTP_GRIDRECORDITEM_CLICKARGS
	//-----------------------------------------------------------------------
	virtual void OnLButtonDown(XTP_GRIDRECORDITEM_CLICKARGS* pClickArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes mouse left button up clicks.
	// Parameters:
	//     pClickArgs - Pointer to an XTP_GRIDRECORDITEM_CLICKARGS
	//                  structure containing mouse click arguments.
	// Remarks:
	//     Usually this function is called by GridRecordItem's OnLButtonUp function.
	// See Also:
	//     XTP_GRIDRECORDITEM_CLICKARGS
	//-----------------------------------------------------------------------
	virtual void OnLButtonUp(XTP_GRIDRECORDITEM_CLICKARGS* pClickArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Notifies the item control that the mouse cursor has entered
	//     the item button's area.
	// Parameters:
	//     nFlags - Additional flags.
	//     point  - Mouse cursor position.
	// Remarks:
	//     Usually this function is called by GridRecordItem's OnMouseMove function.
	//-----------------------------------------------------------------------
	virtual void OnMouseEnter(UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     Notifies the item control that the mouse cursor has left
	//     the item button's area.
	// Parameters:
	//     nFlags - Additional flags.
	//     point  - Mouse cursor position.
	// Remarks:
	//     Usually this function is called by GridRecordItem's OnMouseMove function.
	//-----------------------------------------------------------------------
	virtual void OnMouseLeave(UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes the move mouse event.
	// Parameters:
	//     nFlags - Additional flags.
	//     point  - Mouse cursor position.
	// Remarks:
	//     Usually this function is called by GridRecordItem's OnMouseMove function.
	//-----------------------------------------------------------------------
	virtual void OnMouseMove(UINT nFlags, CPoint point);

protected:
	int m_nSavedState; // Button state when the mouse cursor leaves the button area.

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPGridRecordItemButton);
//}}AFX_CODEJOCK_PRIVATE
#	endif
};

//===========================================================================
// Summary:
//      This typedef is used to declare a base class for CXTPGridRecordItemControls.
//===========================================================================
typedef CXTPArrayT<CXTPGridRecordItemControl*, CXTPGridRecordItemControl*, LPDISPATCH>
	CXTPGridRecordItemControls_base;

//===========================================================================
// Summary:
//     This class represents the inplace controls of a single item.
//===========================================================================
//===========================================================================
// Summary:
//      This class represents an array of pointers to CXTPGridRecordItemControl objects.
//      InternalRelease is called for each item in the array destructor.
//      Also serialization for array items is provided (see the DoPropExchange method).
//===========================================================================
class _XTP_EXT_CLASS CXTPGridRecordItemControls : public CXTPGridRecordItemControls_base
// class _XTP_EXT_CLASS CXTPGridRecordItemControls : public
// CXTPHeapObjectT<CXTPGridRecordItemControls_base, CXTPGridDataAllocator>
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNAMIC(CXTPGridRecordItemControls)
	typedef CXTPGridRecordItemControls_base TBase;
	//}}AFX_CODEJOCK_PRIVATE
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPGridRecordItemControls object.
	// Parameters:
	//     pRecordItem - Pointer to the parent record item.
	//-------------------------------------------------------------------------
	CXTPGridRecordItemControls();
	CXTPGridRecordItemControls(CXTPGridRecordItem* pRecordItem);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPGridRecordItemControls object,
	//     handles cleanup and deallocation
	//-------------------------------------------------------------------------
	~CXTPGridRecordItemControls();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to store/load a grid item controls list
	//     using the specified data object.
	// Parameters:
	//     pPX - Source/destination CXTPPropExchange data object reference.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a new control to the controls list.
	// Parameters:
	//     nType  - Type of control to be added.
	//     nIndex - Ordinal for position.
	// Returns:
	//     A pointer to the newly added grid record item control object.
	//-----------------------------------------------------------------------
	CXTPGridRecordItemControl* AddControl(int nType, int nIndex = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a new control to the controls list.
	// Parameters:
	//     pControl - Pointer to the control to be added.
	//     nIndex   - Ordinal for position
	// Returns:
	//     A pointer to the newly added grid record item control object.
	//-----------------------------------------------------------------------
	CXTPGridRecordItemControl* AddControl(CXTPGridRecordItemControl* pControl, int nIndex = -1);

	//-------------------------------------------------------------------------
	// Summary:
	//     Removes all controls from the list.
	//-------------------------------------------------------------------------
	// void RemoveControls();

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all controls from the list.
	//-----------------------------------------------------------------------
	virtual void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes a control from the list by its index.
	// Parameters:
	//     nIndex - Index of the control to remove.
	//          nCount - The number of elements to remove.
	//-----------------------------------------------------------------------
	virtual void RemoveAt(int nIndex);
	virtual void RemoveAt(int nIndex,
						  int nCount); // <combine CXTPGridRecordItemControls@RemoveAt@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Copies all controls from a specified CXTPGridRecordItemControls object
	//     to this CXTPGridRecordItemControls object.
	// Parameters:
	//     pSrc - Pointer to a CXTPGridRecordItemControls object to copy from.
	//-----------------------------------------------------------------------
	virtual void CopyFrom(CXTPGridRecordItemControls* pSrc);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Recalculates the indices of all record item controls in the list
	//     that appear at or after a specified index.
	// Parameters:
	//     nStartFrom - Index of the first item whose index must be recalculated.
	//-----------------------------------------------------------------------
	virtual void RefreshIndexes(int nIndexStart = 0);

public:
	CXTPGridRecordItem* m_pRecordItem; // Pointer to parent record item.

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPGridRecordItemControls);
	DECLARE_ENUM_VARIANT(CXTPGridRecordItemControls)

	afx_msg int OleGetCount();
	afx_msg LPDISPATCH OleAddButton(int nIndex);
	afx_msg void OleRemoveAt(int nIndex);
	// afx_msg LPDISPATCH OleGetButton(int nIndex);
	virtual LPDISPATCH OleGetItem(long nIndex);
//}}AFX_CODEJOCK_PRIVATE
#	endif
};

//===========================================================================
// Summary:
//     This class represents an item control hook window.
//     An instance of this window is created whenever the user presses the
//     left mouse button on an item control. Furthermore, all mouse input will
//     go to this window until the user releases the left mouse button. The
//     window hooks mouse events and redirects them to the grid record item.
// See Also:
//     CXTPGridRecordItem, CXTPGridRecordItemControl
//===========================================================================
class _XTP_EXT_CLASS CXTPGridRecordItemControlHookWnd : public CWnd
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPGridRecordItemControlHookWnd window.
	// Parameters:
	//     pClickArgs - Pointer to an XTP_GRIDRECORDITEM_CLICKARGS
	//                  structure containing the parameters of the cell.
	//-----------------------------------------------------------------------
	CXTPGridRecordItemControlHookWnd(XTP_GRIDRECORDITEM_CLICKARGS* pClickArgs);
	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPGridRecordItemControlHookWnd object,
	//     handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	~CXTPGridRecordItemControlHookWnd();

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_CODEJOCK_PRIVATE

	XTP_GRIDRECORDITEM_CLICKARGS m_ClickArgs; // Parameters of the cell.
};

//////////////////////////////////////////////////////////////////////////
AFX_INLINE void CXTPGridRecordItemControl::SetCaption(LPCTSTR szCaption)
{
	m_strCaption = szCaption;
}
AFX_INLINE CString CXTPGridRecordItemControl::GetCaption() const
{
	return m_strCaption;
}
AFX_INLINE int CXTPGridRecordItemControl::GetType() const
{
	return m_nType;
}
AFX_INLINE int CXTPGridRecordItemControl::GetIndex() const
{
	return m_nIndex;
}
AFX_INLINE COLORREF CXTPGridRecordItemControl::GetCaptionColor() const
{
	return m_clrCaption;
}
AFX_INLINE void CXTPGridRecordItemControl::SetCaptionColor(COLORREF clrCaption)
{
	m_clrCaption = clrCaption;
}
AFX_INLINE CSize CXTPGridRecordItemControl::GetSize() const
{
	return m_sizeControl;
}
AFX_INLINE void CXTPGridRecordItemControl::SetSize(CSize sizeControl)
{
	m_sizeControl = sizeControl;
}
AFX_INLINE int CXTPGridRecordItemControl::GetWidth() const
{
	return m_sizeControl.cx;
}
AFX_INLINE void CXTPGridRecordItemControl::SetWidth(int nWidth)
{
	m_sizeControl.cx = nWidth;
}
AFX_INLINE int CXTPGridRecordItemControl::GetHeight() const
{
	return m_sizeControl.cy;
}
AFX_INLINE void CXTPGridRecordItemControl::SetHeight(int nHeight)
{
	m_sizeControl.cy = nHeight;
}
AFX_INLINE CRect CXTPGridRecordItemControl::GetRect() const
{
	return m_rcControl;
}
AFX_INLINE void CXTPGridRecordItemControl::SetFlags(UINT unFlags)
{
	m_unFlags = unFlags;
}
AFX_INLINE UINT CXTPGridRecordItemControl::GetFlags() const
{
	return m_unFlags;
}
AFX_INLINE int CXTPGridRecordItemControl::GetAlignment() const
{
	return m_Alignment;
}
AFX_INLINE void CXTPGridRecordItemControl::SetAlignment(int nAlignment)
{
	m_Alignment = (XTPGridItemControlAlignment)nAlignment;
}
AFX_INLINE BOOL CXTPGridRecordItemControl::GetEnable() const
{
	return m_bEnabled;
}
AFX_INLINE void CXTPGridRecordItemControl::SetEnable(BOOL bEnable)
{
	m_bEnabled = bEnable;
}
AFX_INLINE int CXTPGridRecordItemControl::GetState() const
{
	return m_nState;
}
AFX_INLINE void CXTPGridRecordItemControl::SetThemed(BOOL bThemed)
{
	m_bThemed = bThemed;
}
AFX_INLINE BOOL CXTPGridRecordItemControl::GetThemed() const
{
	return m_bThemed;
}
AFX_INLINE void CXTPGridRecordItemControl::SetIconIndex(int nState, int nIconIndex)
{
	m_mapIcon.SetAt(nState, nIconIndex);
}
AFX_INLINE void CXTPGridRecordItemControl::SetRect(CRect rcControl)
{
	m_rcControl = rcControl;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPGRIDRECORDITEMCONTROLS_H__)
