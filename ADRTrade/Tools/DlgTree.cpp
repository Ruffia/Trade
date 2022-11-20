#include "stdafx.h"
#include "DlgTree.h"

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
	m_ctrlTree.InsertItem("seven");
	m_ctrlTree.InsertItem("eight");
	m_ctrlTree.InsertItem("nine");
	m_ctrlTree.InsertItem("ten");
	m_ctrlTree.InsertItem("eleven");
	m_ctrlTree.InsertItem("twelf");
	m_ctrlTree.InsertItem("thirteen");
	m_ctrlTree.InsertItem("fourteen");
	m_ctrlTree.InsertItem("fivteen");
	m_ctrlTree.InsertItem("sixteen");
	m_ctrlTree.InsertItem("seventeen");
	m_ctrlTree.InsertItem("eighteen");
	m_ctrlTree.InsertItem("nineteen");
	m_ctrlTree.InsertItem("twenty");

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
