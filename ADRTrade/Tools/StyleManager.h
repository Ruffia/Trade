#pragma once
#include <map>
using namespace std;

class CStyleMgr
{
protected:
	CStyleMgr();
	~CStyleMgr(){};

public:
	static CStyleMgr& Instance();
	void Initialize();
	//根据组合风格字符串获取组合风格值
	DWORD GetStyle(const string& sStyle);

protected:
	//根据风格名称获取单个风格值
	DWORD _GetStyle(const string& sName);
	void _Register(const string& sName,DWORD dwStyle);

private:
	//style string -->style
	map<string,DWORD> m_mapName2Style;
};