#include "StdAfx.h"
#include <vector>
using namespace std;
#include "DialogStock_TrackPhraseIn1Week.h"
#include "Factory.h"
#include "UIData.h"
#include "DialogIDManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_FACTORY(CDialog,CDialogStock_TrackPhraseIn1Week,string,"CDialogStock_TrackPhraseIn1Week")
CDialogStock_TrackPhraseIn1Week::CDialogStock_TrackPhraseIn1Week()
{
	CDialogIDMgr::Instance().Register("CDialogStock_TrackPhraseIn1Week",CDialogStock_TrackPhraseIn1Week::IDD);
}