#pragma once
#include "EditTreeCtrlEx.h"



class CTestTreeCtrl : public CEditTreeCtrlEx {
	protected:
		virtual void OnNewItem(HTREEITEM) {
					TRACE0("User created a new item.\n");
				}
};


class CDlgTree: public CDialog
{
	CImageList m_listTreeImages;

	// Construction
	public:
		CDlgTree(CWnd* pParent = NULL);	// standard constructor

	// Dialog Data
		//{{AFX_DATA(CDlg)
	enum { IDD = IDD_ChildDialog_Tree };
		CTestTreeCtrl	m_ctrlTree;
	BOOL	m_bMultiSelect;
	//}}AFX_DATA

		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CDlg)
		protected:
		virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
		//}}AFX_VIRTUAL

	// Implementation
	protected:
		HICON m_hIcon;

		// Generated message map functions
		//{{AFX_MSG(CDlg)
		virtual BOOL OnInitDialog();
		afx_msg void OnPaint();
		afx_msg HCURSOR OnQueryDragIcon();
	    afx_msg void OnMultiSelect();
	//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
};
