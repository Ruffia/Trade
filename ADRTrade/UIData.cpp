#include "StdAfx.h"
#include "UIData.h"

CUIData::CUIData()
{
	m_sName = "";
	m_strUIClassName = "";
	m_strLayout = "";
	m_strTitle = "";
	m_nID = 0;
	m_nLeft = 0;
	m_nTop = 0;
	m_nWidth = 1;
	m_nHeight = 1;
	m_sCaption = "";
	m_dwTotalStyle = 0;
	m_sDataProvider = "";
	m_pWnd = NULL;
}