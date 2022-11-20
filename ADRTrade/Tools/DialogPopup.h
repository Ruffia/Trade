#pragma once
#include <memory>
#include <map>
#include <string>
using namespace std;
#include "Tools/pugixml.hpp"
#include "Tools/pugiconfig.hpp"
using namespace pugi;
#include "Business/AbstractObject.h"

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
	map<string,shared_ptr<CWnd>>  m_mapUIName2Wnd;
	IAbstractObject* m_pBusiness;
};
