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

	void AddPrimaryKey(string& sKeyName,FieldValue& KeyValue)
	{
		m_mapPrimaryKey2Value[sKeyName] = KeyValue;
	}

	//将UI上的数据保存到数据库中
	virtual void UpdateUI2DB();

	//根据从数据库查询得到的数据记录值更新UI控件的显示
	virtual void UpdateDB2UI( CRecord* pRecord );

// 对话框数据
	enum { IDD = IDD_DLG_Common };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	//从XML中加载UI布局
	virtual void _InitLayOut();

	//将数据库中的数据记录加载到UI上显示
	virtual void _LoadData2UI();

	//判断当日记录是否存在
	bool _CheckExistsTradeDayRecord();

	DECLARE_MESSAGE_MAP()

protected:
	string m_sLayout;    //页面布局配置文件
	xml_document m_doc;
	map<string,CWnd*>  m_mapUIName2Wnd;

	//业务字段--->UI对象
	map<string,CWnd*> m_mapBusiness2Control;

	//主键--->值
	map<string,FieldValue>  m_mapPrimaryKey2Value;

	string m_sBusiness;  //业务名
};