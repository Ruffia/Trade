#pragma once
#include "ChartPageBase.h"

//上涨，下跌比例
class CChartPage_RiseFall : public CChartPageBase
{
public:
	CChartPage_RiseFall(CWnd* pParent = NULL); 

protected:
	virtual void AddTitles();
	virtual void AddSeries();
	virtual void ShowChart();
};


//涨停板，跌停板
class CChartPage_RiseFall2Limit : public CChartPageBase
{
public:
	CChartPage_RiseFall2Limit(CWnd* pParent = NULL); 

protected:
	virtual void AddTitles();
	virtual void AddSeries();
	virtual void ShowChart();
};



//成交量
class CChartPage_TradeVolume : public CChartPageBase
{
public:
	CChartPage_TradeVolume(CWnd* pParent = NULL); 

protected:
	virtual void AddTitles();
	virtual void AddSeries();
	virtual void ShowChart();
};


//指数涨跌
class CChartPage_ARiseFall : public CChartPageBase
{
public:
	CChartPage_ARiseFall(CWnd* pParent = NULL); 

protected:
	virtual void AddTitles();
	virtual void AddSeries();
	virtual void ShowChart();
};