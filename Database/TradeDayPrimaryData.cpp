#include "stdafx.h"
#include "TradeDayPrimaryData.h"


CTradeDayPrimaryData::CTradeDayPrimaryData()
{
	m_strTradeDay = "";
	m_strFutureContractName_LastTime = "";
	m_strFutureContractName = "";
	m_bNeed2UpdateFutureContractName = false;
	m_nPlace2UpdateFutureContractName = 0;
}

CTradeDayPrimaryData& CTradeDayPrimaryData::Instance()
{
	static CTradeDayPrimaryData Inst;
	return Inst;
}
