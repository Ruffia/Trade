// XTPHelpers.h interface
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTHELPERS_H__)
#	define __XTHELPERS_H__

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

// Safely selects object into device context
template<class T>
class CXTPContextObjectHandler
{
	T* m_pObject;
	CDC* m_pDC;

public:
	CXTPContextObjectHandler(CDC* pDC, T* object)
		: m_pDC(pDC)
		, m_pObject(pDC->SelectObject(object))
	{
	}

	~CXTPContextObjectHandler()
	{
		if (m_pObject)
		{
			m_pDC->SelectObject(m_pObject);
		}
	}
};

// Background mode context handler
class CXTPContextBkModeHandler
{
	int m_nMode;
	CDC* m_pDC;

public:
	CXTPContextBkModeHandler(CDC* pDC, int nNewMode)
		: m_nMode(pDC->SetBkMode(nNewMode))
		, m_pDC(pDC)
	{
	}

	~CXTPContextBkModeHandler()
	{
		m_pDC->SetBkMode(m_nMode);
	}
};

// Text color context handler
class CXTPContextTextColorHandler
{
	COLORREF m_clrFore;
	CDC* m_pDC;

public:
	CXTPContextTextColorHandler(CDC* pDC, int clrFore)
		: m_clrFore(pDC->SetTextColor(static_cast<COLORREF>(clrFore)))
		, m_pDC(pDC)
	{
	}

	~CXTPContextTextColorHandler()
	{
		m_pDC->SetTextColor(m_clrFore);
	}
};

// Background color context handler
class CXTPContextBkColorHandler
{
	COLORREF m_clrBack;
	CDC* m_pDC;

public:
	CXTPContextBkColorHandler(CDC* pDC, int clrBack)
		: m_clrBack(pDC->SetBkColor(static_cast<COLORREF>(clrBack)))
		, m_pDC(pDC)
	{
	}

	~CXTPContextBkColorHandler()
	{
		m_pDC->SetBkColor(m_clrBack);
	}
};

// Handles viewport origin
class CXTPContextViewPortOrgHandler
{
	CPoint m_pointOrigin;
	CDC* m_pDC;

public:
	CXTPContextViewPortOrgHandler(CDC* pDC, const CPoint& pointOrigin)
		: m_pointOrigin(pDC->SetViewportOrg(pointOrigin))
		, m_pDC(pDC)
	{
	}

	~CXTPContextViewPortOrgHandler()
	{
		m_pDC->SetViewportOrg(m_pointOrigin);
	}
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTHELPERS_H__)

//}}AFX_CODEJOCK_PRIVATE
