// DialogPlaceHolder.cpp : 实现文件
//
#include "stdafx.h"
#include "ADRTrade.h"
#include "DialogLayout.h"
#include "afxdialogex.h"
#include "Factory.h"
#include "DialogIDManager.h"
#include "Tools/StyleManager.h"
#include "UIData.h"
#include "Tools/CollectiveComponentProvider.h"
#include "Util.h"

// CDialogPlaceHolder 对话框

IMPLEMENT_DYNAMIC(CDialogLayout, CDialog)

CDialogLayout::CDialogLayout(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogLayout::IDD, pParent)
{
}

CDialogLayout::~CDialogLayout()
{
}

void CDialogLayout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogLayout, CDialog)
END_MESSAGE_MAP()


// CDialogLayout 消息处理程序
BOOL CDialogLayout::OnInitDialog()
{
	CDialog::OnInitDialog();
	_InitLayOut();
	return TRUE;
}