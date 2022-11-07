#include "stdafx.h"
#include "CollectiveComponentProvider.h"

CCollectiveComponentProvider::CCollectiveComponentProvider()
{
	m_pFont = NULL;
	_CreateFont();
}


CCollectiveComponentProvider::~CCollectiveComponentProvider()
{
	if(m_pFont)
	{
		delete m_pFont;
		m_pFont = NULL;
	}
};


CCollectiveComponentProvider& CCollectiveComponentProvider::Instance()
{
	static CCollectiveComponentProvider inst;
	return inst;
}


void CCollectiveComponentProvider::_CreateFont()
{
	m_pFont = new CFont;
	m_pFont->CreateFont(18, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_THIN, // nWeight 
		FALSE, // bItalic 
		FALSE, // bUnderline 
		0, // cStrikeOut 
		ANSI_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("Î¢ÈíÑÅºÚ")); // lpszFac
}


void CCollectiveComponentProvider::Release()
{
	if (m_pFont)
	{
		delete m_pFont;
		m_pFont = NULL;
	}
}