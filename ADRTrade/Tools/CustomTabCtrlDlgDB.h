#pragma once
#include "CustomTabCtrlDlg.h"
#include "DataProvider.h"

class CCustomTabCtrlDlgBusiness : public CCustomTabCtrlDlg
{
public:
	CCustomTabCtrlDlgBusiness(CWnd* pParent = NULL);
	~CCustomTabCtrlDlgBusiness();

protected:
	virtual BOOL OnInitDialog();
	virtual void _InitLayOut();
	virtual void _DesignLayout(UINT nType, int cx, int cy);

	//���� CUIData ��������ݴ���ʵ�ʵĿؼ� 
	virtual void _CreateUI( CUIData &data, xml_node node );

	//�����ݿ��е����ݼ�¼���ص�UI����ʾ
	virtual void _LoadData2UI();

	//�жϵ��ռ�¼�Ƿ����
	bool _CheckExistsTradeDayRecord();

protected:
	IDataProvider*  m_pDataProvider;
};
