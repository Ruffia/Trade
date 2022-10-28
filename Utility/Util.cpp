#include "StdAfx.h"
#include "Util.h"

string GetFunctionID(const char* sBoolParameterNameA,bool bA, const char* sBoolParameterNameB,bool bB, const int nPrefix)
{
	char* sA = bA?"True":"False";
	char* sB = bB?"True":"False";
	char sz[128] = {0};
	sprintf_s(sz,128,"%s_%s_%s_%s_%d",sBoolParameterNameA,sA,sBoolParameterNameB,sB,nPrefix);
	string sID = sz;
	return sID;
}


string GetFunctionID(const char* sBoolParameterNameA,bool bA, const char* sBoolParameterNameB,bool bB, const char* sProcessName)
{
	char* sA = bA?"True":"False";
	char* sB = bB?"True":"False";
	char sz[128] = {0};
	sprintf_s(sz,128,"%s_%s_%s_%s_%s",sBoolParameterNameA,sA,sBoolParameterNameB,sB,sProcessName);
	string sID = sz;
	return sID;
}


string GetFunctionID(const char* sBoolParameterNameA,bool bA, const char* sBoolParameterNameB,bool bB,const char* sBoolParameterNameC,bool bC,const int nPrefix)
{
	char* sA = bA?"True":"False";
	char* sB = bB?"True":"False";
	char* sC = bC?"True":"False";
	char sz[128] = {0};
	sprintf_s(sz,128,"%s_%s_%s_%s_%s_%s_%d",sBoolParameterNameA,sA,sBoolParameterNameB,sB,sBoolParameterNameC,sC,nPrefix);
	string sID = sz;
	return sID;
}


string GetFunctionID(const char* sBoolParameterNameA,bool bA, const char* sBoolParameterNameB,bool bB,const char* sBoolParameterNameC,bool bC, const char* sProcessName)
{
	char* sA = bA?"True":"False";
	char* sB = bB?"True":"False";
	char* sC = bC?"True":"False";
	char sz[128] = {0};
	sprintf_s(sz,128,"%s_%s_%s_%s_%s_%s_%s",sBoolParameterNameA,sA,sBoolParameterNameB,sB,sBoolParameterNameC,sC,sProcessName);
	string sID = sz;
	return sID;
}


string GetFunctionID(const char* sBoolParameterNameA, bool bA, int nSegment, const char* sProcessName)
{
	char* sA = bA?"True":"False";
	char sz[128] = {0};
	sprintf_s(sz,128,"%s_%s%d_%s",sA,"Segment",nSegment,sProcessName);
	string sID = sz;
	return sID;
}


std::string GetTimeStamp() 
{
	time_t rawtime;
	struct tm * timeinfo;
	time( &rawtime );
	timeinfo = localtime( &rawtime );
	char year[5], mon[3], mday[3], hour[3], minute[3], sec[3];
	sprintf(year, "%d", timeinfo->tm_year + 1900);
	sprintf(mon, "%d", timeinfo->tm_mon + 1);
	sprintf(mday, "%d", timeinfo->tm_mday);
	sprintf(hour, "%d", timeinfo->tm_hour);
	sprintf(minute, "%d", timeinfo->tm_min);
	sprintf(sec, "%d", timeinfo->tm_sec);

	std::string yearStr = std::string(year);
	std::string monStr = std::string(mon);
	if (monStr.size() == 1) monStr = "0" + monStr;
	std::string mdayStr = std::string(mday);
	if (mdayStr.size() == 1) mdayStr = "0" + mdayStr;
	std::string hourStr = std::string(hour);
	if (hourStr.size() == 1) hourStr = "0" + hourStr;
	std::string minuteStr = std::string(minute);
	if (minuteStr.size() == 1) minuteStr = "0" + minuteStr;
	std::string secStr = std::string(sec);
	if (secStr.size() == 1) secStr = "0" + secStr;
	return yearStr + monStr + mdayStr +\
		hourStr + minuteStr + secStr;
}


__int64 GetCurrentTimeStampMS()
{
	SYSTEMTIME currentTime;
	GetLocalTime(&currentTime);

	tm temptm = { currentTime.wSecond,
		currentTime.wMinute,
		currentTime.wHour,
		currentTime.wDay,
		currentTime.wMonth - 1,
		currentTime.wYear - 1900
	};
	__int64 i64TimeStamp = mktime(&temptm) * 1000 + currentTime.wMilliseconds;

	return i64TimeStamp;
}


//根据时间戳获取时间字符串
std::string GetFormatTime(const __int64 _i64Time,const char* sFormat)
{
	time_t lTimeSatamp = _i64Time;
	lTimeSatamp += 28800;//GTM偏移8个时区得到北京时间
	tm stime;
	gmtime_s(&stime, &lTimeSatamp);
	char sz[64] = {0};
	strftime(sz, 64, sFormat, &stime);
	return std::string(sz);
}


//字符串分割函数
void Split(string str, string pattern,vector<string>& result)
{
	string::size_type pos;
	str += pattern;//扩展字符串以方便操作
	int size = str.size();
	for (int i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
}


double GetThirdSignificandWeight( double dblValue, int nPower, BOOL boolIncrease )
{
	double dblSignificandWeight = 1.0;

	if ( boolIncrease )
	{
		if (dblValue < 10.0)
			dblSignificandWeight = 0.01 * nPower;
		else if (dblValue < 100.0)
			dblSignificandWeight = 0.1 * nPower;
		else if (dblValue < 1000.0)
			dblSignificandWeight = 1.0 * nPower;
		else if (dblValue < 10000.0)
			dblSignificandWeight = 10.0 * nPower;
		else if (dblValue < 100000.0)
			dblSignificandWeight = 100.0 * nPower;
		else
			dblSignificandWeight = 1000.0 * nPower;
	}
	else
	{
		if (dblValue < 10.0 + 0.1 * nPower)
		{
			if (dblValue >= 10.0)
				dblSignificandWeight = dblValue - 9.99;
			else
				dblSignificandWeight = 0.01 * nPower;
		}
		else if (dblValue < 100.0 + 1.0 * nPower)
		{
			if (dblValue >= 100.0)
				dblSignificandWeight = dblValue - 99.9;
			else
				dblSignificandWeight = 0.1 * nPower;
		}
		else if (dblValue < 1000.0 + 10.0 * nPower)
		{
			if (dblValue >= 1000.0)
				dblSignificandWeight = dblValue - 999.0;
			else
				dblSignificandWeight = 1.0 * nPower;
		}
		else if (dblValue < 10000.0 + 100.0 * nPower)
		{
			if (dblValue >= 10000.0)
				dblSignificandWeight = dblValue - 9990.0;
			else
				dblSignificandWeight = 10.0 * nPower;
		}
		else if (dblValue < 100000.0 + 1000.0 * nPower)
		{
			if (dblValue >= 100000.0)
				dblSignificandWeight = dblValue - 99900.0;
			else
				dblSignificandWeight = 100.0 * nPower;
		}
		else
			dblSignificandWeight = 1000.0 * nPower;
	}

	return dblSignificandWeight;
}


bool IsLittleEndian()
{
	int x = 1;
	if (*(char*) &x == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}


int GetExponent(const double dValue)
{
	double dTemp = dValue;
	int n = 0;
	if(0 == dTemp) return n;
	if(1 == dValue) return n;

	if (dTemp < 1)
	{
		while (dTemp < 1)
		{
			dTemp *= 10;
			n--;
		}
	}
	else
	{
		int nTemp = dTemp;
		while (nTemp > 1)
		{
			nTemp /= 10;
			n++;
		}
	}

	return n;
}



__int64 GetRelativeTime(const __int64 _i64Time,bool bSmaller)
{
	__int64 timestamp = 0;
	COleDateTime dt(_i64Time);
	if (bSmaller)
	{
		COleDateTime tDateTime(dt.GetYear(), dt.GetMonth(), dt.GetDay(), 
			dt.GetHour(), dt.GetMinute(), 0);

		SYSTEMTIME systime;
		VariantTimeToSystemTime(tDateTime, &systime);
		//时间戳最小值为北京时间:1970-01-01 08:00:00
		if (systime.wYear <= 1970 && systime.wMonth <= 1 && systime.wDay <= 1 && systime.wHour <= 7 
			&& systime.wMinute <= 59 && systime.wSecond <= 59)
			return -1;
		CTime cTimeFromDB(systime);
		timestamp = cTimeFromDB.GetTime();//CTime->时间戳
	}
	else
	{
		int nMinute = dt.GetMinute();
		int nHour = dt.GetHour();
		int nDay = dt.GetDay();
		int nMonth = dt.GetMonth();
		int nYear = dt.GetYear();
		const int MonthDay[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
		if (nMinute == 59)
		{
			nMinute = 0;
			nHour = nHour + 1;
			if (24 == nHour)
			{
				nHour = 0;
				nDay = nDay + 1;
			}

			//一个月最多几天，在此做比较
			const int nMaxDay = MonthDay[dt.GetMonth()];
			if(nDay > nMaxDay)
			{
				nDay = 1;
				nMonth = nMonth + 1;
			}

			if (nMonth > 12)
			{
				nMonth = 1;
				nYear = nYear + 1;
			}

		}
		else
		{
			nMinute++;
		}

		COleDateTime tDateTime(nYear, nMonth, nDay, nHour,nMinute,0);

		SYSTEMTIME systime;
		VariantTimeToSystemTime(tDateTime, &systime);
		//时间戳最小值为北京时间:1970-01-01 08:00:00
		if (systime.wYear <= 1970 && systime.wMonth <= 1 && systime.wDay <= 1 && systime.wHour <= 7 
			&& systime.wMinute <= 59 && systime.wSecond <= 59)
			return -1;
		CTime cTimeFromDB(systime);
		timestamp = cTimeFromDB.GetTime();//CTime->时间戳

	}

	return timestamp;
}



string wstring2string(const wstring& ws)
{
	_bstr_t t = ws.c_str();  
	char* pchar = (char*)t;  
	string result = pchar;  
	return result;  
}

wstring string2wstring(const string& s)
{
	_bstr_t t = s.c_str();  
	wchar_t* pwchar = (wchar_t*)t;  
	wstring result = pwchar;  
	return result; 
}


string trim(string& text)
{
	if(!text.empty())
	{
		text.erase(0, text.find_first_not_of(_T(" \n\r\t")));
		text.erase(text.find_last_not_of(_T(" \n\r\t")) + 1);
	}
	return text;
}


string GetModulePath()
{
	char exeFullPath[255] = {0};
	GetModuleFileNameA(NULL,exeFullPath,MAX_PATH);
	char path[255] = {0};
	sprintf(path, "%s", exeFullPath);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';

	return string(path);
}