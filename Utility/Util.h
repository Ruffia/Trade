#pragma once
#include <Math.h>
#include <vector>
#include <comdef.h>
#include <string>
using namespace std;

//背景:得到双if 判断条件下，对应应该执行的逻辑功能的ID
//example:
//void Test(bool bA, bool bB)
//{
//	if(bA)
//	{
//		if (bB)
//		{
//			//执行复杂逻辑1
//		}
//		else
//		{
//			//执行复杂逻辑2
//		}
//	}
//	else
//	{
//		if (bB)
//		{
//			//执行复杂逻辑3
//		}
//		else
//		{
//			//执行复杂逻辑4
//		}
//	}
//}

//为了将上述代码写成更为直观简单的逻辑，配合Factory的功能， 采取以下步骤：

//void Test(bool bA, bool bB)
//{
//	string sFuctionID = GetFunctionID(bA,bAB);
//	ILogicExecuter* pExecuter = Factory<ILogicExecuter,string>::Instance().BuildProduct(sFuctionID);
//	pExecuter->Do();
//	delete pExecuter;pExecuter = NULL;
//}

//该方法的优点：
//code 阅读者只需要关注某一特定条件下的逻辑执行情况，不用关心巨大的条件判断结构
//使用建议：
//在遇到if 嵌套层次较深或者嵌套执行逻辑复杂的情况下使用

//双层if 嵌套使用
//sBoolParameterNameA, 条件A变量名
//bA,  条件A
//sBoolParameterNameB，条件B变量名
//bB,  条件B
//nPrefix, 前缀，当有多个双层if嵌套逻辑需要用这个方式重构时，用此值进行区分
string AFX_EXT_API GetFunctionID(const char* sBoolParameterNameA,bool bA, const char* sBoolParameterNameB,bool bB, const int nPrefix);


//双层if 嵌套使用
//sBoolParameterNameA, 条件A变量名
//bA,  条件A
//sBoolParameterNameB，条件B变量名
//bB,  条件B
//sProcessName 过程名
string AFX_EXT_API GetFunctionID(const char* sBoolParameterNameA,bool bA, const char* sBoolParameterNameB,bool bB, const char* sProcessName = "" );

//3层if 嵌套使用
//sBoolParameterNameA, 条件A变量名
//bA,  条件A
//sBoolParameterNameB，条件B变量名
//bB,  条件B
//sBoolParameterNameC，条件B变量名
//bC,  条件C
//nPrefix, 前缀，当有多个双层if嵌套逻辑需要用这个方式重构时，用此值进行区分
string AFX_EXT_API GetFunctionID(const char* sBoolParameterNameA,bool bA, const char* sBoolParameterNameB,bool bB,const char* sBoolParameterNameC,bool bC,const int nPrefix);

//3层if 嵌套使用
//sBoolParameterNameA, 条件A变量名
//bA,  条件A
//sBoolParameterNameB，条件B变量名
//bB,  条件B
//sBoolParameterNameC，条件B变量名
//bC,  条件C
//sProcessName 过程名
string AFX_EXT_API GetFunctionID(const char* sBoolParameterNameA,bool bA, const char* sBoolParameterNameB,bool bB,const char* sBoolParameterNameC,bool bC, const char* sProcessName = "" );


//背景:得到双if 判断条件下，对应应该执行的逻辑功能的ID
//双层if 嵌套使用
//sBoolParameterNameA, 条件A变量名
//bA,  条件A
//nSegment,  条件区段值，例如以下代码：
//void test(int n)
//{
//	if(n > 100)
//	{
//		//执行逻辑1
//	}
//	else if(n > 90)
//	{
//		//执行逻辑2
//	}
//	else if (n > 80)
//	{
//		//执行逻辑3
//	}
//	else if (n > 70)
//	{
//		//执行逻辑4
//	}
//    //...
//	////...
//}
//此时可以根据值所在区间的不同指定segment值
//sProcessName 过程名
string AFX_EXT_API GetFunctionID(const char* sBoolParameterNameA, bool bA, int nSegment, const char* sProcessName = "" );


std::string AFX_EXT_API GetTimeStamp();

//获取时间戳(毫秒)
__int64 AFX_EXT_API GetCurrentTimeStampMS();

//给定一个时间戳，按需要转换成时间字符串
std::string AFX_EXT_API GetFormatTime(const __int64 _i64Time,const char* sFormat);


//字符串分割函数
void AFX_EXT_API Split(string str, string pattern,vector<string>& result);

//根据放大倍数计算每次调整的最小单位(1倍，10倍，100倍，1000倍)
double AFX_EXT_API GetThirdSignificandWeight( double dblValue, int nPower = 1, BOOL boolIncrease = TRUE );


//计算机类型
bool AFX_EXT_API IsLittleEndian();


//获取一个浮点数的指数位大小
int AFX_EXT_API GetExponent(const double dValue);


//获取时间戳附近的整数分钟时间戳
__int64 AFX_EXT_API GetRelativeTime(const __int64 _i64Time,bool bSmaller);

//wstring 转换 string
string AFX_EXT_API wstring2string(const wstring& ws);

//string  转换 wstring
wstring AFX_EXT_API string2wstring(const string& s);

//去掉回车换行，暂时保留
string AFX_EXT_API trim(string& text);

//获取模块的路径
string AFX_EXT_API GetModulePath();


enum AFX_EXT_API WeekDay
{
	Monday = 0,
	Tuesday,
	Wednesday,
	Thursday,
	Friday,
	Saturday,
	Sunday,
};

//计算某一日是周几
//输入参数：nYear 年, nMonth 月，nDay 日
//返回值:0 周一，1 周二，2 周三，3 周四，4 周五，5 周六，6 周日
int AFX_EXT_API CaculateWeekDay(int nYear, int nMonth, int nDay);
