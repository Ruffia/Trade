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

