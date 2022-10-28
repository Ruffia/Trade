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

	//打开日志数据库,并加载元数据
	bool InitializeLogDatabase();

	//初始化电源类型,CPS3603 or Spellman771
	void SetHVType(DWORD dwHVtype);

	//加载"表-->字段"元数据
	void LoadFieldMetaData(const string &sTableName);

	//加载"字段-->属性"数据
	void LoadFieldAttribute(const string &sTableName);

	//从数据库中读取数据
	//strDataSource, SQL 语句
	//ds,输出参数，保存数据
	//mapFieldName2FieldDesc
	int LoadData(const string& strDataSource,const string& strTable, CDataSet& ds);

	//执行特定SQL
	bool Exec(const string strSQL);

	//根据表名获得表的字段元数据集合
	map<string,CFieldDesc*>& GetTableMeta(const string& strTableName);

	void GetMetaFilter(const string& strTableName,vector<string>& vField,map<string,CFieldDesc*>& mapFieldID2Desc);

	//过滤"表-->字段"元数据
	void GetFieldMetaData(const string &sTableName,vector<string>& vFieldID);

	//返回主数据表名称 CPS3603，Spellman771
	string GetLogTableName()
	{
		return m_strTableName;
	}

private:
	CSQLiteDatabase m_db;
	//表名--->表元数据
	map<string,TableMeta>   m_mapTable2Meta;
	//根据条件过滤出来部分字段
	map<string,CFieldDesc*> m_mapFieldName2FieldDescFilter;
	DWORD     m_dwHVtype;       //电源类型,0 -- CPS3603, 1 -- Spellman771
	string    m_strTableName;   //主数据表名称 CPS3603，Spellman771
};