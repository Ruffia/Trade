// XTPPopupItem.h: interface for the CXTPPopupItem class.
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
#if !defined(_XTPPOPUPITEM_H__)
#	define _XTPPOPUPITEM_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPPopupControl;
class CXTPImageManagerIcon;
class CXTPRichRender;
class CXTPMarkupUIElement;

//-----------------------------------------------------------------------
// Summary:
//     Enumeration used to determine the icon type of CXTPPopupItem.
// Example:
// <code>
// CXTPPopupItem* pItemIcon = m_wndPopupControl.AddItem(
//     new CXTPPopupItem(CRect(14, 40, 20, 49)));
// pItemIcon->SetIcons(IDB_LOGOOFFICE, 0, xtpPopupItemIconNormal);
// </code>
// See Also:
//     CXTPPopupItem::SetIcon, CXTPPopupItem::SetIcons
//
// <KEYWORDS xtpPopupItemIconNormal, xtpPopupItemIconSelected, xtpPopupItemIconPressed>
//-----------------------------------------------------------------------
enum XTPPopupItemIcon
{
	xtpPopupItemIconNormal   = 1, // Normal icon handler.
	xtpPopupItemIconSelected = 2, // Selected icon handler.
	xtpPopupItemIconPressed  = 4  // Pressed icon handler.
};

//===========================================================================
// Summary:
//     Special ID for close item.
//===========================================================================
#	define XTP_ID_POPUP_CLOSE -1

//===========================================================================
// Summary:
//     CXTPPopupItem is a class used to manage a pop-up item object.
//===========================================================================
class _XTP_EXT_CLASS CXTPPopupItem : public CXTPCmdTarget
{
	DECLARE_DYNCREATE(CXTPPopupItem)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPopupItem object.
	// Parameters:
	//     rcItem        - Bounding rectangle of the item.
	//     lpszCaption   - NULL-terminated string specifying the caption text.
	//     clrBack       - Background color of the item.
	//     clrBorder     - Border color of the item
	//-----------------------------------------------------------------------
	CXTPPopupItem(LPRECT rcItem = NULL, LPCTSTR lpszCaption = NULL, COLORREF clrBack = (COLORREF)-1,
				  COLORREF clrBorder = (COLORREF)-1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPopupItem object.
	//-----------------------------------------------------------------------
	~CXTPPopupItem();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the item.
	// Parameters:
	//     pDC - A CDC pointer that represents the current device context.
	//-----------------------------------------------------------------------
	virtual void Draw(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the bounding rectangle of an item.
	// Returns:
	//     The bounding rectangle of the item.
	//-----------------------------------------------------------------------
	CRect GetRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the button state of an item.
	// Returns:
	//     TRUE if button state is enabled, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsButton() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the button state for an item.
	// Parameters:
	//     bButton - TRUE to enable button state, FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetButton(BOOL bButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the selected state of an item.
	// Returns:
	//     TRUE if the item is selected, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsSelected() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the pressed state of an item.
	// Returns:
	//     TRUE if the item is pressed, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsPressed() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the ID number for an item.
	// Parameters:
	//     nID - The ID number value for the item.
	//-----------------------------------------------------------------------
	void SetID(int nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the ID number of an item.
	// Returns:
	//     The ID number value of the item.
	//-----------------------------------------------------------------------
	int GetID() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the text for an item.
	// Parameters:
	//     str - A LPCTSTR object that represents the item text.
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR str);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the text of an item.
	// Returns:
	//     A CString object that represents the item text.
	//-----------------------------------------------------------------------
	CString GetCaption() const;

	// ------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the RichText for an item.
	// Parameters:
	//     str         -  A LPCTSTR object that represents the item text.
	//     nIDResource -  The ID number value of the resource used.
	// ------------------------------------------------------------------
	void SetRTFText(LPCTSTR str);
	void SetRTFText(UINT nIDResource); // <combine CXTPPopupItem::SetRTFText@LPCTSTR>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the Markup text for the item.
	// Parameters:
	//     str - A LPCTSTR object that represents the Markup text.
	//-----------------------------------------------------------------------
	void SetMarkupText(LPCTSTR str);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to set item size using the size of its content.
	//-------------------------------------------------------------------------
	void FitToContent();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the color for an item's text.
	// Parameters:
	//     clrText    - An RGB color value that represents the item's text color.
	//     clrHotText - An RGB color value that represents the item's text color
	//                  when the mouse is hovering over it.
	//-----------------------------------------------------------------------
	void SetTextColor(COLORREF clrText, COLORREF clrHotText = (COLORREF)-1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the color of an item's text.
	// Returns:
	//     A COLORREF object that represents the color of an item's text.
	//-----------------------------------------------------------------------
	COLORREF GetTextColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the background color of an item.
	// Returns:
	//     The background color of a pop-up control item.
	//-----------------------------------------------------------------------
	COLORREF GetBackgroundColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the border color of an item.
	// Returns:
	//     The border color of a pop-up control item.
	//-----------------------------------------------------------------------
	COLORREF GetBorderColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the alignment style for an item's text.
	// Parameters:
	//     nAlign - The alignment style of an item's text.
	//-----------------------------------------------------------------------
	void SetTextAlignment(int nAlign);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the alignment style of an item's text.
	// Returns:
	//     The alignment style of an item's text.
	//-----------------------------------------------------------------------
	int GetTextAlignment() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the font for an item's text.
	// Parameters:
	//     pFntText - Pointer to a CFont object.
	//     pLogfText - Pointer to a LOGFONT structure.
	//-----------------------------------------------------------------------
	void SetTextFont(CFont* pFntText);
	void SetTextFont(PLOGFONT pLogfText); // <combine CXTPPopupItem::SetTextFont@CFont*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the font of an item's text.
	// Returns:
	//     Pointer to a CFont object.
	//-----------------------------------------------------------------------
	virtual CFont* GetTextFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if hyperlink support is enabled
	//     for a text item.
	// Returns:
	//     TRUE if hyperlink support is enabled, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsHyperLink() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the sytem hand cursor should
	//     be displayed when the mouse moves over this item.
	// Returns:
	//     TRUE if the hand cursor should be displayed, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsHandCursor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable hyperlink support for a text item.
	// Parameters:
	//     bHyperLink - TRUE to enable hyperlink support, FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetHyperLink(BOOL bHyperLink);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable the system hand cursor displayed
	//     when the mouse moves over this item.
	// Parameters:
	//     bHandCursor - TRUE to enable the system hand cursor, FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetHandCursor(BOOL bHandCursor);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the bold state of a text item.
	// Returns:
	//     TRUE if the item state is bold, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsBold() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the underlined state of a text item.
	//     The item state is underlined if the item is a hyperlink whose
	//     text is drawn underlined when the mouse hovers over it.
	// Returns:
	//     TRUE if the item state is underlined when the mouse hovers over it,
	//     otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsUnderline() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the bold state for a text item.
	// Parameters:
	//     bBold - TRUE to set the item state to bold, FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetBold(BOOL bBold);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the underlined state of a text item.
	//     If bUnderline is TRUE and the item is a hyperlink, then text
	//     will be drawn underlined when the mouse hovers over it.
	// Parameters:
	//     bUnderline - TRUE to set the item state to underlined when the mouse
	//                  hovers over it, FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetUnderline(BOOL bUnderline);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to update the rectangular height of a text.
	//-----------------------------------------------------------------------
	void CalculateHeight();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to update the rectangular width of a text.
	//-----------------------------------------------------------------------
	void CalculateWidth();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the normal, selected, or pressed
	//     icon from an icon item.
	// Parameters:
	//     hIcon - Handle to the icon.
	//     itemIcon - Type of icon in use. See XTPPopupItemIcon.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL SetIcon(HICON hIcon, XTPPopupItemIcon itemIcon = xtpPopupItemIconNormal);
	BOOL SetIcon(UINT nIDIcon,
				 XTPPopupItemIcon itemIcon =
					 xtpPopupItemIconNormal); // <combine
											  // CXTPPopupItem::SetIcon@HICON@XTPPopupItemIcon>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the normal, selected, or pressed
	//     icon from a resource bitmap picture.
	// Parameters:
	//     nIDBitmap - Resource ID for the bitmap.
	//     hBitmap - Bitmap handle to set.
	//     clrTransparent - Transparent color.
	//     itemIcon - Type of icon in use. See XTPPopupItemIcon.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL SetIcons(UINT nIDBitmap, COLORREF clrTransparent, int itemIcon);
	BOOL SetIcons(HBITMAP hBitmap, COLORREF clrTransparent, int itemIcon,
				  BOOL bAlphaBitmap = FALSE); // <combine CXTPPopupItem::SetIcons@UINT@COLORREF@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the icon image associated with the item.
	// Returns:
	//     A CXTPImageManagerIcon pointer to the icon.
	//-----------------------------------------------------------------------
	CXTPImageManagerIcon* GetImage() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the parent CXTPPopupControl object.
	// Returns:
	//     A pointer to the parent CXTPPopupControl object.
	//-----------------------------------------------------------------------
	CXTPPopupControl* GetPopupControl() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Calculates the offset to center an icon.
	//-------------------------------------------------------------------------
	void CenterIcon();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the icon offset.
	// Returns:
	//     Position of the icon inside the item's bounding rectangle.
	//-----------------------------------------------------------------------
	CPoint GetIconOffset() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the icon index of the item.
	// Returns:
	//     Icon index.
	// See Also:
	//     SetIconIndex, CXTPPopupControl::GetImageManager
	//-----------------------------------------------------------------------
	int GetIconIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the icon index of the item.
	// Parameters:
	//     nIconIndex - Icon index to be set.
	// See Also:
	//     GetIconIndex, CXTPPopupControl::GetImageManager
	//-----------------------------------------------------------------------
	void SetIconIndex(int nIconIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the Markup element that renders the item.
	// Returns:
	//     Pointer to the Markup element.
	//-----------------------------------------------------------------------
	CXTPMarkupUIElement* GetMarkupUIElement() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the Rich Render element that renders the item.
	// Returns:
	//     Pointer to the Rich Render element.
	//-----------------------------------------------------------------------
	CXTPRichRender* GetRichRender() const;

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to redraw the parent control.
	//-------------------------------------------------------------------------
	void RedrawControl();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called when an item is inserted in the control.
	//-------------------------------------------------------------------------
	virtual void OnItemInserted();

protected:
	int m_nID;		   // ID of the pop-up item.
	int m_nIndex;	  // Zero-based index of the item in the parent pop-up control's item list.
	int m_nIconIndex;  // Zero-based index of the item's icon in the parent control's image list.
	BOOL m_bButton;	// TRUE if the pop-up item is a button, otherwise FALSE.
	BOOL m_bHyperLink; // TRUE if the pop-up item is a hyperlink, otherwise FALSE.
	BOOL m_bBold;	  // TRUE if the pop-up item's text is bold, otherwise FALSE.
	BOOL m_bUnderline; // TRUE if the pop-up item's text is underlined on mouse over, otherwise
					   // FALSE.
	UINT m_nAlign; // Specifies the method of formatting the text. It can be any combination of the
				   // values described for the nAlign parameter in CDC::DrawText.

	CXTPFont m_xtpFontText; // Pop-up item's fntText.
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fntText, m_xtpFontText, GetTextFontHandle);

	CRect m_rcItem;	// Size and location of the pop-up item.
	CPoint m_ptOffset; // x- and y- coordinates representing the offset for the pop-up item's icon.
	CString m_strCaption;  // Pop-up item's display text.
	COLORREF m_clrText;	// An RGB color value representing the item's normal text color.
	COLORREF m_clrHotText; // An RGB color value representing the item's mouse over text color.
	COLORREF m_clrBack;	// An RGB color value representing the item's background color.
	COLORREF m_clrBorder;  // An RGB color value representing the item's border color.
	CXTPPopupControl* m_pControl;  // Pointer to the CXTPPopupControl object containing this item/
	CXTPRichRender* m_pRichRender; // Richtext Render.
	CXTPMarkupUIElement* m_pUIElement; // Markup Element that renders the item.

protected:
#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPPopupItem)

	void OleSetIcons(int hBitmap, COLORREF clrTransparent, int itemIcon);
	void OleSetIcon(int hIcon, int itemIcon);
	LPFONTDISP OleGetFont();
	void OleSetFont(LPFONTDISP pFontDisp);

	afx_msg long OleGetLeft();
	afx_msg void OleSetLeft(long nLeft);
	afx_msg long OleGetTop();
	afx_msg void OleSetTop(long nTop);
	afx_msg long OleGetWidth();
	afx_msg void OleSetWidth(long nWidth);
	afx_msg long OleGetHeight();
	afx_msg void OleSetHeight(long nHeight);
	afx_msg BSTR OleGetCaption();

	enum
	{
		dispidCaption = 1L,
	};
//}}AFX_CODEJOCK_PRIVATE
#	endif
	friend class CXTPPopupControl;
	friend class CXTPPopupPaintManager;
};

AFX_INLINE CRect CXTPPopupItem::GetRect() const
{
	return m_rcItem;
}
AFX_INLINE BOOL CXTPPopupItem::IsButton() const
{
	return m_bButton;
}
AFX_INLINE void CXTPPopupItem::SetButton(BOOL bButton)
{
	m_bButton = bButton;
}
AFX_INLINE void CXTPPopupItem::SetID(int nID)
{
	m_nID = nID;
}
AFX_INLINE int CXTPPopupItem::GetID() const
{
	return m_nID;
}
AFX_INLINE COLORREF CXTPPopupItem::GetTextColor() const
{
	return IsSelected() ? ((m_clrHotText != (COLORREF)-1) ? m_clrHotText : m_clrText) : m_clrText;
}
AFX_INLINE int CXTPPopupItem::GetTextAlignment() const
{
	return XTPToInt(m_nAlign);
}
AFX_INLINE void CXTPPopupItem::SetTextAlignment(int nAlign)
{
	m_nAlign = XTPToUInt(nAlign);
	RedrawControl();
}
AFX_INLINE CString CXTPPopupItem::GetCaption() const
{
	return m_strCaption;
}
AFX_INLINE void CXTPPopupItem::SetTextColor(COLORREF clrText, COLORREF clrHotText)
{
	m_clrText	= clrText;
	m_clrHotText = clrHotText;
	RedrawControl();
}
AFX_INLINE BOOL CXTPPopupItem::IsHyperLink() const
{
	return m_bHyperLink;
}
AFX_INLINE BOOL CXTPPopupItem::IsBold() const
{
	return m_bBold;
}
AFX_INLINE BOOL CXTPPopupItem::IsUnderline() const
{
	return m_bUnderline;
}
AFX_INLINE void CXTPPopupItem::SetBold(BOOL bBold)
{
	m_bBold = bBold;
	RedrawControl();
}
AFX_INLINE void CXTPPopupItem::SetUnderline(BOOL bUnderline)
{
	m_bUnderline = bUnderline;
	RedrawControl();
}
AFX_INLINE void CXTPPopupItem::SetHyperLink(BOOL bHyperLink)
{
	m_bHyperLink = bHyperLink;
}
AFX_INLINE COLORREF CXTPPopupItem::GetBackgroundColor() const
{
	return m_clrBack;
}
AFX_INLINE COLORREF CXTPPopupItem::GetBorderColor() const
{
	return m_clrBorder;
}
AFX_INLINE CPoint CXTPPopupItem::GetIconOffset() const
{
	return m_ptOffset;
}
AFX_INLINE void CXTPPopupItem::SetIconIndex(int nIconIndex)
{
	m_nIconIndex = nIconIndex;
	RedrawControl();
}
AFX_INLINE int CXTPPopupItem::GetIconIndex() const
{
	return m_nIconIndex == -1 ? m_nIndex : m_nIconIndex;
}
AFX_INLINE CXTPPopupControl* CXTPPopupItem::GetPopupControl() const
{
	return m_pControl;
}
AFX_INLINE CXTPMarkupUIElement* CXTPPopupItem::GetMarkupUIElement() const
{
	return m_pUIElement;
}
AFX_INLINE CXTPRichRender* CXTPPopupItem::GetRichRender() const
{
	return m_pRichRender;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(_XTPPOPUPITEM_H__)
