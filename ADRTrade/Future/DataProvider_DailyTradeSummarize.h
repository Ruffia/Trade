#pragma  once
#include "../Tools/DataProvider.h"
#include "Factory.h"


class DataProvider_DailyTradeSummarize : public IDataProvider
{
public:
	//从数据库中加载数据
	virtual void LoadData(CDataSet& ds);

	//将UI上的数据保存到数据库中
	virtual void UpdateUI2DB(CDataSet& ds);

	//根据从数据库查询得到的数据记录值更新UI控件的显示
	virtual void UpdateDB2UI(CDataSet& ds);
};


class DataProvider_LongPeriodicAnalyze : public IDataProvider
{
public:
	//从数据库中加载数据
	virtual void LoadData(CDataSet& ds);

	//将UI上的数据保存到数据库中
	virtual void UpdateUI2DB(CDataSet& ds);

	//根据从数据库查询得到的数据记录值更新UI控件的显示
	virtual void UpdateDB2UI(CDataSet& ds);
};


class DataProvider_DailyTraceEvidence : public IDataProvider
{
public:
	//从数据库中加载数据
	virtual void LoadData(CDataSet& ds);

	//将UI上的数据保存到数据库中
	virtual void UpdateUI2DB(CDataSet& ds);

	//根据从数据库查询得到的数据记录值更新UI控件的显示
	virtual void UpdateDB2UI(CDataSet& ds);
};


class DataProvider_DailyTraceConflict : public IDataProvider
{
public:
	//从数据库中加载数据
	virtual void LoadData(CDataSet& ds);

	//将UI上的数据保存到数据库中
	virtual void UpdateUI2DB(CDataSet& ds);

	//根据从数据库查询得到的数据记录值更新UI控件的显示
	virtual void UpdateDB2UI(CDataSet& ds);
};