// DlgSEMLog.cpp : 实现文件
//
#include "stdafx.h"
#include "DlgTab_WorldEcnomic.h"
#include "../Common/Factory.h"
#include "DialogIDManager.h"

IMPLEMENT_FACTORY(CDlgTab,CDlgTab_WorldEcnomic,string,"CDlgTab_WorldEcnomic")
CDlgTab_WorldEcnomic::CDlgTab_WorldEcnomic()
{
	CDialogIDMgr::Instance().Register("CDlgTab_WorldEcnomic",CDlgTab_WorldEcnomic::IDD);
}