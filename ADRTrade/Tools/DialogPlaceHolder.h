#pragma once
#include <memory>
#include <map>
#include <string>
using namespace std;
#include "Tools/pugixml.hpp"
#include "Tools/pugiconfig.hpp"
using namespace pugi;
#include "DialogIDManager.h"
#include "UIData.h"

// CDialogPlaceHolder �Ի���

#define DefineClass(ClassName,BaseClass,ID) \
 class ClassName: public BaseClass  \
 {                                  \
   public:                          \
	 ClassName();                   \
	 enum { IDD = ID };             \
};                                  \


#define ImplementClass(ClassName,BaseClass)                       \
IMPLEMENT_FACTORY(BaseClass,ClassName,string,#ClassName)            \
ClassName::ClassName()                                            \
{                                                                 \
	CDialogIDMgr::Instance().Register(#ClassName,ClassName::IDD); \
}                                                                 \


class CDialogPlaceHolder : public CDialog
{
	DECLARE_DYNAMIC(CDialogPlaceHolder)

public:
	CDialogPlaceHolder(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogPlaceHolder();
	void SetLayout(string& sLayout);

// �Ի�������
	enum { IDD = IDD_DLG_Common };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//��XML�м���UI����
	virtual void _InitLayOut();

	//�������ţ���Ҫ��������
	virtual void _DesignLayout(UINT nType, int cx, int cy);

	//��XML �е�UI���ö�ȡ�� CUIData ������
	void _ReadUIData( xml_node &node, CUIData &data );

	//���� CUIData ��������ݴ���ʵ�ʵĿؼ� 
	virtual void _CreateUI( CUIData &data, xml_node node );

	DECLARE_MESSAGE_MAP()

protected:
	string m_sLayout;            //ҳ�沼�������ļ�
	string m_strDataProvider;    //�����ṩ�ߵ�����
	xml_document m_doc;
	map<string,CWnd*>  m_mapUIName2Wnd;
};