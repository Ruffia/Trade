#pragma once
#include "DialogPlaceHolder.h"
using namespace pugi;

// CDialogPlaceHolderComposite �Ի���
class CDialogPlaceHolderComposite : public CDialogPlaceHolder
{
	DECLARE_DYNAMIC(CDialogPlaceHolderComposite)

public:
	CDialogPlaceHolderComposite(CWnd* pParent = NULL);   // ��׼���캯��

	// �Ի�������
	enum { IDD = IDD_DLG_Common };

protected:
	virtual void _InitLayOut();
};
