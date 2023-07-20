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

#include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"
#include <atlbase.h>
#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceManager.h"

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
// CXTPWebBrowserProvider

CXTPWebBrowserProvider::CXTPWebBrowserProvider(IXTPWebBrowserHost& host)
	: m_Host(host)
	, m_pCtrlContainer(new COleControlContainer(&host.GetHostWindow()))
	, m_pBrowserObject(NULL)
	, m_pSite(NULL)
	, m_parrClientInterfaces(new ClientInterfaceArray())
{
}

CXTPWebBrowserProvider::~CXTPWebBrowserProvider()
{
	ReleaseSite();

	INT_PTR nCount = m_parrClientInterfaces->GetSize();
	for (INT_PTR i = 0; i < nCount; ++i)
		m_parrClientInterfaces->GetAt(i).pUnk->Release();

	SAFE_DELETE(m_parrClientInterfaces);
	SAFE_DELETE(m_pCtrlContainer);
}

void CXTPWebBrowserProvider::SetClientInterface(REFIID riid, IUnknown* pUnk)
{
	ASSERT(NULL != pUnk);
	if (NULL == pUnk)
		return;

	LPUNKNOWN pUnkExisting = GetClientInterface(riid);
	if (NULL != pUnkExisting)
	{
		ASSERT(!"The specified client interface has already been set");
		return;
	}

	ClientInterface ci;
	ci.iid  = riid;
	ci.pUnk = pUnk;
	m_parrClientInterfaces->Add(ci);

	pUnk->AddRef();
}

IUnknown* CXTPWebBrowserProvider::GetClientInterface(REFIID riid) const
{
	INT_PTR nCount = m_parrClientInterfaces->GetSize();

	for (INT_PTR i = 0; i < nCount; ++i)
	{
		const ClientInterface& ci = m_parrClientInterfaces->GetAt(i);
		if (IsEqualIID(ci.iid, riid))
			return ci.pUnk;
	}

	return NULL;
}

void CXTPWebBrowserProvider::RemoveClientInterface(REFIID riid)
{
	INT_PTR nCount = m_parrClientInterfaces->GetSize();

	for (INT_PTR i = 0; i < nCount; ++i)
	{
		const ClientInterface& ci = m_parrClientInterfaces->GetAt(i);
		if (IsEqualIID(ci.iid, riid))
		{
			ci.pUnk->Release();
			m_parrClientInterfaces->RemoveAt(i);
			return;
		}
	}
}

void CXTPWebBrowserProvider::OnCreateBrowser(IUnknown* pObject)
{
	ASSERT(NULL != pObject);

	ReleaseSite();

	m_pBrowserObject = pObject;

	m_pSite = CreateControlSite(m_pCtrlContainer);
	ASSERT(NULL != m_pSite);

	CComPtr<IOleObject> pOleObject;
	if (SUCCEEDED(m_pBrowserObject->QueryInterface(&pOleObject)))
	{
		CComQIPtr<IOleClientSite> pClientSite = m_pSite->GetControllingUnknown();
		ASSERT(pClientSite);
		if (FAILED(pOleObject->SetClientSite(pClientSite)))
		{
			TRACE(_T("Unable to set WebBrowser object site using IOleObject\n"));
			ReleaseSite();
		}
	}
	else
	{
		CComPtr<IObjectWithSite> pObjWithSite;
		if (SUCCEEDED(m_pBrowserObject->QueryInterface(&pObjWithSite)))
		{
			if (FAILED(pObjWithSite->SetSite(m_pSite->GetControllingUnknown())))
			{
				TRACE(_T("Unable to set WebBrowser object site using IObjectWithSite\n"));
				ReleaseSite();
			}
		}
		else
		{
			TRACE(_T("Unable to obtain any site interface for the WebBrowser object\n"));
			ReleaseSite();
		}
	}
}

void CXTPWebBrowserProvider::OnDestroyBrowser()
{
	ReleaseSite();
	m_pBrowserObject = NULL;
}

CXTPWebBrowserControlSite*
	CXTPWebBrowserProvider::CreateControlSite(COleControlContainer* pCtrlContainer)
{
	return new CXTPWebBrowserControlSite(pCtrlContainer, this);
}

void CXTPWebBrowserProvider::UpdateExternal()
{
	// do nothing
}

void CXTPWebBrowserProvider::ReleaseSite()
{
	if (NULL != m_pSite)
	{
		m_pSite->ExternalRelease();
		m_pSite = NULL;
	}
}

#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
