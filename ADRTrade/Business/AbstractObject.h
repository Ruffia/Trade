#pragma once
#include <string>
#include <memory>
#include <map>
using namespace std;
#include "Attribute.h"

class IAbstractObject
{
public:
	IAbstractObject();
	virtual ~IAbstractObject();

	void SetName(const string& sName)
	{
		m_strBusinessName = sName;
	}

	void AddAttribute(const string& strAttributeID,shared_ptr<CAttribute>& ptrAttribute);
	shared_ptr<CAttribute> GetAttribute(const string& strAttributeID);

protected:
	string m_strBusinessName;
	map<string,shared_ptr<CAttribute>>  m_mapName2Attribute;
};