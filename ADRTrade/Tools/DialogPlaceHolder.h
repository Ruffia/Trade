#pragma once
#include <memory>
#include <map>
#include <string>
using namespace std;
#include "Tools/pugixml.hpp"
#include "Tools/pugiconfig.hpp"
using namespace pugi;
#include "DialogIDManager.h"
#include "DBDataManager.h"
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


//ҵ���ֶεĿؼ�����
enum BusinessControlType
{
	Business_Edit = 0,
	Business_ComboBox,
	Business_CheckBox,
};


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

	//��UI�ϵ����ݱ��浽���ݿ���
	virtual void UpdateUI2DB();

	//UpdateUI2DBʱ����ȡ�ֶβ��ֵĸ���SQL���
	virtual string _CreateUpdateSQL();

	//���ݴ����ݿ��ѯ�õ������ݼ�¼ֵ����UI�ؼ�����ʾ
	virtual void UpdateDB2UI( CDataSet& ds,int index = 0);

// �Ի�������
	enum { IDD = IDD_DLG_Common };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	//��XML�м���UI����
	virtual void _InitLayOut();

	//�����ݿ��е����ݼ�¼���ص�UI����ʾ
	virtual void _LoadTradeDayData2UI();

	//�жϵ��ռ�¼�Ƿ����
	bool _CheckExistsTradeDayRecord();

	DECLARE_MESSAGE_MAP()

protected:
	string m_sLayout;    //ҳ�沼�������ļ�
	xml_document m_doc;
	map<string,CWnd*>  m_mapUIName2Wnd;

	//ҵ���ֶ�--->UI����
	map<string,CWnd*> m_mapBusiness2Control;

	string m_sBusiness;  //ҵ����
};