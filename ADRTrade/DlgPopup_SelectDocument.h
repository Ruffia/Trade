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
	CGridPage_SelectDocument(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGridPage_SelectDocument();


protected:
	virtual BOOL OnInitDialog();

	//移动Grid
	void _MoveGrid();

	//设计控件布局
	void _DesignLayout();

	//加载数据
	void _LoadData(const string& sTableName);

	//根据过滤条件显示数据
	void ShowData( map<string,CFieldDesc*> &mapFieldName2FieldDesc, CDataSet* pDs );

	//获取单元格数据
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
	CDlg_SelectDocument(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_SelectDocument();
	enum { IDD = IDD_DLG_Common_Popup };

protected:
	virtual BOOL OnInitDialog();

	//设计控件布局
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

