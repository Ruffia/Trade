#pragma once
#include "LoadLayout.h"
#include "DialogPlaceHolder.h"
#include "Factory.h"

class CDailyMacroEnvironmentDlg :public CDialogPlaceHolder,public ILoadLayout
{
public:
	CDailyMacroEnvironmentDlg();
	~CDailyMacroEnvironmentDlg();
	enum { IDD = IDD_DLG_Common };

protected:
	virtual void _LoadLayout();
	virtual void _InitLayOut();	

protected:
	map<string,CWnd*>  m_mapUIName2Wnd;
};