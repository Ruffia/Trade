// XTPWebBrowserProvider.cpp : Implementation of the CXTPWebViewProvider ActiveX Control class.
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

#include "Controls/WebBrowser/XTPWebBrowserAbstract.h"
#include "Controls/WebBrowser/Providers/XTPWebBrowserProvider.h"
#include "Controls/WebBrowser/Providers/XTPWebBrowserControlSite.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// CXTPWebBrowserAggregate

class CXTPWebBrowserAggregate : IUnknown
{
	CXTPWebBrowserProvider* m_pProvider;
	ULONG m_nRefs;

public:
	CXTPWebBrowserAggregate(CXTPWebBrowserProvider* pProvider)
		: m_pProvider(pProvider)
		, m_nRefs(1)
	{
		ASSERT(NULL != m_pProvider);
	}

	virtual ~CXTPWebBrowserAggregate()
	{
	}

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ void** ppvObject)
	{
		LPUNKNOWN pUnk = m_pProvider->GetClientInterface(riid);
		if (NULL != pUnk)
		{
			*ppvObject = pUnk;
			pUnk->AddRef();
			return S_OK;
		}

		return static_cast<HRESULT>(m_pProvider->ExternalQueryInterface(&riid, ppvObject));
	}

	virtual ULONG STDMETHODCALLTYPE AddRef(void)
	{
		m_pProvider->ExternalAddRef();
		return ++m_nRefs;
	}

	virtual ULONG STDMETHODCALLTYPE Release(void)
	{
		if (0 == --m_nRefs)
		{
			delete this;
			return 0;
		}

		m_pProvider->ExternalRelease();
		return m_nRefs;
	}
};

///////////////////////////////////////////////////////////////////////////////
// CXTPWebBrowserControlSite

CXTPWebBrowserControlSite::CXTPWebBrowserControlSite(COleControlContainer* pCtrlCont,
													 CXTPWebBrowserProvider* pProvider)
	: COleControlSite(pCtrlCont)
	, m_pProvider(pProvider)
	, m_pAggregate(NULL)
{
	EnableAggregation();
	OnCreateAggregates();
}

CXTPWebBrowserControlSite::~CXTPWebBrowserControlSite()
{
	SAFE_RELEASE(m_pAggregate);
}

BOOL CXTPWebBrowserControlSite::OnCreateAggregates()
{
	ASSERT(NULL == m_pAggregate);
	m_pAggregate = new CXTPWebBrowserAggregate(m_pProvider);
	return TRUE;
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_INTERFACE_MAP(CXTPWebBrowserControlSite, COleControlSite)
	INTERFACE_AGGREGATE(CXTPWebBrowserControlSite, m_pAggregate)
END_INTERFACE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
