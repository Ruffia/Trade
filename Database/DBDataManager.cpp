#include "stdafx.h"
#include "DBDataManager.h"


CDBDataManager::CDBDataManager()
{
	m_mapTable2Meta.clear();
}

CDBDataManager& CDBDataManager::Instance()
{
	static CDBDataManager Inst;
	return Inst;
}

bool CDBDataManager::OpenDatabase(const CString& sDatabaseFile)
{
	CString sErr = "";
	bool bRet = m_db.Connect(sDatabaseFile,"","",sErr);
	return bRet;
}


void CDBDataManager::Close()
{
	for (map<string,TableMeta>::iterator it = m_mapTable2Meta.begin();
		it != m_mapTable2Meta.end();it++)
	{
		map<string,CFieldDesc*>& mapFieldName2FieldDesc = it->second;
		for (map<string,CFieldDesc*>::iterator it = mapFieldName2FieldDesc.begin();
			it != mapFieldName2FieldDesc.end();it++)
		{
			CFieldDesc* pFieldDesc = it->second;
			if(pFieldDesc)
			{
				delete pFieldDesc;
				pFieldDesc = NULL;
			}
		}

		mapFieldName2FieldDesc.clear();
	}


	m_db.Disconnect();
}


map<string,CFieldDesc*>& CDBDataManager::GetTableMeta(const string& strTableName)
{
	return m_mapTable2Meta[strTableName];
}


void CDBDataManager::GetMetaFilter(const string& strTableName,vector<string>& vField,map<string,CFieldDesc*>& mapFieldID2Desc)
{
	map<string,CFieldDesc*>& TableMeta = m_mapTable2Meta[strTableName];
	if (0 == vField.size())
	{
		mapFieldID2Desc = TableMeta;
	}
	else
	{
		for (int i = 0; i < vField.size(); i++)
		{
			const string& sFieldID = vField[i];	
			map<string,CFieldDesc*>::iterator it = TableMeta.find(sFieldID);
			if(TableMeta.end() == it ) continue;
			mapFieldID2Desc[it->first] = it->second;
		}
	}
}


int CDBDataManager::LoadData(const string& strSQL,const string& strTableName, CDataSet& ds)
{
	CString sError = "";
	CAbstractRecordset *pRecordset = NULL;
	BOOL bQuery = m_db.Query(strSQL.c_str(),sError,&pRecordset);
	if (!bQuery) {
		return -1;
	}

	ds.Clear();

	map<string,CFieldDesc*>& mapFieldName2FieldDesc = m_mapTable2Meta[strTableName];
	int iCount = 0;
	while (pRecordset->Next())
	{
		CRecord* pRecord = new CRecord;
		const int nColumnCount = pRecordset->GetFieldCount();
	
		for (int i = 0; i < nColumnCount; i++)
		{	
			CFieldDesc* pFieldDesc = NULL;
			string sFieldName = pRecordset->GetFieldName(i);
			map<string,CFieldDesc*>::iterator it =  mapFieldName2FieldDesc.find(sFieldName);
			if (it == mapFieldName2FieldDesc.end())
			{
				continue;
			}

			pFieldDesc = it->second;
			if(!pFieldDesc) continue;	

			CField* pField = new CField;
			pField->SetFieldDesc(pFieldDesc);

			string sDataType = pFieldDesc->m_strDataType;
			if("" == sDataType)
			{
				delete pField;
				pField = NULL;
			}

			FieldValue* pValue = new FieldValue;
			pValue->SetDataType(sDataType);

			if (sDataType.find("string") != string::npos)
			{
				string sValue = pRecordset->AsString(i);
				pValue->SetValueString(sValue.c_str());
			}
			else if (sDataType.find("int") != string::npos)
			{
				int nValue = pRecordset->AsInteger(i);
				pValue->SetValueInt(nValue);
			}
			else if (sDataType.find("double") != string::npos)
			{
				double dValue = pRecordset->AsFloat(i);
				pValue->SetValueDouble(dValue);
			}
			else if (sDataType.find("float") != string::npos)
			{
				double dValue = pRecordset->AsFloat(i);
				pValue->SetValueFloat(dValue);
			}
			else if (sDataType.find("long") != string::npos)
			{
				__int64 nValue = pRecordset->AsLong(i);
				pValue->SetValueLongLong(nValue);
			}

			pField->SetFieldValue(pValue);
			pRecord->Add(sFieldName,pField);
		}

		ds.Add(iCount,pRecord);
		iCount++;
	}
	
	return iCount;
}


bool CDBDataManager::RecordExists(const string& strTableName,const string& strFieldName, const string& strFieldDataType, FieldValue& vField)
{
	string sSQL = "select count(*) from ";
	sSQL += strTableName;
	sSQL += " where ";
	sSQL += strFieldName;
	sSQL += " = ";
	char szValue[128] = {0};
	if (strFieldDataType == "string")
	{
		sprintf_s(szValue,128,"'%s'",vField.GetValueAsString());
	}
	else if (strFieldDataType == "int" || strFieldDataType == "integer" )
	{
		sprintf_s(szValue,128,"%d",vField.GetValueAsInt());
	}
	else if(strFieldDataType == "long")
	{
		sprintf_s(szValue,128,"%ld",vField.GetValueAsLong());
	}

	sSQL += szValue;

	bool bRet = _ExecExistsSQL(sSQL);
	return bRet;
}


bool CDBDataManager::RecordExists(const string& strTableName,map<CFieldDesc*,FieldValue>& mapFieldDesc2Value)
{
	string sSQL = "select count(*) from ";
	sSQL += strTableName;
	sSQL += " where ";

	const int nFieldCount = mapFieldDesc2Value.size();
	int nIndex = 0;
	for (map<CFieldDesc*,FieldValue>::iterator it = mapFieldDesc2Value.begin();
		 it != mapFieldDesc2Value.end(); it++)
	{
		CFieldDesc* pFieldDesc = it->first;
		if(!pFieldDesc) continue;
		FieldValue& vField = it->second; 
		string strFieldDataType = pFieldDesc->m_strDataType;

		char szValue[128] = {0};
		if (strFieldDataType == "string")
		{
			sprintf_s(szValue,128," %s = '%s' ",pFieldDesc->m_strFieldName.c_str(), vField.GetValueAsString());
		}
		else if (strFieldDataType == "int" || strFieldDataType == "integer" )
		{
			sprintf_s(szValue,128," %s = %d ",pFieldDesc->m_strFieldName.c_str(), vField.GetValueAsInt());
		}
		else if(strFieldDataType == "long")
		{
			sprintf_s(szValue,128," %s = %ld ",pFieldDesc->m_strFieldName.c_str(), vField.GetValueAsLong());
		}

		sSQL += szValue;

		if (nFieldCount -1 != nIndex)
		{
			sSQL += " and ";
		}
	}

	bool bRet = _ExecExistsSQL(sSQL);
	return bRet;
}


bool CDBDataManager::InsertRecordWithPrimaryKey(const string& strTableName,const string& strFieldName, const string& strFieldDataType, FieldValue& vField)
{
	string sSQL = "insert into ";
	sSQL += strTableName;
	sSQL += "(";
	sSQL += strFieldName;
	sSQL += ") values(";

	char szValue[128] = {0};
	if (strFieldDataType == "string")
	{
		sprintf_s(szValue,128,"'%s'",vField.GetValueAsString());
	}
	else if (strFieldDataType == "int" || strFieldDataType == "integer" )
	{
		sprintf_s(szValue,128,"%d",vField.GetValueAsInt());
	}
	else if(strFieldDataType == "long")
	{
		sprintf_s(szValue,128,"%ld",vField.GetValueAsLong());
	}

	sSQL += szValue;
	sSQL += ")";

	CString strErr;
	bool bRet = m_db.Query(sSQL.c_str(),strErr);
	return bRet;
}


bool CDBDataManager::Exec(const string strSQL)
{
	CString strErr;
	bool bRet = m_db.Query(strSQL.c_str(),strErr);
	return bRet;
}

void CDBDataManager::LoadFieldMetaData(const string &sTableName)
{
	string sFieldMetaSQL = "SELECT  *  from FieldMeta where TableName = '" + sTableName + "'";
	CString sError = "";
	CAbstractRecordset *pRecordset = NULL;
	BOOL bQuery = m_db.Query(sFieldMetaSQL.c_str(),sError,&pRecordset);
	if (!bQuery) {
		return;
	}

	map<string,CFieldDesc*>& mapFieldName2FieldDesc = m_mapTable2Meta[sTableName];
	while (pRecordset->Next())
	{	
		CFieldDesc* pFieldDesc = new CFieldDesc;
		const int nFieldColumnCount = pRecordset->GetFieldCount();
		for (int i = 0; i < nFieldColumnCount; i++)
		{	
			string sFieldName = pRecordset->GetFieldName(i);	
			if(sFieldName.find("FieldName") != string::npos )
			{
				string sValue = pRecordset->AsString(i);
				pFieldDesc->SetFieldName(sValue);
			}
			else if (sFieldName.find("TableName") != string::npos )
			{
				string sValue = pRecordset->AsString(i);
				pFieldDesc->SetTableName(sValue);
			}
			else if (sFieldName.find("DataType") != string::npos )
			{
				string sValue = pRecordset->AsString(i);
				pFieldDesc->SetDataType(sValue);
			}
			else if (sFieldName.find("PrimaryKey") != string::npos )
			{
				int nValue = pRecordset->AsInteger(i);
				pFieldDesc->m_bPrimaryKey = nValue == 1;
			}
			else if (sFieldName.find("ShowOnUI") != string::npos )
			{
				int nValue = pRecordset->AsInteger(i);
				pFieldDesc->m_bShowOnUI = nValue == 1;
			}
			else if(sFieldName.find("SequenceID") != string::npos )
			{
				int nValue = pRecordset->AsInteger(i);
				pFieldDesc->SetSequenceID(nValue);
			}
		}

		const string strFieldName = pFieldDesc->m_strFieldName;
		mapFieldName2FieldDesc[strFieldName] = pFieldDesc;
	}

	if (pRecordset)
	{
		pRecordset->Close();
		delete pRecordset;
		pRecordset = NULL;
	}
	
}

void CDBDataManager::GetFieldMetaData(const string &sTableName,vector<CFieldDesc*>& vFieldDesc)
{
	map<string,CFieldDesc*>& mapFieldName2FieldDesc = m_mapTable2Meta[sTableName];
	for (map<string,CFieldDesc*>::iterator it = mapFieldName2FieldDesc.begin();
		it != mapFieldName2FieldDesc.begin(); it++)
	{
		CFieldDesc* pFieldDesc = it->second;
		vFieldDesc.push_back(pFieldDesc);
	}
}


void CDBDataManager::GetPrimaryKey(const string &sTableName,vector<CFieldDesc*>& vPrimaryKeyField)
{
	map<string,CFieldDesc*>& mapFieldName2FieldDesc = m_mapTable2Meta[sTableName];
	for (map<string,CFieldDesc*>::iterator it = mapFieldName2FieldDesc.begin();
		it != mapFieldName2FieldDesc.end(); it++)
	{
		CFieldDesc* pFieldDesc = it->second;
		if (pFieldDesc->m_bPrimaryKey)
		{
			vPrimaryKeyField.push_back(pFieldDesc);
		}
	}
}

void CDBDataManager::LoadFieldAttribute(const string &sTableName)
{
	string sFieldAttributeSQL = "SELECT * from FieldAttribute where TableName = '" + sTableName + "';";
	
	CString sError = "";
	CAbstractRecordset *pRecordset = NULL;
	BOOL bQuery = m_db.Query(sFieldAttributeSQL.c_str(),sError,&pRecordset);
	if (!bQuery) {
		return;
	}

	map<string,CFieldDesc*>& mapFieldName2FieldDesc = m_mapTable2Meta[sTableName];

	while (pRecordset->Next())
	{	
		CFieldDesc* pFieldDesc = NULL;

		string sFieldAttributeName = "";
		string sFieldName = "";
		string sTableName = ""; 
		string sDataType = ""; 
		string sAttributeValue = "";
		FieldValue* pAttributeValue = new FieldValue;

		const int nFieldColumnCount = pRecordset->GetFieldCount();
		for (int i = 0; i < nFieldColumnCount; i++)
		{	
			string sFieldCaption = pRecordset->GetFieldName(i);
			
			if (sFieldCaption.find("FieldName") != string::npos )
			{
				sFieldName = pRecordset->AsString(i);
				pFieldDesc = mapFieldName2FieldDesc[sFieldName];	
			}
			else if(sFieldCaption.find("FieldAttributeName") != string::npos )
			{
				sFieldAttributeName = pRecordset->AsString(i);
			}
			else if (sFieldCaption.find("TableName") != string::npos)
			{
				sTableName = pRecordset->AsString(i);
			}
			else if (sFieldCaption.find("DataType") != string::npos)
			{
				sDataType = pRecordset->AsString(i);
			}
			else if (sFieldCaption.find("AttributeValue") != string::npos)
			{
				sAttributeValue = pRecordset->AsString(i);
			}
		}

		if (!pFieldDesc)
		{
			continue;
		}

		if (sDataType.find("string") != string::npos )
		{
			pAttributeValue->SetDataType("string");
			pAttributeValue->SetValueString(sAttributeValue.c_str());
		}
		else if (sDataType.find("int") != string::npos )
		{
			pAttributeValue->SetDataType("int");
			int nValue = atoi(sAttributeValue.c_str());
			pAttributeValue->SetValueInt(nValue);
		}

		CAttribute* pAttribute = new CAttribute;
		pAttribute->Initialize(sFieldAttributeName,sFieldName,sTableName,sDataType,pAttributeValue);
		pFieldDesc->AddAttribute(sFieldAttributeName,pAttribute);
	}

	if (pRecordset)
	{
		pRecordset->Close();
		delete pRecordset;
		pRecordset = NULL;
	}
}



bool CDBDataManager::InitializeDatabase(const char* sDataBase)
{
	char exeFullPath[255] = {0};
	GetModuleFileNameA(NULL,exeFullPath,MAX_PATH);
	char path[255] = {0};
	sprintf(path, "%s", exeFullPath);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';

	sprintf_s(path,255,"%s\\%s",path,sDataBase);

	//打开数据库
	bool bRet = OpenDatabase(path);
	if(!bRet) return false;
	
	string strSQL = "select * from Business";

	CString sError = "";
	CAbstractRecordset *pRecordset = NULL;
	BOOL bQuery = m_db.Query(strSQL.c_str(),sError,&pRecordset);
	if (!bQuery) {
		return false;
	}

	vector<string> vBusiness;
	while (pRecordset->Next())
	{
		const int nColumnCount = pRecordset->GetFieldCount();
		const int nTableNameColumn = 0;  //结果集的第0列保存的是表的名称
		string sFieldName = pRecordset->GetFieldName(nTableNameColumn);
		string sValue = pRecordset->AsString(nTableNameColumn);
		vBusiness.push_back(sValue);
	}

	if (pRecordset)
	{
		pRecordset->Close();
		delete pRecordset;
		pRecordset = NULL;
	}

	for (int i = 0; i < vBusiness.size(); i++)
	{
		const string& strTableName = vBusiness[i];
		LoadFieldMetaData(strTableName);
		LoadFieldAttribute(strTableName);
	}

	return true;
}


bool CDBDataManager::_ExecExistsSQL( string &sSQL )
{
	int nRet = -1;
	sqlite3_stmt* pStmt;
	nRet = sqlite3_prepare_v2(m_db.m_pDatabase, sSQL.c_str(), sSQL.length(), &pStmt, NULL);
	if (nRet != SQLITE_OK)
	{
		return false;
	}

	char** result = NULL;
	char* sMsg = NULL;
	int nRowCount = 0;
	int nColumnCount = 0;
	nRet = sqlite3_get_table( m_db.m_pDatabase, sSQL.c_str(), &result, &nRowCount, &nColumnCount, &sMsg); //查询数据库
	if( nRet != SQLITE_OK )
	{
		return false;
	}

	//对于 select count(*) 语句而言，执行SQL语句返回的结果是
	//第一行   count(*)   列标题
	//第二行    0
	//所以此时 nRowCount == 1，且 nColumnCount == 1
	if(nRowCount != 1 || nColumnCount != 1) 
	{
		return false;
	}

	char* sValue = result[1];   //获取第二行的字符串值
	int nValue = atoi(sValue);  //将字符串值转换为数字值

	sqlite3_free_table(result);  //释放掉 result 的内存空间
	sqlite3_finalize(pStmt);     //销毁一个SQL语句对象

	return nValue > 0;
}
