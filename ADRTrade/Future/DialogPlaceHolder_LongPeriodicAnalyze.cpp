#include "stdafx.h"
#include "DialogPlaceHolder_LongPeriodicAnalyze.h"
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


IMPLEMENT_FACTORY(CDialogPlaceHolder,CDialogFutureContract_LongPeriodicAnalyze, string,"CDialogFutureContract_LongPeriodicAnalyze")
CDialogFutureContract_LongPeriodicAnalyze::CDialogFutureContract_LongPeriodicAnalyze(CWnd* pParent /*=NULL*/)
	: CDialogPlaceHolder(pParent)
{
	CDialogIDMgr::Instance().Register("CDialogFutureContract_LongPeriodicAnalyze",CDialogFutureContract_LongPeriodicAnalyze::IDD); 
}

void CDialogFutureContract_LongPeriodicAnalyze::_LoadTradeDayData2UI()
{
	bool bExists = _CheckExistsTradeDayRecord();
	if (!bExists) return;

	const string strTradeDay = "TradeDay";
	const string strFutureContractName = "FutureContractName";
	CTradeDayPrimaryData::Instance().m_strFutureContractName = _QueryFutureContractName(strTradeDay,strFutureContractName);
	if ("" == CTradeDayPrimaryData::Instance().m_strFutureContractName)
	{
		return;
	}
	CTradeDayPrimaryData::Instance().m_strFutureContractName_LastTime = CTradeDayPrimaryData::Instance().m_strFutureContractName;

	vector<CFieldDesc*> vFieldDesc;
	CDBDataManager::Instance().GetFieldMetaData(m_sBusiness,vFieldDesc);

	vector<CFieldDesc*> vPrimaryKey;
	CDBDataManager::Instance().GetPrimaryKey(m_sBusiness,vPrimaryKey);

	CFieldDesc* pTradeDayDesc = NULL;
	CFieldDesc* pFutureContractNameDesc = NULL;
	for (int i = 0;i < vPrimaryKey.size();i++)
	{
		CFieldDesc* pFieldDesc = vPrimaryKey[i];
		if (pFieldDesc && pFieldDesc->m_strFieldName.find(strTradeDay) != string::npos)
		{
			pTradeDayDesc = pFieldDesc;
		}
		else if (pFieldDesc && pFieldDesc->m_strFieldName.find(strFutureContractName) != string::npos)
		{
			pFutureContractNameDesc = pFieldDesc;
		}
	}

	if(!pTradeDayDesc || !pFutureContractNameDesc) return;

	string sSQL = "select * from ";
	sSQL += m_sBusiness;
	sSQL += " where ";
	sSQL += pTradeDayDesc->m_strFieldName;
	sSQL += " = '";
	sSQL += CTradeDayPrimaryData::Instance().m_strTradeDay;
	sSQL += "'";
	sSQL += " and ";
	sSQL += pFutureContractNameDesc->m_strFieldName;
	sSQL += " = '";
	sSQL += CTradeDayPrimaryData::Instance().m_strFutureContractName;
	sSQL += "'";

	CDataSet ds;
	CDBDataManager::Instance().LoadData(sSQL,m_sBusiness,ds);

	UpdateDB2UI(ds);
}


void CDialogFutureContract_LongPeriodicAnalyze::UpdateUI2DB()
{
	bool bExists = _CheckExistsTradeDayRecord();
	if(!bExists) return;

	map<string,CFieldDesc*>& mapTableName2FieldDesc = CDBDataManager::Instance().GetTableMeta(m_sBusiness);

	string sSQL = "update ";
	sSQL += m_sBusiness;
	sSQL += " set ";

	int nUIControlCount = 0;
	for(map<string,CWnd*>::iterator it = m_mapBusiness2Control.begin();
		it != m_mapBusiness2Control.end();it++)
	{
		CWnd* pWnd = it->second;
		nUIControlCount++;
		bool bComBox = false;
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
			if (pFieldDesc->m_strFieldName.find("FutureContractName") != string::npos)
			{
				if (!CTradeDayPrimaryData::Instance().m_bNeed2UpdateFutureContractName)
				{
					CTradeDayPrimaryData::Instance().m_strFutureContractName_LastTime = CTradeDayPrimaryData::Instance().m_strFutureContractName;
					CTradeDayPrimaryData::Instance().m_strFutureContractName = szValue;
					if (CTradeDayPrimaryData::Instance().m_strFutureContractName_LastTime != CTradeDayPrimaryData::Instance().m_strFutureContractName)
					{
						CTradeDayPrimaryData::Instance().m_bNeed2UpdateFutureContractName = true;
						CTradeDayPrimaryData::Instance().m_nPlace2UpdateFutureContractName = Place2UpdateFutureContractName;
					}
				}
			}
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


string CDialogFutureContract_LongPeriodicAnalyze::_QueryFutureContractName(const string& strTradeDay,const string& strFutureContractName)
{
	string strSQL = "select ";
	strSQL += strFutureContractName;
	strSQL += " from ";
	strSQL += m_sBusiness;
	strSQL += " where ";
	strSQL += strTradeDay;
	strSQL += " = '";
	strSQL += CTradeDayPrimaryData::Instance().m_strTradeDay;
	strSQL += "'";

	CDataSet ds;
	CDBDataManager::Instance().LoadData(strSQL,m_sBusiness,ds);
	if(ds.Size() != 1) return "";   //此处有且仅有一条记录

	CRecord* pRecord = ds[0];
	if(!pRecord) return "";
	CField* pField = pRecord->GetField(strFutureContractName);
	string sFieldValue = pField->GetValueAsString();
	return sFieldValue;
}


void CDialogFutureContract_LongPeriodicAnalyze::UpdateDB2UI( CDataSet& ds,int index )
{
	if(ds.Size() != 1) return;   //此处有且仅有一条记录

	CRecord* pRecord = ds[index];
	if(!pRecord) return;

	map<string,CFieldDesc*>& mapTableName2FieldDesc = CDBDataManager::Instance().GetTableMeta(m_sBusiness);
	for(map<string,CWnd*>::iterator it = m_mapBusiness2Control.begin();
		it != m_mapBusiness2Control.end();it++)
	{
		CWnd* pWnd = it->second;
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


		//根据FieldID 找到 CFieldDesc
		map<string,CFieldDesc*>::iterator itFieldDesc = mapTableName2FieldDesc.find(sBusinessField);
		if(mapTableName2FieldDesc.end() == itFieldDesc) continue;

		CFieldDesc* pFieldDesc = itFieldDesc->second;
		if (!pFieldDesc) continue;

		//根据FieldDesc 找到 CField
		CField* pField = pRecord->GetField(pFieldDesc->m_strFieldName);
		if(!pField) return;

		CString strValue = "";
		const string strDataType = pFieldDesc->m_strDataType;
		const string strDisplayType = pFieldDesc->GetAttributeString("DisplayType");
		if (strDataType.find("string") != string::npos)
		{
			string sValue = pField->GetValueAsString();
			strValue.Format(strDisplayType.c_str(),sValue.c_str());
		}
		else if (strDataType.find("int") != string::npos)
		{
			if (ControlType[Business_ComboBox])
			{
				string sMeaning = "";
				bool bFindValue = pComboBoxControl->GetTranslation(pField->GetValueAsInt(),sMeaning);
				strValue = sMeaning.c_str();
			}
			else if (ControlType[Business_CheckBox])
			{
				int nValue = pField->GetValueAsInt();
				pCheckBoxControl->SetCheck(nValue);
				pCheckBoxControl->GetWindowTextA(strValue);
			}
			else
			{
				strValue.Format(strDisplayType.c_str(),pField->GetValueAsInt());
			}	

		}
		else if (strDataType.find("float") != string::npos)
		{
			strValue.Format(strDisplayType.c_str(),pField->GetValueAsFloat());
		}
		else if (strDataType.find("double") != string::npos)
		{
			strValue.Format(strDisplayType.c_str(),pField->GetValueAsDouble());
		}

		pWnd->SetWindowTextA(strValue);
	}
}