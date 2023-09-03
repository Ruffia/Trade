#pragma  once
#include <string>
using namespace std;
#include "DataSet.h"
class CDialogPlaceHolderBusiness;

class IDataProvider
{
public:
	IDataProvider();

	void Attach(CDialogPlaceHolderBusiness* pHolder);

	//从数据库中加载数据
	virtual void LoadData(CDataSet& ds) = 0;

	//将UI上的数据保存到数据库中
	virtual void UpdateUI2DB(CDataSet& ds) = 0;

	//根据从数据库查询得到的数据记录值更新UI控件的显示
	virtual void UpdateDB2UI(CDataSet& ds) = 0;

public:
	string m_sBusiness;                          //业务名
	CDataSet m_ds;                               //数据集
	CDialogPlaceHolderBusiness*     m_pHolder;   //关联的UI
};