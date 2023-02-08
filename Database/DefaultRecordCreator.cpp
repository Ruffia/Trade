#include "stdafx.h"
#include "DefaultRecordCreator.h"
#include "DBDataManager.h"
#include "../Common/Factory.h"
#include "PrimaryKeyRule.h"

IDefaultRecordCreator::IDefaultRecordCreator()
{
	m_strTableName = "";
}


IMPLEMENT_FACTORY(IDefaultRecordCreator,CDefaultRecordCreator_Common, string,"CDefaultRecordCreator_Common")
void CDefaultRecordCreator_Common::CreateRecord()
{
	vector<CFieldDesc*> vPrimaryKey;
	CDBDataManager::Instance().GetPrimaryKey(m_strTableName,vPrimaryKey);

	string sSQL = "insert into ";
	sSQL += m_strTableName;
	sSQL += "(";

	string sPrimaryKey = "";
	int nSize = vPrimaryKey.size();
	for (int i = 0;i < nSize;i++)
	{
		CFieldDesc* pFieldDesc = vPrimaryKey[i];
		if(!pFieldDesc) continue;
		IPrimaryKeyRule* pRule = Factory<IPrimaryKeyRule,string>::Instance().BuildProduct(pFieldDesc->m_strFieldName);
		if(!pRule) continue;
		pRule->SetFieldDesc(pFieldDesc);

		if (i != nSize -1)
		{
			sPrimaryKey += pRule->GetInsertSQL();
			sPrimaryKey += ",";
			sSQL += pFieldDesc->m_strFieldName;
			sSQL += ",";
		}
		else
		{
			sPrimaryKey += pRule->GetInsertSQL();
			sSQL += pFieldDesc->m_strFieldName;
		}

		delete pRule;
		pRule = NULL;
	}

	sSQL += ") values(";
	sSQL += sPrimaryKey;
	sSQL += ")";

	CString strErr;
	bool bRet = CDBDataManager::Instance().GetDB().Query(sSQL.c_str(),strErr);
}


IMPLEMENT_FACTORY(IDefaultRecordCreator,CDefaultRecordCreator_MinorCycleAnalyze, string,"Future_DailyMinorCycleAnalyze")
void CDefaultRecordCreator_MinorCycleAnalyze::CreateRecord()
{
	const int nRecordCount = 5;
	string sRecordTimeKey[nRecordCount] = {"9:00","10:00","11:15","14:15","16:15"};

	vector<CFieldDesc*> vPrimaryKey;
	CDBDataManager::Instance().GetPrimaryKey(m_strTableName,vPrimaryKey);

	for (int i = 0; i < nRecordCount;i++)
	{
		string sSQL = "insert into ";
		sSQL += m_strTableName;
		sSQL += "(";

		string sPrimaryKey = "";
		int nSize = vPrimaryKey.size();
		for (int i = 0;i < nSize;i++)
		{
			CFieldDesc* pFieldDesc = vPrimaryKey[i];
			if(!pFieldDesc) continue;
			IPrimaryKeyRule* pRule = Factory<IPrimaryKeyRule,string>::Instance().BuildProduct(pFieldDesc->m_strFieldName);
			if(!pRule) continue;
			pRule->SetFieldDesc(pFieldDesc);

			const type_info &typeInfo = typeid(*pRule);
			string sTypeName = typeInfo.raw_name();
			if (i != nSize -1)
			{
				if (string::npos != sTypeName.find("CPrimaryKey_RecordTime"))
				{
					sPrimaryKey += "'";
					sPrimaryKey += sRecordTimeKey[i];
					sPrimaryKey += "'";
				}
				else
				{
					sPrimaryKey += pRule->GetInsertSQL();
				}			
				sPrimaryKey += ",";
				sSQL += pFieldDesc->m_strFieldName;
				sSQL += ",";
			}
			else
			{
				if (string::npos != sTypeName.find("CPrimaryKey_RecordTime"))
				{
					sPrimaryKey += "'";
					sPrimaryKey += sRecordTimeKey[i];
					sPrimaryKey += "'";
				}
				else
				{
					sPrimaryKey += pRule->GetInsertSQL();
				}	
				sSQL += pFieldDesc->m_strFieldName;
			}

			delete pRule;
			pRule = NULL;
		}

		sSQL += ") values(";
		sSQL += sPrimaryKey;
		sSQL += ")";

		CString strErr;
		bool bRet = CDBDataManager::Instance().GetDB().Query(sSQL.c_str(),strErr);
	}
}
