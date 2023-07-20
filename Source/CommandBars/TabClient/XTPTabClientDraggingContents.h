// XTPTabClientDraggingContents.h : interface for the CXTPTabClientDraggingContents class.
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
#if !defined(__XTPTABCLIENTDRAGGINGCONTENTS_H__)
#	define __XTPTABCLIENTDRAGGINGCONTENTS_H__
//{{AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//=======================================================================
// Summary:
//      Managers tab client dragging contents.
//=======================================================================
class CXTPTabClientDraggingContents : public CWnd
{
	DECLARE_DYNCREATE(CXTPTabClientDraggingContents);

	CXTPTabClientDraggingContents();
	CXTPTabClientDraggingContents(CXTPTabClientWnd* pTabClientWnd);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//      Performs resource cleanup on object destruction.
	//-----------------------------------------------------------------------
	virtual ~CXTPTabClientDraggingContents();

	//-----------------------------------------------------------------------
	// Summary:
	//      Create the contents object.
	// Returns:
	//      TRUE if the contents object is successfully created, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL Create();

	using CWnd::Create;

	//-----------------------------------------------------------------------
	// Summary:
	//      Obtains contents height.
	// Returns:
	//      Contents height value.
	// See also:
	//      GetWidth
	//-----------------------------------------------------------------------
	int GetHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Obtains contents width.
	// Returns:
	//      Contents width value.
	// See also:
	//      GetHeight
	//-----------------------------------------------------------------------
	int GetWidth() const;

private:
	int m_Height;
	int m_Width;
	int m_imgHeight;
	int m_imgWidth;
	int m_borderX;
	int m_borderY;

	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPDockingPaneContextStickerWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

	CXTPTabClientWnd* m_pTarget;
	CBitmap m_bitmap;
};

AFX_INLINE int CXTPTabClientDraggingContents::GetHeight() const
{
	return m_Height;
}

AFX_INLINE int CXTPTabClientDraggingContents::GetWidth() const
{
	return m_Width;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTABCLIENTDRAGGINGCONTENTS_H__)
