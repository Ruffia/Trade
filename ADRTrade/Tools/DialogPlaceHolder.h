#pragma once
#include <memory>
#include <map>
#include <string>
using namespace std;
#include "Tools/pugixml.hpp"
#include "Tools/pugiconfig.hpp"
using namespace pugi;
#include "Business/BusinessNode.h"
#include "DialogIDManager.h"
class CBusinessEdit;
class CRecord;

// CDialogPlaceHolder �Ի���

#define DefineClass(ClassName,BaseClass,ID) \
 class ClassName: public BaseClass  \
 {                                  \
   public:                          \
	 ClassName();                   \
	 enum { IDD = ID };             \
};                                  \


#define ImplementClass(ClassName,BaseClass)                       \
IMPLEMENT_FACTORY(CDialogPlaceHolder,ClassName,string,#ClassName)            \
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
	void SetLayout(string& sLayout)
	{
		m_sLayout = sLayout;
	}

	void SetBusiness(string& sBusiness)
	{
		m_sBusiness = sBusiness;
	}

	void AddPrimaryKey(string& sKeyName,FieldValue& KeyValue)
	{
		m_mapPrimaryKey2Value[sKeyName] = KeyValue;
	}

	//��UI�ϵ����ݱ��浽���ݿ���
	virtual void UpdateUI2DB();

	//���ݴ����ݿ��ѯ�õ������ݼ�¼ֵ����UI�ؼ�����ʾ
	virtual void UpdateDB2UI( CRecord* pRecord );

// �Ի�������
	enum { IDD = IDD_DLG_Common };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	//��XML�м���UI����
	virtual void _InitLayOut();

	//�����ݿ��е����ݼ�¼���ص�UI����ʾ
	virtual void _LoadData2UI();

	//�жϵ��ռ�¼�Ƿ����
	bool _CheckExistsTradeDayRecord();

	DECLARE_MESSAGE_MAP()

protected:
	string m_sLayout;    //ҳ�沼�������ļ�
	xml_document m_doc;
	map<string,CWnd*>  m_mapUIName2Wnd;

	//ҵ���ֶ�--->UI����
	map<string,CWnd*> m_mapBusiness2Control;

	//����--->ֵ
	map<string,FieldValue>  m_mapPrimaryKey2Value;

	string m_sBusiness;  //ҵ����
};