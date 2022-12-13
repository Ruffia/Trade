#pragma once
#include "CustomTabCtrlDlg.h"

class CCustomTabCtrlLeftDlg : public CCustomTabCtrlDlg
{
public:
	CCustomTabCtrlLeftDlg(CWnd* pParent = NULL);

protected:
	virtual void _CreateTabCtrl(const CRect& rc);
	virtual void _AddPage() = 0;
	virtual void _Resize(int cx, int cy);
};
