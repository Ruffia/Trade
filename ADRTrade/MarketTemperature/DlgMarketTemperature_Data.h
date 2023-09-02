#pragma once
#include "afxcmn.h"
#include "DataSet.h"
#include "afxdtctl.h"
#include "Tools/DlgGridPage.h"
#include "Tools/DialogPlaceHolder.h"

class CField;
class CFieldDesc;
class CCustomButton;

#if _MSC_VER > 1000
#pragma once
#endif 

class CGridPage_MarketTemperatureRecord : public CDlgGridPage
{
	DECLARE_DYNAMIC(CGridPage_MarketTemperatureRecord)

public:
	CGridPage_MarketTemperatureRecord(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGridPage_MarketTemperatureRecord();


protected:
	virtual BOOL OnInitDialog();

	//从XML中加载UI布局
	virtual void _InitLayOut();

	//将数据库中的数据记录加载到UI上显示
	virtual void _LoadData2UI();

	//移动Grid
	void _MoveGrid();

	//设计控件布局
	void _DesignLayout();

	//加载数据
	void _LoadData(const string& sTableName);

	//根据过滤条件显示数据
	void ShowData( map<string,CFieldDesc*> &mapFieldName2FieldDesc, CDataSet* pDs );

	//获取单元格数据
	void GetCellData( CDataSet* pDs,vector<CFieldDesc*>& vColumn,int row, int col, CString &str );

protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult); 

public:
	string m_strSelectTemplate;

private:
	CDataSet m_ds;
	CRecord* m_pSelectRecord;
};