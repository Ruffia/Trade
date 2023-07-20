// XTPPasswordPeek.h interface for the CXTPPasswordPeek class.
//
// (c)1998-2021 Codejock Software, All Rights Reserved.
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
#if !defined(__XTPPASSWORDPEEK_H__)
#	define __XTPPASSWORDPEEK_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPPasswordPeek is a CXTPEdit derived class.
//===========================================================================
class _XTP_EXT_CLASS CXTPPasswordPeek : public CXTPEdit
{
	DECLARE_DYNAMIC(CXTPPasswordPeek)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPasswordPeek object.
	//-----------------------------------------------------------------------
	CXTPPasswordPeek();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPasswordPeek object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPPasswordPeek();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the image manager.
	// Returns:
	//     A pointer to a CXTPImageManager object.
	//-----------------------------------------------------------------------
	CXTPImageManager* GetImageManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the specified image manager.
	// Parameters:
	//     pImageManager - Pointer to a CXTPImageManager object to be set.
	//-----------------------------------------------------------------------
	void SetImageManager(CXTPImageManager* pImageManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to enable or disable peek button.
	// Parameters:
	//     bEnabled - TRUE to enable the item, FALSE to disable the item.
	// See Also:
	//     IsPeekButtonEnabled
	//-----------------------------------------------------------------------
	void PeekButtonEnable(BOOL bEnable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the state of the peek button.
	// Returns:
	//     TRUE if the item is enabled, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsPeekButtonEnabled();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to icons initialization.
	//-----------------------------------------------------------------------
	virtual void InitIcons();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is used to draw the peek button.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     rect  - Rect of peek button.
	//     bIsButtonPressed - TRUE if peek button pressed.
	//     bIsButtonHot - TRUE if peek button hot.
	//-----------------------------------------------------------------------
	virtual void OnDrawButton(CDC* pDC, CRect rect, BOOL bIsButtonPressed, BOOL bIsButtonHot);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is used to configure peek button layout.
	//-----------------------------------------------------------------------
	virtual void OnChangeLayout();

protected:
	enum PasswordPeekMode
	{
		PasswordPeekModeEnabled,
		PasswordPeekModeDisabled,
	};

	int m_nButtonWidth;
	BOOL m_bIsButtonPressed;
	BOOL m_bIsButtonHighlighted;
	BOOL m_bIsButtonCaptured;
	CRect m_rectBtn;
	CSize m_sizeImage;
	PasswordPeekMode m_Mode;
	CXTPImageManager* m_pImageManager;

	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCancelMode();

	DECLARE_MESSAGE_MAP()
};

AFX_INLINE CXTPImageManager* CXTPPasswordPeek::GetImageManager()
{
	return m_pImageManager;
}

AFX_INLINE void CXTPPasswordPeek::SetImageManager(CXTPImageManager* pImageManager)
{
	m_pImageManager = pImageManager;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPPASSWORDPEEK_H__)
