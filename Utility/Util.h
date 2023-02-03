#pragma once
#include <Math.h>
#include <vector>
#include <comdef.h>
#include <string>
using namespace std;

//����:�õ�˫if �ж������£���ӦӦ��ִ�е��߼����ܵ�ID
//example:
//void Test(bool bA, bool bB)
//{
//	if(bA)
//	{
//		if (bB)
//		{
//			//ִ�и����߼�1
//		}
//		else
//		{
//			//ִ�и����߼�2
//		}
//	}
//	else
//	{
//		if (bB)
//		{
//			//ִ�и����߼�3
//		}
//		else
//		{
//			//ִ�и����߼�4
//		}
//	}
//}

//Ϊ�˽���������д�ɸ�Ϊֱ�ۼ򵥵��߼������Factory�Ĺ��ܣ� ��ȡ���²��裺

//void Test(bool bA, bool bB)
//{
//	string sFuctionID = GetFunctionID(bA,bAB);
//	ILogicExecuter* pExecuter = Factory<ILogicExecuter,string>::Instance().BuildProduct(sFuctionID);
//	pExecuter->Do();
//	delete pExecuter;pExecuter = NULL;
//}

//�÷������ŵ㣺
//code �Ķ���ֻ��Ҫ��עĳһ�ض������µ��߼�ִ����������ù��ľ޴�������жϽṹ
//ʹ�ý��飺
//������if Ƕ�ײ�ν������Ƕ��ִ���߼����ӵ������ʹ��

//˫��if Ƕ��ʹ��
//sBoolParameterNameA, ����A������
//bA,  ����A
//sBoolParameterNameB������B������
//bB,  ����B
//nPrefix, ǰ׺�����ж��˫��ifǶ���߼���Ҫ�������ʽ�ع�ʱ���ô�ֵ��������
string AFX_EXT_API GetFunctionID(const char* sBoolParameterNameA,bool bA, const char* sBoolParameterNameB,bool bB, const int nPrefix);


//˫��if Ƕ��ʹ��
//sBoolParameterNameA, ����A������
//bA,  ����A
//sBoolParameterNameB������B������
//bB,  ����B
//sProcessName ������
string AFX_EXT_API GetFunctionID(const char* sBoolParameterNameA,bool bA, const char* sBoolParameterNameB,bool bB, const char* sProcessName = "" );

//3��if Ƕ��ʹ��
//sBoolParameterNameA, ����A������
//bA,  ����A
//sBoolParameterNameB������B������
//bB,  ����B
//sBoolParameterNameC������B������
//bC,  ����C
//nPrefix, ǰ׺�����ж��˫��ifǶ���߼���Ҫ�������ʽ�ع�ʱ���ô�ֵ��������
string AFX_EXT_API GetFunctionID(const char* sBoolParameterNameA,bool bA, const char* sBoolParameterNameB,bool bB,const char* sBoolParameterNameC,bool bC,const int nPrefix);

//3��if Ƕ��ʹ��
//sBoolParameterNameA, ����A������
//bA,  ����A
//sBoolParameterNameB������B������
//bB,  ����B
//sBoolParameterNameC������B������
//bC,  ����C
//sProcessName ������
string AFX_EXT_API GetFunctionID(const char* sBoolParameterNameA,bool bA, const char* sBoolParameterNameB,bool bB,const char* sBoolParameterNameC,bool bC, const char* sProcessName = "" );


//����:�õ�˫if �ж������£���ӦӦ��ִ�е��߼����ܵ�ID
//˫��if Ƕ��ʹ��
//sBoolParameterNameA, ����A������
//bA,  ����A
//nSegment,  ��������ֵ���������´��룺
//void test(int n)
//{
//	if(n > 100)
//	{
//		//ִ���߼�1
//	}
//	else if(n > 90)
//	{
//		//ִ���߼�2
//	}
//	else if (n > 80)
//	{
//		//ִ���߼�3
//	}
//	else if (n > 70)
//	{
//		//ִ���߼�4
//	}
//    //...
//	////...
//}
//��ʱ���Ը���ֵ��������Ĳ�ָͬ��segmentֵ
//sProcessName ������
string AFX_EXT_API GetFunctionID(const char* sBoolParameterNameA, bool bA, int nSegment, const char* sProcessName = "" );


std::string AFX_EXT_API GetTimeStamp();

//��ȡʱ���(����)
__int64 AFX_EXT_API GetCurrentTimeStampMS();

//����һ��ʱ���������Ҫת����ʱ���ַ���
std::string AFX_EXT_API GetFormatTime(const __int64 _i64Time,const char* sFormat);


//�ַ����ָ��
void AFX_EXT_API Split(string str, string pattern,vector<string>& result);

//���ݷŴ�������ÿ�ε�������С��λ(1����10����100����1000��)
double AFX_EXT_API GetThirdSignificandWeight( double dblValue, int nPower = 1, BOOL boolIncrease = TRUE );


//���������
bool AFX_EXT_API IsLittleEndian();


//��ȡһ����������ָ��λ��С
int AFX_EXT_API GetExponent(const double dValue);


//��ȡʱ�����������������ʱ���
__int64 AFX_EXT_API GetRelativeTime(const __int64 _i64Time,bool bSmaller);

//wstring ת�� string
string AFX_EXT_API wstring2string(const wstring& ws);

//string  ת�� wstring
wstring AFX_EXT_API string2wstring(const string& s);

//ȥ���س����У���ʱ����
string AFX_EXT_API trim(string& text);

//��ȡģ���·��
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

//����ĳһ�����ܼ�
//���������nYear ��, nMonth �£�nDay ��
//����ֵ:0 ��һ��1 �ܶ���2 ������3 ���ģ�4 ���壬5 ������6 ����
int AFX_EXT_API CaculateWeekDay(int nYear, int nMonth, int nDay);
