#pragma once
#include "Tools/DialogPlaceHolder.h"

#define Combox_TechnicalIndex0         10013
#define Combox_TechnicalIndexValue0    10014
#define Combox_TechnicalIndex1         10023
#define Combox_TechnicalIndexValue1    10024
#define Combox_TechnicalIndex2         10033
#define Combox_TechnicalIndexValue2    10034

class CDialogFutureContract_DailyTraceEvidence : public CDialogPlaceHolder
{
public:
	CDialogFutureContract_DailyTraceEvidence(CWnd* pParent = NULL);
	enum { IDD = IDD_DLG_Common };

	virtual void UpdateUI2DB();
	virtual void UpdateDB2UI(CDataSet& ds,int index = 0);

protected:
	virtual void _InitLayOut();
	virtual void _LoadTradeDayData2UI();
	void _ChangeComboxDropdownList( const string& strComboxTechnicalIndex_Name, const string& strComboxTechnicalIndexValue_Name ); 
	afx_msg void OnSelComboChange_TechnicalIndex0();
	afx_msg void OnSelComboChange_TechnicalIndex1();
	afx_msg void OnSelComboChange_TechnicalIndex2();
	DECLARE_MESSAGE_MAP()
	
};