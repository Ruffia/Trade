
// ADRTrade.h : ADRTrade Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CADRTradeApp:
// �йش����ʵ�֣������ ADRTrade.cpp
//

class CADRTradeApp : public CWinAppEx
{
public:
	CADRTradeApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	afx_msg void OnFileNew();
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

protected:
	BOOL _InitDocumentTemplateList();
};

extern CADRTradeApp theApp;
