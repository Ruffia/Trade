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

	//打开数据库,并加载元数据
	bool InitializeDatabase(const char* sDataBase);


	//加载"表-->字段"元数据
	void LoadFieldMetaData(const string &sTableName);

	//加载"字段-->属性"数据
	void LoadFieldAttribute(const string &sTableName);

	//从数据库中读取数据
	//strDataSource, SQL 语句
	//ds,输出参数，保存数据
	//mapFieldName2FieldDesc
	int LoadData(const string& strDataSource,const string& strTable, CDataSet& ds);

	//判断是否存在满足特定条件的记录
	//strTableName       表名
	//strFieldName       字段名
	//strFieldDataType   字段数据类型
	//FieldValue& v      字段值
	bool RecordExists(const string& strTableName,const string& strFieldName, const string& strFieldDataType, FieldValue& v);

	bool RecordExists(const string& strTableName,map<CFieldDesc*,FieldValue>& mapFieldDesc2Value);

	bool InsertRecordWithPrimaryKey(const string& strTableName,const string& strFieldName, const string& strFieldDataType, FieldValue& v);

	//执行特定SQL
	bool Exec(const string strSQL);

	//根据表名获得表的字段元数据集合
	map<string,CFieldDesc*>& GetTableMeta(const string& strTableName);

	void GetMetaFilter(const string& strTableName,vector<string>& vField,map<string,CFieldDesc*>& mapFieldID2Desc);

	//过滤"表-->字段"元数据
	void GetFieldMetaData(const string &sTableName,vector<CFieldDesc*>& vFieldDesc);

	//获取某个表的主键
	void GetPrimaryKey(const string &sTableName,vector<CFieldDesc*>& vPrimaryKeyField);

protected:
	bool _ExecExistsSQL( string &sSQL );

private:
	CSQLiteDatabase m_db;
	//表名--->表元数据
	map<string,TableMeta>   m_mapTable2Meta;
	//根据条件过滤出来部分字段
	//map<string,CFieldDesc*> m_mapFieldName2FieldDescFilter;
};