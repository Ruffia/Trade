#pragma once
#include "afxcmn.h"
#include "GridCtrl.h"
#include "afxdtctl.h"

class CDlgGridPage: public CDialog
{
	DECLARE_DYNAMIC(CDlgGridPage)

public:
	CDlgGridPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgGridPage();

// 对话框数据
	enum { IDD = IDD_DLG_Grid };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	//初始化Grid
	virtual void _InitializeGrid();

	//设计控件布局
	virtual void _DesignLayout() = 0;

	virtual void _MoveGrid() = 0;

	//垂直移动控件
	//nVeticalPix,垂直移动的像素单位
	//nHorizontalPix,水平移动的像素单位
	virtual void _MoveControl(int nCtrlID,int nVeticalPix,int nHorizontalPix);

protected:
	CSize m_OldSize;
	static bool VirtualCompare(int c1, int c2);
	void Trace(LPCTSTR szFmt, ...);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridCtrlDemoDlg)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

	static BOOL CALLBACK GridCallback(GV_DISPINFO *pDispInfo, LPARAM lParam);
	void OnGridLines();
	afx_msg void OnUpdateEditCols();
	afx_msg void OnUpdateEditFixcols();
	afx_msg void OnUpdateEditFixrows();
	afx_msg void OnUpdateEditRows();
	afx_msg void OnHorzGridLines();
	afx_msg void OnVertGridLines();
	afx_msg void OnListmode();
	afx_msg void OnEditable();
	afx_msg void OnPrintButton();
	afx_msg void OnAllowSelection();
	afx_msg void OnRowResize();
	afx_msg void OnColResize();
	afx_msg void OnFontButton();
	afx_msg void OnHeaderSort();
	afx_msg void OnEditSelectall();
	afx_msg void OnAppAbout();
	afx_msg void OnItalics();
	afx_msg void OnTitletips();
	afx_msg void OnInsertRow();
	afx_msg void OnDeleteRow();
	afx_msg void OnSingleselmode();
	afx_msg void OnSingleColMode();
	afx_msg void OnExpandToFit();
	afx_msg void OnAutoSize();
	afx_msg void OnFill();
	afx_msg void OnCellUrl();
	afx_msg void OnCellNormal();
	afx_msg void OnCellCombo();
	afx_msg void OnCellNumeric();
	afx_msg void OnCellReadonly();
	afx_msg void OnCellCheckbox();
	afx_msg void OnCellDateTime();
	afx_msg void OnTrackfocus();
	afx_msg void OnFramefocus();
	afx_msg void OnExpandLast();
	afx_msg void OnSetFocus11();
	virtual void OnVirtualMode();
	afx_msg void OnCallbackFunction();
	afx_msg void OnFixedRowSelection();
	afx_msg void OnFixedColSelection();
	afx_msg void OnVertical();
	afx_msg void OnExpandUseFixed();
	afx_msg void OnRejectEditAttempts();
	afx_msg void OnRejectEditChanges();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnCleartrace();
	//}}AFX_MSG
	afx_msg void OnGridDblClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridRClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridStartEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridStartSelChange(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridEndSelChange(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridBeginDrag(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCopyOrCut(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnHide2ndrowcolumn();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

protected:
	int		m_nFixCols;
	int		m_nFixRows;
	int		m_nCols;
	int		m_nRows;
	BOOL	m_bEditable;
	BOOL	m_bHorzLines;
	BOOL	m_bListMode;
	BOOL	m_bVertLines;
	BOOL	m_bSelectable;
	BOOL	m_bAllowColumnResize;
	BOOL	m_bAllowRowResize;
	BOOL	m_bHeaderSort;
	BOOL	m_bItalics;
	BOOL	m_btitleTips;
	BOOL	m_bSingleSelMode;
	BOOL    m_bSingleColSelMode;
	BOOL	m_bTrackFocus;
	BOOL	m_bFrameFocus;
	BOOL	m_bVirtualMode;
	BOOL	m_bCallback;
	BOOL    m_bVertical;
	BOOL    m_bExpandUseFixed;
	BOOL    m_bRejectEditAttempts;
	BOOL    m_bRejectEditChanges;
	BOOL    m_bRow2Col2Hidden;

public:
	CGridCtrl m_Grid;
};
