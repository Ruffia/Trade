#pragma once
#include <memory>
using namespace std;

// CDialogPlaceHolder 对话框

class CDialogPlaceHolder : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogPlaceHolder)

public:
	CDialogPlaceHolder(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogPlaceHolder();

// 对话框数据
	enum { IDD = IDD_DLG_Common };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual void _InitLayOut() = 0;
	DECLARE_MESSAGE_MAP()
};
