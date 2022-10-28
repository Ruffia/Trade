// DialogPlaceHolder.cpp : 实现文件
//

#include "stdafx.h"
#include "ADRTrade.h"
#include "DialogPlaceHolder.h"
#include "afxdialogex.h"


// CDialogPlaceHolder 对话框

IMPLEMENT_DYNAMIC(CDialogPlaceHolder, CDialogEx)

CDialogPlaceHolder::CDialogPlaceHolder(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogPlaceHolder::IDD, pParent)
{

}

CDialogPlaceHolder::~CDialogPlaceHolder()
{
}

void CDialogPlaceHolder::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogPlaceHolder, CDialogEx)
END_MESSAGE_MAP()


// CDialogPlaceHolder 消息处理程序
// CDlgTab 消息处理程序
BOOL CDialogPlaceHolder::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	_InitLayOut();
	return TRUE;
}