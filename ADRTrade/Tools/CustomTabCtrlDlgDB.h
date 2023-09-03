#pragma once
#include "CustomTabCtrlDlg.h"
#include "DataProvider.h"

class CCustomTabCtrlDlgBusiness : public CCustomTabCtrlDlg
{
public:
	CCustomTabCtrlDlgBusiness(CWnd* pParent = NULL);
	~CCustomTabCtrlDlgBusiness();

protected:
	virtual BOOL OnInitDialog();
	virtual void _InitLayOut();
	virtual void _DesignLayout(UINT nType, int cx, int cy);

	//根据 CUIData 对象的数据创建实际的控件 
	virtual void _CreateUI( CUIData &data, xml_node node );

	//将数据库中的数据记录加载到UI上显示
	virtual void _LoadData2UI();

	//判断当日记录是否存在
	bool _CheckExistsTradeDayRecord();

protected:
	IDataProvider*  m_pDataProvider;
};
