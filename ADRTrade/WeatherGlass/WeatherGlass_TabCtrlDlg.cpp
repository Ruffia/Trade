#include "stdafx.h"
#include "ADRTrade.h"
#include "afxdialogex.h"
#include "Util.h"
#include "UIData.h"
#include "Factory.h"
#include "DialogIDManager.h"
#include "DBDataManager.h"
#include "WeatherGlass_TabCtrlDlg.h"
#include "TradeDayPrimaryData.h"
#include "ChartPageBase.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CWeatherGlass_TabCtrlDlg::CWeatherGlass_TabCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWeatherGlass_TabCtrlDlg::IDD, pParent),m_pTab(NULL),m_nCurSel(0)
{
	
}

CWeatherGlass_TabCtrlDlg::~CWeatherGlass_TabCtrlDlg()
{
	if(m_pTab)
	{
		delete m_pTab;
		m_pTab = NULL;
	}

	for (int i = 0; i < m_vPage.size();i++)
	{
		CDialog* pDlg = m_vPage[i];
		delete pDlg;
		pDlg = NULL;
	}

	m_vPage.clear();
}

void CWeatherGlass_TabCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWeatherGlass_TabCtrlDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_NOTIFY(CTCN_CLICK, IDC_TAB, OnLButtonClickedTab)
	ON_NOTIFY(CTCN_RCLICK, IDC_TAB, OnRButtonClickedTab)
	ON_NOTIFY(CTCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_NOTIFY(CTCN_HIGHLIGHTCHANGE, IDC_TAB, OnHighlightchangeTab)
	ON_NOTIFY(CTCN_ITEMMOVE, IDC_TAB, OnMoveitemTab)
	ON_NOTIFY(CTCN_ITEMCOPY, IDC_TAB, OnCopyitemTab)
	ON_NOTIFY(CTCN_LABELUPDATE, IDC_TAB, OnLabelupdateTab)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomTabCtrlDlg message handlers

BOOL CWeatherGlass_TabCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//移动窗口，并改变尺寸
	SetWindowPos(&wndTop, 130, 130, 860, 650, SWP_SHOWWINDOW);

	CRect rcClient;
	GetClientRect(rcClient);

	m_pTab = new CCustomTabCtrl;
	CRect rcTab(rcClient.left + 17,rcClient.top + 17,rcClient.Width() - 10,rcClient.Height() - 10);
	m_pTab->Create(WS_CHILD|WS_VISIBLE|CTCS_DRAGMOVE|CTCS_TOP|CTCS_EDITLABELS|CTCS_CLOSEBUTTON|CTCS_AUTOHIDEBUTTONS|CTCS_MULTIHIGHLIGHT|CTCS_DRAGCOPY|CTCS_TOP,rcTab,this,IDC_TAB);
	m_pTab->SetDragCursors(AfxGetApp()->LoadCursor(IDC_CURSORMOVE),AfxGetApp()->LoadCursor(IDC_CURSORCOPY));

	_InitPage(rcTab);

	m_nCurSel = 0;
	//显示初始页面
	_ShowPage(m_nCurSel);
	LOGFONT lf = {10, 0, 0, 0, FW_NORMAL, 0, 0, 0,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Courier"};

	m_pTab->SetControlFont(lf, TRUE);
	_Resize(rcClient.Width(),rcClient.Height());

	return TRUE; 
}


void CWeatherGlass_TabCtrlDlg::_InitPage(CRect& rcTab) 
{
	string subject[] = {"RiseFall","RiseFall2Limit"};

	//设定在Tab内显示的范围
	CRect rcHold;
	rcHold.left = rcTab.left + 5;
	rcHold.top = rcTab.top + 5;
	rcHold.right = rcTab.right - 5;
	rcHold.bottom = rcTab.bottom - 10;

	for (int i = 0; i < 2;i++)
	{
		CDialog* pDlg = Factory<CChartPageBase,string>::Instance().BuildProduct(subject[i]);
		if(!pDlg) continue;

		pDlg->Create(CChartPageBase::IDD,this);
		pDlg->MoveWindow(&rcHold);
		pDlg->ShowWindow(SW_SHOW);

		m_pTab->InsertItem(i,subject[i].c_str());

		m_vPage.push_back(pDlg);
	}

}

void CWeatherGlass_TabCtrlDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	_Resize(cx,cy);
}

void CWeatherGlass_TabCtrlDlg::_Resize(int cx, int cy)
{
	if(!m_pTab) return; 	
	if(!m_pTab->m_hWnd) return;

	int nTabLeft = 5;
	int nTabTop = 5;
	int nTabItemHeight = 25;

	int nMarginWidth = 20;
	int nMarginHeight = 36;

	int nHolderPosition[Four] = {0}; 
	int nTabPosition[Four] = {0};

	nTabPosition[Left] = nTabLeft;
	nTabPosition[Top] = nTabTop;
	nTabPosition[Width] = cx - nMarginWidth;
	nTabPosition[Height] = nTabItemHeight;	

	nHolderPosition[Left] = nTabLeft;
	nHolderPosition[Top] = nTabTop + nTabItemHeight;
	nHolderPosition[Width] = cx - nMarginWidth;
	nHolderPosition[Height] = cy - nMarginHeight;

	for (int i = 0; i < m_vPage.size();i++)
	{
		CDialog* pPage = m_vPage[i];
		pPage->MoveWindow(nHolderPosition[Left],nHolderPosition[Top],nHolderPosition[Width],nHolderPosition[Height]);
	}

	m_pTab->MoveWindow(nTabPosition[Left],nTabPosition[Top],nTabPosition[Width],nTabPosition[Height]);
	RedrawWindow(NULL,NULL,RDW_ALLCHILDREN|RDW_ERASE|RDW_INVALIDATE);

}

void CWeatherGlass_TabCtrlDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TRACE("Notify(CTCN_SELCHANGE): nItem(%d) pszText(\'%s\') lParam(%d) point(x=%d,y=%d) rect(l=%d,t=%d,r=%d,b=%d) bSelected(%d) bHighlighted(%d)\n",
				((CTC_NMHDR*)pNMHDR)->nItem,
				((CTC_NMHDR*)pNMHDR)->pszText,
				((CTC_NMHDR*)pNMHDR)->lParam,
				((CTC_NMHDR*)pNMHDR)->ptHitTest.x,
				((CTC_NMHDR*)pNMHDR)->ptHitTest.y,
				((CTC_NMHDR*)pNMHDR)->rItem.left,
				((CTC_NMHDR*)pNMHDR)->rItem.top,
				((CTC_NMHDR*)pNMHDR)->rItem.right,
				((CTC_NMHDR*)pNMHDR)->rItem.bottom,
				((CTC_NMHDR*)pNMHDR)->fSelected,
				((CTC_NMHDR*)pNMHDR)->fHighlighted);
	m_nCurSel = ((CTC_NMHDR*)pNMHDR)->nItem;
	_ShowPage(m_nCurSel);
	*pResult = 0;
}

void CWeatherGlass_TabCtrlDlg::OnHighlightchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TRACE("Notify(CTCN_HIGHLIGHTCHANGE): nItem(%d) pszText(\'%s\') lParam(%d) point(x=%d,y=%d) rect(l=%d,t=%d,r=%d,b=%d) bSelected(%d) bHighlighted(%d)\n",
				((CTC_NMHDR*)pNMHDR)->nItem,
				((CTC_NMHDR*)pNMHDR)->pszText,
				((CTC_NMHDR*)pNMHDR)->lParam,
				((CTC_NMHDR*)pNMHDR)->ptHitTest.x,
				((CTC_NMHDR*)pNMHDR)->ptHitTest.y,
				((CTC_NMHDR*)pNMHDR)->rItem.left,
				((CTC_NMHDR*)pNMHDR)->rItem.top,
				((CTC_NMHDR*)pNMHDR)->rItem.right,
				((CTC_NMHDR*)pNMHDR)->rItem.bottom,
				((CTC_NMHDR*)pNMHDR)->fSelected,
				((CTC_NMHDR*)pNMHDR)->fHighlighted);
	

	*pResult = 0;
}

void CWeatherGlass_TabCtrlDlg::OnLButtonClickedTab(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	switch(((CTC_NMHDR*)pNMHDR)->nItem)
	{
	case CTCHT_ONCLOSEBUTTON:
		TRACE("OnLButtonClicked: Close\n");
		if(m_pTab->DeleteItem(m_pTab->GetCurSel())!=CTCERR_NOERROR)
			AfxMessageBox("DeleteItem(...) failed.\nPossible errors:\n1. Item index out of range.");
		break;
	case CTCHT_ONFIRSTBUTTON:
		TRACE("OnLButtonClicked: First\n");
		break;
	case CTCHT_ONPREVBUTTON:
		TRACE("OnLButtonClicked: Prev\n");
		break;
	case CTCHT_ONNEXTBUTTON:
		TRACE("OnLButtonClicked: Next\n");
		break;
	case CTCHT_ONLASTBUTTON:
		TRACE("OnLButtonClicked: Last\n");
		break;
	case CTCHT_NOWHERE:
		TRACE("OnLButtonClicked: Nowhere\n");
		break;
	default:
		{
			TRACE("Notify(CTCN_CLICK): nItem(%d) pszText(\'%s\') lParam(%d) point(x=%d,y=%d) rect(l=%d,t=%d,r=%d,b=%d) bSelected(%d) bHighlighted(%d)\n",
				((CTC_NMHDR*)pNMHDR)->nItem,
				((CTC_NMHDR*)pNMHDR)->pszText,
				((CTC_NMHDR*)pNMHDR)->lParam,
				((CTC_NMHDR*)pNMHDR)->ptHitTest.x,
				((CTC_NMHDR*)pNMHDR)->ptHitTest.y,
				((CTC_NMHDR*)pNMHDR)->rItem.left,
				((CTC_NMHDR*)pNMHDR)->rItem.top,
				((CTC_NMHDR*)pNMHDR)->rItem.right,
				((CTC_NMHDR*)pNMHDR)->rItem.bottom,
				((CTC_NMHDR*)pNMHDR)->fSelected,
				((CTC_NMHDR*)pNMHDR)->fHighlighted);
		}
		break;
	}
	*pResult = 0;
}

void CWeatherGlass_TabCtrlDlg::OnRButtonClickedTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	switch(((CTC_NMHDR*)pNMHDR)->nItem)
	{
	case CTCHT_ONCLOSEBUTTON:
		TRACE("OnLButtonClicked: Close\n");
		break;
	case CTCHT_ONFIRSTBUTTON:
		TRACE("OnLButtonClicked: First\n");
		break;
	case CTCHT_ONPREVBUTTON:
		TRACE("OnLButtonClicked: Prev\n");
		break;
	case CTCHT_ONNEXTBUTTON:
		TRACE("OnLButtonClicked: Next\n");
		break;
	case CTCHT_ONLASTBUTTON:
		TRACE("OnLButtonClicked: Last\n");
		break;
	default:
		{
			int nInsNdx;
			CMenu menu;
			menu.CreatePopupMenu();
			if(((CTC_NMHDR*)pNMHDR)->nItem==CTCHT_NOWHERE)
			{
				TRACE("OnLButtonClicked: Nowhere\n");
				nInsNdx = m_pTab->GetItemCount();
				menu.AppendMenu(MF_STRING,1,_T("Insert Item"));
				menu.AppendMenu(MF_STRING|MF_GRAYED,2,_T("Delete Item"));
				menu.AppendMenu(MF_STRING|MF_GRAYED,3,_T("Rename"));
			}
			else
			{
				TRACE("Notify(CTCN_RCLICK): nItem(%d) pszText(\'%s\') lParam(%d) point(x=%d,y=%d) rect(l=%d,t=%d,r=%d,b=%d) bSelected(%d) bHighlighted(%d)\n",
						((CTC_NMHDR*)pNMHDR)->nItem,
						((CTC_NMHDR*)pNMHDR)->pszText,
						((CTC_NMHDR*)pNMHDR)->lParam,
						((CTC_NMHDR*)pNMHDR)->ptHitTest.x,
						((CTC_NMHDR*)pNMHDR)->ptHitTest.y,
						((CTC_NMHDR*)pNMHDR)->rItem.left,
						((CTC_NMHDR*)pNMHDR)->rItem.top,
						((CTC_NMHDR*)pNMHDR)->rItem.right,
						((CTC_NMHDR*)pNMHDR)->rItem.bottom,
						((CTC_NMHDR*)pNMHDR)->fSelected,
						((CTC_NMHDR*)pNMHDR)->fHighlighted);
				m_pTab->SetCurSel(((CTC_NMHDR*)pNMHDR)->nItem);
				nInsNdx = ((CTC_NMHDR*)pNMHDR)->nItem;
				menu.AppendMenu(MF_STRING,1,_T("Insert Item"));
				menu.AppendMenu(MF_STRING,2,_T("Delete Item"));
				menu.AppendMenu(MF_STRING,3,_T("Rename"));
			}
	
			CPoint pt(((CTC_NMHDR*)pNMHDR)->ptHitTest);
			m_pTab->ClientToScreen(&pt);

			int nRet = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, 
								pt.x, pt.y, this);
			switch(nRet)
			{
			case 1:
				{
					if(m_pTab->InsertItem(nInsNdx,_T("New SS_WHITERECT Item"),SS_WHITERECT)<0)
						AfxMessageBox("InsertItem(...) failed.\nPossible errors:\n1. Item index out of range.");
				}
				break;
			case 2:
				{
					if(m_pTab->DeleteItem(((CTC_NMHDR*)pNMHDR)->nItem)!=CTCERR_NOERROR)
						AfxMessageBox("DeleteItem(...) failed.\nPossible errors:\n1. Item index out of range.");
				}
				break;
			case 3:
				{
					if(m_pTab->EditLabel(((CTC_NMHDR*)pNMHDR)->nItem)!=CTCERR_NOERROR)
						AfxMessageBox("EditLabel(...) failed.\nPossible errors:\n1. Item index out of range.\n2. Item not selected.\n3. CTCS_EDITLABELS style not specified.");
				}
				break;
			}
		}
		break;
	}
	*pResult = 0;
}

void CWeatherGlass_TabCtrlDlg::OnMoveitemTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TRACE("Notify(CTCN_ITEMMOVE): nItem(%d) pszText(\'%s\') lParam(%d) point(x=%d,y=%d) rect(l=%d,t=%d,r=%d,b=%d) bSelected(%d) bHighlighted(%d)\n",
						((CTC_NMHDR*)pNMHDR)->nItem,
						((CTC_NMHDR*)pNMHDR)->pszText,
						((CTC_NMHDR*)pNMHDR)->lParam,
						((CTC_NMHDR*)pNMHDR)->ptHitTest.x,
						((CTC_NMHDR*)pNMHDR)->ptHitTest.y,
						((CTC_NMHDR*)pNMHDR)->rItem.left,
						((CTC_NMHDR*)pNMHDR)->rItem.top,
						((CTC_NMHDR*)pNMHDR)->rItem.right,
						((CTC_NMHDR*)pNMHDR)->rItem.bottom,
						((CTC_NMHDR*)pNMHDR)->fSelected,
						((CTC_NMHDR*)pNMHDR)->fHighlighted);
	*pResult = 0;
}

void CWeatherGlass_TabCtrlDlg::OnCopyitemTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TRACE("Notify(CTCN_ITEMCOPY): nItem(%d) pszText(\'%s\') lParam(%d) point(x=%d,y=%d) rect(l=%d,t=%d,r=%d,b=%d) bSelected(%d) bHighlighted(%d)\n",
						((CTC_NMHDR*)pNMHDR)->nItem,
						((CTC_NMHDR*)pNMHDR)->pszText,
						((CTC_NMHDR*)pNMHDR)->lParam,
						((CTC_NMHDR*)pNMHDR)->ptHitTest.x,
						((CTC_NMHDR*)pNMHDR)->ptHitTest.y,
						((CTC_NMHDR*)pNMHDR)->rItem.left,
						((CTC_NMHDR*)pNMHDR)->rItem.top,
						((CTC_NMHDR*)pNMHDR)->rItem.right,
						((CTC_NMHDR*)pNMHDR)->rItem.bottom,
						((CTC_NMHDR*)pNMHDR)->fSelected,
						((CTC_NMHDR*)pNMHDR)->fHighlighted);
	*pResult = 0;
}

void CWeatherGlass_TabCtrlDlg::OnLabelupdateTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TRACE("Notify(CTCN_LABELUPDATE): nItem(%d) pszText(\'%s\') lParam(%d) point(x=%d,y=%d) rect(l=%d,t=%d,r=%d,b=%d) bSelected(%d) bHighlighted(%d)\n",
						((CTC_NMHDR*)pNMHDR)->nItem,
						((CTC_NMHDR*)pNMHDR)->pszText,
						((CTC_NMHDR*)pNMHDR)->lParam,
						((CTC_NMHDR*)pNMHDR)->ptHitTest.x,
						((CTC_NMHDR*)pNMHDR)->ptHitTest.y,
						((CTC_NMHDR*)pNMHDR)->rItem.left,
						((CTC_NMHDR*)pNMHDR)->rItem.top,
						((CTC_NMHDR*)pNMHDR)->rItem.right,
						((CTC_NMHDR*)pNMHDR)->rItem.bottom,
						((CTC_NMHDR*)pNMHDR)->fSelected,
						((CTC_NMHDR*)pNMHDR)->fHighlighted);
	if(CString(((CTC_NMHDR*)pNMHDR)->pszText).IsEmpty())
	{
		*pResult = 1; // Invalid label name
		AfxMessageBox("Invalid label name");
	}
	else
		*pResult = 0; // Label name OK
}

LPCTSTR CWeatherGlass_TabCtrlDlg::GetTooltipText(int nStyle)
{
	static LPCTSTR s[] = {"Sets the static window style to SS_BLACKRECT",
						"Sets the static window style to SS_GRAYRECT",
						"Sets the static window style to SS_WHITERECT"};
	switch(nStyle)
	{
	case SS_BLACKRECT:
		return s[0];
	case SS_GRAYRECT:
		return s[1];
	case SS_WHITERECT:
		return s[2];
	}
	return NULL;
}

void CWeatherGlass_TabCtrlDlg::_ShowPage(const int nCurPage)
{
	for (int i = 0; i < m_vPage.size();i++)
	{
		CDialog* pPage = m_vPage[i];
		if (!pPage) continue;
		if (nCurPage == i)
		{
			pPage->ShowWindow(SW_SHOW);
		}
		else
		{
			pPage->ShowWindow(SW_HIDE);
		}
	}
}