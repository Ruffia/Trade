// DialogPlaceHolder.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "ADRTrade.h"
#include "DialogPlaceHolderDB.h"
#include "afxdialogex.h"
#include "../Common/Factory.h"
#include "DialogIDManager.h"
#include "Tools/StyleManager.h"
#include "UIData.h"
#include "Tools/CollectiveComponentProvider.h"
#include "BusinessEdit.h"
#include "BusinessComboBox.h"
#include "BusinessCheckBox.h"
#include "Util.h"
#include "EditTreeCtrlEx.h"
#include "PrimaryKeyRule.h"
#include "TradeDayPrimaryData.h"
#include "ControlCreator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CDialogPlaceHolderBusiness �Ի���

IMPLEMENT_DYNAMIC(CDialogPlaceHolderBusiness, CDialogPlaceHolder)

CDialogPlaceHolderBusiness::CDialogPlaceHolderBusiness(CWnd* pParent /*=NULL*/)
	: CDialogPlaceHolder(pParent),m_pDataProvider(NULL)
{
	m_mapBusiness2Control.clear();
}

CDialogPlaceHolderBusiness::~CDialogPlaceHolderBusiness()
{
	for (map<string,CWnd*>::iterator it = m_mapUIName2Wnd.begin();
		it != m_mapUIName2Wnd.end();it++)
	{
		CWnd* pWnd = it->second;
		if (pWnd)
		{
			pWnd->DestroyWindow();
			delete pWnd;
			pWnd = NULL;
		}
	}
	m_mapUIName2Wnd.clear();

	if (m_pDataProvider)
	{
		delete m_pDataProvider;
		m_pDataProvider = NULL;
	}
}


// CDialogPlaceHolderBusiness ��Ϣ�������
BOOL CDialogPlaceHolderBusiness::OnInitDialog()
{
	CDialog::OnInitDialog();
	_InitLayOut();
	_LoadData2UI();
	return TRUE;
}


void CDialogPlaceHolderBusiness::_CreateUI( CUIData &data, xml_node node )
{
	IControlCreator* pCreator = Factory<IControlCreator,string>::Instance().BuildProduct(data.m_strUIClassName);
	if(!pCreator) return;
	pCreator->SetBussinessControl(&m_mapBusiness2Control);
	pCreator->Create(data,node,m_mapUIName2Wnd,this);
	delete pCreator; pCreator = NULL;
}

void CDialogPlaceHolderBusiness::_DesignLayout()
{
}

void CDialogPlaceHolderBusiness::_LoadData2UI()
{
	if(!m_pDataProvider) return;
	bool bExists = _CheckExistsTradeDayRecord();
	if (!bExists) return;

	vector<CFieldDesc*> vFieldDesc;
	CDBDataManager::Instance().GetFieldMetaData(m_pDataProvider->m_sBusiness,vFieldDesc);

	vector<CFieldDesc*> vPrimaryKey;
	CDBDataManager::Instance().GetPrimaryKey(m_pDataProvider->m_sBusiness,vPrimaryKey);

	const string strTradeDay = "TradeDay";
	CFieldDesc* pTradeDayDesc = NULL;
	for (int i = 0;i < vPrimaryKey.size();i++)
	{
		CFieldDesc* pFieldDesc = vPrimaryKey[i];
		if (pFieldDesc && pFieldDesc->m_strFieldName.find(strTradeDay) != string::npos)
		{
			pTradeDayDesc = pFieldDesc;
			break;
		}
	}
		
	if(!pTradeDayDesc) return;

	string sSQL = "select * from ";
	sSQL += m_pDataProvider->m_sBusiness;
	sSQL += " where ";
	sSQL += pTradeDayDesc->m_strFieldName;
	sSQL += " = '";
	sSQL += CTradeDayPrimaryData::Instance().m_strTradeDay;
	sSQL += "'";

	CDataSet ds;
	CDBDataManager::Instance().LoadData(sSQL,m_pDataProvider->m_sBusiness,ds);

	UpdateDB2UI(ds);
}


bool CDialogPlaceHolderBusiness::_CheckExistsTradeDayRecord()
{
	vector<CFieldDesc*> vPrimaryKey;
	CDBDataManager::Instance().GetPrimaryKey(m_pDataProvider->m_sBusiness,vPrimaryKey);

	const string strFieldSelect = "TradeDay";
	CFieldDesc* pTradeDayDesc = NULL;
	string sPrimaryKey = "";
	int nSize = vPrimaryKey.size();
	for (int i = 0;i < nSize;i++)
	{
		CFieldDesc* pFieldDesc = vPrimaryKey[i];
		if(!pFieldDesc) continue;
		if (string::npos != pFieldDesc->m_strFieldName.find(strFieldSelect))
		{
			pTradeDayDesc = pFieldDesc;
			break;
		}
	}

	if(!pTradeDayDesc) return false;

	COleDateTime dtNOw = COleDateTime::GetCurrentTime();
	CString strDate = dtNOw.Format("%Y-%m-%d");

	FieldValue vKey;
	vKey.SetDataType("string");
	vKey.SetValueString(strDate);
	bool bExists = CDBDataManager::Instance().RecordExists(m_pDataProvider->m_sBusiness,pTradeDayDesc->m_strFieldName,pTradeDayDesc->m_strDataType,vKey);
	if (!bExists)
	{
		return false; 
	}

	return true;
}

void CDialogPlaceHolderBusiness::UpdateDB2UI(CDataSet& ds,int index)
{
	//�˴����ҽ���һ����¼
	CRecord* pRecord = ds[index];
	if(!pRecord) return;

	map<string,CFieldDesc*>& mapTableName2FieldDesc = CDBDataManager::Instance().GetTableMeta(m_pDataProvider->m_sBusiness);
	for(map<string,CWnd*>::iterator it = m_mapBusiness2Control.begin();
		it != m_mapBusiness2Control.end();it++)
	{
		CWnd* pWnd = it->second;
		string sBusinessField = "";

		//����Ŀǰ����3��Business �ֶ�����
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

		//����FieldID �ҵ� CFieldDesc
		map<string,CFieldDesc*>::iterator itFieldDesc = mapTableName2FieldDesc.find(sBusinessField);
		if(mapTableName2FieldDesc.end() == itFieldDesc) continue;

		CFieldDesc* pFieldDesc = itFieldDesc->second;
		if (!pFieldDesc) continue;

		//����FieldDesc �ҵ� CField
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
		else if (pFieldDesc->m_strDataType == "integer" || pFieldDesc->m_strDataType == "int")
		{
			if (ControlType[Business_ComboBox])
			{
				string sMeaning = "";
				const int nCurSel = pComboBoxControl->GetCurSel();
				const int nFieldValue = pField->GetValueAsInt();
				bool bFindValue = pComboBoxControl->GetTranslation(nFieldValue,sMeaning);
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


void CDialogPlaceHolderBusiness::UpdateUI2DB()
{
	bool bExists = _CheckExistsTradeDayRecord();
	if(!bExists) return;

	string sSQL = _CreateUpdateSQL();

	vector<CFieldDesc*> vPrimaryKey;
	CDBDataManager::Instance().GetPrimaryKey(m_pDataProvider->m_sBusiness,vPrimaryKey);
	//�˴�ֻ����ֻ��һ������"TradeDay"�����
	if(vPrimaryKey.size() != 1) return;

	CFieldDesc* pPrimaryKeyDesc = vPrimaryKey[0];
	if(!pPrimaryKeyDesc) return;

	char szChereClause[512] = {0};
	sprintf_s(szChereClause,512,"%s = '%s'",pPrimaryKeyDesc->m_strFieldName.c_str(),CTradeDayPrimaryData::Instance().m_strTradeDay.c_str());

	sSQL += szChereClause;
	CDBDataManager::Instance().Exec(sSQL);
}


string CDialogPlaceHolderBusiness::_CreateUpdateSQL()
{
	map<string,CFieldDesc*>& mapTableName2FieldDesc = CDBDataManager::Instance().GetTableMeta(m_pDataProvider->m_sBusiness);

	string sSQL = "update ";
	sSQL += m_pDataProvider->m_sBusiness;
	sSQL += " set ";

	int nUIControlCount = 0;
	for(map<string,CWnd*>::iterator it = m_mapBusiness2Control.begin();
		it != m_mapBusiness2Control.end();it++)
	{
		CWnd* pWnd = it->second;
		nUIControlCount++;

		//����Ŀǰ����3��Business �ֶ�����
		CBusinessEdit* pBusinessControl = NULL;
		CBusinessComboBox* pComboBoxControl = NULL;
		CBusinessCheckBox* pCheckBoxControl = NULL;

		string sBusinessField = "";
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

	return sSQL;
}
