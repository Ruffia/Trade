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
	string m_strRecordTime_Old;  //�����޸�ҳǩ����֮ǰ������
	string m_strRecordTime;
};