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
	nRet = sqlite3_get_table( m_db.m_pDatabase, sSQL.c_str(), &result, &nRowCount, &nColumnCount, &sMsg); //��ѯ���ݿ�
	if( nRet != SQLITE_OK )
	{
		return false;
	}

	//���� select count(*) �����ԣ�ִ��SQL��䷵�صĽ����
	//��һ��   count(*)   �б���
	//�ڶ���    0
	//���Դ�ʱ nRowCount == 1���� nColumnCount == 1
	if(nRowCount != 1 || nColumnCount != 1) 
	{
		return false;
	}

	char* sValue = result[1];   //��ȡ�ڶ��е��ַ���ֵ
	int nValue = atoi(sValue);  //���ַ���ֵת��Ϊ����ֵ

	sqlite3_free_table(result);  //�ͷŵ� result ���ڴ�ռ�
	sqlite3_finalize(pStmt);     //����һ��SQL������

	return nValue > 0;
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
			if (sFieldName.find("FieldID") != string::npos )
			{
				string sValue = pRecordset->AsString(i);
				pFieldDesc->SetFieldID(sValue);
			}
			else if(sFieldName.find("FieldName") != string::npos )
			{
				string sValue = pRecordset->AsString(i);
				pFieldDesc->SetFieldName(sValue);
			}
			else if (sFieldName.find("DataType") != string::npos )
			{
				string sValue = pRecordset->AsString(i);
				pFieldDesc->SetDataType(sValue);
			}
			else if (sFieldName.find("TableName") != string::npos )
			{
				string sValue = pRecordset->AsString(i);
				pFieldDesc->SetTableName(sValue);
			}
			else if(sFieldName.find("SequenceID") != string::npos )
			{
				int nValue = pRecordset->AsInteger(i);
				pFieldDesc->SetSequenceID(nValue);
			}

		}

		const string strFieldID = pFieldDesc->GetFieldID();
		mapFieldName2FieldDesc[strFieldID] = pFieldDesc;
		m_mapFieldName2FieldDescFilter[strFieldID] = pFieldDesc;
	}

	if (pRecordset)
	{
		pRecordset->Close();
		delete pRecordset;
		pRecordset = NULL;
	}
	
}

void CDBDataManager::GetFieldMetaData(const string &sTableName,vector<string>& vFieldID)
{
	m_mapFieldName2FieldDescFilter.clear();
	map<string,CFieldDesc*>& mapFieldName2FieldDesc = m_mapTable2Meta[sTableName];
	for (int i = 0; i < vFieldID.size();i++)
	{
		map<string,CFieldDesc*>::iterator it = mapFieldName2FieldDesc.find(vFieldID[i]);
		if (it != mapFieldName2FieldDesc.end())
		{
			CFieldDesc* pFieldDesc = it->second;
			if (pFieldDesc && sTableName == pFieldDesc->m_strTableName )
			{
				m_mapFieldName2FieldDescFilter[vFieldID[i]] = it->second;
			}			
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

		string sFieldAttributeID = "";
		string sFieldID = "";
		string sTableName = ""; 
		string sDataType = ""; 
		string sAttributeValue = "";
		FieldValue* pAttributeValue = new FieldValue;

		const int nFieldColumnCount = pRecordset->GetFieldCount();
		for (int i = 0; i < nFieldColumnCount; i++)
		{	
			string sFieldCaption = pRecordset->GetFieldName(i);
			
			if (sFieldCaption.find("FieldID") != string::npos )
			{
				sFieldID = pRecordset->AsString(i);
				pFieldDesc = mapFieldName2FieldDesc[sFieldID];	
			}
			else if(sFieldCaption.find("FieldAttributeID") != string::npos )
			{
				sFieldAttributeID = pRecordset->AsString(i);
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
		pAttribute->Initialize(sFieldAttributeID,sFieldID,sTableName,sDataType,pAttributeValue);
		pFieldDesc->AddAttribute(sFieldAttributeID,pAttribute);
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

	//�����ݿ�
	bool bRet = OpenDatabase(path);
	if(!bRet) return false;

	//LoadFieldMetaData(sViewName);
	//LoadFieldAttribute(sViewName);
	LoadFieldMetaData("OperationLog");
	LoadFieldAttribute("OperationLog");
	LoadFieldMetaData("Settings");
	LoadFieldAttribute("Settings");
	LoadFieldMetaData("AlarmLog");
	LoadFieldAttribute("AlarmLog");

	return true;
}