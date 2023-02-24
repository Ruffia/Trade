#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;
#include "Field.h"

class EXPORT_CALSS CRecord
{
public:
	CRecord();

	~CRecord();

public:
	CField* GetField(string strFieldID);

	void Add(string sFieldName,CField* pField)
	{
		m_mapFieldName2Field[sFieldName] = pField;
	}

	int Size()
	{
		return m_mapFieldName2Field.size();
	}

	map<string,CField*>& GetFieldName2FieldMap()
	{
		return m_mapFieldName2Field;
	}

	virtual void GetSortedFieldSet(vector<CField*>& vField);
protected:
	//ÁÐºÅ-->×Ö¶Î
	map<string,CField*> m_mapFieldName2Field; 
};