#pragma once
#include "resource.h"

// TestPlace 对话框

class AFX_EXT_CLASS TestPlace : public CDialog
{
	DECLARE_DYNAMIC(TestPlace)

public:
	TestPlace(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~TestPlace();

// 对话框数据
	enum { IDD = IDD_DIALOG_TEST_PLACE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
