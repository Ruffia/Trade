#include "stdafx.h"
#include "ADRTrade.h"
#include "afxdialogex.h"
#include "Util.h"
#include "UIData.h"
#include "Factory.h"
#include "DialogIDManager.h"
#include "DialogPlaceHolder.h"
#include "DialogPlaceHolder_MinorCycleAnalyze.h"
#include "DialogPlaceHolderComposite.h"

#include "CustomTabCtrlDlg.h"
//#include "DlgSEMLog_OperationQueryKeyWordSettings.h"
//#include "DlgSEMLog_GridHeaderSettings.h"
//#include "DlgSEMLog_PageB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_FACTORY(CDialog,CCustomTabCtrlDlg,string,"CCustomTabCtrlDlg")
CCustomTabCtrlDlg::CCustomTabCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCustomTabCtrlDlg::IDD, pParent),m_pTab(NULL),m_nCurSel(0)
{
	CDialogIDMgr::Instance().Register("CCustomTabCtrlDlg",CCustomTabCtrlDlg::IDD);
}

CCustomTabCtrlDlg::~CCustomTabCtrlDlg()
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

void CCustomTabCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCustomTabCtrlDlg, CDialog)
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

BOOL CCustomTabCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rcClient;
	GetClientRect(rcClient);

	m_pTab = new CCustomTabCtrl;
	CRect rcTab(rcClient.left + 17,rcClient.top + 27,rcClient.Width() - 20,rcClient.Height() - 20);
	m_pTab->Create(WS_CHILD|WS_VISIBLE|CTCS_DRAGMOVE|CTCS_TOP|CTCS_EDITLABELS|CTCS_CLOSEBUTTON|CTCS_AUTOHIDEBUTTONS|CTCS_MULTIHIGHLIGHT|CTCS_DRAGCOPY|CTCS_TOP,rcTab,this,IDC_TAB);
	m_pTab->SetDragCursors(AfxGetApp()->LoadCursor(IDC_CURSORMOVE),AfxGetApp()->LoadCursor(IDC_CURSORCOPY));

	_InitPage(rcTab);

	m_nCurSel = 0;
	//显示初始页面
	_ShowPage(m_nCurSel);
	LOGFONT lf = {15, 0, 0, 0, FW_NORMAL, 0, 0, 0,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Courier"};

	m_pTab->SetControlFont(lf, TRUE);
	_Resize(rcClient.Width(),rcClient.Height());

	return TRUE; 
}


void CCustomTabCtrlDlg::_InitPage(CRect& rcTab) 
{
	string sFileName = GetModulePath() + "/UI/" + m_sLayout;
	if (!m_doc.load_file(sFileName.c_str())) 
	{ //加载xml文件
		return;
	}

	int nCount = 0;
	xml_node root = m_doc.child("root");  //根节点
	xml_node nodeLayout = root.child("Layout");
	xml_node node = nodeLayout.first_child();
	while (!node.empty())
	{
		CUIData data;
		const string sName = node.attribute("Name").as_string("");
		data.m_strUIClassName = node.attribute("ClassName").as_string("");
		data.m_strLayout = node.attribute("Layout").as_string("");
		data.m_nID = node.attribute("ID").as_int();
		data.m_nLeft = node.attribute("Left").as_int();
		data.m_nTop = node.attribute("Top").as_int();
		data.m_nWidth = node.attribute("Width").as_int();
		data.m_nHeight = node.attribute("Height").as_int();
		string sTitle = node.attribute("Title").as_string("");

		//设定在Tab内显示的范围
		CRect rcHold;
		rcHold.top = rcTab.top + data.m_nTop;
		rcHold.left = rcTab.left;
		rcHold.right = rcTab.right;
		rcHold.bottom = rcTab.bottom;
	

		CDialog* pDlg = Factory<CDialog,string>::Instance().BuildProduct(data.m_strUIClassName);
		const int nIDD = CDialogIDMgr::Instance().GetDialogResourceID(data.m_strUIClassName);
		ASSERT(-1 != nIDD);
		CDialogTabItem_MinorCycleAnalyze* pDlgItem = dynamic_cast<CDialogTabItem_MinorCycleAnalyze*>(pDlg);
		if (pDlgItem)
		{
			pDlgItem->SetLayout(data.m_strLayout);
			pDlgItem->SetItem(sName);
		}
		else
		{
			CDialogPlaceHolderComposite* pDlgComposite = dynamic_cast<CDialogPlaceHolderComposite*>(pDlg);
			if (pDlgComposite)
			{
				pDlgComposite->SetLayout(data.m_strLayout);
			}
		}

		pDlg->Create(nIDD,this);
		pDlg->MoveWindow(&rcHold);

		m_pTab->InsertItem(nCount,sName.c_str());

		m_vPage.push_back(pDlg);
		data.m_pWnd = pDlg;

		nCount++;
		node = node.next_sibling();
	}

}

void CCustomTabCtrlDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	_Resize(cx,cy);
}

void CCustomTabCtrlDlg::_Resize(int cx, int cy)
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

void CCustomTabCtrlDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CCustomTabCtrlDlg::OnHighlightchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CCustomTabCtrlDlg::OnLButtonClickedTab(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CCustomTabCtrlDlg::OnRButtonClickedTab(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CCustomTabCtrlDlg::OnMoveitemTab(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CCustomTabCtrlDlg::OnCopyitemTab(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CCustomTabCtrlDlg::OnLabelupdateTab(NMHDR* pNMHDR, LRESULT* pResult) 
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

LPCTSTR CCustomTabCtrlDlg::GetTooltipText(int nStyle)
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

void CCustomTabCtrlDlg::_ShowPage(const int nCurPage)
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