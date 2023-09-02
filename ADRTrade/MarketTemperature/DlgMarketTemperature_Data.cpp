// DlgSEMLog.cpp : 实现文件
//
#include "stdafx.h"
#include "DlgMarketTemperature_Data.h"
#include "Factory.h"
#include "FieldMeta.h"
#include "DBDataManager.h"
#include <algorithm>
#include "Util.h"
#include "GirdBehaviour/GridCellBehaviour.h"
#include "Tools/CustomButton.h"


IMPLEMENT_FACTORY(CDialogPlaceHolder,CGridPage_MarketTemperatureRecord, string,"CGridPage_MarketTemperatureRecord")

IMPLEMENT_DYNAMIC(CGridPage_MarketTemperatureRecord, CDlgGridPage)
CGridPage_MarketTemperatureRecord::CGridPage_MarketTemperatureRecord(CWnd* pParent /*=NULL*/)
	: CDlgGridPage(pParent),m_pSelectRecord(NULL),m_strSelectTemplate("")
{
	m_bEditable  = FALSE;
	m_bSingleSelMode = TRUE;
	CDialogIDMgr::Instance().Register("CGridPage_MarketTemperatureRecord",CDlgGridPage::IDD);
}

CGridPage_MarketTemperatureRecord::~CGridPage_MarketTemperatureRecord()
{
	m_ds.Clear();
}


BOOL CGridPage_MarketTemperatureRecord::OnInitDialog()
{
	CDialogPlaceHolder::OnInitDialog();
	_InitializeGrid();
	m_Grid.EnableSelection(TRUE);
	m_Grid.SetSingleRowSelection(TRUE);

	const string& sTableName = "Stock_MarketTemperature";
	_LoadData(sTableName);

	map<string,CFieldDesc*> &mapFieldName2FieldDesc = CDBDataManager::Instance().GetTableMeta(sTableName);

	ShowData(mapFieldName2FieldDesc,&m_ds);
	m_Grid.SetEditable(FALSE);
	return TRUE;
}


BOOL CGridPage_MarketTemperatureRecord::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if (wParam == (WPARAM)m_Grid.GetDlgCtrlID())
	{
		*pResult = 1;
		NM_GRIDVIEW* nmgv = (NM_GRIDVIEW*)(lParam);
		int nRow = nmgv->iRow;
		int nColumn = nmgv->iColumn;
		if (nRow < 1)
		{
			return FALSE;
		}
		m_pSelectRecord = m_ds[nRow - 1];
		ASSERT(m_pSelectRecord);
	}

	return CDialog::OnNotify(wParam, lParam, pResult);
}


void CGridPage_MarketTemperatureRecord::_LoadData(const string& sTableName) 
{
	char szSQL[256] = {0};
	sprintf_s(szSQL,256,"select * from %s",sTableName.c_str());
	CDBDataManager::Instance().LoadData(string(szSQL),sTableName,m_ds);
}


void CGridPage_MarketTemperatureRecord::_DesignLayout()
{
	_MoveGrid();
}

void CGridPage_MarketTemperatureRecord::_MoveGrid()
{
	CRect rc;
	CDialogPlaceHolder::GetClientRect(rc);

	CWnd* pCtrl = CDialogPlaceHolder::GetDlgItem(IDC_Grid);
	if(!pCtrl) return;
	pCtrl->MoveWindow(rc);
}


static bool Comp(CFieldDesc* pLeft,CFieldDesc* pRight)
{
	if(!pLeft || !pRight) return false;
	return pLeft->m_nSequenceID < pRight->m_nSequenceID;
}


void CGridPage_MarketTemperatureRecord::GetCellData( CDataSet* pDs, vector<CFieldDesc*>& vColumn,int row, int col, CString &str )
{
	CFieldDesc* pFieldDesc = vColumn[col];
	if(!pFieldDesc) return;

	CRecord* pRecord = pDs->GetAt(row - 1);
	if(!pRecord) return;
	CField* pField = pRecord->GetField(pFieldDesc->m_strFieldName);
	if(!pField) return;

	GridCellBehaviour* pBehaviour = Factory<GridCellBehaviour,string>::Instance().BuildProduct(pFieldDesc->m_strDataType);
	if (pBehaviour)
	{
		pBehaviour->Attach(&m_Grid);
		pBehaviour->Behave(row,col,pField,str);
	}
	else
	{
		pBehaviour = new GridCellBehaviour();
		pBehaviour->Attach(&m_Grid);
		pBehaviour->Behave(row,col,pField,str);
	}

	delete pBehaviour;
	pBehaviour= NULL;
}



void CGridPage_MarketTemperatureRecord::ShowData( map<string,CFieldDesc*> &mapFieldName2FieldDesc, CDataSet* pDs )
{
	vector<CFieldDesc*> vColumn;
	for (map<string,CFieldDesc*>::iterator it = mapFieldName2FieldDesc.begin(); it != mapFieldName2FieldDesc.end();it++)
	{
		CFieldDesc* pDesc = it->second;
		if (pDesc && pDesc->GetAttributeInt("Show"))
		{
			vColumn.push_back(it->second);
		}	
	}

	::sort(vColumn.begin(),vColumn.end(),Comp);

	m_nFixCols = 0;
	m_nFixRows = 1;
	m_nCols = vColumn.size();
	m_nRows = pDs->Size() + 1;

	m_Grid.SetAutoSizeStyle();

	TRY {
		m_Grid.SetRowCount(m_nRows);
		m_Grid.SetColumnCount(m_nCols);
		m_Grid.SetFixedRowCount(m_nFixRows);
		m_Grid.SetFixedColumnCount(m_nFixCols);
	}
	CATCH (CMemoryException, e)
	{
		e->ReportError();
		return;
	}
	END_CATCH

	for (int row = 0; row < m_Grid.GetRowCount(); row++)
	{
		for (int col = 0; col < m_Grid.GetColumnCount(); col++)
		{ 
			CFieldDesc* pFieldDesc = vColumn[col];
			if (!pFieldDesc) continue;

			CString str;
			GV_ITEM Item;

			Item.mask = GVIF_TEXT;
			Item.row = row;
			Item.col = col;

			if (row < m_nFixRows && col < m_nFixCols)
			{
				str.Format(_T("%s"),_T(""));
			}
			else if (row < m_nFixRows && col >= m_nFixCols)
			{				
				int nColumnWidth = pFieldDesc->GetAttributeInt("ColumnWidth");
				std::string sHeader = pFieldDesc->GetAttributeString("Meaning");
				m_Grid.SetColumnWidth(col,nColumnWidth);
				str.Format(_T("%s"),sHeader.c_str());
			}
			else if (col < m_nFixCols && row >= m_nFixRows)
			{
				str.Format(_T("%d"), row);
			}
			else if (row >= m_nFixRows && col >= m_nFixCols)
			{
				GetCellData(pDs,vColumn,row,col,str);	
			}

			Item.strText = str;
			m_Grid.SetItemState(row,col, m_Grid.GetItemState(row,col) & ~GVIS_READONLY);			

			m_Grid.SetItem(&Item);
		}
	}

	m_Grid.Invalidate();
	CDialogPlaceHolder::UpdateData(FALSE);
}

//屏蔽 CDialogPlaceHolder 的部分功能
void CGridPage_MarketTemperatureRecord::_InitLayOut()
{
}

//屏蔽 CDialogPlaceHolder 的部分功能
void CGridPage_MarketTemperatureRecord::_LoadData2UI()
{
}