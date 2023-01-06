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

	void Initialize(string strFieldName,string strTableName,string strDataType);

	void SetFieldName(string strFieldName)  {m_strFieldName = strFieldName;}
	void SetDataType(string strDataType)    {m_strDataType = strDataType;}
	void SetSequenceID(int nSequenceID)     {m_nSequenceID = nSequenceID;}
	void SetTableName(string strTableName)  {m_strTableName = strTableName;}
	
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
	string  m_strFieldName;
	string  m_strDataType;
	string  m_strTableName;
	int     m_nSequenceID;
	bool    m_bPrimaryKey;
	bool    m_bShowOnUI;

	//һ�� Field ���ܰ����ܶ���Ϣ��������һ��"������-->����ֵ"�ļ�������ʾ
	map<string,CAttribute*> m_mapName2Attribute;
};


bool AFX_EXT_API CompareBySequenceID(pair<string,CFieldDesc*>& l,pair<string,CFieldDesc*>& r);

bool AFX_EXT_API CompareFieldBySequenceID(CFieldDesc* l,CFieldDesc* r);