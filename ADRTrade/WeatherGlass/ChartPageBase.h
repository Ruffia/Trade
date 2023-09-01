#pragma once

class CChartPageBase : public CDialog
{
public:
	CChartPageBase(CWnd* pParent = NULL); 
	enum{IDD = IDD_WeatherGlass_Chart};

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

protected:
	CList<CRect,CRect&> m_listRect; 
	//CXTPChartControl m_wndChartControl;

	void ShowControls(BOOL bShowLegend, BOOL bShow3DText);
	void GetInitRect();
	virtual void AddTitles() = 0;
	virtual void AddSeries() = 0;
	virtual void ShowChart() = 0;

	// Generated message map functions
	//{{AFX_MSG(CChartDialogDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};
