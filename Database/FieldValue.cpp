#include "stdafx.h"
#include "FieldValue.h"

FieldValue::FieldValue()
{
	m_strDataType = "";
	ZeroMemory(_value.sValue,512);
}


FieldValue::FieldValue(const string& strType,bool bValue)
{
	m_strDataType = strType;
	_value.bValue = bValue;
}

FieldValue::FieldValue(const string& strType,char* sValue)
{
	m_strDataType = strType;
	memset(_value.sValue,0,strlen(sValue) + 1);
	strcpy(_value.sValue,sValue);
}

FieldValue::FieldValue(const string& strType,int nValue)
{
	m_strDataType = strType;
	_value.iValue = nValue;
}

FieldValue::FieldValue(const string& strType,__int64 iValue64)
{
	m_strDataType = strType;
	_value.iValue64 = iValue64;
}

FieldValue::FieldValue(const string& strType,float fValue)
{
	m_strDataType = strType;
	_value.fValue = fValue;
}

FieldValue::FieldValue(const string& strType,double dValue)
{
	m_strDataType = strType;
	_value.dValue = dValue;
}

FieldValue::FieldValue(const string& strType,long lValue)
{
	m_strDataType = strType;
	_value.lValue = lValue;
}

FieldValue::~FieldValue()
{
	//if (_value.sValue)
	//{
	//	delete _value.sValue; 
	//	_value.sValue = NULL;
	//}
}

void FieldValue::SetValueString(const char* sValue)
{
	//_value.sValue = new char[strlen(sValue) + 1];
	memset(_value.sValue,0,strlen(sValue) + 1);
	strcpy(_value.sValue,sValue);
}