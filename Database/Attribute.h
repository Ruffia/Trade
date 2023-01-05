#pragma once
#include <string>
using namespace std;
#include <map>
#include "FieldValue.h"
 
class EXPORT_CALSS CAttribute
{
public: 
	CAttribute();

	~CAttribute();

	void Initialize(string strAttributeName,string strFieldName,string strTableName,string strDataType,FieldValue* pValue);

	void SetAttributeID(string strAttributeID)      {m_strAttributeName = strAttributeID;}
	void SetFieldID(string strFieldID)              {m_strFieldName = strFieldID;}
	void SetTableName(string strTableName)          {m_strTableName = strTableName;}
	void SetDataType(string strDataType)            {m_strDataType = strDataType;}
	void SetAttributeValue(FieldValue* pValue)      {m_pAttributeValue = pValue;}
	string GetAttributeID()   {return m_strAttributeName;}
	string GetAttributeValueString();
	float GetAttributeValueFloat();
	int GetAttributeValueInt();
	
public:
	string  m_strAttributeName;
	string  m_strFieldName;
	string  m_strTableName;
	string  m_strDataType;
	FieldValue* m_pAttributeValue;
};