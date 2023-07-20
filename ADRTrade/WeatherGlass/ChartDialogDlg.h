// ChartDialogDlg.h
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

#if !defined(AFX_CHARTDIALOGDLG_H__9F1720D3_90FC_46D0_A55A_F09BE1970E19__INCLUDED_)
#	define AFX_CHARTDIALOGDLG_H__9F1720D3_90FC_46D0_A55A_F09BE1970E19__INCLUDED_

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CChartDialogDlg dialog

class CChartDialogDlg : public CDialog
{
	// Construction
public:
	CChartDialogDlg(CWnd* pParent = NULL); // standard constructor

	// Dialog Data
	//{{AFX_DATA(CChartDialogDlg)
	enum
	{
		IDD = IDD_CHARTDIALOG_DIALOG
	};
	CStatic m_wnd3dText;
	CButton m_wndLegend;
	CComboBox m_wndPalette;
	CComboBox m_wndAppearance;
	int m_nAppearance;
	int m_nStyle;
	int m_nPalette;
	BOOL m_bLegend;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartDialogDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
													 //}}AFX_VIRTUAL

	// Implementation
protected:
	HICON m_hIcon;
	CXTPChartControl m_wndChartControl;

	void ShowControls(BOOL bShowLegend, BOOL bShow3DText);
	void AddTitles();
	void AddSeries();

	// Generated message map functions
	//{{AFX_MSG(CChartDialogDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnChangeComboStyle();
	afx_msg void OnChangeComboAppearance();
	afx_msg void OnChangeComboPalette();
	afx_msg void OnCheckLegend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTDIALOGDLG_H__9F1720D3_90FC_46D0_A55A_F09BE1970E19__INCLUDED_)
