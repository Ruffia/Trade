#include "stdafx.h"
#include "DialogPlaceHolder_MinorCycleAnalyze.h"
#include "../Common/Factory.h"
#include "DBDataManager.h"
#include "Util.h"
#include "PrimaryKeyRule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CDialogTabItem_MinorCycleAnalyze, CDialogPlaceHolder)
CDialogTabItem_MinorCycleAnalyze::CDialogTabItem_MinorCycleAnalyze(CWnd* pParent /*=NULL*/)
	: CDialogPlaceHolder(pParent)
{
	m_strItemName = "";
}

void CDialogTabItem_MinorCycleAnalyze::_LoadTradeDayData2UI()
{
	bool bExists = _CheckExistsTradeDayRecord();
	if (!bExists) return;

	vector<CFieldDesc*> vFieldDesc;
	CDBDataManager::Instance().GetFieldMetaData(m_sBusiness,vFieldDesc);

	vector<CFieldDesc*> vPrimaryKey;
	CDBDataManager::Instance().GetPrimaryKey(m_sBusiness,vPrimaryKey);

	const string strTradeDay = "TradeDay";
	const string strRecordTime = "RecordTime";
	CFieldDesc* pTradeDayDesc = NULL;
	CFieldDesc* pRecordTimeDesc = NULL;
	for (int i = 0;i < vPrimaryKey.size();i++)
	{
		CFieldDesc* pFieldDesc = vPrimaryKey[i];
		if (pFieldDesc && pFieldDesc->m_strFieldName.find(strTradeDay) != string::npos)
		{
			pTradeDayDesc = pFieldDesc;
		}
		else if (pFieldDesc && pFieldDesc->m_strFieldName.find(strRecordTime) != string::npos)
		{
			pRecordTimeDesc = pFieldDesc;
		}
	}

	if(!pTradeDayDesc || !pRecordTimeDesc) return;

	COleDateTime dtNOw = COleDateTime::GetCurrentTime();
	CString strDate = dtNOw.Format("%Y-%m-%d");

	FieldValue vKey;
	vKey.SetDataType("string");
	vKey.SetValueString(strDate);

	string sSQL = "select * from ";
	sSQL += m_sBusiness;
	sSQL += " where ";
	sSQL += pTradeDayDesc->m_strFieldName;
	sSQL += " = '";
	sSQL += vKey.GetValueAsString();
	sSQL += "'";
	sSQL += " and ";
	sSQL += pRecordTimeDesc->m_strFieldName;
	sSQL += " = '";
	sSQL += m_strItemName;
	sSQL += "'";

	CDataSet ds;
	CDBDataManager::Instance().LoadData(sSQL,m_sBusiness,ds);
	if(ds.Size() != 1) return;   //此处有且仅有一条记录

	CRecord* pRecord = ds[0];
	if(!pRecord) return;

	UpdateDB2UI(pRecord);
}