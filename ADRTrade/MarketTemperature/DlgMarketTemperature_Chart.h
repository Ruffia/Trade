#pragma once
#include "../Tools/DialogPlaceHolder.h"
#include "../High-Speed-Charting-Control/ChartCtrl.h"

class CDlgMarketTemperature_Chart : public CDialogPlaceHolder
{
	DECLARE_DYNAMIC(CDlgMarketTemperature_Chart)
public:
	CDlgMarketTemperature_Chart(CWnd* pParent = NULL);

	void SetRecordTime(const string& strRecordTime)
	{
		m_strRecordTime = strRecordTime;
	}

	void SetRecordTime_Old(const string& strRecordTime)
	{
		m_strRecordTime_Old = strRecordTime;
	}

	string GetRecordTime()
	{
		return m_strRecordTime;
	}

	string GetRecordTime_Old()
	{
		return m_strRecordTime_Old;
	}

	void SetNeed2UpdateRecordTime(bool bNeed2Update)
	{
		m_bNeed2UpdateRecordTime = bNeed2Update;
	}

	bool Need2UpdateRecordTime()
	{
		return m_bNeed2UpdateRecordTime;
	}

	virtual void UpdateUI2DB();

protected:
	BOOL OnInitDialog();
	//初始化Layout
	virtual void _InitLayOut();
	//根据缩放情况调整Layout
	virtual void _DesignLayout();
	//从数据库加载数据
	virtual void _LoadData2UI();
	//创建图形UI
	virtual void _CreateChart();

protected:
	string m_strRecordTime_Old;  //保存修改页签名称之前的名称
	string m_strRecordTime;
	bool   m_bNeed2UpdateRecordTime;

	CChartCtrl* m_pChartCtrl;
};