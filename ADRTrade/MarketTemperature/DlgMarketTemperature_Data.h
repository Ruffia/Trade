#pragma once
#include "afxcmn.h"
#include "DataSet.h"
#include "afxdtctl.h"
#include "Tools/DlgGridPage.h"
#include "Tools/DialogPlaceHolder.h"

class CField;
class CFieldDesc;
class CCustomButton;

#if _MSC_VER > 1000
#pragma once
#endif 

class CGridPage_MarketTemperatureRecord : public CDlgGridPage
{
	DECLARE_DYNAMIC(CGridPage_MarketTemperatureRecord)

public:
	CGridPage_MarketTemperatureRecord(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGridPage_MarketTemperatureRecord();


protected:
	virtual BOOL OnInitDialog();

	//��XML�м���UI����
	virtual void _InitLayOut();

	//�����ݿ��е����ݼ�¼���ص�UI����ʾ
	virtual void _LoadData2UI();

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
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult); 

public:
	string m_strSelectTemplate;

private:
	CDataSet m_ds;
	CRecord* m_pSelectRecord;
};