#include "StdAfx.h"
#include "MacroEnvironmentDlg.h"
#include "Factory.h"
#include "DialogIDManager.h"

IMPLEMENT_FACTORY(CDialog,CMacroEnvironmentDlg,string,"CMacroEnvironmentDlg")
CMacroEnvironmentDlg::CMacroEnvironmentDlg()
{
	CDialogIDMgr::Instance().Register("CMacroEnvironmentDlg",CMacroEnvironmentDlg::IDD);
}


void CMacroEnvironmentDlg::_LoadLayout()
{

}


void CMacroEnvironmentDlg::_InitLayOut()
{

}