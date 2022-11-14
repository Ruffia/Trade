#pragma once
#include "AbstractObject.h"
#include "Export.h"

class CDailyMacroEnvironment:public IAbstractObject
{
};


class CLongPeriodicAnalyze:public IAbstractObject
{
};


class CMinorCycleAnalyze:public IAbstractObject
{
};


class CDailyFutureContractAnalyze:public IAbstractObject
{
public:
	CLongPeriodicAnalyze m_oLongPeriodicAnalyze;
	map<string,CMinorCycleAnalyze> m_mapName2MinorCycleAnalyze;
};



class CSingleTradeTrack:public IAbstractObject
{

};


class CDailyTradeSummarize:public IAbstractObject
{

};


class CTradeDay:public IAbstractObject
{
public:
	CDailyMacroEnvironment m_oEnvirioment;
	map<string,CDailyFutureContractAnalyze> m_mapName2FutureContractAnalyze;
	map<string,CSingleTradeTrack>  m_mapID2Trade;
	CDailyTradeSummarize m_oTradeSummarize;
};