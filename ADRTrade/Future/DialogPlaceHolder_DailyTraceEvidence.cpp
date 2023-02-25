#include "stdafx.h"
#include <iostream>
#include <tuple>
using namespace std;
#include "DialogPlaceHolder_DailyTraceEvidence.h"
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


IMPLEMENT_FACTORY(CDialogPlaceHolder,CDialogFutureContract_DailyTraceEvidence, string,"CDialogFutureContract_DailyTraceEvidence")
CDialogFutureContract_DailyTraceEvidence::CDialogFutureContract_DailyTraceEvidence(CWnd* pParent /*=NULL*/)
	: CDialogPlaceHolder(pParent)
{
	CDialogIDMgr::Instance().Register("CDialogFutureContract_DailyTraceEvidence",CDialogFutureContract_DailyTraceEvidence::IDD); 
}

BEGIN_MESSAGE_MAP(CDialogFutureContract_DailyTraceEvidence,CDialogPlaceHolder)
	ON_CBN_SELCHANGE(Combox_TechnicalIndex0,&CDialogFutureContract_DailyTraceEvidence::OnSelComboChange_TechnicalIndex0)
	ON_CBN_SELCHANGE(Combox_TechnicalIndex1,&CDialogFutureContract_DailyTraceEvidence::OnSelComboChange_TechnicalIndex1)
	ON_CBN_SELCHANGE(Combox_TechnicalIndex2,&CDialogFutureContract_DailyTraceEvidence::OnSelComboChange_TechnicalIndex2)
END_MESSAGE_MAP()


void CDialogFutureContract_DailyTraceEvidence::_LoadTradeDayData2UI()
{
	bool bExists = _CheckExistsTradeDayRecord();
	if (!bExists) return;

	const string strTradeDay = "TradeDay";
	const string strFutureContractName = "FutureContractName";
	const string strNumber = "number";

	vector<CFieldDesc*> vFieldDesc;
	CDBDataManager::Instance().GetFieldMetaData(m_sBusiness,vFieldDesc);

	vector<CFieldDesc*> vPrimaryKey;
	CDBDataManager::Instance().GetPrimaryKey(m_sBusiness,vPrimaryKey);

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


void CDialogFutureContract_DailyTraceEvidence::UpdateUI2DB()
{
	bool bExists = _CheckExistsTradeDayRecord();
	if(!bExists) return;

	//if (!CTradeDayPrimaryData::Instance().m_bNeed2UpdateFutureContractName)
	//{
	//	CTradeDayPrimaryData::Instance().m_strFutureContractName_LastTime = CTradeDayPrimaryData::Instance().m_strFutureContractName;
	//	CTradeDayPrimaryData::Instance().m_strFutureContractName = szValue;
	//	if (CTradeDayPrimaryData::Instance().m_strFutureContractName_LastTime != CTradeDayPrimaryData::Instance().m_strFutureContractName)
	//	{
	//		CTradeDayPrimaryData::Instance().m_bNeed2UpdateFutureContractName = true;
	//		CTradeDayPrimaryData::Instance().m_nPlace2UpdateFutureContractName = Place2UpdateFutureContractName;
	//	}
	//}

	typedef tuple<CWnd*, CWnd*, CWnd*,CWnd*> TraceEvidence;
	vector<TraceEvidence> vEvidence;

	//三组数值，控件下标为0，1，2
	//四个控件: number,TimePeriod,TechniqueIndex,IndexValue
	const char* szControl[4] = {"number","TimePeriod","TechnicalIndex","IndexValue"};
	for (int i = 0; i < 3;i++)
	{
		TraceEvidence controlSet;
		for (int j = 0; j < 4;j++)
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
			else if (3 == j)
			{
				get<3>(controlSet) = pControl;
			}	
		}

		vEvidence.push_back(controlSet);
	}


	map<string,CFieldDesc*>& mapTableName2FieldDesc = CDBDataManager::Instance().GetTableMeta(m_sBusiness);
	for (int i = 0; i < vEvidence.size();i++)
	{
		string sSQL = "update ";
		sSQL += m_sBusiness;
		sSQL += " set ";

		string sSQLField = "";
		TraceEvidence& objEvidence = vEvidence[i];
		CBusinessEdit* pNumber = dynamic_cast<CBusinessEdit*>(get<0>(objEvidence));
		if(!pNumber) continue;
		{
			char sz[256] = {0};
			sprintf_s(sz,256," %s = %d, ",pNumber->m_sBusinessField.c_str(),i);
			sSQLField += sz;
		}

		CBusinessComboBox* pTimePeriod = dynamic_cast<CBusinessComboBox*>(get<1>(objEvidence));
		if(!pTimePeriod) continue;
		{
			int nValue = pTimePeriod->GetCurSel();
			char sz[256] = {0};
			sprintf_s(sz,256," %s = %d, ",pTimePeriod->m_sBusinessField.c_str(),nValue);
			sSQLField += sz;
		}

		CBusinessComboBox* pTechniqueIndex = dynamic_cast<CBusinessComboBox*>(get<2>(objEvidence));
		if(!pTechniqueIndex) continue;
		{
			int nValue = pTechniqueIndex->GetCurSel();
			char sz[256] = {0};
			sprintf_s(sz,256," %s = %d, ",pTechniqueIndex->m_sBusinessField.c_str(),nValue);
			sSQLField += sz;
		}

		CBusinessComboBox* pIndexValue = dynamic_cast<CBusinessComboBox*>(get<3>(objEvidence));
		if(!pIndexValue) continue;
		{
			int nValue = pIndexValue->GetCurSel();
			char sz[256] = {0};
			sprintf_s(sz,256," %s = %d ",pIndexValue->m_sBusinessField.c_str(),nValue);
			sSQLField += sz;
		}

		sSQL += sSQLField;
		sSQL += " where ";
		vector<CFieldDesc*> vPrimaryKey;
		CDBDataManager::Instance().GetPrimaryKey(m_sBusiness,vPrimaryKey);

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
		

	}
}

void CDialogFutureContract_DailyTraceEvidence::UpdateDB2UI(CDataSet& ds,int index)
{	
	map<string,CFieldDesc*>& mapTableName2FieldDesc = CDBDataManager::Instance().GetTableMeta(m_sBusiness);
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
					else if (string::npos != strFieldName.find("IndexValue"))
					{
						pComboBoxControl->Reset();
						string sSQL = "select * from Dictionary_Field where Subject ";
						sSQL += "='";
						sSQL += sTechnicalIndexMeaning;
						sSQL += "'";
						sSQL += " order by value asc";

						CDataSet ds;
						CDBDataManager::Instance().LoadData(sSQL,"Dictionary_Field",ds);

						const int nRecordCount = ds.Size();
						for (int i = 0; i < nRecordCount;i++)
						{
							CRecord* pRecord = ds[i];
							if(!pRecord) continue;

							CField* pFieldValue = pRecord->GetField("Value");
							if(!pFieldValue) continue;
							int nValue = pFieldValue->GetValueAsInt();

							CField* pFieldTranslation = pRecord->GetField("Translation");
							if(!pFieldTranslation) continue;
							CField* pFieldMeaning = pRecord->GetField("Meaning");
							if(!pFieldMeaning) continue;

							string sTranslation = pFieldTranslation->GetValueAsString();
							string sMeaning = pFieldMeaning->GetValueAsString();
							pComboBoxControl->AddString(sTranslation.c_str());
							pComboBoxControl->Add2Map(nValue,sMeaning,sTranslation);
						}

						bool bFindValue = pComboBoxControl->GetTranslation(nValue,sTranslation);
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


void CDialogFutureContract_DailyTraceEvidence::_ChangeComboxDropdownList( const string& strComboxTechnicalIndex_Name, const string& strComboxTechnicalIndexValue_Name ) 
{
	CBusinessComboBox* pCombox_TechnicalIndex = dynamic_cast<CBusinessComboBox*>(m_mapBusiness2Control[strComboxTechnicalIndex_Name]);
	if(!pCombox_TechnicalIndex) return;

	CBusinessComboBox* pCombox_TechnicalIndexValue = dynamic_cast<CBusinessComboBox*>(m_mapBusiness2Control[strComboxTechnicalIndexValue_Name]);
	if(!pCombox_TechnicalIndexValue) return;

	int nCurSel = pCombox_TechnicalIndex->GetCurSel();
	if(-1 == nCurSel) return;

	string sMeaning = "";
	pCombox_TechnicalIndex->GetMeaning(nCurSel,sMeaning);

	string sSQL = "select * from Dictionary_Field where Subject ";
	sSQL += "='";
	sSQL += sMeaning;
	sSQL += "'";
	sSQL += " order by value asc";

	CDataSet ds;
	CDBDataManager::Instance().LoadData(sSQL,"Dictionary_Field",ds);

	pCombox_TechnicalIndexValue->ResetContent();
	const int nRecordCount = ds.Size();
	for (int i = 0; i < nRecordCount;i++)
	{
		CRecord* pRecord = ds[i];
		if(!pRecord) continue;

		CField* pFieldValue = pRecord->GetField("Value");
		if(!pFieldValue) continue;
		int nValue = pFieldValue->GetValueAsInt();

		CField* pFieldTranslation = pRecord->GetField("Translation");
		if(!pFieldTranslation) continue;
		CField* pFieldMeaning = pRecord->GetField("Meaning");
		if(!pFieldMeaning) continue;

		string sTranslation = pFieldTranslation->GetValueAsString();
		string sMeaning = pFieldMeaning->GetValueAsString();
		pCombox_TechnicalIndexValue->Reset();
		pCombox_TechnicalIndexValue->AddString(sTranslation.c_str());
		pCombox_TechnicalIndexValue->Add2Map(nValue,sMeaning,sTranslation);
	}
}

void CDialogFutureContract_DailyTraceEvidence::OnSelComboChange_TechnicalIndex0()
{
	const string strComboxTechnicalIndex_Name = "TechnicalIndex0";
	const string strComboxTechnicalIndexValue_Name = "IndexValue0";
	_ChangeComboxDropdownList(strComboxTechnicalIndex_Name, strComboxTechnicalIndexValue_Name);
}

void CDialogFutureContract_DailyTraceEvidence::OnSelComboChange_TechnicalIndex1()
{
	const string strComboxTechnicalIndex_Name = "TechnicalIndex1";
	const string strComboxTechnicalIndexValue_Name = "IndexValue1";
	_ChangeComboxDropdownList(strComboxTechnicalIndex_Name, strComboxTechnicalIndexValue_Name);
}

void CDialogFutureContract_DailyTraceEvidence::OnSelComboChange_TechnicalIndex2()
{
	const string strComboxTechnicalIndex_Name = "TechnicalIndex2";
	const string strComboxTechnicalIndexValue_Name = "IndexValue2";
	_ChangeComboxDropdownList(strComboxTechnicalIndex_Name, strComboxTechnicalIndexValue_Name);
}