// XTPControlGalleryItem.h
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
#if !defined(__XTPCONTROLGALLERYITEM_H__)
#	define __XTPCONTROLGALLERYITEM_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPControlGalleryItem is a CXTPCmdTarget derived class. It represents
//     a single item of a gallery object.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlGalleryItem : public CXTPCmdTarget
{
	DECLARE_DYNAMIC(CXTPControlGalleryItem);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlGalleryItem object.
	//-----------------------------------------------------------------------
	CXTPControlGalleryItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPControlGalleryItem object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	~CXTPControlGalleryItem();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get the size of the item.
	// Returns:
	//     A CSize object containing the size of the item.
	// See Also:
	//     SetSize, CXTPControlGalleryItems::SetItemSize
	//-----------------------------------------------------------------------
	virtual CSize GetSize();

	static int AFX_CDECL GetDefaultHeight();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the size of the individual item. You can call
	//     CXTPControlGalleryItems::SetItemSize to set the default size for items.
	// Parameters:
	//     szItem - Size of the item to be set.
	//-----------------------------------------------------------------------
	void SetSize(CSize szItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the item.
	// Parameters:
	//     pDC       - Pointer to a valid device context.
	//     pGallery  - Pointer to the parent gallery of the item.
	//     rcItem    - Bounding rectangle of the item.
	//     bEnabled  - TRUE to draw the item enabled, FALSE to draw the item disabled.
	//     bSelected - TRUE to draw the item selected.
	//     bPressed  - TRUE to draw the item pressed.
	//     bChecked  - TRUE to draw the item checked.
	//-----------------------------------------------------------------------
	virtual void Draw(CDC* pDC, CXTPControlGallery* pGallery, CRect rcItem, BOOL bEnabled,
					  BOOL bSelected, BOOL bPressed, BOOL bChecked);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the image of the item.
	// Returns:
	//     A pointer to the image of the item.
	//-----------------------------------------------------------------------
	CXTPImageManagerIcon* GetImage();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if the item is a label.
	// Returns:
	//     TRUE if the item is a label, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsLabel() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the caption of the item.
	// Parameters:
	//     lpszCaption - Caption of the item to be set.
	// See Also:
	//     GetCaption, SetToolTip, GetToolTip
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR lpszCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the tooltip of the item.
	// Parameters:
	//     lpszToolTip - Tooltip of the item to be set.
	// See Also:
	//     GetToolTip, SetCaption, GetCaption
	//-----------------------------------------------------------------------
	void SetToolTip(LPCTSTR lpszToolTip);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the tooltip of the item.
	// Returns:
	//     The tooltip of the item.
	// See Also:
	//     SetToolTip, SetCaption, GetCaption
	//-----------------------------------------------------------------------
	CString GetToolTip() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the caption of the item.
	// Returns:
	//     The caption of the item.
	// See Also:
	//     SetCaption, SetToolTip, GetToolTip
	//-----------------------------------------------------------------------
	CString GetCaption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to enable or disable the item.
	// Parameters:
	//     bEnabled - TRUE to enable the item, FALSE to disable the item.
	// See Also:
	//     GetEnabled
	//-----------------------------------------------------------------------
	void SetEnabled(BOOL bEnabled);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the state of the item.
	// Returns:
	//     TRUE if the item is enabled, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL GetEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the index of the item.
	// Returns:
	//     The index of the item.
	//-----------------------------------------------------------------------
	int GetIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the identifier of the item.
	// Parameters:
	//     nId - Identifier to be set.
	//-----------------------------------------------------------------------
	void SetID(int nId);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the item's identifier.
	// Returns:
	//     The identifier of the item.
	//-----------------------------------------------------------------------
	int GetID() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the 32-bit value associated with the item.
	// Parameters:
	//     dwData - New value to be set.
	//-----------------------------------------------------------------------
	void SetData(DWORD_PTR dwData);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the application-supplied, 32-bit value associated
	//     with the item.
	// Returns:
	//     The 32-bit value associated with the item.
	//-----------------------------------------------------------------------
	DWORD_PTR GetData() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the description of the item.
	// Parameters:
	//     lpszDescription - Description to be set.
	// See Also:
	//     SetCaption, SetTooltip
	//-----------------------------------------------------------------------
	void SetDescription(LPCTSTR lpszDescription);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the item's description.
	// Returns:
	//     The description of the control.
	//-----------------------------------------------------------------------
	CString GetDescription() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Set the preferred image width.
	// Parameters:
	//     nImageWidth - preferred image width.
	//-----------------------------------------------------------------------
	void SetImageWidth(int nImageWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Get the preferred image width.
	// Returns:
	//     Preferred image width.
	//-----------------------------------------------------------------------
	int GetImageWidth() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member functions are used to get stored CXTPMarkupUIElement object.
	// Returns:
	//     A pointer to a CXTPMarkupUIElement object.
	//-----------------------------------------------------------------------
	CXTPMarkupUIElement* GetMarkupUIElement() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the index of the used image.
	// Returns:
	//     The index of the image.
	//-----------------------------------------------------------------------
	virtual int GetImageIndex();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the preferred image index.
	// Parameters:
	//     nImageIndex - New image index.
	//-----------------------------------------------------------------------
	virtual void SetImageIndex(int nImageIndex);

	virtual BOOL OnClick();

protected:
	int m_nId;						   // Identifier
	int m_nIndex;					   // Index of the item
	int m_nImage;					   // Image index
	CString m_strCaption;			   // Caption of the item
	CString m_strToolTip;			   // Tooltip of the item
	CString m_strDescriptionText;	  // Description of the item
	CXTPControlGalleryItems* m_pItems; // Parent items collection
	DWORD_PTR m_dwData;				   // 32-bit data
	CSize m_szItem;					   // Size of the item; CSize(0, 0) - to use default value
	BOOL m_bLabel;					   // TRUE if item is label
	BOOL m_bEnabled;				   // TRUE to enable, FALSE to disable.
	int m_nImageWidth;				   // preferred image width (0) - to use default value

	CXTPMarkupUIElement* m_pMarkupUIElement;

#	ifdef _XTP_COMMANDBARS_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPControlGalleryItem);
	COleVariant m_oleTag;
	BOOL OleGetLabel();
	DECLARE_PROPERTY(ImageIndex, int);
	DECLARE_PROPERTY(ItemWidth, int);
	DECLARE_PROPERTY(ItemHeight, int);

	//}}AFX_CODEJOCK_PRIVATE
#	endif

	friend class CXTPControlGalleryItems;
	friend class CXTPControlGallery;
};

AFX_INLINE CString CXTPControlGalleryItem::GetToolTip() const
{
	return m_strToolTip;
}
AFX_INLINE CString CXTPControlGalleryItem::GetCaption() const
{
	return m_strCaption;
}
AFX_INLINE void CXTPControlGalleryItem::SetDescription(LPCTSTR lpszDescription)
{
	m_strDescriptionText = lpszDescription;
}
AFX_INLINE CString CXTPControlGalleryItem::GetDescription() const
{
	return m_strDescriptionText;
}
AFX_INLINE void CXTPControlGalleryItem::SetID(int nId)
{
	m_nId = nId;
}
AFX_INLINE int CXTPControlGalleryItem::GetID() const
{
	return m_nId;
}

AFX_INLINE BOOL CXTPControlGalleryItem::IsLabel() const
{
	return m_bLabel;
}
AFX_INLINE void CXTPControlGalleryItem::SetToolTip(LPCTSTR lpszToolTip)
{
	m_strToolTip = lpszToolTip;
}

AFX_INLINE void CXTPControlGalleryItem::SetData(DWORD_PTR dwData)
{
	m_dwData = dwData;
}
AFX_INLINE DWORD_PTR CXTPControlGalleryItem::GetData() const
{
	return m_dwData;
}

AFX_INLINE void CXTPControlGalleryItem::SetImageWidth(int nImageWidth)
{
	m_nImageWidth = nImageWidth;
}

AFX_INLINE int CXTPControlGalleryItem::GetImageWidth() const
{
	return m_nImageWidth;
}

AFX_INLINE BOOL CXTPControlGalleryItem::OnClick()
{
	return FALSE;
}
AFX_INLINE CXTPMarkupUIElement* CXTPControlGalleryItem::GetMarkupUIElement() const
{
	return m_pMarkupUIElement;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
//{{AFX_CODEJOCK_PRIVATE
#endif // __XTPCONTROLGALLERYITEM_H__
//}}AFX_CODEJOCK_PRIVATE
