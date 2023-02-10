#pragma once
#include "Tools/DialogPlaceHolder.h"

class CDialogTabItem_MinorCycleAnalyze : public CDialogPlaceHolder
{
	DECLARE_DYNAMIC(CDialogTabItem_MinorCycleAnalyze)
public:
	CDialogTabItem_MinorCycleAnalyze(CWnd* pParent = NULL);

	void SetRecordTime(const string& strRecordTime)
	{
		m_strRecordTime = strRecordTime;
	}

	string GetRecordTime()
	{
		return m_strRecordTime;
	}

	virtual void UpdateUI2DB();

protected:
	virtual void _LoadTradeDayData2UI();


protected:
	string m_strRecordTime_Old;  //保存修改页签名称之前的名称
	string m_strRecordTime;
};