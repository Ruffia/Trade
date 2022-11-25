#pragma once
#include "Tools/DialogPlaceHolderComposite.h"
#include "Factory.h"

class CDlgStock:public CDialogPlaceHolderComposite
{
public:
	CDlgStock();
	enum { IDD = IDD_DLG_Common };
};