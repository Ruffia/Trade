#pragma once
#include "LoadLayout.h"
#include "Tools/ChildDlgTab.h"
#include "Factory.h"

//√ø»’∑÷Œˆ
class CDlgDailyAnalyze :public CChildDlgTab,public ILoadLayout
{
public:
	CDlgDailyAnalyze();
	~CDlgDailyAnalyze();
	enum { IDD = IDD_ChildDLG_Tab_DailyAnalyze };

protected:
	virtual void _LoadLayout();
	virtual void InitPage();
};