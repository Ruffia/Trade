#pragma once
#include <memory>
#include <map>
#include <string>
using namespace std;
#include "Tools/pugixml.hpp"
#include "Tools/pugiconfig.hpp"
using namespace pugi;
#include "Business/BusinessNode.h"
#include "DialogIDManager.h"

// CDialogPlaceHolder 对话框

#define DefineClass(ClassName,BaseClass,ID) \
 class ClassName: public BaseClass  \
 {                                  \
   public:                          \
	 ClassName();                   \
	 enum { IDD = ID };             \
};                                  \


#define ImplementClass(ClassName,BaseClass)                       \
IMPLEMENT_FACTORY(CDialog,ClassName,string,#ClassName)            \
ClassName::ClassName()                                            \
{                                                                 \
	CDialogIDMgr::Instance().Register(#ClassName,ClassName::IDD); \
}                                                                 \

class CDialogPlaceHolder : public CDialog
{
	DECLARE_DYNAMIC(CDialogPlaceHolder)

public:
	CDialogPlaceHolder(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogPlaceHolder();
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
	map<string,CWnd*>  m_mapUIName2Wnd;
};
