#include "stdafx.h"
#include "FieldValue.h"

FieldValue::FieldValue()
{
	m_strDataType = "";
	ZeroMemory(_value.sValue,128);
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