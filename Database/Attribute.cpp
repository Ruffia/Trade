#include "stdafx.h"
#include "Attribute.h"

CAttribute::CAttribute()
{
	m_pAttributeValue = NULL;
}

CAttribute::~CAttribute()
{
	if (m_pAttributeValue)
	{
		delete m_pAttributeValue;
		m_pAttributeValue = NULL;
	}
}


void CAttribute::Initialize(string strAttributeID,string strFieldID,string strTableName,string strDataType,FieldValue* pValue)
{
	m_strAttributeID = strAttributeID;
	m_strFieldID = strFieldID;
	m_strTableName = strTableName;
	m_strDataType = strDataType;
	m_pAttributeValue = pValue;
}


string CAttribute::GetAttributeValueString()
{
	if(!m_pAttributeValue) return "";
	const string sValue = m_pAttributeValue->GetValueAsString();
	return sValue;
}


float CAttribute::GetAttributeValueFloat()
{
	if(!m_pAttributeValue) return -1.0f;
	const float fValue = m_pAttributeValue->GetValueAsFloat();
	return fValue;
}


int CAttribute::GetAttributeValueInt()
{
	if(!m_pAttributeValue) return -1;
	const int nValue = m_pAttributeValue->GetValueAsInt();
	return nValue;
}