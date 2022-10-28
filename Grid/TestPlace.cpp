// TestPlace.cpp : 实现文件
//

#include "stdafx.h"
#include "TestPlace.h"
#include "afxdialogex.h"


// TestPlace 对话框

IMPLEMENT_DYNAMIC(TestPlace, CDialog)

TestPlace::TestPlace(CWnd* pParent /*=NULL*/)
	: CDialog(TestPlace::IDD, pParent)
{

}

TestPlace::~TestPlace()
{
}

void TestPlace::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TestPlace, CDialog)
END_MESSAGE_MAP()


// TestPlace 消息处理程序
