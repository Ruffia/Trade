#pragma once
#include <memory>
#include <map>
#include <string>
using namespace std;
#include "Tools/pugixml.hpp"
#include "Tools/pugiconfig.hpp"
using namespace pugi;
#include "EditTreeCtrlEx.h"


class CDlgTree: public CDialog
{
	CImageList m_listTreeImages;

	// Construction
	public:
		CDlgTree(CWnd* pParent = NULL);	// standard constructor

	// Dialog Data
		//{{AFX_DATA(CDlg)
	enum { IDD = IDD_ChildDialog_Tree };
		CEditTreeCtrlEx	m_ctrlTree;
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
		virtual void _InitLayOut();
		afx_msg void OnPaint();
		afx_msg HCURSOR OnQueryDragIcon();
	    afx_msg void OnMultiSelect();
	//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

protected:
	string m_sLayout;    //页面布局配置文件
	xml_document m_doc;
	map<string,shared_ptr<CWnd>>  m_mapUIName2Wnd;
};
