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


void CBusinessComboBox::Add2Map(const int nValue,const string& sMeaning,const string& sTranslation)
{
	m_mapValue2Meaning[nValue] = sMeaning;
	m_mapValue2Translation[nValue] = sTranslation;
}


bool CBusinessComboBox::GetMeaning(const int nValue,string& sMeaning)
{
	map<int,string>::iterator it = m_mapValue2Meaning.find(nValue);
	if(m_mapValue2Meaning.end() == it) return false;

	sMeaning = it->second;
	return true;
}


bool CBusinessComboBox::GetTranslation(const int nValue,string& sTranslation)
{
	map<int,string>::iterator it = m_mapValue2Translation.find(nValue);
	if(m_mapValue2Translation.end() == it) return false;

	sTranslation = it->second;
	return true;
}


bool CBusinessComboBox::GetValueByTranslation(const string& sTranslation,int& nValue)
{
	for (map<int,string>::iterator it = m_mapValue2Translation.begin();
		it != m_mapValue2Translation.end();it++)
	{
		if (sTranslation == it->second)
		{
			nValue = it->first;
			return true;
		}
	}

	return false;
}


void CBusinessComboBox::Reset()
{
	m_mapValue2Meaning.clear();
	m_mapValue2Translation.clear();
}

BOOL CBusinessComboBox::PreTranslateMessage(MSG* pMsg)
{
	return pMsg->message == WM_MOUSEWHEEL ? TRUE : CComboBox::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CBusinessComboBox, CComboBox)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


CBusinessComboBox_TechnicalIndex::CBusinessComboBox_TechnicalIndex()
{}

