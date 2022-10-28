#include "StdAfx.h"
#include "LoadLayout.h"

void ILoadLayout::_LoadXML(const CString& strLayoutFile)
{
	bool bRet = _OpenXML(strLayoutFile);
	if (!bRet) return;

	_LoadLayout();
}


bool ILoadLayout::_OpenXML(const CString& strLayoutFile)
{
	bool bRet = true;
	if (!m_doc.load_file((LPSTR)(LPCTSTR)strLayoutFile)) { //¼ÓÔØxmlÎÄ¼þ
		bRet = false;
	}
	return bRet;
}


void ILoadLayout::_CloseXML()
{
}


CUIData::CUIData()
{
	m_strUIClassName = "";
	m_nLeft = 0;
	m_nTop = 0;
	m_nWidth = 1;
	m_nHeight = 1;
}