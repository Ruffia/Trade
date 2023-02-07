#pragma once
#include <string>
using namespace std;
class CFieldDesc;

class IPrimaryKeyRule
{
public:
	IPrimaryKeyRule();

	void SetFieldDesc(CFieldDesc* pFieldDesc)
	{
		m_pFieldDesc = pFieldDesc;
	}

	virtual string GetSQLPair() = 0;
protected:
	CFieldDesc* m_pFieldDesc;
};


class CPrimaryKey_TradeDay : public IPrimaryKeyRule
{
public:
	virtual string GetSQLPair();
};


class CPrimaryKey_FutureContractName: public IPrimaryKeyRule
{
public:
	virtual string GetSQLPair();
};


class CPrimaryKey_RecordTime: public IPrimaryKeyRule
{
public:
	virtual string GetSQLPair();
};