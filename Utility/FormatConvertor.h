#pragma once
#include <string>
#include <map>
using namespace std;

//��ʽת����
class AFX_EXT_CLASS CFormatConvertor
{
public:
	CFormatConvertor();

	string GetFormat(const string& sDataType );

private:
	void _Init();

private:
	map<string,string> m_DataType2Format;
};