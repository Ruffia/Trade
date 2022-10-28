#include "stdafx.h"
#include "DataSet.h"

CDataSet::CDataSet()
{
}


CDataSet::~CDataSet()
{
	Clear();
}


CRecord* CDataSet::operator[](int nRowIndex)
{
	if (nRowIndex < 0 || nRowIndex >= m_mapRowIndex2Record.size())
	{
		return NULL;
	}

	return m_mapRowIndex2Record[nRowIndex];
}


CRecord* CDataSet::GetAt(int nRowIndex)
{
	if (nRowIndex < 0 || nRowIndex >= m_mapRowIndex2Record.size())
	{
		return NULL;
	}

	return m_mapRowIndex2Record[nRowIndex];
}


void CDataSet::Clear()
{
	for (map<int,CRecord*>::iterator it = m_mapRowIndex2Record.begin();
		it != m_mapRowIndex2Record.end(); it++)
	{
		CRecord* pRecord = it->second;
		if(pRecord)
		{
			delete pRecord;
			pRecord = NULL;
		}
	}

	m_mapRowIndex2Record.clear();
}