#pragma once
#include <string>
using namespace std;
#include "Export.h"

//在 LongPeriodicAnalyze 界面更新FutureContractName 后,导致其他
//需要更新 FutureContractName 的地方
//1.MinorCycleAnalyze
//2.Future_DailyTraceEvidence
//3.Future_DailyTraceConflict
#define Place2UpdateFutureContractName 3

enum  ESynchronizeBusiness
{
	UI_MinorCycleAnalyze,
	UI_DailyTraceEvidence,
	UI_DailyTraceConflict,
};

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
	//当 m_bNeed2UpdateFutureContractName 值为true时， 
	// m_Synchronize[X] = true 表示已同步, m_Synchronize[X] = false 表示未同步
	bool    m_Synchronize[Place2UpdateFutureContractName];  

};