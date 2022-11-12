#pragma once
#include <string>
using namespace std;
#include "pugixml.hpp"
#include "pugiconfig.hpp"
using namespace pugi;

// CDialogLayout 对话框
class CDialogLayout : public CDialog
{
	DECLARE_DYNAMIC(CDialogLayout)

public:
	CDialogLayout(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogLayout();
	void SetLayout(string& sLayout)
	{
		m_sLayout = sLayout;
	}

// 对话框数据
	enum { IDD = IDD_DLG_Common };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual void _InitLayOut() = 0;
	virtual void _ReLayout() = 0;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

protected:
	string m_sLayout;    //页面布局配置文件
	xml_document m_doc;
};
