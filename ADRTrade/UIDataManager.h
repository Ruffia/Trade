#pragma once
#include <map>
#include "UIData.h"
using namespace std;

//UI--->���� ������
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

	//����UI���ݵ�DB��
	void UpdateUI2DB();

private:
	//ҵ���-->UIData
	map<string,CUIData>  m_mapUIName2Data;
};