#pragma once
#include <string>
#include <vector>
using namespace std;
#include "Export.h"
#include "FieldMeta.h"
#include "DataSet.h"
#include "SQLiteDatabase.h"

typedef  map<string,CFieldDesc*>  TableMeta;

class EXPORT_CALSS CDBDataManager
{
protected:
	CDBDataManager();

public:
	static CDBDataManager& Instance();

	bool OpenDatabase(const CString& sDatabaseFile);
	void Close();

	//�����ݿ�,������Ԫ����
	bool InitializeDatabase(const char* sDataBase);


	//����"��-->�ֶ�"Ԫ����
	void LoadFieldMetaData(const string &sTableName);

	//����"�ֶ�-->����"����
	void LoadFieldAttribute(const string &sTableName);

	//�����ݿ��ж�ȡ����
	//strDataSource, SQL ���
	//ds,�����������������
	//mapFieldName2FieldDesc
	int LoadData(const string& strDataSource,const string& strTable, CDataSet& ds);

	//�ж��Ƿ���������ض������ļ�¼
	//strTableName       ����
	//strFieldName       �ֶ���
	//strFieldDataType   �ֶ���������
	//FieldValue& v      �ֶ�ֵ
	bool RecordExists(const string& strTableName,const string& strFieldName, const string& strFieldDataType, FieldValue& v);

	bool RecordExists(const string& strTableName,map<CFieldDesc*,FieldValue>& mapFieldDesc2Value);

	bool InsertRecordWithPrimaryKey(const string& strTableName,const string& strFieldName, const string& strFieldDataType, FieldValue& v);

	//ִ���ض�SQL
	bool Exec(const string strSQL);

	//���ݱ�����ñ���ֶ�Ԫ���ݼ���
	map<string,CFieldDesc*>& GetTableMeta(const string& strTableName);

	void GetMetaFilter(const string& strTableName,vector<string>& vField,map<string,CFieldDesc*>& mapFieldID2Desc);

	//����"��-->�ֶ�"Ԫ����
	void GetFieldMetaData(const string &sTableName,vector<CFieldDesc*>& vFieldDesc);

	//��ȡĳ���������
	void GetPrimaryKey(const string &sTableName,vector<CFieldDesc*>& vPrimaryKeyField);

protected:
	bool _ExecExistsSQL( string &sSQL );

private:
	CSQLiteDatabase m_db;
	//����--->��Ԫ����
	map<string,TableMeta>   m_mapTable2Meta;
	//�����������˳��������ֶ�
	//map<string,CFieldDesc*> m_mapFieldName2FieldDescFilter;
};