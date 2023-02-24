#pragma once
#include <memory>
#include <map>
#include <string>
using namespace std;
#include "Tools/pugixml.hpp"
#include "Tools/pugiconfig.hpp"
using namespace pugi;
#include "DialogIDManager.h"
#include "DBDataManager.h"
class CBusinessEdit;
class CRecord;

// CDialogPlaceHolder 对话框

#define DefineClass(ClassName,BaseClass,ID) \
 class ClassName: public BaseClass  \
 {                                  \
   public:                          \
	 ClassName();                   \
	 enum { IDD = ID };             \
};                                  \


#define ImplementClass(ClassName,BaseClass)                       \
IMPLEMENT_FACTORY(CDialogPlaceHolder,ClassName,string,#ClassName)            \
ClassName::ClassName()                                            \
{                                                                 \
	CDialogIDMgr::Instance().Register(#ClassName,ClassName::IDD); \
}                                                                 \


//业务字段的控件类型
enum BusinessControlType
{
	Business_Edit = 0,
	Business_ComboBox,
	Business_CheckBox,
};


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

	void SetBusiness(string& sBusiness)
	{
		m_sBusiness = sBusiness;
	}

	//将UI上的数据保存到数据库中
	virtual void UpdateUI2DB();

	//UpdateUI2DB时，获取字段部分的更新SQL语句
	virtual string _CreateUpdateSQL();

	//根据从数据库查询得到的数据记录值更新UI控件的显示
	virtual void UpdateDB2UI( CDataSet& ds,int index = 0);

// 对话框数据
	enum { IDD = IDD_DLG_Common };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	//从XML中加载UI布局
	virtual void _InitLayOut();

	//将数据库中的数据记录加载到UI上显示
	virtual void _LoadTradeDayData2UI();

	//判断当日记录是否存在
	bool _CheckExistsTradeDayRecord();

	DECLARE_MESSAGE_MAP()

protected:
	string m_sLayout;    //页面布局配置文件
	xml_document m_doc;
	map<string,CWnd*>  m_mapUIName2Wnd;

	//业务字段--->UI对象
	map<string,CWnd*> m_mapBusiness2Control;

	string m_sBusiness;  //业务名
};