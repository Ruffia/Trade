#pragma once
#include <memory>
#include <map>
#include <string>
using namespace std;
#include "Tools/pugixml.hpp"
#include "Tools/pugiconfig.hpp"
using namespace pugi;

class CDialogPopup : public CDialog
{
	DECLARE_DYNAMIC(CDialogPopup)

public:
	CDialogPopup(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogPopup();
	void SetLayout(string& sLayout)
	{
		m_sLayout = sLayout;
	}

	void SetBusiness(string& sBusiness)
	{
		m_sBusinessSQL = sBusiness;
	}

// 对话框数据
	enum { IDD = IDD_DLG_Common_Popup };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual void _InitLayOut();
	DECLARE_MESSAGE_MAP()

protected:
	string m_sLayout;    //页面布局配置文件
	xml_document m_doc;
	map<string,CWnd*>  m_mapUIName2Wnd;

	//业务字段--->UI对象
	map<string,CWnd*> m_mapBusiness2Control;

	string m_sBusinessSQL;  //业务内容
};
