#pragma once
#include "resource.h"

// TestPlace �Ի���

class AFX_EXT_CLASS TestPlace : public CDialog
{
	DECLARE_DYNAMIC(TestPlace)

public:
	TestPlace(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~TestPlace();

// �Ի�������
	enum { IDD = IDD_DIALOG_TEST_PLACE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
