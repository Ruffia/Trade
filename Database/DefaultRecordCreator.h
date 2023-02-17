#pragma once
#include <string>
#include <vector>
using namespace std;
#include "FieldMeta.h"

class EXPORT_CALSS IDefaultRecordCreator
{
public:
	IDefaultRecordCreator();

	void SetTable(string strTableName)
	{
		m_strTableName = strTableName;
	}

	virtual void CreateRecord() = 0;
protected:
	string m_strTableName;
};


class CDefaultRecordCreator_Common : public IDefaultRecordCreator
{
public:
	virtual void CreateRecord();
};


class CDefaultRecordCreator_MinorCycleAnalyze : public IDefaultRecordCreator
{
public:
	virtual void CreateRecord();
};


class CDefaultRecordCreator_DailyTraceEvidence : public IDefaultRecordCreator
{
public:
	virtual void CreateRecord();
};