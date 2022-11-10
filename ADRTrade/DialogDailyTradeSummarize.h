#pragma once
#include "LoadLayout.h"
#include "DialogPlaceHolder.h"
#include "Factory.h"

class CDialogDailyTradeSummarize :public CDialogPlaceHolder,public ILoadLayout
{
public:
	CDialogDailyTradeSummarize();
	~CDialogDailyTradeSummarize();
	enum { IDD = IDD_DLG_Common };

protected:
	virtual void _LoadLayout();
	virtual void _InitLayOut();	

protected:
	map<string,shared_ptr<CEdit>>  m_mapUIName2Wnd;
};