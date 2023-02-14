#pragma once
#include <memory>
#include <map>
#include <string>
using namespace std;
#include "Tools/pugixml.hpp"
#include "Tools/pugiconfig.hpp"
using namespace pugi;

class CDialogPopup : public CDialog
{
	DECLARE_DYNAMIC(CDialogPopup)

public:
	CDialogPopup(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogPopup();
	void SetLayout(string& sLayout)
	{
		m_sLayout = sLayout;
	}

	void SetBusiness(string& sBusiness)
	{
		m_sBusinessSQL = sBusiness;
	}

// �Ի�������
	enum { IDD = IDD_DLG_Common_Popup };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual void _InitLayOut();
	DECLARE_MESSAGE_MAP()

protected:
	string m_sLayout;    //ҳ�沼�������ļ�
	xml_document m_doc;
	map<string,CWnd*>  m_mapUIName2Wnd;

	//ҵ���ֶ�--->UI����
	map<string,CWnd*> m_mapBusiness2Control;

	string m_sBusinessSQL;  //ҵ������
};
