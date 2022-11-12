#pragma once
#include "Tools/ChildDlgTab.h"
#include "Factory.h"

//每日交易
class CDlgDailyTrades :public CChildDlgTab
{
public:
	CDlgDailyTrades();
	~CDlgDailyTrades();
	enum { IDD = IDD_ChildDLG_Tab_DailyAnalyze };

protected:
	virtual void _InitLayOut();
};