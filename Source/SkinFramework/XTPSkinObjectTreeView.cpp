// XTPSkinObjectTreeView.cpp: implementation of the CXTPSkinObjectTreeView class.
//
// (c)1998-2023 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"

#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/ScrollBar/XTPScrollBase.h"

#include "SkinFramework/XTPSkinObject.h"
#include "SkinFramework/XTPSkinObjectFrame.h"
#include "SkinFramework/XTPSkinObjectTreeView.h"
#include "SkinFramework/XTPSkinManager.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CXTPSkinObjectTreeView, CXTPSkinObjectFrame)

CXTPSkinObjectTreeView::CXTPSkinObjectTreeView()
{
	m_strClassName	 = _T("TREEVIEW");
	m_nCtlColorMessage = WM_CTLCOLORLISTBOX;
}

CXTPSkinObjectTreeView::~CXTPSkinObjectTreeView()
{
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPSkinObjectTreeView, CXTPSkinObjectComCtl32Control)
	//{{AFX_MSG_MAP(CXTPSkinObjectTreeView)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

#ifndef TVM_SETLINECOLOR
#	define TVM_SETLINECOLOR (TV_FIRST + 40)
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPSkinObjectTreeView message handlers

void CXTPSkinObjectTreeView::OnSkinChanged(BOOL bPrevState, BOOL bNewState)
{
	SendMessage(TVM_SETBKCOLOR, 0, XTPToLPARAM(bNewState ? GetColor(COLOR_WINDOW) : CLR_NONE));
	SendMessage(TVM_SETTEXTCOLOR, 0,
				XTPToLPARAM(bNewState ? GetColor(COLOR_WINDOWTEXT) : CLR_NONE));
	SendMessage(TVM_SETLINECOLOR, 0,
				XTPToLPARAM(bNewState ? GetColor(COLOR_GRAYTEXT) : CLR_DEFAULT));

	CXTPSkinObjectFrame::OnSkinChanged(bPrevState, bNewState);
}

int CXTPSkinObjectTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTPSkinObjectFrame::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (GetSkinManager()->IsEnabled())
	{
		SendMessage(TVM_SETBKCOLOR, 0, XTPToLPARAM(GetColor(COLOR_WINDOW)));
		SendMessage(TVM_SETTEXTCOLOR, 0, XTPToLPARAM(GetColor(COLOR_WINDOWTEXT)));
	}

	return 0;
}
