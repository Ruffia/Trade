#include "stdafx.h"
#include "DialogPlaceHolder_MinorCycleAnalyze.h"
#include "../Common/Factory.h"
#include "DBDataManager.h"
#include "Util.h"
#include "Tools/CollectiveComponentProvider.h"
#include "Tools/BusinessEdit.h"
#include "Tools/BusinessComboBox.h"
#include "Tools/BusinessCheckBox.h"
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
	m_strRecordTime_Old = "";
	m_bNeed2UpdateRecordTime = false;
}

void CDialogTabItem_MinorCycleAnalyze::_LoadData2UI()
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

	string sSQL = "select * from ";
	sSQL += m_sBusiness;
	sSQL += " where ";
	sSQL += pTradeDayDesc->m_strFieldName;
	sSQL += " = '";
	sSQL += CTradeDayPrimaryData::Instance().m_strTradeDay;
	sSQL += "'";
	sSQL += " and ";
	sSQL += pRecordTimeDesc->m_strFieldName;
	sSQL += " = '";
	sSQL += m_strRecordTime;
	sSQL += "'";

	CDataSet ds;
	CDBDataManager::Instance().LoadData(sSQL,m_sBusiness,ds);

	UpdateDB2UI(ds);
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
	sSQL += " RecordTime = '";
	sSQL += m_strRecordTime;
	sSQL += "'";
	sSQL += ",";

	int nUIControlCount = 0;
	for(map<string,CWnd*>::iterator it = m_mapBusiness2Control.begin();
		it != m_mapBusiness2Control.end();it++)
	{
		CWnd* pWnd = it->second;
		nUIControlCount++;

		string sBusinessField = "";
		//截至目前，有3中Business 字段类型
		CBusinessEdit* pBusinessControl = NULL;
		CBusinessComboBox* pComboBoxControl = NULL;
		CBusinessCheckBox* pCheckBoxControl = NULL;

		const int nBussinessType = 3;
		bool ControlType[3] = {false,false,false}; 
		const type_info &typeInfo = typeid(*pWnd);
		string sTypeName = typeInfo.raw_name();
		if (string::npos != sTypeName.find("CBusinessEdit"))
		{
			ControlType[Business_Edit] = true;
			pBusinessControl = dynamic_cast<CBusinessEdit*>(pWnd);
			if(!pBusinessControl) continue;
			sBusinessField = pBusinessControl->m_sBusinessField;
		}
		else if (string::npos != sTypeName.find("CBusinessComboBox"))
		{
			ControlType[Business_ComboBox] = true;
			pComboBoxControl = dynamic_cast<CBusinessComboBox*>(pWnd);
			if(!pComboBoxControl) continue;
			sBusinessField = pComboBoxControl->m_sBusinessField;
		}
		else if (string::npos != sTypeName.find("CBusinessCheckBox"))
		{
			ControlType[Business_CheckBox] = true;
			pCheckBoxControl = dynamic_cast<CBusinessCheckBox*>(pWnd);
			if(!pCheckBoxControl) continue;
			sBusinessField = pCheckBoxControl->m_sBusinessField;
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
			if (ControlType[Business_ComboBox])
			{
				int nValue = -1;
				bool bFindValue = pComboBoxControl->GetValueByTranslation(szValue,nValue);
				sprintf_s(szSQL,512," %s = %d ", sBusinessField.c_str(), nValue);
			}
			else if (ControlType[Business_CheckBox])
			{
				int nValue = pCheckBoxControl->GetCheck();
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
			sprintf_s(sz,256,"%s = '%s'",pFieldDesc->m_strFieldName.c_str(),m_strRecordTime_Old.c_str());
			if (m_bNeed2UpdateRecordTime)
			{
				m_bNeed2UpdateRecordTime = false;
			}
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