#include "stdafx.h"
#include "PrimaryKeyRule.h"
#include "FieldMeta.h"
#include "../Common/Factory.h"

IPrimaryKeyRule::IPrimaryKeyRule()
{
	m_pFieldDesc = NULL;
}

IMPLEMENT_FACTORY(IPrimaryKeyRule,CPrimaryKey_TradeDay,string,"TradeDay")
string CPrimaryKey_TradeDay::GetInsertSQL()
{
	COleDateTime dtNOw = COleDateTime::GetCurrentTime();
	CString strDate = dtNOw.Format("%Y-%m-%d");

	char sz[256] = {0};
	sprintf_s(sz,256,"%s%s%s","'",strDate,"'");
	return string(sz);
}

string CPrimaryKey_TradeDay::GetUpdateSQL()
{
	COleDateTime dtNOw = COleDateTime::GetCurrentTime();
	CString strDate = dtNOw.Format("%Y-%m-%d");

	char sz[256] = {0};
	sprintf_s(sz,256,"%s = %s%s%s","TradeDay","'",strDate,"'");
	return string(sz);
}


IMPLEMENT_FACTORY(IPrimaryKeyRule,CPrimaryKey_FutureContractName,string,"FutureContractName")
string CPrimaryKey_FutureContractName::GetInsertSQL()
{
	char sz[256] = {0};
	COleDateTime dtNOw = COleDateTime::GetCurrentTime();
	CString strDate = dtNOw.Format("%Y%m");
	sprintf_s(sz,256,"%sFuture%s%s","'",strDate,"'");
	return string(sz);
}

string CPrimaryKey_FutureContractName::GetUpdateSQL()
{
	char sz[256] = {0};
	COleDateTime dtNOw = COleDateTime::GetCurrentTime();
	CString strDate = dtNOw.Format("%Y%m");
	sprintf_s(sz,256,"%s = %sFuture%s%s","FutureContractName","'",strDate,"'");
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