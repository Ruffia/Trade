#pragma once
#include <map>
#include "UIData.h"
using namespace std;

//UI--->数据 管理器
class CUIDataMgr
{
protected:
	CUIDataMgr();
	~CUIDataMgr(){};

public:
	static CUIDataMgr& Instance();
	void Release();
	void Register(const string& sBusinessName,const CUIData& uiData);
	bool GetUIData(const string& sBusinessName,CUIData& uiData);
	map<string,CUIData>& GetUIData()
	{
		return m_mapUIName2Data;
	}

	//更新UI数据到DB中
	void UpdateUI2DB();

private:
	//业务表-->UIData
	map<string,CUIData>  m_mapUIName2Data;
};