#pragma once
#include <string>
using namespace std;

class CBusinessEdit : public CEdit
{
	DECLARE_DYNAMIC(CBusinessEdit)

public:
	CBusinessEdit();
	virtual ~CBusinessEdit();

public:
	string m_sBusinessField;

protected:
	DECLARE_MESSAGE_MAP()
};


class CBusinessEditNumber:public CBusinessEdit
{
public:
	CBusinessEditNumber(){}
};


class CBusinessEdit_ConflictDetailedDescription:public CBusinessEdit
{
public:
	CBusinessEdit_ConflictDetailedDescription(){}
};
