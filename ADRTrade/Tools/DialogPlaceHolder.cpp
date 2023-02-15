// DialogPlaceHolder.cpp : 实现文件
//
#include "stdafx.h"
#include "ADRTrade.h"
#include "DialogPlaceHolder.h"
#include "afxdialogex.h"
#include "../Common/Factory.h"
#include "DialogIDManager.h"
#include "Tools/StyleManager.h"
#include "UIData.h"
#include "Tools/CollectiveComponentProvider.h"
#include "BusinessEdit.h"
#include "BusinessComboBox.h"
#include "BusinessCheckBox.h"
#include "DBDataManager.h"
#include "Util.h"
#include "EditTreeCtrlEx.h"
#include "PrimaryKeyRule.h"
#include "TradeDayPrimaryData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CDialogPlaceHolder 对话框

IMPLEMENT_DYNAMIC(CDialogPlaceHolder, CDialog)

CDialogPlaceHolder::CDialogPlaceHolder(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogPlaceHolder::IDD, pParent)
{
	m_mapUIName2Wnd.clear();
	m_sBusiness = "";
}

CDialogPlaceHolder::~CDialogPlaceHolder()
{
	for (map<string,CWnd*>::iterator it = m_mapUIName2Wnd.begin();
		it != m_mapUIName2Wnd.end();it++)
	{
		CWnd* pWnd = it->second;
		if (pWnd)
		{
			delete pWnd;
			pWnd = NULL;
		}
	}
	m_mapUIName2Wnd.clear();
}

void CDialogPlaceHolder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogPlaceHolder, CDialog)
END_MESSAGE_MAP()


// CDialogPlaceHolder 消息处理程序
BOOL CDialogPlaceHolder::OnInitDialog()
{
	CDialog::OnInitDialog();
	_InitLayOut();
	_LoadTradeDayData2UI();
	return TRUE;
}


void CDialogPlaceHolder::_InitLayOut()
{
	string sFileName = GetModulePath() + "/UI/" + m_sLayout;
	bool bRet = true;
	if (!m_doc.load_file(sFileName.c_str())) 
	{ //加载xml文件
		return;
	}
	
	CRect rc;
	GetClientRect(rc);
	ClientToScreen(rc);

	xml_node root = m_doc.child("root");  //根节点
	xml_node nodeLayout = root.child("Layout");
	xml_node node = nodeLayout.first_child();
	while (!node.empty())
	{
		CUIData data;
		const string sName = node.attribute("Name").as_string("");
		data.m_strUIClassName = node.attribute("ClassName").as_string("");
		data.m_nID = node.attribute("ID").as_int();
		data.m_nLeft = node.attribute("Left").as_int();
		data.m_nTop = node.attribute("Top").as_int();
		data.m_nWidth = node.attribute("Width").as_int();
		data.m_nHeight = node.attribute("Height").as_int();
		const string sStyle = node.attribute("Style").as_string();
		DWORD dwTotalStyle = CStyleMgr::Instance().GetStyle(sStyle);
		const string sCaption = node.attribute("Caption").as_string();
		CFont* pFont = CCollectiveComponentProvider::Instance().GetFont();
		if (data.m_strUIClassName.find("CEditTreeCtrlEx") != string::npos)
		{
			CEditTreeCtrlEx* pTree = new CEditTreeCtrlEx;
			CRect rcTree(data.m_nLeft,data.m_nTop,data.m_nLeft + data.m_nWidth ,data.m_nTop + data.m_nHeight);
			pTree->Create(dwTotalStyle,rcTree,this,data.m_nID);		
			pTree->SetFont(pFont);
			pTree->ShowWindow(SW_SHOW);
			m_mapUIName2Wnd[sName] = pTree;	
		}
		else if (data.m_strUIClassName.find("CBusinessEdit") != string::npos)
		{
			CBusinessEdit* pEdit = new CBusinessEdit;
			CRect rc(data.m_nLeft,data.m_nTop,data.m_nLeft + data.m_nWidth ,data.m_nTop + data.m_nHeight);
			pEdit->Create(dwTotalStyle,rc,this,data.m_nID);	
			pEdit->SetFont(pFont);
			pEdit->ShowWindow(SW_SHOW);
			pEdit->SetWindowText(sCaption.c_str());
			const string& sBusiness = node.attribute("business").as_string();
			pEdit->m_sBusinessField = sBusiness;
			m_mapBusiness2Control[sBusiness] = pEdit;
			m_mapUIName2Wnd[sName] = pEdit;	
		}
		else if (data.m_strUIClassName.find("CEdit") != string::npos)
		{
			CEdit* pEdit = new CEdit;
			CRect rc(data.m_nLeft,data.m_nTop,data.m_nLeft + data.m_nWidth ,data.m_nTop + data.m_nHeight);
			pEdit->Create(dwTotalStyle,rc,this,data.m_nID);	
			pEdit->SetFont(pFont);
			pEdit->ShowWindow(SW_SHOW);
			pEdit->SetWindowText(sCaption.c_str());
			m_mapUIName2Wnd[sName] = pEdit;	
		}
		else if (data.m_strUIClassName.find("CBusinessComboBox") != string::npos)
		{
			CBusinessComboBox* pCombox = new CBusinessComboBox;
			CRect rc(data.m_nLeft,data.m_nTop,data.m_nLeft + data.m_nWidth ,data.m_nTop + data.m_nHeight);
			pCombox->Create(dwTotalStyle,rc,this,data.m_nID);	
			xml_node nodeDropdownItem = node.child("Dropdown");
			string sSubject = nodeDropdownItem.attribute("Subject").as_string("");

			string sSQL = "select * from Dictionary_Field where Subject ";
			sSQL += "='";
			sSQL += sSubject;
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
				string sMeaning = pFieldTranslation->GetValueAsString();
				pCombox->AddString(sMeaning.c_str());
				pCombox->Add2Map(nValue,sMeaning);
			}

			const string& sBusiness = node.attribute("business").as_string();
			pCombox->m_sBusinessField = sBusiness;

			pCombox->SetFont(pFont);
			pCombox->ShowWindow(SW_SHOW);
			pCombox->SetWindowText(sCaption.c_str());
			m_mapBusiness2Control[sBusiness] = pCombox;
			m_mapUIName2Wnd[sName] = pCombox;	
		}
		else if(data.m_strUIClassName.find("CBusinessCheckBox") != string::npos)
		{
			CBusinessCheckBox* pCheckBox = new CBusinessCheckBox;
			CRect rc(data.m_nLeft,data.m_nTop,data.m_nLeft + data.m_nWidth ,data.m_nTop + data.m_nHeight);	
			pCheckBox->Create(sCaption.c_str(), dwTotalStyle,rc,this, data.m_nID);
			pCheckBox->SetFont(pFont);
			const string& sBusiness = node.attribute("business").as_string();
			pCheckBox->m_sBusinessField = sBusiness;
			m_mapBusiness2Control[sName] = pCheckBox;
			m_mapUIName2Wnd[sName] = pCheckBox;	
		}
		node = node.next_sibling();
	}
}


void CDialogPlaceHolder::_LoadTradeDayData2UI()
{
	bool bExists = _CheckExistsTradeDayRecord();
	if (!bExists) return;

	vector<CFieldDesc*> vFieldDesc;
	CDBDataManager::Instance().GetFieldMetaData(m_sBusiness,vFieldDesc);

	vector<CFieldDesc*> vPrimaryKey;
	CDBDataManager::Instance().GetPrimaryKey(m_sBusiness,vPrimaryKey);

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
	sSQL += m_sBusiness;
	sSQL += " where ";
	sSQL += pTradeDayDesc->m_strFieldName;
	sSQL += " = '";
	sSQL += CTradeDayPrimaryData::Instance().m_strTradeDay;
	sSQL += "'";

	CDataSet ds;
	CDBDataManager::Instance().LoadData(sSQL,m_sBusiness,ds);
	if(ds.Size() != 1) return;   //此处有且仅有一条记录
	 
	CRecord* pRecord = ds[0];
	if(!pRecord) return;

	UpdateDB2UI(pRecord);
}


bool CDialogPlaceHolder::_CheckExistsTradeDayRecord()
{
	vector<CFieldDesc*> vPrimaryKey;
	CDBDataManager::Instance().GetPrimaryKey(m_sBusiness,vPrimaryKey);

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
	bool bExists = CDBDataManager::Instance().RecordExists(m_sBusiness,pTradeDayDesc->m_strFieldName,pTradeDayDesc->m_strDataType,vKey);
	if (!bExists)
	{
		return false; 
	}

	return true;
}

void CDialogPlaceHolder::UpdateDB2UI( CRecord* pRecord )
{
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
		else if (pFieldDesc->m_strDataType == "integer" || pFieldDesc->m_strDataType == "int")
		{
			if (ControlType[Business_ComboBox])
			{
				string sMeaning = "";
				int nCurSel = pComboBoxControl->GetCurSel();
				bool bFindValue = pComboBoxControl->GetMeaning(pField->GetValueAsInt(),sMeaning);
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


void CDialogPlaceHolder::UpdateUI2DB()
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

		//截至目前，有3中Business 字段类型
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
				bool bFindValue = pComboBoxControl->GetValue(szValue,nValue);
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
	//此处只处理只有一个主键"TradeDay"的情况
	if(vPrimaryKey.size() != 1) return;

	CFieldDesc* pPrimaryKeyDesc = vPrimaryKey[0];
	if(!pPrimaryKeyDesc) return;

	char szChereClause[512] = {0};
	sprintf_s(szChereClause,512,"%s = '%s'",pPrimaryKeyDesc->m_strFieldName.c_str(),CTradeDayPrimaryData::Instance().m_strTradeDay.c_str());

	sSQL += szChereClause;
	CDBDataManager::Instance().Exec(sSQL);
}
