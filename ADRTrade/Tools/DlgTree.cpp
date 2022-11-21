#include "stdafx.h"
#include "DlgTree.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg dialog

CDlgTree::CDlgTree(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTree::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg)
	m_bMultiSelect = TRUE;
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDlgTree::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg)
	DDX_Control(pDX, IDC_Tree, m_ctrlTree);
	DDX_Check(pDX, IDC_MultiSelect, m_bMultiSelect);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgTree, CDialog)
	//{{AFX_MSG_MAP(CDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_MultiSelect, OnMultiSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg message handlers

BOOL CDlgTree::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Without an imagelist we won't get a drag image...
	VERIFY(m_listTreeImages.Create(16, 16, ILC_COLOR8|ILC_MASK, 1, 1));
	m_listTreeImages.Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	m_ctrlTree.SetImageList(&m_listTreeImages, TVSIL_NORMAL);

	m_ctrlTree.InsertItem("one");
	m_ctrlTree.InsertItem("two");
	m_ctrlTree.InsertItem("three");
	m_ctrlTree.InsertItem("four");
	m_ctrlTree.InsertItem("five");
	m_ctrlTree.InsertItem("six");

	_InitLayOut();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDlgTree::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

HCURSOR CDlgTree::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CDlgTree::OnMultiSelect() 
{
	UpdateData();
	m_ctrlTree.EnableMultiSelect(m_bMultiSelect != FALSE);
	if(!m_bMultiSelect) {
		HTREEITEM hItem = m_ctrlTree.GetSelectedItem();
		m_ctrlTree.ClearSelection(hItem);
	}
}



void CDlgTree::_InitLayOut()
{
	//string sFileName = GetModulePath() + "/UI/" + m_sLayout;
	//bool bRet = true;
	//if (!m_doc.load_file(sFileName.c_str())) 
	//{ //加载xml文件
	//	return;
	//}

	//CRect rc;
	//GetClientRect(rc);
	//ClientToScreen(rc);

	//xml_node root = m_doc.child("root");  //根节点
	//xml_node nodeLayout = root.child("Layout");
	//xml_node node = nodeLayout.first_child();
	//while (!node.empty())
	//{
	//	CUIData data;
	//	const string sName = node.attribute("Name").as_string("");
	//	data.m_strUIClassName = node.attribute("ClassName").as_string("");
	//	data.m_nID = node.attribute("ID").as_int();
	//	data.m_nLeft = node.attribute("Left").as_int();
	//	data.m_nTop = node.attribute("Top").as_int();
	//	data.m_nWidth = node.attribute("Width").as_int();
	//	data.m_nHeight = node.attribute("Height").as_int();
	//	const string sStyle = node.attribute("Style").as_string();
	//	DWORD dwTotalStyle = CStyleMgr::Instance().GetStyle(sStyle);
	//	const string sCaption = node.attribute("Caption").as_string();
	//	if (data.m_strUIClassName.find("CEdit") != string::npos)
	//	{
	//		CEdit* pEdit = new CEdit;
	//		CRect rc(data.m_nLeft,data.m_nTop,data.m_nLeft + data.m_nWidth ,data.m_nTop + data.m_nHeight);
	//		pEdit->Create(dwTotalStyle,rc,this,data.m_nID);	
	//		CFont* pFont = CCollectiveComponentProvider::Instance().GetFont();
	//		pEdit->SetFont(pFont);
	//		pEdit->ShowWindow(SW_SHOW);
	//		pEdit->SetWindowText(sCaption.c_str());
	//		shared_ptr<CEdit> ptr(pEdit);
	//		m_mapUIName2Wnd[sName] = ptr;	
	//	}
	//	else if (data.m_strUIClassName.find("CComBox") != string::npos)
	//	{
	//		CComboBox* pCombox = new CComboBox;
	//		CRect rc(data.m_nLeft,data.m_nTop,data.m_nLeft + data.m_nWidth ,data.m_nTop + data.m_nHeight);
	//		pCombox->Create(dwTotalStyle,rc,this,data.m_nID);	
	//		xml_node nodeDropdownItem = node.child("DropdownItem");
	//		while (!nodeDropdownItem.empty())
	//		{
	//			string sDropItem = nodeDropdownItem.attribute("Name").as_string("");
	//			pCombox->AddString(sDropItem.c_str());
	//			nodeDropdownItem = nodeDropdownItem.next_sibling();
	//		}


	//		CFont* pFont = CCollectiveComponentProvider::Instance().GetFont();
	//		pCombox->SetFont(pFont);
	//		pCombox->ShowWindow(SW_SHOW);
	//		pCombox->SetWindowText(sCaption.c_str());
	//		shared_ptr<CComboBox> ptr(pCombox);
	//		m_mapUIName2Wnd[sName] = ptr;	
	//	}
	//	node = node.next_sibling();
	//}
}