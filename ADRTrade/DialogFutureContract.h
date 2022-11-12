#pragma once
#include "DialogPlaceHolderComposite.h"
#include "Factory.h"

class CDlgDailyFutureContract :public CDialogPlaceHolderComposite
{
public:
	CDlgDailyFutureContract();
	enum { IDD = IDD_DLG_Common };
};