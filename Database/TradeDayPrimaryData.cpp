#include "stdafx.h"
#include "TradeDayPrimaryData.h"


CTradeDayPrimaryData::CTradeDayPrimaryData()
{
	m_strTradeDay = "";
	m_strFutureContractName_LastTime = "";
	m_strFutureContractName = "";
	m_bNeed2UpdateFutureContractName = false;
	for (int i = 0;i < Place2UpdateFutureContractName;i++)
	{
		m_Synchronize[i] = false;
	}
}

CTradeDayPrimaryData& CTradeDayPrimaryData::Instance()
{
	static CTradeDayPrimaryData Inst;
	return Inst;
}
