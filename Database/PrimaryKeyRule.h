#pragma once
#include <string>
using namespace std;
#include "FieldMeta.h"

class EXPORT_CALSS IPrimaryKeyRule
{
public:
	IPrimaryKeyRule();

	void SetFieldDesc(CFieldDesc* pFieldDesc)
	{
		m_pFieldDesc = pFieldDesc;
	}

	virtual string GetInsertSQL() = 0;
	virtual string GetUpdateSQL() = 0;
protected:
	CFieldDesc* m_pFieldDesc;
};


class CPrimaryKey_TradeDay : public IPrimaryKeyRule
{
public:
	virtual string GetInsertSQL();
	virtual string GetUpdateSQL();
};


class CPrimaryKey_FutureContractName: public IPrimaryKeyRule
{
public:
	virtual string GetInsertSQL();
	virtual string GetUpdateSQL();
};


class CPrimaryKey_RecordTime: public IPrimaryKeyRule
{
public:
	virtual string GetInsertSQL();
	virtual string GetUpdateSQL();
};


class CPrimaryKey_TraceEvidenceNumber: public IPrimaryKeyRule
{
public:
	virtual string GetInsertSQL();
	virtual string GetUpdateSQL();
protected:
	static int nSequence;
};


class CPrimaryKey_TraceConflictNumber: public IPrimaryKeyRule
{
public:
	virtual string GetInsertSQL();
	virtual string GetUpdateSQL();
protected:
	static int nSequence;
};