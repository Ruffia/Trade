#pragma once
#include "afxcmn.h"
#include "DataSet.h"
#include "afxdtctl.h"
#include "Tools/DlgGridPage.h"

class CField;
class CFieldDesc;
class CCustomButton;

#if _MSC_VER > 1000
#pragma once
#endif 

class CGridPage_SelectDocument : public CDlgGridPage
{
	DECLARE_DYNAMIC(CGridPage_SelectDocument)

public:
	CGridPage_SelectDocument(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGridPage_SelectDocument();


protected:
	virtual BOOL OnInitDialog();

	//�ƶ�Grid
	void _MoveGrid();

	//��ƿؼ�����
	void _DesignLayout();

	//��������
	void _LoadData(const string& sTableName);

	//���ݹ���������ʾ����
	void ShowData( map<string,CFieldDesc*> &mapFieldName2FieldDesc, CDataSet* pDs );

	//��ȡ��Ԫ������
	void GetCellData( CDataSet* pDs,vector<CFieldDesc*>& vColumn,int row, int col, CString &str );

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult); 
	DECLARE_MESSAGE_MAP()

public:
	string m_strSelectTemplate;

private:
	CDataSet m_ds;
	CRecord* m_pSelectRecord;
};



class CDlg_SelectDocument : public CDialog
{
	DECLARE_DYNAMIC(CDlg_SelectDocument)

public:
	CDlg_SelectDocument(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_SelectDocument();
	enum { IDD = IDD_DLG_Common_Popup };

protected:
	virtual BOOL OnInitDialog();

	//��ƿؼ�����
	void _DesignLayout();
	CCustomButton* _CreateButton(const CRect& rectButton,int nButtonID);

protected:
	afx_msg void OnBnClickedButtonSelectDocument();
	DECLARE_MESSAGE_MAP()

public:
	string m_strSelectTemplate;

private:
	CGridPage_SelectDocument* m_pGridPage;
	CCustomButton* m_pBtnSelect; 
};

