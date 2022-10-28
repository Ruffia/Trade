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

	void Initialize(string strAttributeID,string strFieldID,string strTableName,string strDataType,FieldValue* pValue);

	void SetAttributeID(string strAttributeID)      {m_strAttributeID = strAttributeID;}
	void SetFieldID(string strFieldID)              {m_strFieldID = strFieldID;}
	void SetTableName(string strTableName)          {m_strTableName = strTableName;}
	void SetDataType(string strDataType)            {m_strDataType = strDataType;}
	void SetAttributeValue(FieldValue* pValue)      {m_pAttributeValue = pValue;}
	string GetAttributeID()   {return m_strAttributeID;}
	string GetAttributeValueString();
	float GetAttributeValueFloat();
	int GetAttributeValueInt();
	
public:
	string  m_strAttributeID;
	string  m_strFieldID;
	string  m_strTableName;
	string  m_strDataType;
	FieldValue* m_pAttributeValue;
};