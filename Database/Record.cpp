#include "stdafx.h"
#include "Record.h"

IMPLEMENT_FACTORY(CRecord,CRecord,string,"CRecord")
CRecord::CRecord()
{
    m_mapFieldName2Field.clear();
}


CRecord::~CRecord()
{
	for (map<string,CField*>::iterator it = m_mapFieldName2Field.begin();
		 it != m_mapFieldName2Field.end();it++)
	{
		CField* pField = it->second;
		if (pField)
		{
			delete pField;
			pField = NULL;
		}
	}

	m_mapFieldName2Field.clear();
}


CField* CRecord::GetField(string strFieldID)
{
	map<string,CField*>::iterator it = m_mapFieldName2Field.find(strFieldID);
	if (it != m_mapFieldName2Field.end())
	{
		return it->second;
	}
	return NULL;
}


bool compare(CField* pFieldL,CField* pFieldR)
{
	if(!pFieldL || !pFieldR) return false;
	if(!pFieldL->GetFieldDesc() || !pFieldR->GetFieldDesc()) return false;
	return pFieldL->GetFieldDesc()->m_nSequenceID < pFieldR->GetFieldDesc()->m_nSequenceID; 
}


void CRecord::GetSortedFieldSet(vector<CField*>& vField)
{
	for (map<string,CField*>::iterator it = m_mapFieldName2Field.begin();
		it != m_mapFieldName2Field.end();it++)
	{
		vField.push_back(it->second);
	}

	sort(vField.begin(),vField.end(),compare);
}

