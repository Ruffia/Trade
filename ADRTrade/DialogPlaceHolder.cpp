// DialogPlaceHolder.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ADRTrade.h"
#include "DialogPlaceHolder.h"
#include "afxdialogex.h"


// CDialogPlaceHolder �Ի���

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


// CDialogPlaceHolder ��Ϣ�������
// CDlgTab ��Ϣ�������
BOOL CDialogPlaceHolder::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	_InitLayOut();
	return TRUE;
}