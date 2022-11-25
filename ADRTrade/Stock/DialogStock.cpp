#include "StdAfx.h"
#include <vector>
using namespace std;
#include "DialogStock.h"
#include "Factory.h"
#include "DialogIDManager.h"

IMPLEMENT_FACTORY(CDialog,CDlgStock,string,"CDlgStock")
CDlgStock::CDlgStock()
{
	CDialogIDMgr::Instance().Register("CDlgStock",CDlgStock::IDD);
}