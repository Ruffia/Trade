#pragma once
#include "LoadLayout.h"
#include "Tools/ChildDlgTab.h"
#include "Factory.h"

class CDlgDailyTradeTrack :public CChildDlgTab,public ILoadLayout
{
public:
	CDlgDailyTradeTrack();
	~CDlgDailyTradeTrack();
	enum { IDD = IDD_ChildDLG_Tab_DailyTradeTrack };

protected:
	virtual void _LoadLayout();
	virtual void InitPage();
};