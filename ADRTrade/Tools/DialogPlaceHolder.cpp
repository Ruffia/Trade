// DialogPlaceHolder.cpp : 实现文件
//
#include "stdafx.h"
#include "ADRTrade.h"
#include "DialogPlaceHolder.h"
#include "afxdialogex.h"
#include "Factory.h"
#include "DialogIDManager.h"
#include "Tools/StyleManager.h"
#include "UIData.h"
#include "Tools/CollectiveComponentProvider.h"
#include "BusinessEdit.h"
#include "DBDataManager.h"
#include "Util.h"
#include "EditTreeCtrlEx.h"

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
	_LoadData2UI();
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
		else if (data.m_strUIClassName.find("CComBox") != string::npos)
		{
			CComboBox* pCombox = new CComboBox;
			CRect rc(data.m_nLeft,data.m_nTop,data.m_nLeft + data.m_nWidth ,data.m_nTop + data.m_nHeight);
			pCombox->Create(dwTotalStyle,rc,this,data.m_nID);	
			xml_node nodeDropdownItem = node.child("DropdownItem");
			while (!nodeDropdownItem.empty())
			{
				string sDropItem = nodeDropdownItem.attribute("Name").as_string("");
				pCombox->AddString(sDropItem.c_str());
				nodeDropdownItem = nodeDropdownItem.next_sibling();
			}

			pCombox->SetFont(pFont);
			pCombox->ShowWindow(SW_SHOW);
			pCombox->SetWindowText(sCaption.c_str());
			m_mapUIName2Wnd[sName] = pCombox;	
		}
		node = node.next_sibling();
	}
}


void CDialogPlaceHolder::_LoadData2UI()
{
	bool bExists = _CheckExistsRecord();
	if (!bExists) return;

	vector<CFieldDesc*> vFieldDesc;
	CDBDataManager::Instance().GetFieldMetaData(m_sBusiness,vFieldDesc);

	vector<CFieldDesc*> vPrimaryKey;
	CDBDataManager::Instance().GetPrimaryKey(m_sBusiness,vPrimaryKey);
	//此处只处理只有一个主键"TradeDay"的情况
	if(vPrimaryKey.size() != 1) return;

	CFieldDesc* pPrimaryKeyDesc = vPrimaryKey[0];
	if(!pPrimaryKeyDesc) return;

	COleDateTime dtNOw = COleDateTime::GetCurrentTime();
	CString strDate = dtNOw.Format("%Y-%m-%d");

	FieldValue vKey;
	vKey.SetDataType("string");
	vKey.SetValueString(strDate);

	string sSQL = "select * from ";
	sSQL += m_sBusiness;
	sSQL += " where ";
	sSQL += pPrimaryKeyDesc->m_strFieldName;
	sSQL += " = ";
	sSQL += vKey.GetValueAsString();

	CDataSet ds;
	CDBDataManager::Instance().LoadData(sSQL,m_sBusiness,ds);
	if(ds.Size() != 1) return;   //此处有且仅有一条记录
	 
	CRecord* pRecord = ds[0];
	if(!pRecord) return;

	_UpdateDB2UI(pRecord);
}


bool CDialogPlaceHolder::_CheckExistsRecord()
{
	vector<CFieldDesc*> vPrimaryKey;
	CDBDataManager::Instance().GetPrimaryKey(m_sBusiness,vPrimaryKey);
	//此处只处理只有一个主键"TradeDay"的情况
	if(vPrimaryKey.size() != 1) return false;

	CFieldDesc* pPrimaryKeyDesc = vPrimaryKey[0];
	if(!pPrimaryKeyDesc) return false;

	COleDateTime dtNOw = COleDateTime::GetCurrentTime();
	CString strDate = dtNOw.Format("%Y-%m-%d");

	FieldValue vKey;
	vKey.SetDataType("string");
	vKey.SetValueString(strDate);
	bool bExists = CDBDataManager::Instance().RecordExists(m_sBusiness,pPrimaryKeyDesc->m_strFieldName,pPrimaryKeyDesc->m_strDataType,vKey);
	if (!bExists)
	{
		return false; 
	}

	return true;
}

void CDialogPlaceHolder::_UpdateDB2UI( CRecord* pRecord )
{
	map<string,CFieldDesc*>& mapTableName2FieldDesc = CDBDataManager::Instance().GetTableMeta(m_sBusiness);
	for(map<string,CBusinessEdit*>::iterator it = m_mapBusiness2Control.begin();
		it != m_mapBusiness2Control.end();it++)
	{
		CBusinessEdit* pBusinessControl = it->second;
		if(!pBusinessControl) continue;

		//根据FieldID 找到 CFieldDesc
		map<string,CFieldDesc*>::iterator itFieldDesc = mapTableName2FieldDesc.find(pBusinessControl->m_sBusinessField);
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
			strValue.Format(strDisplayType.c_str(),pField->GetValueAsInt());
		}
		else if (strDataType.find("float") != string::npos)
		{
			strValue.Format(strDisplayType.c_str(),pField->GetValueAsFloat());
		}
		else if (strDataType.find("double") != string::npos)
		{
			strValue.Format(strDisplayType.c_str(),pField->GetValueAsDouble());
		}

		pBusinessControl->SetWindowTextA(strValue);
	}
}


void CDialogPlaceHolder::UpdateUI2DB()
{
	bool bExists = _CheckExistsRecord();
	if(!bExists) return;

	map<string,CFieldDesc*>& mapTableName2FieldDesc = CDBDataManager::Instance().GetTableMeta(m_sBusiness);

	string sSQL = "update ";
	sSQL += m_sBusiness;
	sSQL += " set ";

	int nUIControlCount = 0;
	for(map<string,CBusinessEdit*>::iterator it = m_mapBusiness2Control.begin();
		it != m_mapBusiness2Control.end();it++)
	{
		CBusinessEdit* pBusinessControl = it->second;
		if(!pBusinessControl) continue;

		map<string,CFieldDesc*>::iterator itFieldDesc = mapTableName2FieldDesc.find(pBusinessControl->m_sBusinessField);
		if(mapTableName2FieldDesc.end() == itFieldDesc) continue;

		CFieldDesc* pFieldDesc = itFieldDesc->second;
		if (!pFieldDesc) continue;

		string sFieldValue2Update = pBusinessControl->m_sBusinessField;
		sFieldValue2Update += " = ";

		char szValue[512] = {0}; 
		pBusinessControl->GetWindowTextA(szValue,512);

		char szSQL[512] = {0}; 
		if (pFieldDesc->m_strDataType == "string")
		{
			sprintf_s(szSQL,512," %s = '%s' ",pBusinessControl->m_sBusinessField.c_str(),szValue);
		}
		else if (pFieldDesc->m_strDataType == "float" || pFieldDesc->m_strDataType == "integer" ||
			pFieldDesc->m_strDataType == "int" )
		{
			sprintf_s(szSQL,512," %s = %s ", pBusinessControl->m_sBusinessField.c_str(), szValue);
		}

		sSQL += szSQL;

		if (m_mapBusiness2Control.size() - 1 != nUIControlCount)
		{
			sSQL += ",";
		}

		nUIControlCount++;
	}

	sSQL += " where ";

	vector<CFieldDesc*> vPrimaryKey;
	CDBDataManager::Instance().GetPrimaryKey(m_sBusiness,vPrimaryKey);
	//此处只处理只有一个主键"TradeDay"的情况
	if(vPrimaryKey.size() != 1) return;

	CFieldDesc* pPrimaryKeyDesc = vPrimaryKey[0];
	if(!pPrimaryKeyDesc) return;

	COleDateTime dtNOw = COleDateTime::GetCurrentTime();
	CString strDate = dtNOw.Format("%Y-%m-%d");

	char szChereClause[512] = {0};
	sprintf_s(szChereClause,512,"%s = '%s'",pPrimaryKeyDesc->m_strFieldName,strDate);

	sSQL += szChereClause;

	CDBDataManager::Instance().Exec(sSQL);
}