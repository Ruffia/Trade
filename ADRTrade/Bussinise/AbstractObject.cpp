#include "stdafx.h"
#include "AbstractObject.h"

IAbstractObject::IAbstractObject()
{
	m_mapName2Attribute.clear();
}

IAbstractObject::~IAbstractObject()
{
	m_mapName2Attribute.clear();
}

void IAbstractObject::AddAttribute(const string& strAttributeID,shared_ptr<CAttribute>& ptrAttribute)
{
	m_mapName2Attribute[strAttributeID] = ptrAttribute;
}

shared_ptr<CAttribute> IAbstractObject::GetAttribute(const string& strAttributeID)
{
	map<string,shared_ptr<CAttribute>>::iterator it = m_mapName2Attribute.find(strAttributeID);
	if (it == m_mapName2Attribute.end())
	{
		return shared_ptr<CAttribute>(new CAttribute);
	}

	return it->second;
}


