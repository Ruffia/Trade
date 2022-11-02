#include "stdafx.h"
#include "StyleManager.h"
#include "Util.h"

CStyleMgr::CStyleMgr()
{
	m_mapName2Style.clear();
}

CStyleMgr& CStyleMgr::Instance()
{
	static CStyleMgr inst;
	return inst;
}


DWORD CStyleMgr::_GetStyle(const string& sName)
{
	map<string,DWORD>::iterator it = m_mapName2Style.find(sName);
	if (it != m_mapName2Style.end())
	{
		return it->second;
	}

	return -1;
}


DWORD CStyleMgr::GetStyle(const string& sStyle)
{
	vector<string> vStyle;
	Split(sStyle, "|",vStyle);
	DWORD dwTotalStyle = 0;
	for (int i = 0; i < vStyle.size();i++)
	{
		DWORD dwStyle = _GetStyle(vStyle[i]);
		dwTotalStyle = dwTotalStyle|dwStyle;
	}

	return dwTotalStyle;
}

void CStyleMgr::_Register(const string& sName,DWORD dwStyle)
{
	m_mapName2Style[sName] = dwStyle;
}


void CStyleMgr::Initialize()
{
	_Register("WS_BORDER",WS_BORDER);
	_Register("WS_CHILD",WS_CHILD);
	_Register("WS_VISIBLE",WS_VISIBLE);
	_Register("WS_TABSTOP",WS_TABSTOP);
	_Register("ES_AUTOHSCROLL",ES_AUTOHSCROLL);
	_Register("ES_CENTER",ES_CENTER);
	_Register("ES_LEFT",ES_LEFT);
	_Register("ES_READONLY",ES_READONLY);	
}