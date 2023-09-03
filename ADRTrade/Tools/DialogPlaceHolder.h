#pragma once
#include <memory>
#include <map>
#include <string>
using namespace std;
#include "Tools/pugixml.hpp"
#include "Tools/pugiconfig.hpp"
using namespace pugi;
#include "DialogIDManager.h"
#include "UIData.h"

// CDialogPlaceHolder 对话框

#define DefineClass(ClassName,BaseClass,ID) \
 class ClassName: public BaseClass  \
 {                                  \
   public:                          \
	 ClassName();                   \
	 enum { IDD = ID };             \
};                                  \


#define ImplementClass(ClassName,BaseClass)                       \
IMPLEMENT_FACTORY(BaseClass,ClassName,string,#ClassName)            \
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
	void SetLayout(string& sLayout);

// 对话框数据
	enum { IDD = IDD_DLG_Common };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//从XML中加载UI布局
	virtual void _InitLayOut();

	//界面缩放，需要调整布局
	virtual void _DesignLayout(UINT nType, int cx, int cy);

	//将XML 中的UI配置读取到 CUIData 对象中
	void _ReadUIData( xml_node &node, CUIData &data );

	//根据 CUIData 对象的数据创建实际的控件 
	virtual void _CreateUI( CUIData &data, xml_node node );

	DECLARE_MESSAGE_MAP()

protected:
	string m_sLayout;            //页面布局配置文件
	string m_strDataProvider;    //数据提供者的名称
	xml_document m_doc;
	map<string,CWnd*>  m_mapUIName2Wnd;
};