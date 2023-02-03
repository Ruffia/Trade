#pragma once
#include "DialogPlaceHolder.h"
using namespace pugi;

// CDialogPlaceHolderComposite 对话框
class CDialogPlaceHolderComposite : public CDialogPlaceHolder
{
	DECLARE_DYNAMIC(CDialogPlaceHolderComposite)

public:
	CDialogPlaceHolderComposite(CWnd* pParent = NULL);   // 标准构造函数

	// 对话框数据
	enum { IDD = IDD_DLG_Common };

protected:
	virtual void _InitLayOut();
};
