#pragma once
#include "ChartPageBase.h"

//���ǣ��µ�����
class CChartPage_RiseFall : public CChartPageBase
{
public:
	CChartPage_RiseFall(CWnd* pParent = NULL); 

protected:
	virtual void AddTitles();
	virtual void AddSeries();
	virtual void ShowChart();
};


//��ͣ�壬��ͣ��
class CChartPage_RiseFall2Limit : public CChartPageBase
{
public:
	CChartPage_RiseFall2Limit(CWnd* pParent = NULL); 

protected:
	virtual void AddTitles();
	virtual void AddSeries();
	virtual void ShowChart();
};



//�ɽ���
class CChartPage_TradeVolume : public CChartPageBase
{
public:
	CChartPage_TradeVolume(CWnd* pParent = NULL); 

protected:
	virtual void AddTitles();
	virtual void AddSeries();
	virtual void ShowChart();
};


//ָ���ǵ�
class CChartPage_ARiseFall : public CChartPageBase
{
public:
	CChartPage_ARiseFall(CWnd* pParent = NULL); 

protected:
	virtual void AddTitles();
	virtual void AddSeries();
	virtual void ShowChart();
};