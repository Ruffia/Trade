#pragma once
#include <map>
using namespace std;
#include "Record.h"


class EXPORT_CALSS CDataSet
{
public:
	CDataSet();

	~CDataSet();

public:
    CRecord* operator[](int nRowIndex);

    CRecord* GetAt(int nRowIndex);

	void Add(int nRow,CRecord* pRecord)
	{
		m_mapRowIndex2Record[nRow] = pRecord;
	}

	int Size()
	{
		return m_mapRowIndex2Record.size();
	}

	void Clear();

private:
	map<int,CRecord*>  m_mapRowIndex2Record;
};