#pragma once
#include "afxcmn.h"
#include <memory>
#include <vector>
#include <string>
using namespace std;
#include "CustomTabCtrl.h"
#include "Tools/pugixml.hpp"
#include "Tools/pugiconfig.hpp"
using namespace pugi;

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

	void SetLayout(string& sLayout)
	{
		m_sLayout = sLayout;
	}

	
	void ShowPage(const int nCurPage);

// 对话框数据
	enum { IDD = IDD_ChildDLG_Tab };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual void OnTcnSelchangeTabLog(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void _InitLayOut(); 
	virtual void _ReLayout();

	DECLARE_MESSAGE_MAP()

public:
	string m_sLayout;    //页面布局配置文件
	xml_document m_doc;
	CCustomTabCtrl* m_pTab;
	int m_CurSelTab;
	vector<CDialog*> m_vPage; 
};
