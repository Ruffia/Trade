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

IMPLEMENT_DYNAMIC(CGridPage_MarketTemperatureRecord, CDlgGridPage)

CGridPage_MarketTemperatureRecord::CGridPage_MarketTemperatureRecord(CWnd* pParent /*=NULL*/)
	: CDlgGridPage(pParent),m_pSelectRecord(NULL),m_strSelectTemplate("")
{
	m_bHorzLines = TRUE;
	m_bVertLines = TRUE;
	m_bVertical = FALSE;
	m_bEditable  = FALSE;
	m_bSingleSelMode = TRUE;
}

CGridPage_MarketTemperatureRecord::~CGridPage_MarketTemperatureRecord()
{
	m_ds.Clear();
}


BEGIN_MESSAGE_MAP(CGridPage_MarketTemperatureRecord, CDlgGridPage)
	ON_WM_SIZE()
END_MESSAGE_MAP()


BOOL CGridPage_MarketTemperatureRecord::OnInitDialog()
{
	CDlgGridPage::OnInitDialog();
	_InitializeGrid();
	m_Grid.EnableSelection(TRUE);
	m_Grid.SetSingleRowSelection(TRUE);

	const string& sTableName = "DocumentTemplate";
	_LoadData(sTableName);

	map<string,CFieldDesc*> &mapFieldName2FieldDesc = CDBDataManager::Instance().GetTableMeta(sTableName);

	ShowData(mapFieldName2FieldDesc,&m_ds);
	m_Grid.SetEditable(m_bEditable);
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
		CField* pField = m_pSelectRecord->GetField("DocumentTemplate");
		ASSERT(pField);
		m_strSelectTemplate = pField->GetValueAsString();
	}

	return CDialog::OnNotify(wParam, lParam, pResult);
}


void CGridPage_MarketTemperatureRecord::_LoadData(const string& sTableName) 
{
	char szSQL[256] = {0};
	sprintf_s(szSQL,256,"select * from %s",sTableName.c_str());
	CDBDataManager::Instance().LoadData(string(szSQL),sTableName,m_ds);
}

void CGridPage_MarketTemperatureRecord::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType,cx,cy);
	_DesignLayout();
}

void CGridPage_MarketTemperatureRecord::_DesignLayout()
{
	_MoveGrid();
}

void CGridPage_MarketTemperatureRecord::_MoveGrid()
{
	CRect rc;
	GetClientRect(rc);

	CWnd* pCtrl = GetDlgItem(IDC_Grid);
	if(!pCtrl) return;
	pCtrl->MoveWindow(rc);
}


static bool Comp(CFieldDesc* pLeft,CFieldDesc* pRight)
{
	if(!pLeft || !pRight) return false;
	return pLeft->GetAttributeInt("SortID") < pRight->GetAttributeInt("SortID");
}


void CGridPage_MarketTemperatureRecord::GetCellData( CDataSet* pDs, vector<CFieldDesc*>& vColumn,int row, int col, CString &str )
{
	CFieldDesc* pFieldDesc = vColumn[col];
	if(!pFieldDesc) return;

	CRecord* pRecord = pDs->GetAt(row - 1);
	if(!pRecord) return;
	CField* pField = pRecord->GetField(pFieldDesc->m_strFieldName);
	if(!pField) return;

	GridCellBehaviour* pBehaviour = Factory<GridCellBehaviour,string>::Instance().BuildProduct(pFieldDesc->m_strFieldName);
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

	const int   nColumnWidth[2] = {120,322};
	const string sGridHeader[2] = {"模板","涵义"};

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
				m_Grid.SetColumnWidth(col,nColumnWidth[col]);
				str.Format(_T("%s"),sGridHeader[col].c_str());
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
	UpdateData(FALSE);
}