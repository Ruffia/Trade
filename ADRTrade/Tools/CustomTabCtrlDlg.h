#pragma once
#include <vector>
using namespace std;
#include "CustomTabCtrl.h"

class CCustomTabCtrlDlg : public CDialog
{
public:
	CCustomTabCtrlDlg(CWnd* pParent = NULL);
	~CCustomTabCtrlDlg();

	enum { IDD = IDD_DLG_Common };
	CCustomTabCtrl*	m_pTab;
	int m_nCurSel;
	vector<CDialog*> m_vPage; 

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	// Notify messages from CCustomTabCtrl
	afx_msg void OnLButtonClickedTab(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg void OnRButtonClickedTab(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHighlightchangeTab(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg void OnMoveitemTab(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg void OnCopyitemTab(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg void OnLabelupdateTab(NMHDR* pNMHDR, LRESULT* pResult) ;
	DECLARE_MESSAGE_MAP()

protected:
	virtual void _InitTabCtrl();
	virtual void _CreateTabCtrl(const CRect& rc) = 0;
	virtual void _AddPage() = 0;
	virtual void _Resize(int cx, int cy) = 0;
	void _ShowPage(const int nCurPage);
	LPCTSTR GetTooltipText(int nStyle);
};
