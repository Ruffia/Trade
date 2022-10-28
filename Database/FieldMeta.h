#pragma once
#include <string>
using namespace std;
#include <map>
#include "FieldValue.h"
#include "Attribute.h"
 
class EXPORT_CALSS CFieldDesc
{
public: 
	CFieldDesc();

	~CFieldDesc();

	void Initialize(string strFieldID,string strFieldName,string strTableName,string strDataType);

	void SetFieldID(string strFieldID)      {m_strFieldID = strFieldID;}
	void SetFieldName(string strFieldName)  {m_strFieldName = strFieldName;}
	void SetDataType(string strDataType)    {m_strDataType = strDataType;}
	void SetSequenceID(int nSequenceID)     {m_nSequenceID = nSequenceID;}
	void SetTableName(string strTableName)  {m_strTableName = strTableName;}
	string GetFieldID() {return m_strFieldID;}
	
	//增加属性到属性集合中
	//strAttributeName,属性名
	//pValue, 属性值(包括数据类型和值)
	void AddAttribute(const string strAttributeName,CAttribute* pAttribute)
	{
		m_mapName2Attribute[strAttributeName] = pAttribute;
	}

	//根据属性名获取属性值
	CAttribute* GetAttribute(const string strAttributeName);

	//获取字符串属性值
	string GetAttributeString(const string strAttributeName);

	//获取整数属性值
	int GetAttributeInt(const string strAttributeName);

	map<string,CAttribute*>& GetAttribute()
	{
		return m_mapName2Attribute;
	}


public:
	string  m_strFieldID;
	string  m_strFieldName;
	string  m_strDataType;
	string  m_strTableName;
	int     m_nSequenceID;

	//一个 Field 可能包含很多信息，这里由一个"属性名-->属性值"的集合来表示
	map<string,CAttribute*> m_mapName2Attribute;
};


bool AFX_EXT_API CompareBySequenceID(pair<string,CFieldDesc*>& l,pair<string,CFieldDesc*>& r);

bool AFX_EXT_API CompareFieldBySequenceID(CFieldDesc* l,CFieldDesc* r);