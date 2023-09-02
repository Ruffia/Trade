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
	//��ʼ��Layout
	virtual void _InitLayOut();
	//���������������Layout
	virtual void _DesignLayout();
	//�����ݿ��������
	virtual void _LoadData2UI();
	//����ͼ��UI
	virtual void _CreateChart();

protected:
	string m_strRecordTime_Old;  //�����޸�ҳǩ����֮ǰ������
	string m_strRecordTime;
	bool   m_bNeed2UpdateRecordTime;

	CChartCtrl* m_pChartCtrl;
};