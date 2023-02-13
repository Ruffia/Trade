#include "stdafx.h"
#include "PrimaryKeyRule.h"
#include "FieldMeta.h"
#include "TradeDayPrimaryData.h"
#include "../Common/Factory.h"

IPrimaryKeyRule::IPrimaryKeyRule()
{
	m_pFieldDesc = NULL;
}

IMPLEMENT_FACTORY(IPrimaryKeyRule,CPrimaryKey_TradeDay,string,"TradeDay")
string CPrimaryKey_TradeDay::GetInsertSQL()
{
	char sz[256] = {0};
	sprintf_s(sz,256,"%s%s%s","'",CTradeDayPrimaryData::Instance().m_strTradeDay.c_str(),"'");
	return string(sz);
}

string CPrimaryKey_TradeDay::GetUpdateSQL()
{
	char sz[256] = {0};
	sprintf_s(sz,256,"%s = %s%s%s","TradeDay","'",CTradeDayPrimaryData::Instance().m_strTradeDay.c_str(),"'");
	return string(sz);
}


IMPLEMENT_FACTORY(IPrimaryKeyRule,CPrimaryKey_FutureContractName,string,"FutureContractName")
string CPrimaryKey_FutureContractName::GetInsertSQL()
{
	char sz[256] = {0};
	CTradeDayPrimaryData::Instance().m_strFutureContractName_LastTime = "Future" + CTradeDayPrimaryData::Instance().m_strTradeDay;
	CTradeDayPrimaryData::Instance().m_strFutureContractName = CTradeDayPrimaryData::Instance().m_strFutureContractName_LastTime;
	sprintf_s(sz,256,"%sFuture%s%s","'",CTradeDayPrimaryData::Instance().m_strTradeDay.c_str(),"'");
	return string(sz);
}

string CPrimaryKey_FutureContractName::GetUpdateSQL()
{
	char sz[256] = {0};
	CTradeDayPrimaryData::Instance().m_strFutureContractName_LastTime = "Future" + CTradeDayPrimaryData::Instance().m_strTradeDay;
	CTradeDayPrimaryData::Instance().m_strFutureContractName = CTradeDayPrimaryData::Instance().m_strFutureContractName_LastTime;
	sprintf_s(sz,256,"%s = %sFuture%s%s","FutureContractName","'",CTradeDayPrimaryData::Instance().m_strTradeDay.c_str(),"'");
	return string(sz);
}


IMPLEMENT_FACTORY(IPrimaryKeyRule,CPrimaryKey_RecordTime,string,"RecordTime")
string CPrimaryKey_RecordTime::GetInsertSQL()
{
	char sz[256] = {0};
	COleDateTime dtNOw = COleDateTime::GetCurrentTime();
	sprintf_s(sz,256,"%s%d:%d%s","'",dtNOw.GetHour(),dtNOw.GetMinute(),"'");
	return string(sz);
}

string CPrimaryKey_RecordTime::GetUpdateSQL()
{
	char sz[256] = {0};
	COleDateTime dtNOw = COleDateTime::GetCurrentTime();
	sprintf_s(sz,256,"%s = %s%d:%d%s","RecordTime","'",dtNOw.GetHour(),dtNOw.GetMinute(),"'");
	return string(sz);
}