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
	
	//�������Ե����Լ�����
	//strAttributeName,������
	//pValue, ����ֵ(�����������ͺ�ֵ)
	void AddAttribute(const string strAttributeName,CAttribute* pAttribute)
	{
		m_mapName2Attribute[strAttributeName] = pAttribute;
	}

	//������������ȡ����ֵ
	CAttribute* GetAttribute(const string strAttributeName);

	//��ȡ�ַ�������ֵ
	string GetAttributeString(const string strAttributeName);

	//��ȡ��������ֵ
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

	//һ�� Field ���ܰ����ܶ���Ϣ��������һ��"������-->����ֵ"�ļ�������ʾ
	map<string,CAttribute*> m_mapName2Attribute;
};


bool AFX_EXT_API CompareBySequenceID(pair<string,CFieldDesc*>& l,pair<string,CFieldDesc*>& r);

bool AFX_EXT_API CompareFieldBySequenceID(CFieldDesc* l,CFieldDesc* r);