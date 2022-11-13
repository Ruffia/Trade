#pragma once
#include <memory>
#include <map>
#include <string>
using namespace std;
#include "UIData.h"
#include "Tools/pugixml.hpp"
#include "Tools/pugiconfig.hpp"
using namespace pugi;

// CDialogPlaceHolderComposite 对话框
class CDialogPlaceHolderComposite : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogPlaceHolderComposite)

public:
	CDialogPlaceHolderComposite(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogPlaceHolderComposite();
	void SetLayout(string& sLayout)
	{
		m_sLayout = sLayout;
	}

	// 对话框数据
	enum { IDD = IDD_DLG_Common };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual void _InitLayOut();
	DECLARE_MESSAGE_MAP()

protected:
	string m_sLayout;    //页面布局配置文件
	xml_document m_doc;
	map<string,CUIData>  m_mapUIName2Data;
};
