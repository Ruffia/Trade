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
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CChartPageBase::ShowControls(BOOL bShowLegend, BOOL bShow3DText)
{
	m_wndChartControl.GetContent()->GetLegend()->SetVisible(bShowLegend);
}


BOOL CChartPageBase::OnInitDialog()
{
	CDialog::OnInitDialog();
	AddTitles();
	AddSeries();
	ShowChart();

	m_wndChartControl.GetContent()->GetLegend()->SetVisible(TRUE);
	SendMessageToDescendants(WM_SETFONT, (WPARAM)(HFONT)XTPFontManager()->GetFont());

	return TRUE; 
}


void CChartPageBase::OnPaint()
{
	CDialog::OnPaint();
}

