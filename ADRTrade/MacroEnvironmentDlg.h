#pragma once
#include "LoadLayout.h"
#include "DialogPlaceHolder.h"

class CMacroEnvironmentDlg :public CDialogPlaceHolder,public ILoadLayout
{
public:
	CMacroEnvironmentDlg();
	enum { IDD = IDD_DLG_Common };

protected:
	virtual void _LoadLayout();
	virtual void _InitLayOut();	
};