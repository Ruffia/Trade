#pragma once
#include <string>
using namespace std;
#include "Export.h"

class EXPORT_CALSS FieldValue
{
public:
	FieldValue();

	~FieldValue();

	bool GetValueAsBool()
	{
		return _value.bValue;
	}

	void SetValueBool(bool bValue)
	{
		_value.bValue = bValue;
	}


	int GetValueAsInt()
	{
		return _value.iValue;
	}

	void SetValueInt(int iValue)
	{
		_value.iValue = iValue;
	}

	int GetValueAsInt64()
	{
		return _value.iValue64;
	}

	void SetValueInt64(__int64 iValue64)
	{
		_value.iValue64 = iValue64;
	}

	double GetValueAsFloat()
	{
		return _value.fValue;
	}

	void SetValueFloat(float fValue)
	{
		_value.fValue = fValue;
	}

	double GetValueAsDouble()
	{
		return _value.dValue;
	}

	void SetValueDouble(double dValue)
	{
		_value.dValue = dValue;
	}

	long GetValueAsLong()
	{
		return _value.lValue;
	}

	void SetValueLong(long lValue)
	{
		_value.lValue = lValue;
	}


	long long GetValueAsLongLong()
	{
		return _value.llValue;
	}

	void SetValueLongLong(long long llValue)
	{
		_value.llValue = llValue;
	}

	char* GetValueAsString()
	{
		return _value.sValue;
	}

	void SetValueString(const char* sValue);

	void SetDataType(const string strType)
	{
		m_strDataType = strType;
	}

public:
	//数据类型
	string m_strDataType;

	union
	{
		bool bValue;
		int iValue;
		__int64 iValue64;
		long lValue;
		long long llValue;
		float fValue;
		double dValue;
		char sValue[128];
	}_value;
};