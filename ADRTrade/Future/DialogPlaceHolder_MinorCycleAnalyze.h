#pragma once
#include "Tools/DialogPlaceHolderDB.h"

class CDialogTabItem_MinorCycleAnalyze : public CDialogPlaceHolderBusiness
{
	DECLARE_DYNAMIC(CDialogTabItem_MinorCycleAnalyze)
public:
	CDialogTabItem_MinorCycleAnalyze(CWnd* pParent = NULL);

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
	virtual void _LoadData2UI();


protected:
	string m_strRecordTime_Old;  //保存修改页签名称之前的名称
	string m_strRecordTime;
	bool   m_bNeed2UpdateRecordTime;
};