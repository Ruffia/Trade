#include "stdafx.h"
#include "DialogPlaceHolder_DailyTraceConflict.h"
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
#include "UIData.h"
#include "Tools/StyleManager.h"
#include "Tools/ControlCreator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_FACTORY(CDialogPlaceHolderBusiness,CDialogFutureContract_DailyTraceConflict, string,"CDialogFutureContract_DailyTraceConflict")
CDialogFutureContract_DailyTraceConflict::CDialogFutureContract_DailyTraceConflict(CWnd* pParent /*=NULL*/)
	: CDialogPlaceHolderBusiness(pParent)
{
	CDialogIDMgr::Instance().Register("CDialogFutureContract_DailyTraceConflict",CDialogFutureContract_DailyTraceConflict::IDD); 
}


void CDialogFutureContract_DailyTraceConflict::_LoadData2UI()
{
	bool bExists = _CheckExistsTradeDayRecord();
	if (!bExists) return;

	const string strTradeDay = "TradeDay";
	const string strFutureContractName = "FutureContractName";
	const string strNumber = "number";

	vector<CFieldDesc*> vFieldDesc;
	CDBDataManager::Instance().GetFieldMetaData(m_pDataProvider->m_sBusiness,vFieldDesc);

	vector<CFieldDesc*> vPrimaryKey;
	CDBDataManager::Instance().GetPrimaryKey(m_pDataProvider->m_sBusiness,vPrimaryKey);

	CFieldDesc* pTradeDayDesc = NULL;
	CFieldDesc* pFutureContractNameDesc = NULL;
	CFieldDesc* pNumberDesc = NULL;
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
		else if (pFieldDesc && pFieldDesc->m_strFieldName.find(strNumber) != string::npos)
		{
			pNumberDesc = pFieldDesc;
		}
	}

	if(!pTradeDayDesc || !pFutureContractNameDesc || !pNumberDesc) return;

	string sSQL = "select * from ";
	sSQL += m_pDataProvider->m_sBusiness;
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
	CDBDataManager::Instance().LoadData(sSQL,m_pDataProvider->m_sBusiness,ds);

	UpdateDB2UI(ds);
}


void CDialogFutureContract_DailyTraceConflict::UpdateUI2DB()
{
	bool bExists = _CheckExistsTradeDayRecord();
	if(!bExists) return;


	typedef tuple<CWnd*, CWnd*, CWnd*> TraceConflict;
	vector<TraceConflict> vConflict;

	//三组数值，控件下标为0，1，2
	//四个控件: number,ConflictType,DetailedDescription
	const char* szControl[3] = {"number","ConflictType","DetailedDescription"};
	for (int i = 0; i < 3;i++)
	{
		TraceConflict controlSet;
		for (int j = 0; j < 3;j++)
		{
			char sz[128] = {0};
			sprintf_s(sz,128,"%s%d",szControl[j],i);
			CWnd* pControl = m_mapBusiness2Control[sz];
			//不支持 get<j>(controlSet) = pControl; 这种用法，无语
			if (0 == j)
			{
				get<0>(controlSet) = pControl;
			}
			else if (1 == j)
			{
				get<1>(controlSet) = pControl;
			}
			else if (2 == j)
			{
				get<2>(controlSet) = pControl;
			}
		}

		vConflict.push_back(controlSet);
	}


	map<string,CFieldDesc*>& mapTableName2FieldDesc = CDBDataManager::Instance().GetTableMeta(m_pDataProvider->m_sBusiness);
	for (int i = 0; i < vConflict.size();i++)
	{
		string sSQL = "update ";
		sSQL += m_pDataProvider->m_sBusiness;
		sSQL += " set ";

		string sSQLField = "";
		TraceConflict& objConflict = vConflict[i];
		CBusinessEdit* pNumber = dynamic_cast<CBusinessEdit*>(get<0>(objConflict));
		if(!pNumber) continue;
		{
			char sz[256] = {0};
			sprintf_s(sz,256," %s = %d, ",pNumber->m_sBusinessField.c_str(),i);
			sSQLField += sz;
		}

		CBusinessComboBox* pTimePeriod = dynamic_cast<CBusinessComboBox*>(get<1>(objConflict));
		if(!pTimePeriod) continue;
		{
			int nValue = pTimePeriod->GetCurSel();
			char sz[256] = {0};
			sprintf_s(sz,256," %s = %d, ",pTimePeriod->m_sBusinessField.c_str(),nValue);
			sSQLField += sz;
		}

		CBusinessEdit* pDetailedDescription = dynamic_cast<CBusinessEdit*>(get<2>(objConflict));
		if(!pDetailedDescription) continue;
		{
			char sz[256] = {0};
			CString strText = "";
			pDetailedDescription->GetWindowTextA(strText);
			sprintf_s(sz,256," %s = '%s' ",pDetailedDescription->m_sBusinessField.c_str(),strText);
			sSQLField += sz;
		}

		sSQL += sSQLField;
		if (CTradeDayPrimaryData::Instance().m_bNeed2UpdateFutureContractName)
		{
			char sz[256] = {0};
			sprintf_s(sz,256,", FutureContractName = '%s'",CTradeDayPrimaryData::Instance().m_strFutureContractName.c_str());
			sSQL += sz;
		}
		
		sSQL += " where ";

		vector<CFieldDesc*> vPrimaryKey;
		CDBDataManager::Instance().GetPrimaryKey(m_pDataProvider->m_sBusiness,vPrimaryKey);

		string strPrimaryKeyClause = "";
		for (int j = 0; j < vPrimaryKey.size();j++)
		{
			char sz[256] = {0};
			CFieldDesc* pFieldDesc = vPrimaryKey[j];
			if(!pFieldDesc) continue;

			if (pFieldDesc->m_strFieldName.find("TradeDay") != string::npos)
			{
				sprintf_s(sz,256,"%s = '%s'",pFieldDesc->m_strFieldName.c_str(),CTradeDayPrimaryData::Instance().m_strTradeDay.c_str());
			}
			else if (pFieldDesc->m_strFieldName.find("FutureContractName") != string::npos)
			{
				sprintf_s(sz,256,"%s = '%s'",pFieldDesc->m_strFieldName.c_str(),CTradeDayPrimaryData::Instance().m_strFutureContractName_LastTime.c_str());
			}
			else if (pFieldDesc->m_strFieldName.find("number") != string::npos)
			{
				sprintf_s(sz,256,"%s = %d",pFieldDesc->m_strFieldName.c_str(),i);
			}

			if (vPrimaryKey.size() - 1 != j)
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

		if (vConflict.size() - 1 == i)
		{
			if (CTradeDayPrimaryData::Instance().m_bNeed2UpdateFutureContractName)
			{
				CTradeDayPrimaryData::Instance().m_Synchronize[UI_DailyTraceConflict] = true;
				if (CTradeDayPrimaryData::Instance().m_Synchronize[UI_MinorCycleAnalyze] &&
					CTradeDayPrimaryData::Instance().m_Synchronize[UI_DailyTraceEvidence] &&
					CTradeDayPrimaryData::Instance().m_Synchronize[UI_DailyTraceConflict])
				{
					for (int k = 0;k < Place2UpdateFutureContractName;k++)
					{
						CTradeDayPrimaryData::Instance().m_Synchronize[k] = false;
					}
					CTradeDayPrimaryData::Instance().m_bNeed2UpdateFutureContractName = false;
				}
			}
		}
	}
}

void CDialogFutureContract_DailyTraceConflict::UpdateDB2UI(CDataSet& ds,int index)
{
	map<string,CFieldDesc*>& mapTableName2FieldDesc = CDBDataManager::Instance().GetTableMeta(m_pDataProvider->m_sBusiness);
	for (int i = 0; i < ds.Size();i++)
	{
		CRecord* pRecord = ds[i];
		if(!pRecord) continue;
		vector<CField*> vField;
		pRecord->GetSortedFieldSet(vField);
		string sTechnicalIndexMeaning = "";
		string sTranslation = "";
		for (int j = 0; j < vField.size();j++)
		{
			CField* pField = vField[j];
			if(!pField) continue;
			CFieldDesc* pFieldDesc = pField->GetFieldDesc();
			if (!pFieldDesc) continue;

			string strFieldName = pField->GetFieldDesc()->m_strFieldName;
			char szControlID[128] = {0};
			sprintf_s(szControlID,128,"%s%d",strFieldName.c_str(),i);
			map<string,CWnd*>::iterator itControl = m_mapBusiness2Control.find(szControlID);
			if (m_mapBusiness2Control.end() == itControl)
			{
				continue;
			}

			string sCaption = itControl->first;
			CWnd* pWnd = itControl->second;
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
				int nValue = pField->GetValueAsInt();
				if (ControlType[Business_ComboBox])
				{
					string strControlID = szControlID;
					if (string::npos != strFieldName.find("TechnicalIndex"))
					{
						bool bFindValue = pComboBoxControl->GetMeaning(nValue,sTechnicalIndexMeaning);
						bFindValue = pComboBoxControl->GetTranslation(nValue,sTranslation);
						pComboBoxControl->SetCurSel(nValue);
						strValue = sTranslation.c_str();
					}
					else
					{
						bool bFindValue = pComboBoxControl->GetTranslation(nValue,sTranslation);
						pComboBoxControl->SetCurSel(nValue);
						strValue = sTranslation.c_str();
					}

				}
				else if (ControlType[Business_CheckBox])
				{
					pCheckBoxControl->SetCheck(nValue);
					pCheckBoxControl->GetWindowTextA(strValue);
				}
				else if(ControlType[Business_Edit])
				{
					CString sText = "";
					pBusinessControl->GetWindowTextA(sText);
					strValue = sText;
				}
				else
				{
					strValue.Format(strDisplayType.c_str(),nValue);
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
}