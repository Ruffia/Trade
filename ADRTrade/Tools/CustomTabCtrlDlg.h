#pragma once
#include <memory>
#include <vector>
#include <string>
using namespace std;
#include "CustomTabCtrl.h"
#include "DialogPlaceHolder.h"

class CCustomTabCtrlDlg : public CDialogPlaceHolder
{
public:
	CCustomTabCtrlDlg(CWnd* pParent = NULL);
	~CCustomTabCtrlDlg();

	enum { IDD = IDD_DLG_Common };

	CCustomTabCtrl*	m_pTab;
	int m_nCurSel;
	int m_nCount;
	vector<CDialog*> m_vPage; 

protected:
	virtual BOOL OnInitDialog();
	void _ShowPage(const int nCurPage);
	LPCTSTR GetTooltipText(int nStyle);
	afx_msg void OnLButtonClickedTab(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg void OnRButtonClickedTab(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHighlightchangeTab(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg void OnMoveitemTab(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg void OnCopyitemTab(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg void OnLabelupdateTab(NMHDR* pNMHDR, LRESULT* pResult) ;
	DECLARE_MESSAGE_MAP()

protected:
	virtual void _InitLayOut();
	virtual void _DesignLayout(UINT nType, int cx, int cy);

	//根据 CUIData 对象的数据创建实际的控件 
	virtual void _CreateUI( CUIData &data, xml_node node );
};
