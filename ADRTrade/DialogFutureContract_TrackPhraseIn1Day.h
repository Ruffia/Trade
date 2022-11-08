#pragma once
#include "LoadLayout.h"
#include "Tools/ChildDlgTab.h"
#include "Factory.h"

class CDialogFutureContract_TrackPhraseIn1Day :public CChildDlgTab,public ILoadLayout
{
public:
	CDialogFutureContract_TrackPhraseIn1Day();
	~CDialogFutureContract_TrackPhraseIn1Day();
	enum { IDD = IDD_ChildDLG_Tab_PhraseIn1Day };

protected:
	virtual void _LoadLayout();
	virtual void InitPage();
};