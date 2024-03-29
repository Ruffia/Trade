// XTPControlSlider.h : interface for the CXTPControlSlider class.
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
#if !defined(__XTPCONTOLSLIDER_H__)
#	define __XTPCONTOLSLIDER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPControlSlider;

//===========================================================================
// Summary:
//     Inplace slider control of CXTPControlSlider.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlSliderCtrl : public CSliderCtrl
{
protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPControlSliderCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void HScroll(UINT nSBCode, UINT nPos);
	afx_msg void VScroll(UINT nSBCode, UINT nPos);
	//}}AFX_MSG
	BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	LRESULT OnWindowFromPoint(WPARAM, LPARAM);
	//}}AFX_CODEJOCK_PRIVATE

protected:
	CXTPControlSlider* m_pControl; // Parent CXTPControlSlider control

private:
	friend class CXTPControlSlider;
};

//===========================================================================
// Summary:
//     CXTPControlSlider is a CXTPControl derived class.
//     It represents a slider control.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlSlider : public CXTPControl
{
	DECLARE_XTP_CONTROL(CXTPControlSlider)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlSlider object.
	//-----------------------------------------------------------------------
	CXTPControlSlider();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPControlSlider object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPControlSlider();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the current position of the slider.
	// Parameters:
	//     nPos - New position of the slider control.
	// See Also:
	//     GetPos, SetRange
	//-----------------------------------------------------------------------
	void SetPos(int nPos);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the current position of the slider.
	// Returns:
	//     The position of the slider control.
	// See Also:
	//     SetPos
	//-----------------------------------------------------------------------
	int GetPos();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the upper and lower limits of the slider control's range.
	// Parameters:
	//     nMin - Specifies the lower limit of the range.
	//     nMax - Specifies the upper limit of the range.
	// See Also:
	//     SetPos
	//-----------------------------------------------------------------------
	void SetRange(int nMin, int nMax);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a control's enabled state is changed.
	//-----------------------------------------------------------------------
	virtual void OnEnabledChanged();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get a pointer to the in-place slider control.
	// Retunrs:
	//     A pointer to the in-place slider control.
	//-----------------------------------------------------------------------
	CSliderCtrl* GetSliderCtrl() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the style of the slider control.
	// Returns:
	//     The style of the slider control.
	// See Also:
	//     SetSliderStyle
	//-----------------------------------------------------------------------
	DWORD GetSliderStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the style of the slider control.
	// Parameters:
	//     dwStyle - The style to be set.
	// See Also:
	//     GetSliderStyle
	//-----------------------------------------------------------------------
	void SetSliderStyle(DWORD dwStyle);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the position of the scrollbar is changed.
	// Parameters:
	//     nSBCode - Specifies a scrollbar code that indicates the user's scrolling request.
	//     nPos - Contains the current scroll-box position.
	//-----------------------------------------------------------------------
	virtual void OnScroll(UINT nSBCode, UINT nPos);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	BOOL HasFocus() const;
	CSize GetSize(CDC* pDC);
	void Draw(CDC* pDC);
	void SetRect(CRect rcControl);
	void SetParent(CXTPCommandBar* pParent);
	void SetHideFlags(DWORD dwFlags);
	void OnCalcDynamicSize(DWORD dwMode);
	void OnClick(BOOL bKeyboard = FALSE, CPoint pt = CPoint(0, 0));
	virtual void OnThemeChanged();
	virtual int GetCustomizeMinWidth() const;
	virtual BOOL IsCustomizeResizeAllow() const;
	void Copy(CXTPControl* pControl, BOOL bRecursive = FALSE);
	void DoPropExchange(CXTPPropExchange* pPX);

	//}}AFX_CODEJOCK_PRIVATE

protected:
	CXTPControlSliderCtrl* m_pSliderCtrl; // Pointer to slider control.
	int m_nMin;							  // Lower range.
	int m_nMax;							  // Upper range.
	int m_nPos;							  // Current position of slider.
	DWORD m_dwSliderStyle;				  // Slider style.
	CRect m_rcPadding;					  // Padding around slider.

	friend class CXTPControlSliderCtrl;

#	ifdef _XTP_COMMANDBARS_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPControlSlider);
	void OleRangeChanged();
//}}AFX_CODEJOCK_PRIVATE
#	endif
};

AFX_INLINE BOOL CXTPControlSlider::IsCustomizeResizeAllow() const
{
	return TRUE;
}
AFX_INLINE int CXTPControlSlider::GetCustomizeMinWidth() const
{
	return 20;
}
AFX_INLINE CSliderCtrl* CXTPControlSlider::GetSliderCtrl() const
{
	return m_pSliderCtrl;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPCONTOLSLIDER_H__)
