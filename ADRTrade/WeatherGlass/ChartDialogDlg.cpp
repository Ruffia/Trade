// ChartDialogDlg.cpp
//
// (c)1998-2023 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartDialogDlg.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChartDialogDlg dialog

CChartDialogDlg::CChartDialogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChartDialogDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChartDialogDlg)
	m_nAppearance = 0;
	m_nStyle	  = 11;
	m_nPalette	= 25;
	m_bLegend	 = TRUE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChartDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChartDialogDlg)
	DDX_Control(pDX, IDC_STATIC_3DTEXT, m_wnd3dText);
	DDX_Control(pDX, IDC_CHECK_LEGEND, m_wndLegend);
	DDX_Control(pDX, IDC_COMBO_PALETTE, m_wndPalette);
	DDX_Control(pDX, IDC_COMBO_APPEARANCE, m_wndAppearance);
	DDX_CBIndex(pDX, IDC_COMBO_APPEARANCE, m_nAppearance);
	DDX_CBIndex(pDX, IDC_COMBO_STYLE, m_nStyle);
	DDX_CBIndex(pDX, IDC_COMBO_PALETTE, m_nPalette);
	DDX_Control(pDX, IDC_CHARTCONTROL, m_wndChartControl);
	DDX_Check(pDX, IDC_CHECK_LEGEND, m_bLegend);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CChartDialogDlg, CDialog)
	//{{AFX_MSG_MAP(CChartDialogDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_COMBO_STYLE, OnChangeComboStyle)
	ON_CBN_SELCHANGE(IDC_COMBO_APPEARANCE, OnChangeComboAppearance)
	ON_CBN_SELCHANGE(IDC_COMBO_PALETTE, OnChangeComboPalette)
	ON_BN_CLICKED(IDC_CHECK_LEGEND, OnCheckLegend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChartDialogDlg message handlers

void CChartDialogDlg::ShowControls(BOOL bShowLegend, BOOL bShow3DText)
{
	m_wndLegend.ShowWindow(bShowLegend ? SW_SHOW : SW_HIDE);
	m_wnd3dText.ShowWindow(bShow3DText ? SW_SHOW : SW_HIDE);

	m_wndChartControl.GetContent()->GetLegend()->SetVisible(bShowLegend && m_bLegend);
}

void CChartDialogDlg::AddTitles()
{
	CXTPChartTitle* pTitle = m_wndChartControl.GetContent()->GetTitles()->Add(new CXTPChartTitle());
	pTitle->SetText(_T("Chart Control"));
}

void CChartDialogDlg::AddSeries()
{
	CXTPChartSeries* pSeries = m_wndChartControl.GetContent()->GetSeries()->Add(
		new CXTPChartSeries());

	pSeries->GetPoints()->Add(new CXTPChartSeriesPoint(6, 4, 4));
	pSeries->GetPoints()->Add(new CXTPChartSeriesPoint(7, 3.3, 3.3));
	pSeries->GetPoints()->Add(new CXTPChartSeriesPoint(8, 2.8, 2.8))->m_bSpecial = TRUE;
	pSeries->GetPoints()->Add(new CXTPChartSeriesPoint(9, 3.7, 3.7));
	pSeries->GetPoints()->Add(new CXTPChartSeriesPoint(10, 1.9, 1.9));
}

BOOL CChartDialogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);  // Set big icon
	SetIcon(m_hIcon, FALSE); // Set small icon

	AddTitles();
	AddSeries();

	OnChangeComboStyle();
	OnChangeComboPalette();
	OnChangeComboAppearance();

	m_wndChartControl.GetContent()->GetLegend()->SetVisible(TRUE);

	// Set the default font for all dialog controls.
	SendMessageToDescendants(WM_SETFONT, (WPARAM)(HFONT)XTPFontManager()->GetFont());

	return TRUE; // return TRUE  unless you set the focus to a control
}

void CChartDialogDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CChartDialogDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CChartDialogDlg::OnQueryDragIcon()
{
	return (HCURSOR)m_hIcon;
}

void CChartDialogDlg::OnChangeComboStyle()
{
	UpdateData();

	CXTPChartSeries* pSeries = m_wndChartControl.GetContent()->GetSeries()->GetAt(0);

	switch (m_nStyle)
	{
		case 0:
		{
			pSeries->SetStyle(new CXTPChartPointSeriesStyle());
			ShowControls(FALSE, FALSE);
		}
		break;
		case 1:
		{
			pSeries->SetStyle(new CXTPChartLineSeriesStyle());
			ShowControls(FALSE, FALSE);
		}
		break;
		case 2:
		{
			pSeries->SetStyle(new CXTPChartSplineSeriesStyle());
			ShowControls(FALSE, FALSE);
		}
		break;
		case 3:
		{
			pSeries->SetStyle(new CXTPChartBarSeriesStyle());
			ShowControls(FALSE, FALSE);
		}
		break;
		case 4:
		{
			pSeries->SetStyle(new CXTPChartAreaSeriesStyle());
			ShowControls(FALSE, FALSE);
		}
		break;
		case 5:
		{
			pSeries->SetStyle(new CXTPChartSplineAreaSeriesStyle());
			ShowControls(FALSE, FALSE);
		}
		break;
		case 6:
		{
			pSeries->SetStyle(new CXTPChartBubbleSeriesStyle());
			ShowControls(TRUE, FALSE);
		}
		break;
		case 7:
		{
			pSeries->SetStyle(new CXTPChart2dPieSeriesStyle());
			ShowControls(TRUE, FALSE);
		}
		break;
		case 8:
		{
			CXTPChartPieSeriesStyle* pPieStyle = (CXTPChartPieSeriesStyle*)pSeries->SetStyle(
				new CXTPChart2dPieSeriesStyle());
			pPieStyle->SetRotation(20);
			pPieStyle->SetHolePercent(60);
			pPieStyle->SetExplodedDistancePercent(20);
			((CXTPChartPieSeriesLabel*)pPieStyle->GetLabel())->SetPosition(xtpChartPieLabelOutside);
			ShowControls(TRUE, FALSE);
		}
		break;
		case 9:
		{
			CXTPChart3dPieSeriesStyle* pPieStyle = (CXTPChart3dPieSeriesStyle*)pSeries->SetStyle(
				new CXTPChart3dPieSeriesStyle());
			pPieStyle->Set3dRotation(10, 20, 50);
			((CXTPChartPieSeriesLabel*)pPieStyle->GetLabel())->SetPosition(xtpChartPieLabelOutside);
			ShowControls(TRUE, TRUE);
		}
		break;
		case 10:
		{
			CXTPChart3dPieSeriesStyle* pPieStyle = (CXTPChart3dPieSeriesStyle*)pSeries->SetStyle(
				new CXTPChart3dPieSeriesStyle());
			pPieStyle->Set3dRotation(10, 20, 30);
			pPieStyle->SetHolePercent(60);
			pPieStyle->SetExplodedDistancePercent(20);
			((CXTPChartPieSeriesLabel*)pPieStyle->GetLabel())->SetPosition(xtpChartPieLabelOutside);
			ShowControls(TRUE, TRUE);
		}
		break;
		case 11:
		{
			CXTPChart3dPieSeriesStyle* pPieStyle = (CXTPChart3dPieSeriesStyle*)pSeries->SetStyle(
				new CXTPChart3dPieSeriesStyle());
			pPieStyle->Set3dRotation(-20, 0, 70);
			pPieStyle->SetExplodedDistancePercent(20);
			pPieStyle->SetTorus(TRUE);
			pPieStyle->SetDepth(pPieStyle->GetDepth() * 2);
			((CXTPChartPieSeriesLabel*)pPieStyle->GetLabel())->SetPosition(xtpChartPieLabelOutside);
			ShowControls(TRUE, TRUE);
		}
		break;
		case 12:
		{
			CXTPChart3dPyramidSeriesStyle* pPyramidStyle =
				(CXTPChart3dPyramidSeriesStyle*)pSeries->SetStyle(
					new CXTPChart3dPyramidSeriesStyle());
			pPyramidStyle->Set3dRotation(20, 20, 90);
			pPyramidStyle->SetPointDistance(5);
			pPyramidStyle->SetHeightToWidthRatio(2);
			pPyramidStyle->SetBaseEdgeCount(6);
			pPyramidStyle->Enable3dEdgeSmoothing(TRUE);
			((CXTPChartPyramidSeriesLabel*)pPyramidStyle->GetLabel())
				->SetPosition(xtpChartPyramidLabelCenter);
			ShowControls(TRUE, TRUE);
		}
		break;
	}
}

void CChartDialogDlg::OnChangeComboAppearance()
{
	UpdateData();

	CString strAppearance;
	m_wndAppearance.GetLBText(m_nAppearance, strAppearance);

	m_wndChartControl.GetContent()->GetAppearance()->LoadAppearance(_T("CHART_APPEARANCE_")
																	+ strAppearance);

	m_wndChartControl.GetContent()->OnChartChanged();
}

void CChartDialogDlg::OnChangeComboPalette()
{
	UpdateData();

	CString strPalette;
	m_wndPalette.GetLBText(m_nPalette, strPalette);
	strPalette.MakeUpper();
	strPalette.Replace(_T(" "), _T(""));

	m_wndChartControl.GetContent()->GetAppearance()->LoadPalette(_T("CHART_PALETTE_") + strPalette);

	m_wndChartControl.GetContent()->OnChartChanged();
}

void CChartDialogDlg::OnCheckLegend()
{
	UpdateData();

	m_wndChartControl.GetContent()->GetLegend()->SetVisible(m_bLegend);
}
