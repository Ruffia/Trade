#pragma once
#include <memory>
#include <vector>
#include <string>
using namespace std;
#include "Tools/pugixml.hpp"
#include "Tools/pugiconfig.hpp"
using namespace pugi;
#include "CustomTabCtrl.h"


class CCustomTabCtrlDlg : public CDialogPlaceHolder
{
public:
	CCustomTabCtrlDlg(CWnd* pParent = NULL);
	~CCustomTabCtrlDlg();

	enum { IDD = IDD_DLG_Common };

	CCustomTabCtrl*	m_pTab;
	int m_nCurSel;
	vector<CDialogPlaceHolder*> m_vPage; 

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonClickedTab(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg void OnRButtonClickedTab(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHighlightchangeTab(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg void OnMoveitemTab(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg void OnCopyitemTab(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg void OnLabelupdateTab(NMHDR* pNMHDR, LRESULT* pResult) ;
	DECLARE_MESSAGE_MAP()

protected:
	virtual void _InitPage(CRect& rcTab); 
	virtual void _LoadData2UI();
	void _Resize(int cx, int cy);
	void _ShowPage(const int nCurPage);
	LPCTSTR GetTooltipText(int nStyle);
};
