#include "stdafx.h"
#include "CustomTabCtrlLeftTopDlg.h"
#include "CustomTabCtrlTopDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCustomTabCtrlLeftTopDlg::CCustomTabCtrlLeftTopDlg(CWnd* pParent)
	: CCustomTabCtrlDlg(pParent)
{
}


void CCustomTabCtrlLeftTopDlg::_CreateTabCtrl(const CRect& rcClient)
{
	m_pTab = new CCustomTabCtrl;
	CRect rcTab(rcClient.left + 17,rcClient.top + 27,rcClient.Width() - 20,rcClient.Height() - 20);
	m_pTab->Create(WS_CHILD|WS_VISIBLE|CTCS_DRAGMOVE|CTCS_TOP|CTCS_EDITLABELS|CTCS_CLOSEBUTTON|CTCS_AUTOHIDEBUTTONS|CTCS_MULTIHIGHLIGHT|CTCS_DRAGCOPY|CTCS_LEFT,rcTab,this,IDC_TAB);
	m_pTab->SetDragCursors(AfxGetApp()->LoadCursor(IDC_CURSORMOVE),AfxGetApp()->LoadCursor(IDC_CURSORCOPY));
	LOGFONT lf = {15, 0, 0, 0, FW_NORMAL, 0, 0, 0,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Courier"};
	m_pTab->SetControlFont(lf, TRUE);
}


void CCustomTabCtrlLeftTopDlg::_AddPage()
{
	m_pTab->InsertItem(0,"SS_BLACKRECT");
	m_pTab->SetItemData(0,SS_BLACKRECT);
	m_pTab->InsertItem(1,"SS_GRAY");
	m_pTab->SetItemData(1,SS_GRAYRECT);
	m_pTab->InsertItem(2,"SS_WHITERECT");
	m_pTab->SetItemData(2,SS_WHITERECT);
	m_pTab->SetCurSel(0);

	CCustomTabCtrlTopDlg* pPage1 = new CCustomTabCtrlTopDlg;
	pPage1->Create(CCustomTabCtrlTopDlg::IDD, this);
	m_vPage.push_back(pPage1);

	CCustomTabCtrlTopDlg* pPage2 = new CCustomTabCtrlTopDlg;
	pPage2->Create(CCustomTabCtrlTopDlg::IDD, this);
	m_vPage.push_back(pPage2);

	CCustomTabCtrlTopDlg* pPage3 = new CCustomTabCtrlTopDlg;
	pPage3->Create(CCustomTabCtrlTopDlg::IDD, this);
	m_vPage.push_back(pPage3);

	m_nCurSel = 0;
	//ÏÔÊ¾³õÊ¼Ò³Ãæ
	_ShowPage(m_nCurSel);
}


void CCustomTabCtrlLeftTopDlg::_Resize(int cx, int cy)
{
	if(!m_pTab) return; 	
	if(!m_pTab->m_hWnd) return;

	int nTabLeft = 5;
	int nTabTop = 5;
	int nTabItemHeight = 20;

	int nMarginWidth = 20;
	int nMarginHeight = 36;

	int nHolderPosition[Four] = {0}; 
	int nTabPosition[Four] = {0};

	nHolderPosition[Left] = nTabLeft + nTabItemHeight;
	nHolderPosition[Top] = nTabTop;
	nHolderPosition[Width] = cx - nMarginHeight;
	nHolderPosition[Height] = cy - nMarginWidth;

	nTabPosition[Left] = nTabLeft;
	nTabPosition[Top] = nTabTop;
	nTabPosition[Width] = nTabItemHeight;
	nTabPosition[Height] = cy - nMarginWidth;	


	for (int i = 0; i < m_vPage.size();i++)
	{
		CDialog* pPage = m_vPage[i];
		pPage->MoveWindow(nHolderPosition[Left],nHolderPosition[Top],nHolderPosition[Width],nHolderPosition[Height]);
	}

	m_pTab->MoveWindow(nTabPosition[Left],nTabPosition[Top],nTabPosition[Width],nTabPosition[Height]);
	RedrawWindow(NULL,NULL,RDW_ALLCHILDREN|RDW_ERASE|RDW_INVALIDATE);

}


