#pragma once
#include "DialogPlaceHolderDB.h"
using namespace pugi;

// CDialogPlaceHolderComposite �Ի���
class CDialogPlaceHolderComposite : public CDialogPlaceHolderBusiness
{
	DECLARE_DYNAMIC(CDialogPlaceHolderComposite)

public:
	CDialogPlaceHolderComposite(CWnd* pParent = NULL);   // ��׼���캯��

	// �Ի�������
	enum { IDD = IDD_DLG_Common };

protected:
	virtual void _InitLayOut();
};
