#pragma once
#include <memory>
#include <map>
#include <string>
using namespace std;
#include "Tools/pugixml.hpp"
#include "Tools/pugiconfig.hpp"
using namespace pugi;
#include "DBDataManager.h"
#include "DialogPlaceHolder.h"
#include "DataProvider.h"


//业务字段的控件类型
enum BusinessControlType
{
	Business_Edit = 0,
	Business_ComboBox,
	Business_CheckBox,
};


class CDialogPlaceHolderBusiness : public CDialogPlaceHolder
{
	DECLARE_DYNAMIC(CDialogPlaceHolderBusiness)

public:
	CDialogPlaceHolderBusiness(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogPlaceHolderBusiness();

	void SetDataProvider(IDataProvider* pDataProvider)
	{
		m_pDataProvider = pDataProvider;
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
	virtual BOOL OnInitDialog();

	//界面缩放，需要调整布局
	virtual void _DesignLayout();

	//根据 CUIData 对象的数据创建实际的控件 
	virtual void _CreateUI( CUIData &data, xml_node node );

	//将数据库中的数据记录加载到UI上显示
	virtual void _LoadData2UI();

	//判断当日记录是否存在
	bool _CheckExistsTradeDayRecord();

protected:
	//业务字段--->UI对象
	map<string,CWnd*> m_mapBusiness2Control;
	IDataProvider*    m_pDataProvider;
};