#pragma once
#include <memory>
#include <map>
#include <string>
using namespace std;
#include "UIData.h"
#include "Tools/pugixml.hpp"
#include "Tools/pugiconfig.hpp"
using namespace pugi;

// CDialogPlaceHolderComposite �Ի���
class CDialogPlaceHolderComposite : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogPlaceHolderComposite)

public:
	CDialogPlaceHolderComposite(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogPlaceHolderComposite();
	void SetLayout(string& sLayout)
	{
		m_sLayout = sLayout;
	}

	// �Ի�������
	enum { IDD = IDD_DLG_Common };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual void _InitLayOut();
	DECLARE_MESSAGE_MAP()

protected:
	string m_sLayout;    //ҳ�沼�������ļ�
	xml_document m_doc;
	map<string,CUIData>  m_mapUIName2Data;
};
