#include "StdAfx.h"
#include "FormatConvertor.h"

CFormatConvertor::CFormatConvertor()
{
	_Init();
}

string CFormatConvertor::GetFormat(const string& sDataType )
{
	map<string,string>::iterator it = m_DataType2Format.find(sDataType);
	if (m_DataType2Format.end() == it) return "%f";
	
	return it->second;
}

void CFormatConvertor::_Init()
{
	m_DataType2Format["long"]    = "%I64d";
	m_DataType2Format["float"]   = "%f";
	m_DataType2Format["double"]  = "%.2e";
	m_DataType2Format["int"]     = "%d";
}