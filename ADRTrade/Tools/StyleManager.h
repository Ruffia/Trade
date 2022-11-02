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
	//������Ϸ���ַ�����ȡ��Ϸ��ֵ
	DWORD GetStyle(const string& sStyle);

protected:
	//���ݷ�����ƻ�ȡ�������ֵ
	DWORD _GetStyle(const string& sName);
	void _Register(const string& sName,DWORD dwStyle);

private:
	//style string -->style
	map<string,DWORD> m_mapName2Style;
};