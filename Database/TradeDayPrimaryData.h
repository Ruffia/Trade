#pragma once
#include <string>
using namespace std;
#include "Export.h"

//交易日主数据
class EXPORT_CALSS CTradeDayPrimaryData
{
protected:
	CTradeDayPrimaryData();

public:
	static CTradeDayPrimaryData& Instance();


public:
	string  m_strTradeDay;
	//由于
	string  m_strFutureContractName_LastTime;  //上次更新时记录的FutureContractName
	string  m_strFutureContractName;           //上次更新时记录的FutureContractName
	bool    m_bNeed2UpdateFutureContractName;

};