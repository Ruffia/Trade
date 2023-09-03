#pragma once
#include "Tools/CustomTabCtrlDlgDB.h"

class CCustomTabCtrlDlg_DailyMinorCycleAnalyze : public CCustomTabCtrlDlgBusiness
{
public:
	CCustomTabCtrlDlg_DailyMinorCycleAnalyze(CWnd* pParent = NULL);

	enum { IDD = IDD_DLG_Common };

public:
	//将UI上的数据保存到数据库中
	virtual void UpdateUI2DB();

protected:
	virtual void _InitPage(CRect& rcTab); 
	virtual void _LoadData2UI();
};
