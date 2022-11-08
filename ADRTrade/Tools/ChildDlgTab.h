#pragma once
#include "afxcmn.h"
#include <vector>
using namespace std;

#if _MSC_VER > 1000
#pragma once
#endif 


//页签界面基类--子对话框
class CChildDlgTab : public CDialog
{
	DECLARE_DYNAMIC(CChildDlgTab)

public:
	CChildDlgTab(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChildDlgTab();

	virtual void InitPage() = 0; 
	void ShowPage(const int nCurPage);

// 对话框数据
	enum { IDD = IDD_ChildDLG_Tab };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual void OnTcnSelchangeTabLog(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void _DesignLayOut();

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_Tab;
	int m_CurSelTab;
	vector<CDialog*> m_vPage; 
};
