#pragma once
#include <memory>
using namespace std;

// CDialogPlaceHolder �Ի���

class CDialogPlaceHolder : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogPlaceHolder)

public:
	CDialogPlaceHolder(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogPlaceHolder();

// �Ի�������
	enum { IDD = IDD_DLG_Common };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual void _InitLayOut() = 0;
	DECLARE_MESSAGE_MAP()
};
