#pragma once
#include <memory>
#include <vector>
#include <string>
using namespace std;
#include "Tools/pugixml.hpp"
#include "Tools/pugiconfig.hpp"
using namespace pugi;
#include "Tools/CustomTabCtrl.h"


class CWeatherGlass_TabCtrlDlg : public CDialog
{
public:
	CWeatherGlass_TabCtrlDlg(CWnd* pParent = NULL);
	~CWeatherGlass_TabCtrlDlg();

	enum { IDD = IDD_DLG_Common_Popup };

	CCustomTabCtrl*	m_pTab;
	int m_nCurSel;
	vector<CDialog*> m_vPage; 

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
	void _Resize(int cx, int cy);
	void _ShowPage(const int nCurPage);
	LPCTSTR GetTooltipText(int nStyle);
};
