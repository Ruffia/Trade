// ScrollRichEditViewEx.h
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

#if !defined(AFX_SCROLLRICHEDITVIEWEX_H__119F8543_B51C_4AE6_8593_75DF43572288__INCLUDED_)
#	define AFX_SCROLLRICHEDITVIEWEX_H__119F8543_B51C_4AE6_8593_75DF43572288__INCLUDED_

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CScrollRichEditViewEx window

class CScrollRichEditViewEx : public CRichEditView
{
	// Construction
public:
	CScrollRichEditViewEx();
	DECLARE_DYNCREATE(CScrollRichEditViewEx)

	// Attributes
public:
	int GetPageSize();

	// Operations
public:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScrollRichEditViewEx)
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CScrollRichEditViewEx();

public:
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	// Generated message map functions
protected:
	//{{AFX_MSG(CScrollRichEditViewEx)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcPaint();
	afx_msg void OnDestroy();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	afx_msg void OnEmptyCommand(UINT);
	afx_msg void OnEmptyCommand();
	afx_msg void OnUpdateEmptyCommand(CCmdUI* pCmdUI);

	// HOME / Font
	afx_msg void OnCharStrikeout();
	afx_msg void OnCharSubscript();
	afx_msg void OnCharSuperscript();
	afx_msg void OnFontGrow();
	afx_msg void OnFontShrink();
	afx_msg void OnEditFontFace(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditFontSize(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnUpdateCharSubscript(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCharSuperscript(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCharStrikeout(CCmdUI* pCmdUI);
	afx_msg void OnUpdateComboFontFace(CCmdUI* pCmd);
	afx_msg void OnUpdateGalleryFontFace(CCmdUI* pCmdUI);
	afx_msg void OnUpdateComboFontSize(CCmdUI* pCmd);
	afx_msg void OnUpdateGalleryFontSize(CCmdUI* pCmdUI);

	// Highlight color
	COLORREF m_clrBack;
	afx_msg void OnHighlightColor();
	afx_msg void OnUpdateHighlightColor(CCmdUI* pCmdUI);
	afx_msg void OnGalleryHighlightColor(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateGalleryHighlightColor(CCmdUI* pCmdUI);
	afx_msg void OnGalleryNoHighlightColors();
	afx_msg void OnUpdateGalleryNoHighlightColors(CCmdUI* pCmdUI);

	// Text color
	COLORREF m_clr;
	afx_msg void OnTextColor();
	afx_msg void OnUpdateTextColor(CCmdUI* pCmd);
	afx_msg void OnGalleryTextColor(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateGalleryTextColor(CCmdUI* pCmd);
	afx_msg void OnGalleryTextAutoColor();
	afx_msg void OnUpdateGalleryTextAutoColor(CCmdUI* pCmd);

	// HOME / Paragraph
	afx_msg void OnParaJustify();
	afx_msg void OnUpdateParaJustify(CCmdUI* pCmdUI);

	// PAGE LAYOUT / Paragraph
	afx_msg void OnIndentLeft(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnIndentLeftSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateIndentLeft(CCmdUI* pCmdUI);
	void SetIndentLeft(long nIndentLeft);
	long GetIndentLeft();

	afx_msg void OnIndentRight(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnIndentRightSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateIndentRight(CCmdUI* pCmdUI);
	void SetIndentRight(long nIndentRight);
	long GetIndentRight();

	afx_msg void OnSpaceBefore(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSpaceBeforeSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateSpaceBefore(CCmdUI* pCmdUI);
	void SetSpaceBefore(long nSpaceBefore);
	long GetSpaceBefore();

	afx_msg void OnSpaceAfter(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSpaceAfterSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateSpaceAfter(CCmdUI* pCmdUI);
	void SetSpaceAfter(long nSpaceAfter);
	long GetSpaceAfter();
	//}}AFX_MSG
	void OnRequestResize(NMHDR* pNotifyStruct, LRESULT* result);
	void OnEditScrollChanged();
	void OnInitialUpdate();

	virtual CRect GetParentViewRect();
	virtual CScrollBar* GetParentScrollbar(int nBar) const;
	virtual COLORREF GetFrameColor();

	void UpdateScrollInfo();

	virtual CScrollBar* GetScrollBarCtrl(int nBar) const;

	void SetScrollPos(int nScrollPos);
	void UpdatePosition();

	int m_nTextHeight;
	int m_nPageHeight;
	int m_nScrollPos;
	HINSTANCE m_hInstance;

	int m_nTotalHeight;
	BOOL m_bInScrollChanged;

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCROLLRICHEDITVIEWEX_H__119F8543_B51C_4AE6_8593_75DF43572288__INCLUDED_)
