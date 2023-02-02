#include "stdafx.h"
#include "UIDataManager.h"
#include "./Tools/DialogPlaceHolder.h"

CUIDataMgr::CUIDataMgr()
{
	m_mapUIName2Data.clear();
}

CUIDataMgr& CUIDataMgr::Instance()
{
	static CUIDataMgr inst;
	return inst;
}


void CUIDataMgr::Release()
{
	for (map<string,CUIData>::iterator it = m_mapUIName2Data.begin();
		it != m_mapUIName2Data.end();it++)
	{
		CUIData& uiData = it->second;
		CWnd* pWnd = uiData.m_pWnd;
		if (pWnd)
		{
			delete pWnd;
			pWnd = NULL;
		}
	}
}

void CUIDataMgr::Register(const string& sBusinessName,const CUIData& uiData)
{
	m_mapUIName2Data[sBusinessName] = uiData;
}


bool CUIDataMgr::GetUIData(const string& sBusinessName,CUIData& uiData)
{
	map<string,CUIData>::iterator it = m_mapUIName2Data.find(sBusinessName) ;
	if(m_mapUIName2Data.end() == it) return false;

	uiData = it->second;
	return true;
}


void CUIDataMgr::UpdateUI2DB()
{
	for (map<string,CUIData>::iterator it = m_mapUIName2Data.begin();
		it != m_mapUIName2Data.end();it++)
	{
		CUIData& ui = it->second;	
		CDialogPlaceHolder* pHolder = dynamic_cast<CDialogPlaceHolder*>(ui.m_pWnd);
		if(!pHolder) continue;
		pHolder->UpdateUI2DB();
	}
}