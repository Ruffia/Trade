#pragma once
#include <memory>
#include <map>
#include <string>
using namespace std;
#include "Tools/pugixml.hpp"
#include "Tools/pugiconfig.hpp"
using namespace pugi;
#include "DBDataManager.h"
#include "DialogPlaceHolder.h"
#include "DataProvider.h"


//ҵ���ֶεĿؼ�����
enum BusinessControlType
{
	Business_Edit = 0,
	Business_ComboBox,
	Business_CheckBox,
};


class CDialogPlaceHolderBusiness : public CDialogPlaceHolder
{
	DECLARE_DYNAMIC(CDialogPlaceHolderBusiness)

public:
	CDialogPlaceHolderBusiness(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogPlaceHolderBusiness();

	void SetDataProvider(IDataProvider* pDataProvider)
	{
		m_pDataProvider = pDataProvider;
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
	virtual BOOL OnInitDialog();

	//�������ţ���Ҫ��������
	virtual void _DesignLayout();

	//���� CUIData ��������ݴ���ʵ�ʵĿؼ� 
	virtual void _CreateUI( CUIData &data, xml_node node );

	//�����ݿ��е����ݼ�¼���ص�UI����ʾ
	virtual void _LoadData2UI();

	//�жϵ��ռ�¼�Ƿ����
	bool _CheckExistsTradeDayRecord();

protected:
	//ҵ���ֶ�--->UI����
	map<string,CWnd*> m_mapBusiness2Control;
	IDataProvider*    m_pDataProvider;
};