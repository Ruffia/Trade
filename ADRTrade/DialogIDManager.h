#pragma once
#include <map>
using namespace std;

class CDialogIDMgr
{
protected:
	CDialogIDMgr();
	~CDialogIDMgr(){};

public:
	static CDialogIDMgr& Instance();
	int GetDialogResourceID(const string& sName);
	void Register(const string& sDialogName,int nIDD);

private:
	//¶Ô»°¿òÃû³Æ-->IDD
	map<string,int> m_mapDialogName2IDD;
};