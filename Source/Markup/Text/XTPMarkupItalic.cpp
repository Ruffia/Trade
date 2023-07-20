// XTPMarkupItalic.cpp: implementation of the CXTPMarkupItalic class.
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

#include "StdAfx.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/Base/Types/XTPSize.h"

#include "Markup/XTPMarkupObject.h"
#include "Markup/XTPMarkupInputElement.h"
#include "Markup/XTPMarkupFrameworkContentElement.h"
#include "Markup/Text/XTPMarkupTextElement.h"
#include "Markup/Text/XTPMarkupInline.h"
#include "Markup/Text/XTPMarkupSpan.h"
#include "Markup/Text/XTPMarkupItalic.h"
#include "Markup/XTPMarkupIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupItalic

IMPLEMENT_MARKUPCLASS(L"Italic", CXTPMarkupItalic, CXTPMarkupSpan);

void CXTPMarkupItalic::RegisterMarkupClass()
{
}

CXTPMarkupItalic::CXTPMarkupItalic()
{
	SetFontStyle(1);
}

/////////////////////////////////////////////////////////////////////////
// CXTPMarkupItalic

BEGIN_DISPATCH_MAP(CXTPMarkupItalic, CXTPMarkupSpan)
END_DISPATCH_MAP()

#ifdef _XTP_ACTIVEX

BEGIN_INTERFACE_MAP(CXTPMarkupItalic, CXTPMarkupSpan)
	INTERFACE_PART(CXTPMarkupItalic, XTPDIID_MarkupItalic, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMarkupItalic, XTPDIID_MarkupItalic)
#endif
