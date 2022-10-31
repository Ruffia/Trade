#include "stdafx.h"
#include "DialogIDManager.h"

CDialogIDMgr::CDialogIDMgr()
{
	m_mapDialogName2IDD.clear();
}

CDialogIDMgr& CDialogIDMgr::Instance()
{
	static CDialogIDMgr inst;
	return inst;
}


void CDialogIDMgr::Register(const string& sDialogName,int nIDD)
{
	m_mapDialogName2IDD[sDialogName] = nIDD;
}

int CDialogIDMgr::GetDialogResourceID(const string& sName)
{
	map<string,int>::iterator it = m_mapDialogName2IDD.find(sName);
	if (it != m_mapDialogName2IDD.end())
	{
		return it->second;
	}

	return -1;
}

