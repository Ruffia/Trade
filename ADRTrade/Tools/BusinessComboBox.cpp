#include "stdafx.h"
#include "BusinessComBoBox.h"

IMPLEMENT_DYNAMIC(CBusinessComboBox, CComboBox)

CBusinessComboBox::CBusinessComboBox()
{
	m_sBusinessField = "";
}

CBusinessComboBox::~CBusinessComboBox()
{
}

bool CBusinessComboBox::GetMeaning(const int nValue,string& sMeaning)
{
	map<int,string>::iterator it = m_mapValue2Meaning.find(nValue);
	if(m_mapValue2Meaning.end() == it) return false;

	sMeaning = it->second;
	return true;
}


bool CBusinessComboBox::GetValue(const string& sMeaning,int& nValue)
{
	for (map<int,string>::iterator it = m_mapValue2Meaning.begin();
		it != m_mapValue2Meaning.end();it++)
	{
		if (sMeaning == it->second)
		{
			nValue = it->first;
			return true;
		}
	}

	return false;
}


BOOL CBusinessComboBox::PreTranslateMessage(MSG* pMsg)
{
	return pMsg->message == WM_MOUSEWHEEL ? TRUE : CComboBox::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CBusinessComboBox, CComboBox)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()




