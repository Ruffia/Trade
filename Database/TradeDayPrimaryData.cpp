#include "stdafx.h"
#include "TradeDayPrimaryData.h"


CTradeDayPrimaryData::CTradeDayPrimaryData()
{
	m_strTradeDay = "";
	m_strFutureContractName = "";
}

CTradeDayPrimaryData& CTradeDayPrimaryData::Instance()
{
	static CTradeDayPrimaryData Inst;
	return Inst;
}
