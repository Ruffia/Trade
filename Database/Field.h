#pragma once
#include <string>
using namespace std;
#include "FieldValue.h"
#include "FieldMeta.h"

class EXPORT_CALSS CField
{
public:
	CField();

	~CField();

     //Create Field object based on m_pFieldDesc
     virtual void Create();

public:
	FieldValue* GetFieldValue()
	{
		return m_pValue;
	}

	void SetFieldValue(FieldValue* pValue)
	{
		m_pValue = pValue;
	}

	void SetFieldDesc(CFieldDesc* pFieldDesc)
	{
		m_pFieldDesc = pFieldDesc;
	}

	CFieldDesc* GetFieldDesc()
	{
		return m_pFieldDesc;
	}


public:
	bool GetValueAsBool()
	{
		return m_pValue->GetValueAsBool();
	}

	int GetValueAsInt()
	{
		return m_pValue->GetValueAsInt();
	}

	double GetValueAsFloat()
	{
		return m_pValue->GetValueAsFloat();
	}

	double GetValueAsDouble()
	{
		return m_pValue->GetValueAsDouble();
	}

	string GetValueAsString()
	{
		return m_pValue->GetValueAsString();
	}

	__int64 GetValueAsInt64()
	{
		return m_pValue->GetValueAsInt64();
	}

	long GetValueAsLong()
	{
		return m_pValue->GetValueAsLong();
	}

	long long GetValueAsLongLong()
	{
		return m_pValue->GetValueAsLongLong();
	}


private:
	CFieldDesc* m_pFieldDesc;
	FieldValue* m_pValue;
};