#include "StdAfx.h"
#include <vector>
using namespace std;
#include "DialogFutureContract_TrackPhraseIn1Day.h"
#include "Factory.h"
#include "UIData.h"
#include "DialogIDManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_FACTORY(CDialog,CDialogFutureContract_TrackPhraseIn1Day,string,"CDialogFutureContract_TrackPhraseIn1Day")
CDialogFutureContract_TrackPhraseIn1Day::CDialogFutureContract_TrackPhraseIn1Day()
{
	CDialogIDMgr::Instance().Register("CDialogFutureContract_TrackPhraseIn1Day",CDialogFutureContract_TrackPhraseIn1Day::IDD);
}