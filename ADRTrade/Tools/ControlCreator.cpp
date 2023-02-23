#include "stdafx.h"
#include "ControlCreator.h"
#include "FieldMeta.h"
#include "Tools/CollectiveComponentProvider.h"
#include "../Common/Factory.h"
#include "Tools/BusinessEdit.h"
#include "Tools/BusinessComboBox.h"
#include "Tools/BusinessCheckBox.h"
#include "Util.h"
#include "Tools/EditTreeCtrlEx.h"
#include "DBDataManager.h"

IControlCreator::IControlCreator()
{
	m_mapUIName2Wnd.clear();
	m_mapBusiness2Control.clear();
}

void IControlCreator::Initialize(map<string,CWnd*>& mapUIName2Wnd,map<string,CWnd*> mapBusiness2Control,CWnd* pParent)
{
	m_mapUIName2Wnd = mapUIName2Wnd;
	m_mapBusiness2Control = mapBusiness2Control;
	m_pParent = pParent;
}


IMPLEMENT_FACTORY(IControlCreator,CControlCreator_CEdit,string,"CEdit")
void CControlCreator_CEdit::Create(const CUIData &data,const string& sName, const string &sCaption, DWORD dwTotalStyle,const xml_node &node)
{
	CFont* pFont = CCollectiveComponentProvider::Instance().GetFont();
	CEdit* pEdit = new CEdit;
	CRect rc(data.m_nLeft,data.m_nTop,data.m_nLeft + data.m_nWidth ,data.m_nTop + data.m_nHeight);
	pEdit->Create(dwTotalStyle,rc,m_pParent,data.m_nID);	
	pEdit->SetFont(pFont);
	pEdit->ShowWindow(SW_SHOW);
	pEdit->SetWindowText(sCaption.c_str());
	m_mapUIName2Wnd[sName] = pEdit;	
}


IMPLEMENT_FACTORY(IControlCreator,CControlCreator_CBusinessEdit,string,"CBusinessEdit")
void CControlCreator_CBusinessEdit::Create(const CUIData &data,const string& sName, const string &sCaption, DWORD dwTotalStyle,const xml_node &node)
{
	CFont* pFont = CCollectiveComponentProvider::Instance().GetFont();
	CBusinessEdit* pEdit = new CBusinessEdit;
	CRect rc(data.m_nLeft,data.m_nTop,data.m_nLeft + data.m_nWidth ,data.m_nTop + data.m_nHeight);
	pEdit->Create(dwTotalStyle,rc,m_pParent,data.m_nID);	
	pEdit->SetFont(pFont);
	pEdit->ShowWindow(SW_SHOW);
	pEdit->SetWindowText(sCaption.c_str());
	const string& sBusiness = node.attribute("business").as_string();
	pEdit->m_sBusinessField = sBusiness;
	m_mapBusiness2Control[sBusiness] = pEdit;
	m_mapUIName2Wnd[sName] = pEdit;	
}


IMPLEMENT_FACTORY(IControlCreator,CControlCreator_CBusinessComboBox,string,"CBusinessComboBox")
void CControlCreator_CBusinessComboBox::Create(const CUIData &data,const string& sName, const string &sCaption, DWORD dwTotalStyle,const xml_node &node)
{
	CFont* pFont = CCollectiveComponentProvider::Instance().GetFont();
	CBusinessComboBox* pCombox = new CBusinessComboBox;
	CRect rc(data.m_nLeft,data.m_nTop,data.m_nLeft + data.m_nWidth ,data.m_nTop + data.m_nHeight);
	pCombox->Create(dwTotalStyle,rc,m_pParent,data.m_nID);	
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
		CField* pFieldMeaning = pRecord->GetField("Meaning");
		if(!pFieldMeaning) continue;

		string sTranslation = pFieldTranslation->GetValueAsString();
		string sMeaning = pFieldMeaning->GetValueAsString();
		pCombox->AddString(sTranslation.c_str());
		pCombox->Add2Map(nValue,sMeaning,sTranslation);
	}

	const string& sBusiness = node.attribute("business").as_string();
	pCombox->m_sBusinessField = sBusiness;

	pCombox->SetFont(pFont);
	pCombox->ShowWindow(SW_SHOW);
	pCombox->SetWindowText(sCaption.c_str());
	m_mapBusiness2Control[sBusiness] = pCombox;
	m_mapUIName2Wnd[sName] = pCombox;	
}


IMPLEMENT_FACTORY(IControlCreator,CControlCreator_CBusinessCheckBox,string,"CBusinessCheckBox")
void CControlCreator_CBusinessCheckBox::Create(const CUIData &data,const string& sName, const string &sCaption, DWORD dwTotalStyle,const xml_node &node)
{
	CFont* pFont = CCollectiveComponentProvider::Instance().GetFont();
	CBusinessCheckBox* pCheckBox = new CBusinessCheckBox;
	CRect rc(data.m_nLeft,data.m_nTop,data.m_nLeft + data.m_nWidth ,data.m_nTop + data.m_nHeight);	
	pCheckBox->Create(sCaption.c_str(), dwTotalStyle,rc,m_pParent, data.m_nID);
	pCheckBox->SetFont(pFont);
	const string& sBusiness = node.attribute("business").as_string();
	pCheckBox->m_sBusinessField = sBusiness;
	m_mapBusiness2Control[sName] = pCheckBox;
	m_mapUIName2Wnd[sName] = pCheckBox;	
}


IMPLEMENT_FACTORY(IControlCreator,CControlCreator_CEditTreeCtrlEx,string,"CEditTreeCtrlEx")
void CControlCreator_CEditTreeCtrlEx::Create(const CUIData &data,const string& sName, const string &sCaption, DWORD dwTotalStyle,const xml_node &node)
{
	CFont* pFont = CCollectiveComponentProvider::Instance().GetFont();
	CEditTreeCtrlEx* pTree = new CEditTreeCtrlEx;
	CRect rcTree(data.m_nLeft,data.m_nTop,data.m_nLeft + data.m_nWidth ,data.m_nTop + data.m_nHeight);
	pTree->Create(dwTotalStyle,rcTree,m_pParent,data.m_nID);		
	pTree->SetFont(pFont);
	pTree->ShowWindow(SW_SHOW);
	m_mapUIName2Wnd[sName] = pTree;	
}

