#pragma once
#include <string>
using namespace std;
#include "pugixml.hpp"
#include "pugiconfig.hpp"
using namespace pugi;

// CDialogLayout �Ի���
class CDialogLayout : public CDialog
{
	DECLARE_DYNAMIC(CDialogLayout)

public:
	CDialogLayout(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogLayout();
	void SetLayout(string& sLayout)
	{
		m_sLayout = sLayout;
	}

// �Ի�������
	enum { IDD = IDD_DLG_Common };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual void _InitLayOut() = 0;
	virtual void _ReLayout() = 0;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

protected:
	string m_sLayout;    //ҳ�沼�������ļ�
	xml_document m_doc;
};
