#pragma once
#include <string>
#include <map>
using namespace std;

class CBusinessComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CBusinessComboBox)

public:
	CBusinessComboBox();
	virtual ~CBusinessComboBox();

	bool GetMeaning(const int nValue,string& sMeaning);
	bool GetTranslation(const int nValue,string& sTranslation);
	bool GetValueByTranslation(const string& sMeaning,int& nValue);
	void Add2Map(const int nValue,const string& sMeaning,const string& sTranslation);
	void Reset();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	string m_sBusinessField;
	map<int,string> m_mapValue2Meaning;
	map<int,string> m_mapValue2Translation;

protected:
	DECLARE_MESSAGE_MAP()
};


