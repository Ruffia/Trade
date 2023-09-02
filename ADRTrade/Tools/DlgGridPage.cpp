// DlgSEMLog.cpp : 实现文件
//
#include "stdafx.h"
#include "DlgGridPage.h"
#include "GridCell.h"
#include "GridURLCell.h"
#include "GridCellCombo.h"
#include "GridCellCheck.h"
#include "GridCellDateTime.h"
#include "GridCellNumeric.h"


IMPLEMENT_DYNAMIC(CDlgGridPage, CDialog)
CDlgGridPage::CDlgGridPage(CWnd* pParent /*=NULL*/)
	: CDialogPlaceHolder(pParent)
{
	m_OldSize = CSize(-1,-1);

	//{{AFX_DATA_INIT(CGridCtrlDemoDlg)
	m_nFixCols = 1;
	m_nFixRows = 1;
	m_nCols = 8;
	m_nRows = 20;
	m_bEditable = FALSE;
	m_bHorzLines = FALSE;
	m_bVertLines = FALSE;
	m_bListMode = TRUE;
	m_bHeaderSort = FALSE;
	m_bSingleSelMode = TRUE;
	m_bSingleColSelMode = TRUE;
	m_bSelectable = FALSE;
	m_bAllowColumnResize = FALSE;
	m_bAllowRowResize = FALSE;
	m_bItalics = FALSE;
	m_btitleTips = FALSE;
	m_bTrackFocus = FALSE;
	m_bFrameFocus = FALSE;
	//m_bVirtualMode = TRUE;
	m_bCallback = TRUE;
	m_bVertical = TRUE;
	m_bExpandUseFixed = FALSE;
	m_bRejectEditAttempts = TRUE;
	m_bRejectEditChanges = TRUE;
	m_bRow2Col2Hidden = FALSE;

}

CDlgGridPage::~CDlgGridPage()
{
}

void CDlgGridPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Grid, m_Grid); 
}


// CDlgSEMLog 消息处理程序
BOOL CDlgGridPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	_InitializeGrid();
	UpdateData(FALSE);

	return TRUE;
}


void CDlgGridPage::OnUpdateEditCols() 
{
	if (!::IsWindow(m_Grid.m_hWnd)) return;
	UpdateData();

	int nOldNumCols = m_Grid.GetColumnCount();

	TRY { 
		m_Grid.SetColumnCount(m_nCols); 
	}
	CATCH (CMemoryException, e)
	{
		e->ReportError();
		return;
	}
	END_CATCH

		m_nCols = m_Grid.GetColumnCount();
	m_nFixCols = m_Grid.GetFixedColumnCount();
	UpdateData(FALSE);

	CString str;
	for (int i = nOldNumCols; i < m_nCols; i++)
	{
		str.Format(_T("Column %d"), i);
		m_Grid.SetItemText(0,i,str);
	}
}

void CDlgGridPage::OnUpdateEditFixcols() 
{
	if (!::IsWindow(m_Grid.m_hWnd)) return;
	UpdateData();

	TRY {
		m_Grid.SetFixedColumnCount(m_nFixCols); 
	}
	CATCH (CMemoryException, e)
	{
		e->ReportError();
		return;
	}
	END_CATCH

		m_nCols = m_Grid.GetColumnCount();
	m_nFixCols = m_Grid.GetFixedColumnCount();
	UpdateData(FALSE);
}

void CDlgGridPage::OnUpdateEditFixrows() 
{
	if (!::IsWindow(m_Grid.m_hWnd)) return;
	UpdateData();

	TRY {
		m_Grid.SetFixedRowCount(m_nFixRows); 
	}
	CATCH (CMemoryException, e)
	{
		e->ReportError();
		return;
	}
	END_CATCH

	m_nRows = m_Grid.GetRowCount();
	m_nFixRows = m_Grid.GetFixedRowCount();
	UpdateData(FALSE);
}

void CDlgGridPage::OnUpdateEditRows() 
{	
	if (!::IsWindow(m_Grid.m_hWnd)) return;
	UpdateData();

	int nOldNumRows = m_Grid.GetRowCount();

	TRY {
		m_Grid.SetRowCount(m_nRows); 
	}
	CATCH (CMemoryException, e)
	{
		e->ReportError();
		return;
	}
	END_CATCH

		m_nRows = m_Grid.GetRowCount();
	m_nFixRows = m_Grid.GetFixedRowCount();
	UpdateData(FALSE);

	CString str;
	for (int i = nOldNumRows; i < m_nRows; i++)
	{
		str.Format(_T("Row %d"), i);
		m_Grid.SetItemText(i,0,str);
	}

	m_Grid.EnsureVisible(m_nRows-1,0);
}

void CDlgGridPage::OnGridLines() 
{
	if (!m_bHorzLines && !m_bVertLines)
		m_Grid.SetGridLines(GVL_NONE);
	else if (m_bHorzLines && !m_bVertLines)
		m_Grid.SetGridLines(GVL_HORZ);
	else if (!m_bHorzLines && m_bVertLines)
		m_Grid.SetGridLines(GVL_VERT);
	else 
		m_Grid.SetGridLines(GVL_BOTH);

	
}

void CDlgGridPage::OnVertGridLines() 
{
	m_bVertLines = !m_bVertLines;
	OnGridLines();
}

void CDlgGridPage::OnHorzGridLines() 
{
	m_bHorzLines = !m_bHorzLines;
	OnGridLines();
}

void CDlgGridPage::OnListmode() 
{
	m_bListMode = !m_bListMode;
	m_Grid.SetListMode(m_bListMode);
	
}

void CDlgGridPage::OnHeaderSort() 
{
	m_bHeaderSort = !m_bHeaderSort;
	m_Grid.SetHeaderSort(m_bHeaderSort);
	
}

void CDlgGridPage::OnSingleselmode() 
{
	m_bSingleSelMode = !m_bSingleSelMode;
	m_Grid.SetSingleRowSelection(m_bSingleSelMode);
	
}

void CDlgGridPage::OnSingleColMode() 
{
	m_bSingleColSelMode = !m_bSingleColSelMode;
	m_Grid.SetSingleColSelection(m_bSingleColSelMode);
	
}

void CDlgGridPage::OnEditable() 
{
	m_bEditable = !m_bEditable;
	m_Grid.SetEditable(m_bEditable);
	
}

void CDlgGridPage::OnAllowSelection() 
{
	m_bSelectable = !m_bSelectable;
	m_Grid.EnableSelection(m_bSelectable);
	
}

void CDlgGridPage::OnRowResize() 
{
	m_bAllowRowResize = !m_bAllowRowResize;
	m_Grid.SetRowResize(m_bAllowRowResize);
	
}

void CDlgGridPage::OnColResize() 
{
	m_bAllowColumnResize = !m_bAllowColumnResize;
	m_Grid.SetColumnResize(m_bAllowColumnResize);
	
}

void CDlgGridPage::OnPrintButton() 
{
#if !defined(WCE_NO_PRINTING) && !defined(GRIDCONTROL_NO_PRINTING)
	m_Grid.Print();
#endif
}

void CDlgGridPage::OnFontButton() 
{
#ifndef _WIN32_WCE
	LOGFONT lf;
	m_Grid.GetFont()->GetLogFont(&lf);

	CFontDialog dlg(&lf);
	if (dlg.DoModal() == IDOK) {
		dlg.GetCurrentFont(&lf);

		CFont Font;
		Font.CreateFontIndirect(&lf);
		m_Grid.SetFont(&Font);
		OnItalics();	
		m_Grid.AutoSize();
		Font.DeleteObject();	
	}
#endif
}

static BOOL CALLBACK EnumProc(HWND hwnd, LPARAM lParam)
{
	CWnd* pWnd = CWnd::FromHandle(hwnd);
	CSize* pTranslate = (CSize*) lParam;

	CDlgGridPage* pDlg = (CDlgGridPage*) pWnd->GetParent();
	if (!pDlg) return FALSE;

	CRect rect;
	pWnd->GetWindowRect(rect);
	//if (hwnd == pDlg->m_Grid.GetSafeHwnd())
	//	TRACE(_T("Wnd rect: %d,%d - %d,%d\n"),rect.left,rect.top, rect.right, rect.bottom);

	pDlg->ScreenToClient(rect);
	//if (hwnd == pDlg->m_Grid.GetSafeHwnd())
	//	TRACE(_T("Scr rect: %d,%d - %d,%d\n"),rect.left,rect.top, rect.right, rect.bottom);

	if (hwnd == pDlg->m_Grid.GetSafeHwnd())
	{
		if (  ((rect.top >= 7 && pTranslate->cy > 0) || rect.Height() > 20) &&
			((rect.left >= 7 && pTranslate->cx > 0) || rect.Width() > 20)   )
			pWnd->MoveWindow(rect.left, rect.top, 
			rect.Width()+pTranslate->cx, 
			rect.Height()+pTranslate->cy, FALSE);
		else
			pWnd->MoveWindow(rect.left+pTranslate->cx, rect.top+pTranslate->cy, 
			rect.Width(), rect.Height(), FALSE);
	}
	//else if (hwnd == pDlg->m_TraceWnd.GetSafeHwnd())
	//{
	//	if (  ((rect.top >= 7 && pTranslate->cy > 0) || rect.Height() > 20) &&
	//		  ((rect.left >= 7 && pTranslate->cx > 0) || rect.Width() > 20)   )
	//		pWnd->MoveWindow(rect.left, rect.top+pTranslate->cy, 
	//						 rect.Width()+pTranslate->cx, 
	//						 rect.Height(), FALSE);
	//	else
	//		pWnd->MoveWindow(rect.left+pTranslate->cx, rect.top+pTranslate->cy, 
	//						 rect.Width(), rect.Height(), FALSE);
	//}
	else 
	{
		//if (pWnd->GetDlgCtrlID() == IDC_SIZEBOX)
			pWnd->MoveWindow(rect.left+pTranslate->cx, rect.top+pTranslate->cy, 
			rect.Width(), rect.Height(), FALSE);
		//else
			pWnd->MoveWindow(rect.left+pTranslate->cx, rect.top, rect.Width(), rect.Height(), FALSE);
	}
	pDlg->Invalidate();

	return TRUE;
}


#ifndef GRIDCONTROL_NO_CLIPBOARD
void CDlgGridPage::OnEditCopy() 
{
	m_Grid.OnEditCopy();	
}

void CDlgGridPage::OnEditCut() 
{
	m_Grid.OnEditCut();	
}

void CDlgGridPage::OnEditPaste() 
{
	m_Grid.OnEditPaste();	
}

void CDlgGridPage::OnUpdateEditCopyOrCut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_Grid.GetSelectedCount() > 0);
}

void CDlgGridPage::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	// Attach a COleDataObject to the clipboard see if there is any data
	COleDataObject obj;
	pCmdUI->Enable(obj.AttachClipboard() && obj.IsDataAvailable(CF_TEXT)); 
}
#endif

void CDlgGridPage::OnEditSelectall() 
{
	m_Grid.OnEditSelectAll();
}


void CDlgGridPage::OnItalics() 
{
	m_bItalics = !m_bItalics;

	CFont *pFont = m_Grid.GetFont();
	if (!pFont) 
		return;

	LOGFONT lf;
	pFont->GetLogFont(&lf);
	lf.lfItalic = (BYTE) m_bItalics;

	m_Grid.GetDefaultCell(TRUE, FALSE)->SetFont(&lf);
	m_Grid.GetDefaultCell(FALSE, TRUE)->SetFont(&lf);
	m_Grid.GetDefaultCell(TRUE, TRUE)->SetFont(&lf);

	m_Grid.Invalidate();
	m_Grid.AutoSizeColumns(GVS_HEADER);

	
}

void CDlgGridPage::OnTitletips() 
{
	m_btitleTips = !m_btitleTips;
	m_Grid.EnableTitleTips(m_btitleTips);
	
}

void CDlgGridPage::OnInsertRow() 
{
	int nRow = m_Grid.GetFocusCell().row;
	if (nRow >= 0)
	{
		m_Grid.InsertRow(_T("Newest Row"), nRow);	
		m_Grid.Invalidate();
	}
}

void CDlgGridPage::OnDeleteRow() 
{
	int nRow = m_Grid.GetFocusCell().row;
	if (nRow >= 0)
	{
		m_Grid.DeleteRow(nRow);	
		m_Grid.Invalidate();
	}
}

// (Thanks to Koay Kah Hoe for this)
BOOL CDlgGridPage::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_RETURN
			|| pMsg->wParam == VK_ESCAPE )
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;                    // DO NOT process further
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}	

// NM_RCLICK
void CDlgGridPage::OnGridRClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	Trace(_T("Right button click on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
}

// GVN_BEGINLABELEDIT
void CDlgGridPage::OnGridStartEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	Trace(_T("Start Edit on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
	*pResult = (m_bRejectEditAttempts)? -1 : 0;
}

// GVN_ENDLABELEDIT
void CDlgGridPage::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	Trace(_T("End Edit on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
	*pResult = (m_bRejectEditChanges)? -1 : 0;
}

// GVN_SELCHANGING
void CDlgGridPage::OnGridStartSelChange(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	Trace(_T("Start Selection Change on row %d, col %d (%d Selected)\n"), 
		pItem->iRow, pItem->iColumn, m_Grid.GetSelectedCount());
}

// GVN_SELCHANGED
void CDlgGridPage::OnGridEndSelChange(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	Trace(_T("End Selection Change on row %d, col %d (%d Selected)\n"), 
		pItem->iRow, pItem->iColumn, m_Grid.GetSelectedCount());
}

// GVN_BEGINDRAG
void CDlgGridPage::OnGridBeginDrag(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	Trace(_T("Begin Drag row %d, col %d\n"), pItem->iRow, pItem->iColumn);
}

void CDlgGridPage::OnGridDblClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	Trace(_T("Double Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
}

void CDlgGridPage::OnGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	Trace(_T("Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
}

void CDlgGridPage::OnExpandToFit() 
{
	m_Grid.ExpandToFit(m_bExpandUseFixed);
}

void CDlgGridPage::OnAutoSize() 
{
	m_Grid.AutoSize();
}

void CDlgGridPage::OnFill() 
{
	m_Grid.DeleteAllItems();
	m_Grid.AutoFill();
}

void CDlgGridPage::OnCellNormal() 
{
	m_Grid.SetCellType(1,1, RUNTIME_CLASS(CGridCell));
	m_Grid.SetItemText(1,1, _T("1"));
	m_Grid.SetItemState(1,1, m_Grid.GetItemState(1,1) & ~GVIS_READONLY);
	m_Grid.Invalidate();
}

void CDlgGridPage::OnCellReadonly() 
{
	OnCellNormal();

	m_Grid.SetItemState(1,1, m_Grid.GetItemState(1,1) | GVIS_READONLY);
}

void CDlgGridPage::OnCellCheckbox()
{
	OnCellNormal();

	m_Grid.SetCellType(1,1, RUNTIME_CLASS(CGridCellCheck));
}

void CDlgGridPage::OnCellDateTime()
{
	OnCellNormal();

	m_Grid.SetCellType(1,1, RUNTIME_CLASS(CGridCellDateTime));

	CGridCellDateTime* pCell = (CGridCellDateTime*) m_Grid.GetCell(1,1);
	pCell->SetTime(CTime::GetCurrentTime());
	m_Grid.RedrawCell(1,1);

	m_Grid.SetColumnWidth(1,100);
}

void CDlgGridPage::OnCellUrl() 
{
	OnCellNormal();

	if (!m_Grid.SetCellType(1,1, RUNTIME_CLASS(CGridURLCell)))
		return;
	m_Grid.SetItemText(1,1, _T("www.codeproject.com"));
}

void CDlgGridPage::OnCellCombo() 
{
	OnCellNormal();

	if (!m_Grid.SetCellType(1,1, RUNTIME_CLASS(CGridCellCombo)))
		return;

	m_Grid.SetItemText(1,1, _T("Option 1"));

	CStringArray options;
	options.Add(_T("Option 1"));
	options.Add(_T("Option 2"));
	options.Add(_T("Option 3"));

	CGridCellCombo *pCell = (CGridCellCombo*) m_Grid.GetCell(1,1);
	pCell->SetOptions(options);
	pCell->SetStyle(CBS_DROPDOWN); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE
}

void CDlgGridPage::OnCellNumeric() 
{
	OnCellNormal();

	if (!m_Grid.SetCellType(1,1, RUNTIME_CLASS(CGridCellNumeric)))
		return;

	m_Grid.SetItemText(1,1, _T("12345"));
}


void CDlgGridPage::OnTrackfocus() 
{
	m_bTrackFocus = !m_bTrackFocus;
	m_Grid.SetTrackFocusCell(m_bTrackFocus);
	m_Grid.Invalidate();
	
}

void CDlgGridPage::OnFramefocus() 
{
	m_bFrameFocus = !m_bFrameFocus;
	m_Grid.SetFrameFocusCell(m_bFrameFocus);
	m_Grid.Invalidate();
	
}

void CDlgGridPage::OnRejectEditAttempts() 
{
	m_bRejectEditAttempts = !m_bRejectEditAttempts;
	
}

void CDlgGridPage::OnRejectEditChanges() 
{
	m_bRejectEditChanges = !m_bRejectEditChanges;
	
}

void CDlgGridPage::OnExpandLast() 
{
	m_Grid.ExpandLastColumn();	
}

void CDlgGridPage::OnSetFocus11() 
{
	CCellID cell(1,1);

	m_Grid.SetFocusCell(cell);

	if (m_Grid.GetListMode())
		m_Grid.SetSelectedRange(1, m_Grid.GetFixedColumnCount(), 1, m_Grid.GetColumnCount()-1, TRUE);
	else
		m_Grid.SetSelectedRange(1,1,1,1, TRUE);
}

void CDlgGridPage::OnCallbackFunction() 
{
	m_bCallback = !m_bCallback;
	m_Grid.SetCallbackFunc(m_bCallback? GridCallback : NULL, 0);
	m_Grid.AllowReorderColumn(m_bCallback!=0); // implemented now only if m_bCallback
	m_Grid.EnableDragRowMode(m_bCallback!=0);
	m_Grid.Refresh();
	
}

bool CDlgGridPage::VirtualCompare(int c1, int c2)
{
	//CGridCtrl *pThis = CGridCtrl::m_This; // Mmm, in big virtual mode you must well optimize this function
	//int col = pThis->m_CurCol;            // a first version with CStrings was catastrophic....
	return (c1<c2); // strange order, whatever the column number...
}

void CDlgGridPage::OnVirtualMode() 
{
	UpdateData();

	m_Grid.SetEditable(m_bEditable);

	m_bHeaderSort = m_Grid.GetHeaderSort();

	{
		m_nFixCols = 1;
		m_nFixRows = 1;
		m_nCols = 6;
		m_nRows = 16;

		m_Grid.SetAutoSizeStyle();

		m_bEditable = m_Grid.IsEditable();
		m_Grid.SetCallbackFunc(m_bCallback? GridCallback : NULL, 0);

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

			// fill rows/cols with text
			for (int row = 0; row < m_Grid.GetRowCount(); row++)
			{
				for (int col = 0; col < m_Grid.GetColumnCount(); col++)
				{ 
					CString str;

					GV_ITEM Item;

					Item.mask = GVIF_TEXT;
					Item.row = row;
					Item.col = col;

					if (row < m_nFixRows)
						str.Format(_T("Column %d"),col);
					else if (col < m_nFixCols) 
						str.Format(_T("Row %d"), row);
					else 
						str.Format(_T("%d"),row*col);
					Item.strText = str;

					if (col < m_Grid.GetFixedColumnCount())
					{
						//Item.iImage = rand()%m_ImageList.GetImageCount();
						Item.mask  |= (GVIF_IMAGE);
					}

					m_Grid.SetItem(&Item);
				}
			}
	}

	//m_Grid.GetDefaultCell(FALSE,FALSE)->SetFormat(DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX|DT_END_ELLIPSIS);
	//m_Grid.GetDefaultCell(TRUE, FALSE)->SetFormat(DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX|DT_END_ELLIPSIS);

	// Having weird Autosize problems with CE 2.11 - possibly just an emulation problem
#if !defined(_WIN32_WCE) || (_WIN32_WCE < 211)
	m_Grid.AutoSize();
#endif

	

	UpdateData(FALSE);
}

BOOL CDlgGridPage::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if (wParam == (WPARAM)m_Grid.GetDlgCtrlID())
	{
		*pResult = 1;
		GV_DISPINFO *pDispInfo = (GV_DISPINFO*)lParam;
		if (GVN_GETDISPINFO == pDispInfo->hdr.code)
		{
			//TRACE2("Getting Display info for cell %d,%d\n", pDispInfo->item.row, pDispInfo->item.col);
			pDispInfo->item.strText.Format(_T("Message %d,%d"),pDispInfo->item.row, pDispInfo->item.col);
			return TRUE;
		}
		else if (GVN_ODCACHEHINT == pDispInfo->hdr.code)
		{
			GV_CACHEHINT *pCacheHint = (GV_CACHEHINT*)pDispInfo;
			Trace(_T("Cache hint received for cell range %d,%d - %d,%d\n"),
				pCacheHint->range.GetMinRow(), pCacheHint->range.GetMinCol(),
				pCacheHint->range.GetMaxRow(), pCacheHint->range.GetMaxCol());
		}
	}

	return CDialog::OnNotify(wParam, lParam, pResult);
}

BOOL CALLBACK CDlgGridPage::GridCallback(GV_DISPINFO *pDispInfo, LPARAM /*lParam*/) 
{
	pDispInfo->item.strText.Format(_T("Callback %d,%d"),pDispInfo->item.row, pDispInfo->item.col);
	return TRUE;
}

void CDlgGridPage::OnFixedRowSelection() 
{
	m_Grid.SetFixedRowSelection(!m_Grid.GetFixedRowSelection());
	
}

void CDlgGridPage::OnFixedColSelection() 
{
	m_Grid.SetFixedColumnSelection(!m_Grid.GetFixedColumnSelection());
	
}

void CDlgGridPage::OnExpandUseFixed() 
{
	m_bExpandUseFixed = !m_bExpandUseFixed;
	
}

void CDlgGridPage::OnVertical() 
{
	m_bVertical = !m_bVertical;

	// Set reasonable format
	//m_Grid.GetDefaultCell(TRUE, FALSE)->SetFormat(DT_SINGLELINE|DT_NOPREFIX|DT_END_ELLIPSIS|DT_LEFT|DT_BOTTOM);
	//m_Grid.GetDefaultCell(TRUE, TRUE)->SetFormat(DT_SINGLELINE|DT_NOPREFIX|DT_END_ELLIPSIS|DT_LEFT|DT_BOTTOM);

	// Get the font
	CFont* pFont = m_Grid.GetFont();
	LOGFONT lf;
	pFont->GetLogFont(&lf);

	// Make it true-type
	memcpy(lf.lfFaceName, _T("Arial"), 6);

	// Vertical or horizontal
	if (m_bVertical)
	{
		lf.lfEscapement = 900;
		lf.lfOrientation = 900;
	}
	else
	{
		lf.lfEscapement = 0;
		lf.lfOrientation = 0;
	}

	// Set the fixed row defaults as this new font
	m_Grid.GetDefaultCell(TRUE, FALSE)->SetFont(&lf);

	// Set cell 1,1 as the more normal font so URL cells look OK
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	m_Grid.SetItemFont(0,1, &lf);


	m_Grid.AutoSize(GVS_HEADER);
	if (m_btitleTips)
	{
		m_btitleTips = !m_bVertical;
		m_Grid.EnableTitleTips(!m_bVertical);
	}

	
}

void CDlgGridPage::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if (pWnd->GetSafeHwnd() == m_Grid.GetSafeHwnd())
	{
		m_Grid.ScreenToClient(&point);
		CCellID cell = m_Grid.GetCellFromPt(point);
		Trace(_T("Context menu called on row %d, col %d\n"), cell.row, cell.col);
	}
}

void CDlgGridPage::Trace(LPCTSTR szFmt, ...)
{    
	CString str;

	// Format the message text
	va_list argptr;
	va_start(argptr, szFmt);
	str.FormatV(szFmt, argptr);
	va_end(argptr);

	str.Replace(_T("\n"), _T("\r\n"));

	CString strWndText;
	//m_TraceWnd.GetWindowText(strWndText);
	//strWndText += str;
	//m_TraceWnd.SetWindowText(strWndText);

	//	m_TraceWnd.SetSel(str.GetLength()-1, str.GetLength()-2, FALSE);
	//m_TraceWnd.LineScroll(-m_TraceWnd.GetLineCount());
	//m_TraceWnd.LineScroll(m_TraceWnd.GetLineCount()-4);
}

void CDlgGridPage::OnCleartrace() 
{
	//m_TraceWnd.SetWindowText(_T(""));
}


void CDlgGridPage::OnHide2ndrowcolumn()
{
	m_bRow2Col2Hidden = !m_bRow2Col2Hidden;

	if (m_bRow2Col2Hidden)
	{
		m_Grid.SetColumnWidth(2,0);
		m_Grid.SetRowHeight(2,0);
	}
	else
	{
		m_Grid.SetColumnWidth(2,1);
		m_Grid.AutoSizeColumn(2);
		m_Grid.SetRowHeight(2,1);
		m_Grid.AutoSizeRow(2);
	}
	m_Grid.Refresh();
	
}


void CDlgGridPage::_InitializeGrid()
{
	if (!m_Grid.m_hWnd) return;
	CRect rect;
	GetClientRect(rect);
	m_OldSize = CSize(rect.Width(), rect.Height());

	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xF0, 0xFF, 0xF0));

	OnEditable();
	OnVirtualMode();    // Sets the grid mode, fills the grid
	OnListmode();
	OnCellReadonly();
	OnItalics();
	OnTitletips();
	OnTrackfocus();
	OnFramefocus();
	OnRowResize();
	OnColResize();
	OnSingleselmode();
	OnSingleColMode();
	OnVertGridLines();
	OnHorzGridLines();
	OnAllowSelection();
	OnCallbackFunction();
	OnVertical();
	OnHeaderSort();
	OnExpandUseFixed();
	OnRejectEditAttempts();
	OnRejectEditChanges();

	m_Grid.SetFixedColumnSelection(TRUE);
	m_Grid.SetFixedRowSelection(TRUE);
	m_Grid.EnableColumnHide();

	m_Grid.AutoSize();

	m_Grid.SetCompareFunction(CGridCtrl::pfnCellNumericCompare);
}


void CDlgGridPage::_MoveControl(int nCtrlID,int nVeticalPix,int nHorizontalPix)
{
	CWnd* pCtrl = GetDlgItem(nCtrlID);
	if(!pCtrl) return;

	CRect rc;
	pCtrl->GetWindowRect(rc);
	rc.top +=  nVeticalPix;
	rc.bottom += nVeticalPix;
	rc.left += nHorizontalPix;
	rc.right += nHorizontalPix;
	ScreenToClient(rc);
	pCtrl->MoveWindow(rc);
}