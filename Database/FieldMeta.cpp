#include "stdafx.h"
#include "FieldMeta.h"

IMPLEMENT_FACTORY(CFieldDesc,CFieldDesc,string,"CFieldDesc")
CFieldDesc::CFieldDesc()
{
	m_mapName2Attribute.clear();
}


CFieldDesc::~CFieldDesc()
{
	for (map<string,CAttribute*>::iterator it = m_mapName2Attribute.begin();
		it != m_mapName2Attribute.end();it++)
	{
		CAttribute* pAttribute = it->second;
		if (pAttribute)
		{
			delete pAttribute;
			pAttribute = NULL;
		}
	}
}


void CFieldDesc::Initialize(string strFieldID,string strFieldName,string strTableName,string strDataType)
{
	m_strFieldID = strFieldID;
	m_strFieldName = strFieldName;
	m_strTableName = strTableName;
	m_strDataType = strDataType;
}


CAttribute* CFieldDesc::GetAttribute(const string strAttributeName)
{
	map<string,CAttribute*>::iterator it = m_mapName2Attribute.find(strAttributeName);
	if(it == m_mapName2Attribute.end()) return NULL;

	return it->second;
}


string CFieldDesc::GetAttributeString(const string strAttributeName)
{
	map<string,CAttribute*>::iterator it = m_mapName2Attribute.find(strAttributeName);
	if(it == m_mapName2Attribute.end()) return "";

	CAttribute* pAttribute = it->second;
	if(!pAttribute) return "";

	FieldValue* pValue = pAttribute->m_pAttributeValue;
	if(!pValue) return "";

	return pValue->GetValueAsString();
}


int CFieldDesc::GetAttributeInt(const string strAttributeName)
{
	map<string,CAttribute*>::iterator it = m_mapName2Attribute.find(strAttributeName);
	if(it == m_mapName2Attribute.end()) return -1;

	CAttribute* pAttribute = it->second;
	if(!pAttribute) return -1;

	FieldValue* pValue = pAttribute->m_pAttributeValue;
	if(!pValue) return -1;

	return pValue->GetValueAsInt();
}


bool CompareBySequenceID(pair<string,CFieldDesc*>& l,pair<string,CFieldDesc*>& r) 
{  
	return l.second->m_nSequenceID < r.second->m_nSequenceID;  
}


bool CompareFieldBySequenceID(CFieldDesc* l,CFieldDesc* r)
{
	return l->m_nSequenceID < r->m_nSequenceID;  
}