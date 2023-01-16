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
	bool GetValue(const string& sMeaning,int& nValue);
	void Add2Map(const int nValue,string& sMeaning)
	{
		m_mapValue2Meaning[nValue] = sMeaning;
	}

public:
	string m_sBusinessField;
	map<int,string> m_mapValue2Meaning;

protected:
	DECLARE_MESSAGE_MAP()
};


