#include "stdafx.h"
#include "DataProvider_DailyTradeSummarize.h"

IMPLEMENT_FACTORY(IDataProvider,DataProvider_DailyTradeSummarize, string,"DailyTradeSummarize")

void DataProvider_DailyTradeSummarize::LoadData(CDataSet& ds)
{

}

void DataProvider_DailyTradeSummarize::UpdateUI2DB(CDataSet& ds)
{

}


void DataProvider_DailyTradeSummarize::UpdateDB2UI(CDataSet& ds)
{

}


IMPLEMENT_FACTORY(IDataProvider,DataProvider_LongPeriodicAnalyze, string,"LongPeriodicAnalyze")
void DataProvider_LongPeriodicAnalyze::LoadData(CDataSet& ds)
{
}

void DataProvider_LongPeriodicAnalyze::UpdateUI2DB(CDataSet& ds)
{
}


void DataProvider_LongPeriodicAnalyze::UpdateDB2UI(CDataSet& ds)
{
}


IMPLEMENT_FACTORY(IDataProvider,DataProvider_DailyTraceEvidence, string,"DailyTraceEvidence")
void DataProvider_DailyTraceEvidence::LoadData(CDataSet& ds)
{
}

void DataProvider_DailyTraceEvidence::UpdateUI2DB(CDataSet& ds)
{
}


void DataProvider_DailyTraceEvidence::UpdateDB2UI(CDataSet& ds)
{
}


IMPLEMENT_FACTORY(IDataProvider,DataProvider_DailyTraceConflict, string,"DailyTraceConflict")
void DataProvider_DailyTraceConflict::LoadData(CDataSet& ds)
{
}

void DataProvider_DailyTraceConflict::UpdateUI2DB(CDataSet& ds)
{
}


void DataProvider_DailyTraceConflict::UpdateDB2UI(CDataSet& ds)
{
}