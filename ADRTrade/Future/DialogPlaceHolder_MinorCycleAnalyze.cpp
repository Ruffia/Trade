#include "stdafx.h"
#include "DialogPlaceHolder_MinorCycleAnalyze.h"
#include "../Common/Factory.h"
#include "DBDataManager.h"
#include "Util.h"
#include "Tools/CollectiveComponentProvider.h"
#include "Tools/BusinessEdit.h"
#include "Tools/BusinessComboBox.h"
#include "Tools/EditTreeCtrlEx.h"
#include "PrimaryKeyRule.h"
#include "TradeDayPrimaryData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CDialogTabItem_MinorCycleAnalyze, CDialogPlaceHolder)
CDialogTabItem_MinorCycleAnalyze::CDialogTabItem_MinorCycleAnalyze(CWnd* pParent /*=NULL*/)
	: CDialogPlaceHolder(pParent)
{
	m_strRecordTime = "";
}

void CDialogTabItem_MinorCycleAnalyze::_LoadTradeDayData2UI()
{
	bool bExists = _CheckExistsTradeDayRecord();
	if (!bExists) return;

	vector<CFieldDesc*> vFieldDesc;
	CDBDataManager::Instance().GetFieldMetaData(m_sBusiness,vFieldDesc);

	vector<CFieldDesc*> vPrimaryKey;
	CDBDataManager::Instance().GetPrimaryKey(m_sBusiness,vPrimaryKey);

	const string strTradeDay = "TradeDay";
	const string strRecordTime = "RecordTime";
	CFieldDesc* pTradeDayDesc = NULL;
	CFieldDesc* pRecordTimeDesc = NULL;
	for (int i = 0;i < vPrimaryKey.size();i++)
	{
		CFieldDesc* pFieldDesc = vPrimaryKey[i];
		if (pFieldDesc && pFieldDesc->m_strFieldName.find(strTradeDay) != string::npos)
		{
			pTradeDayDesc = pFieldDesc;
		}
		else if (pFieldDesc && pFieldDesc->m_strFieldName.find(strRecordTime) != string::npos)
		{
			pRecordTimeDesc = pFieldDesc;
		}
	}

	if(!pTradeDayDesc || !pRecordTimeDesc) return;

	COleDateTime dtNOw = COleDateTime::GetCurrentTime();
	CString strDate = dtNOw.Format("%Y-%m-%d");

	FieldValue vKey;
	vKey.SetDataType("string");
	vKey.SetValueString(strDate);

	string sSQL = "select * from ";
	sSQL += m_sBusiness;
	sSQL += " where ";
	sSQL += pTradeDayDesc->m_strFieldName;
	sSQL += " = '";
	sSQL += vKey.GetValueAsString();
	sSQL += "'";
	sSQL += " and ";
	sSQL += pRecordTimeDesc->m_strFieldName;
	sSQL += " = '";
	sSQL += m_strRecordTime;
	sSQL += "'";

	CDataSet ds;
	CDBDataManager::Instance().LoadData(sSQL,m_sBusiness,ds);
	if(ds.Size() != 1) return;   //此处有且仅有一条记录

	CRecord* pRecord = ds[0];
	if(!pRecord) return;

	UpdateDB2UI(pRecord);
}


void CDialogTabItem_MinorCycleAnalyze::UpdateUI2DB()
{
	bool bExists = _CheckExistsTradeDayRecord();
	if(!bExists) return;

	map<string,CFieldDesc*>& mapTableName2FieldDesc = CDBDataManager::Instance().GetTableMeta(m_sBusiness);

	string sSQL = "update ";
	sSQL += m_sBusiness;
	sSQL += " set ";
	sSQL += " FutureContractName = '";
	sSQL += CTradeDayPrimaryData::Instance().m_strFutureContractName;
	sSQL += "'";
	sSQL += ",";

	int nUIControlCount = 0;
	for(map<string,CWnd*>::iterator it = m_mapBusiness2Control.begin();
		it != m_mapBusiness2Control.end();it++)
	{
		CWnd* pWnd = it->second;
		nUIControlCount++;
		bool bComBox = false;
		string sBusinessField = "";
		CBusinessComboBox* pComboBoxControl = NULL;
		CBusinessEdit* pBusinessControl = dynamic_cast<CBusinessEdit*>(it->second);
		if(!pBusinessControl)
		{
			pComboBoxControl = dynamic_cast<CBusinessComboBox*>(it->second);
			if(!pComboBoxControl) continue;

			sBusinessField = pComboBoxControl->m_sBusinessField;
			bComBox = true;
		}
		else
		{
			sBusinessField = pBusinessControl->m_sBusinessField;
		}

		map<string,CFieldDesc*>::iterator itFieldDesc = mapTableName2FieldDesc.find(sBusinessField);
		if(mapTableName2FieldDesc.end() == itFieldDesc) continue;

		CFieldDesc* pFieldDesc = itFieldDesc->second;
		if (!pFieldDesc) continue;

		char szValue[512] = {0}; 
		pWnd->GetWindowTextA(szValue,512);

		char szSQL[512] = {0}; 
		if (pFieldDesc->m_strDataType == "string")
		{
			sprintf_s(szSQL,512," %s = '%s' ",sBusinessField.c_str(),szValue);
		}
		else if (pFieldDesc->m_strDataType == "float" )
		{
			sprintf_s(szSQL,512," %s = %s ", sBusinessField.c_str(), strcmp(szValue,"")?szValue:"0");
		}
		else if( pFieldDesc->m_strDataType == "integer" || pFieldDesc->m_strDataType == "int")
		{
			if (bComBox)
			{
				int nValue = -1;
				bool bFindValue = pComboBoxControl->GetValue(szValue,nValue);
				sprintf_s(szSQL,512," %s = %d ", sBusinessField.c_str(), nValue);
			}
			else
			{
				sprintf_s(szSQL,512," %s = %s ", sBusinessField.c_str(), strcmp(szValue,"")?szValue:"0");
			}			
		}

		sSQL += szSQL;
		if (m_mapBusiness2Control.size() != nUIControlCount)
		{
			sSQL += ",";
		}
		else
		{
			sSQL += " ";
		}

	}

	sSQL += " where ";

	vector<CFieldDesc*> vPrimaryKey;
	CDBDataManager::Instance().GetPrimaryKey(m_sBusiness,vPrimaryKey);

	string strPrimaryKeyClause = "";
	for (int i = 0; i < vPrimaryKey.size();i++)
	{
		char sz[256] = {0};
		CFieldDesc* pFieldDesc = vPrimaryKey[i];
		if(!pFieldDesc) continue;

		if (pFieldDesc->m_strFieldName.find("TradeDay") != string::npos)
		{
			sprintf_s(sz,256,"%s = '%s'",pFieldDesc->m_strFieldName.c_str(),CTradeDayPrimaryData::Instance().m_strTradeDay.c_str());
		}
		else if (pFieldDesc->m_strFieldName.find("FutureContractName") != string::npos)
		{
			sprintf_s(sz,256,"%s = '%s'",pFieldDesc->m_strFieldName.c_str(),CTradeDayPrimaryData::Instance().m_strFutureContractName_LastTime.c_str());
		}
		else if (pFieldDesc->m_strFieldName.find("RecordTime") != string::npos)
		{
			sprintf_s(sz,256,"%s = '%s'",pFieldDesc->m_strFieldName.c_str(),m_strRecordTime.c_str());
		}

		if (vPrimaryKey.size() - 1 != i)
		{
			strPrimaryKeyClause += sz;
			strPrimaryKeyClause += " and ";
		}
		else
		{
			strPrimaryKeyClause += sz;
		}	
	}

	sSQL += strPrimaryKeyClause;
	CDBDataManager::Instance().Exec(sSQL);
}