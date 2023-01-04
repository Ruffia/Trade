#pragma once
#include <string>
#include <memory>
#include <map>
using namespace std;
#include "FieldMeta.h"
#include "Attribute.h"

class IBusinessNode
{
public:
	IBusinessNode();
	virtual ~IBusinessNode();

	void SetBusinessName(const string& sName)
	{
		m_strBusinessName = sName;
	}

	void LoadData();

	void AddAttribute(const string& strAttributeID,shared_ptr<CAttribute>& ptrAttribute);
	shared_ptr<CAttribute> GetAttribute(const string& strAttributeID);
	

protected:
	string m_strBusinessName;
	map<string,CFieldDesc*>  m_mapFieldName2FieldDesc;
};