#pragma once
#include "LoadLayout.h"
#include "DialogPlaceHolder.h"
#include "Factory.h"

class CDialogFutureContract_MinorCycleAnalyze :public CDialogPlaceHolder,public ILoadLayout
{
public:
	CDialogFutureContract_MinorCycleAnalyze();
	~CDialogFutureContract_MinorCycleAnalyze();
	enum { IDD = IDD_DLG_Common };

protected:
	virtual void _LoadLayout();
	virtual void _InitLayOut();	

protected:
	map<string,CWnd*>  m_mapUIName2Wnd;
};