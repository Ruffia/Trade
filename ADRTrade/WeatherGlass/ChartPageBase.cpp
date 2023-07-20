#include "stdafx.h"
#include "ChartPageBase.h"


CChartPageBase::CChartPageBase(CWnd* pParent /*=NULL*/)
	: CDialog(CChartPageBase::IDD, pParent)
{
}

void CChartPageBase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHARTCONTROL, m_wndChartControl);
}

BEGIN_MESSAGE_MAP(CChartPageBase, CDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CChartPageBase::ShowControls(BOOL bShowLegend, BOOL bShow3DText)
{
	m_wndChartControl.GetContent()->GetLegend()->SetVisible(bShowLegend);
}


BOOL CChartPageBase::OnInitDialog()
{
	CDialog::OnInitDialog();
	GetInitRect();
	AddTitles();
	AddSeries();
	ShowChart();

	m_wndChartControl.GetContent()->GetLegend()->SetVisible(TRUE);
	SendMessageToDescendants(WM_SETFONT, (WPARAM)(HFONT)XTPFontManager()->GetFont());

	return TRUE; 
}


void CChartPageBase::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType,cx,cy);
	// TODO: 在此处添加消息处理程序代码
	if (m_listRect.GetCount() > 0)  
	{  
		CRect dlgNow;  
		GetWindowRect(&dlgNow);  
		POSITION pos = m_listRect.GetHeadPosition();//第一个保存的是对话框的Rect  

		CRect dlgSaved;  
		dlgSaved = m_listRect.GetNext(pos);  
		ScreenToClient(dlgNow);  

		float x = dlgNow.Width() * 1.0 / dlgSaved.Width();//根据当前和之前保存的对话框的宽高求比例  
		float y = dlgNow.Height()  *1.0/ dlgSaved.Height();  
		ClientToScreen(dlgNow);  

		CRect childSaved;  

		CWnd* pWnd = GetWindow(GW_CHILD);  
		while(pWnd)  
		{  
			childSaved = m_listRect.GetNext(pos);//依次获取子窗体的Rect  
			childSaved.left = dlgNow.left + (childSaved.left - dlgSaved.left)*x;//根据比例调整控件上下左右距离对话框的距离  
			childSaved.right = dlgNow.right + (childSaved.right - dlgSaved.right)*x;  
			childSaved.top = dlgNow.top + (childSaved.top - dlgSaved.top)*y;  
			childSaved.bottom = dlgNow.bottom + (childSaved.bottom - dlgSaved.bottom)*y;  
			ScreenToClient(childSaved);  
			pWnd->MoveWindow(childSaved);  
			pWnd = pWnd->GetNextWindow();  
		}  

	}

}


void CChartPageBase::OnPaint()
{
	CDialog::OnPaint();
}


void CChartPageBase::GetInitRect()
{
	CRect rect;  
	GetWindowRect(rect);  
	m_listRect.AddTail(rect);//对话框的区域  
	CWnd* pWnd = GetWindow(GW_CHILD);//获取子窗体  
	while(pWnd)  
	{  
		pWnd->GetWindowRect(rect);//子窗体的区域  
		m_listRect.AddTail(rect);           //CList<CRect,CRect> m_listRect成员变量  
		pWnd = pWnd->GetNextWindow();//取下一个子窗体  
	}
}