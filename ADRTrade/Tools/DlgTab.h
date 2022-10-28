#pragma once
#include "afxcmn.h"
#include <vector>
using namespace std;

#if _MSC_VER > 1000
#pragma once
#endif 


//ҳǩ�������
class CDlgTab : public CDialog
{
	DECLARE_DYNAMIC(CDlgTab)

public:
	CDlgTab(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgTab();

	virtual void InitPage() = 0; 
	void ShowPage(const int nCurPage);

// �Ի�������
	enum { IDD = IDD_DLG_Tab };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual void OnTcnSelchangeTabLog(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnClose();

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_Tab;
	int m_CurSelTab;
	vector<CDialog*> m_vPage; 
};
