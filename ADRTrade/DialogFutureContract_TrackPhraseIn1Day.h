#pragma once
#include "Tools/ChildDlgTab.h"
#include "Factory.h"

class CDialogFutureContract_TrackPhraseIn1Day :public CChildDlgTab
{
public:
	CDialogFutureContract_TrackPhraseIn1Day();
	enum { IDD = IDD_ChildDLG_Tab_PhraseIn1Day };

protected:
	virtual void _InitLayOut();
};