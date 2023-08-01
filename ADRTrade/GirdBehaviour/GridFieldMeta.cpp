#include "stdafx.h"
#include "GridFieldMeta.h"

CGridFieldDesc::CGridFieldDesc()
{

}


void CGridFieldDesc::Initialize(int nParaCount,...)
{
	va_list arg_ptr;
	va_start(arg_ptr, nParaCount);
	m_strDataSourceName = va_arg(arg_ptr, string);
	m_strDataType = va_arg(arg_ptr, string);
	m_strDisplayType = va_arg(arg_ptr, string);
	m_strDisplayName = va_arg(arg_ptr, string);
	m_nColumnWidth = va_arg(arg_ptr, int);
	m_nColumnHeight = va_arg(arg_ptr, int);
	m_strControlType = va_arg(arg_ptr, string);
	m_nTextColor = va_arg(arg_ptr, int);
	va_end(arg_ptr);
}