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
	vector<CMinorCycleAnalyze> m_vMinorCycleAnalyze;
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
	vector<CDailyFutureContractAnalyze> m_vFutureContractAnalyze;

};