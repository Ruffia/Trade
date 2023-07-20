// XTPPropertyPageNavigator.h : interface for the CXTPPropertyPageNavigator class.
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
#if !defined(__XTPPROPERTYPAGENAVIGATOR_H__)
#	define __XTPPROPERTYPAGENAVIGATOR_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPPropertySheet;
class CXTPPropertyPage;

//===========================================================================
// Summary:
//     CXTPPropertyPageNavigator is a standalone class. CXTPPropertyPageNavigator
//     represents a base navigator object for CXTPPropertySheet.
// See Also:
//     CXTPPropertySheet, CXTPPropertySheet::DoModal, CXTPPropertySheet::SetNavigator
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyPageNavigator
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyPageNavigator object.
	//-----------------------------------------------------------------------
	CXTPPropertyPageNavigator();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyPageNavigator object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPPropertyPageNavigator();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to create a page navigator.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL Create() = 0;

	virtual void OnPageChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to reposition a property sheet using the size
	//     of a property page.
	// Parameters:
	//     pLayout - Address of an AFX_SIZEPARENTPARAMS structure.
	//     szPage  - Page size to calculate from.
	//     rcPage  - Result bounding rectangle of property pages.
	//     szClient - Result size of property sheet.
	//-----------------------------------------------------------------------
	virtual void PageToSize(AFX_SIZEPARENTPARAMS* pLayout, CSize szPage, CRect& rcPage,
							CSize& szClient) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to reposition a property page using the size
	//     of a property sheet.
	// Parameters:
	//     pLayout - Address of an AFX_SIZEPARENTPARAMS structure.
	//     szClient - Size of property sheet.
	//     rcPage  - Result bounding rectangle of property pages.
	//-----------------------------------------------------------------------
	virtual void SizeToPage(AFX_SIZEPARENTPARAMS* pLayout, CSize szClient, CRect& rcPage) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a property page becomes selected.
	// Parameters:
	//     pPage - The property page that became selected.
	//-----------------------------------------------------------------------
	virtual void OnPageSelected(CXTPPropertyPage* pPage) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get the window handler of a navigator window.
	// Returns:
	//     The window handler of a navigator window.
	//-----------------------------------------------------------------------
	virtual HWND GetSafeHwnd() const = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves page brush.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     pPage - Property page to draw.
	// Returns:
	//     The page brush.
	//-----------------------------------------------------------------------
	virtual HBRUSH GetPageBrush(CDC* pDC, CXTPPropertyPage* pPage) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves page color.
	// Returns:
	//     The page color.
	//-----------------------------------------------------------------------
	virtual COLORREF GetPageColor() = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to switch the visual theme of the control.
	// Parameters:
	//     nTheme - New visual theme. Can be any of the values listed
	//              in the Remarks section.
	// Remarks:
	//     nTheme can be one of the theme IDs specified by XTPControlTheme.
	// See Also:
	//     XTPControlTheme
	//-----------------------------------------------------------------------
	virtual void SetTheme(XTPControlTheme theme);

	XTPControlTheme GetTheme();

protected:
	virtual void SetPropertySheet(CXTPPropertySheet* pSheet);

public:
	CRect m_rcNavigatorMargin; // Total navigator margin.
	CRect m_rcButtonMargin;	// Total buttons margin.
	CRect m_rcPageMargin;	  // Total page margin.
protected:
	CXTPPropertySheet* m_pSheet; // Parent property sheet object.

	XTPControlTheme m_themeCurrent;

	friend class CXTPPropertySheet;
};

#	if defined(_XTP_INCLUDE_TABMANAGER)

//===========================================================================
// Summary:
//     CXTPPropertyPageTabNavigator is a CXTPTabControl and CXTPPropertyPageNavigator
//     derived class. It represents a tabbed property page navigator.
// See Also:
//     CXTPPropertySheet, CXTPPropertySheet::DoModal, CXTPPropertySheet::SetNavigator
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyPageTabNavigator
	: public CXTPTabControl
	, public CXTPPropertyPageNavigator
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyPageTabNavigator object.
	//-----------------------------------------------------------------------
	CXTPPropertyPageTabNavigator();

	//{{AFX_CODEJOCK_PRIVATE
public:
	virtual BOOL Create();
	virtual void OnPageChanged();

	using CXTPTabControl::Create;

	void PageToSize(AFX_SIZEPARENTPARAMS* pLayout, CSize szPage, CRect& rcPage, CSize& szClient);
	void SizeToPage(AFX_SIZEPARENTPARAMS* pLayout, CSize szClient, CRect& rcPage);

	virtual void OnPageSelected(CXTPPropertyPage* pPage);
	virtual HWND GetSafeHwnd() const
	{
		return m_hWnd;
	}
	virtual HBRUSH GetPageBrush(CDC* pDC, CXTPPropertyPage* pPage);
	virtual COLORREF GetPageColor()
	{
		return m_clrBrush;
	}

	virtual void SetTheme(XTPControlTheme theme);

protected:
	void SetSelectedItem(CXTPTabManagerItem* pItem);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSysColorChange();

private:
	CXTPBrush m_brushPage;
	COLORREF m_clrBrush;
	//}}AFX_CODEJOCK_PRIVATE
};

#	endif // #if defined(_XTP_INCLUDE_TABMANAGER)

//===========================================================================
// Summary:
//     CXTPPropertyPageControlNavigator is a CXTPPropertyPageNavigator derived class.
//     It represents a base control property page navigator.
// See Also:
//     CXTPPropertySheet, CXTPPropertySheet::DoModal, CXTPPropertySheet::SetNavigator
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyPageControlNavigator : public CXTPPropertyPageNavigator
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyPageControlNavigator object.
	//-----------------------------------------------------------------------
	CXTPPropertyPageControlNavigator();

	//{{AFX_CODEJOCK_PRIVATE
public:
	void PageToSize(AFX_SIZEPARENTPARAMS* pLayout, CSize szPage, CRect& rcPage, CSize& szClient);
	void SizeToPage(AFX_SIZEPARENTPARAMS* pLayout, CSize szClient, CRect& rcPage);
	virtual HBRUSH GetPageBrush(CDC* pDC, CXTPPropertyPage* pPage);
	virtual COLORREF GetPageColor()
	{
		return m_clrBrush;
	}

public:
	CSize m_szControl;

private:
	CXTPBrush m_brushPage;
	COLORREF m_clrBrush;
	//}}AFX_CODEJOCK_PRIVATE
};

//===========================================================================
// Summary:
//     CXTPPropertyPageListNavigator is a CXTPListBox and CXTPPropertyPageControlNavigator
//     derived class. It represents a list property page navigator.
// See Also:
//     CXTPPropertySheet, CXTPPropertySheet::DoModal, CXTPPropertySheet::SetNavigator
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyPageListNavigator
	: public CXTPListBox
	, public CXTPPropertyPageControlNavigator
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyPageListNavigator object.
	//-----------------------------------------------------------------------
	CXTPPropertyPageListNavigator();

	//{{AFX_CODEJOCK_PRIVATE
public:
	virtual BOOL Create();
	virtual void OnPageChanged();

#	if _MSC_VER > 1200
	using CListBox::Create;
	using CWnd::Create;
#	endif

	virtual void OnPageSelected(CXTPPropertyPage* pPage);
	virtual HWND GetSafeHwnd() const
	{
		return m_hWnd;
	}
	virtual void SetPropertySheet(CXTPPropertySheet* pSheet);

	virtual void SetTheme(XTPControlTheme theme);
	HBRUSH GetPageBrush(CDC* /*pDC*/, CXTPPropertyPage* /*pPage*/);
	virtual COLORREF GetPageColor()
	{
		return m_clrBrush;
	}

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnSelChanged();

public:
	int m_nItemMargin;

private:
	CXTPBrush m_brushPage;
	COLORREF m_clrBrush;
	//}}AFX_CODEJOCK_PRIVATE
};

//===========================================================================
// Summary:
//     CXTPPropertyPageTreeNavigator is a CTreeCtrl and CXTPPropertyPageControlNavigator
//     derived class. It represents a tree property page navigator.
// See Also:
//     CXTPPropertySheet, CXTPPropertySheet::DoModal, CXTPPropertySheet::SetNavigator
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyPageTreeNavigator
	: public CTreeCtrl
	, public CXTPPropertyPageControlNavigator
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyPageTreeNavigator object.
	// Parameters:
	//     dwStyle - Style of the tree control to be used.
	//-----------------------------------------------------------------------
	CXTPPropertyPageTreeNavigator(DWORD dwStyle = TVS_SHOWSELALWAYS);

	//{{AFX_CODEJOCK_PRIVATE
public:
	virtual BOOL Create();
	virtual void OnPageChanged();

#	if _MSC_VER > 1200
	using CWnd::Create;
#	endif

	virtual void OnPageSelected(CXTPPropertyPage* pPage);
	virtual HWND GetSafeHwnd() const
	{
		return m_hWnd;
	}
	virtual void SetPropertySheet(CXTPPropertySheet* pSheet);

protected:
	virtual BOOL CreateTree();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnSelChanging(NMHDR* pNMHeader, LRESULT* pResult);

protected:
	DWORD m_dwStyle;
	//}}AFX_CODEJOCK_PRIVATE
};

AFX_INLINE void CXTPPropertyPageNavigator::OnPageChanged()
{
	ASSERT(FALSE); // Not implemented ?
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPPROPERTYPAGENAVIGATOR_H__)
