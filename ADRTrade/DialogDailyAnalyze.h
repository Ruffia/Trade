#pragma once
#include "Tools/ChildDlgTab.h"

//ÿ�շ���
class CDlgDailyAnalyze :public CChildDlgTab
{
public:
	CDlgDailyAnalyze();
	~CDlgDailyAnalyze();
	enum { IDD = IDD_ChildDLG_Tab_DailyAnalyze };

protected:
	virtual void _InitLayOut();
};