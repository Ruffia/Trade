// XTPPreviewAbstract.h : interfaces for the Preview control.
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
#if !defined(__XTPPREVIEWABSTRACT_H__)
#	define __XTPPREVIEWABSTRACT_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     The base interface for a preview handler implementation.
// See Also:
//     IXTPPreviewHost, IXTPPreviewProvider
//===========================================================================
struct IXTPPreview
{
	//-----------------------------------------------------------------------
	// Summary:
	//     Handles object destruction.
	//-----------------------------------------------------------------------
	virtual ~IXTPPreview()
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns TRUE if the handler has attempted and finished loading the
	//     preview regardless of the actual loading result.
	// Returns:
	//     TRUE if the handler is ready.
	//-----------------------------------------------------------------------
	virtual BOOL IsReady() const = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the preview is available (i.e. the handler was able
	//     to load preview data successfully).
	// Returns:
	//     TRUE if the preview is available.
	//-----------------------------------------------------------------------
	virtual BOOL IsAvailable() const = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the handler implements its own page navigator.
	// Remarks:
	//     In the case that the handler implements its own page navigator,
	//     it does not require the common page navigator to be used for it.
	// Returns:
	//     TRUE if the handler implements its own page navigator.
	//-----------------------------------------------------------------------
	virtual BOOL HasOwnNavigator() const = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the number of pages available for the common page navigator.
	// Returns:
	//     The number of pages available for the common page navigator,
	//     or 0 if no pages are supported.
	//-----------------------------------------------------------------------
	virtual UINT GetNumberOfPages() const = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Obtains the preferred page size in pixels if that is required by the
	//     preview. If zero size is returned, then it is assumed that the
	//     preview can be scaled to the entire available preview area.
	// Returns:
	//     Preferred page size, or 0,0.
	//-----------------------------------------------------------------------
	virtual SIZE GetPreferredPageSize() const = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     If pages are supported by the preview, then it should cause navigation
	//     to the specified page number. Otherwise, the call should be ignored.
	// Parameters:
	//     page - Zero-based page number to show preview for.
	//-----------------------------------------------------------------------
	virtual void GoToPage(UINT page) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called every time the preview has to draw itself into
	//     the available client area. If the preview drawing is not provided by
	//     the preview handler, then the call should be ignored.
	// Parameters:
	//     dc - Target device context.
	//     clientSize - The size of the available client rectangle starting at 0,0.
	//     updateRect - Rectangular area requested for an update.
	//-----------------------------------------------------------------------
	virtual void Draw(HDC dc, SIZE clientSize, RECT updateRect) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Handles a window message sent to the host preview control. It can
	//     be used by windowless preview handlers for handling events.
	// Parameters:
	//     msg - Message structure pointer.
	//     result - Pointer to the message result.
	// Returns:
	//     TRUE if the message is handled by the preview handler and must not
	//     be forwarded to the host control, otherwise FALSE is expected.
	//-----------------------------------------------------------------------
	virtual BOOL OnHostWndMsg(LPMSG msg, LRESULT* result) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the preview implementation is compatible with
	//     composited top level windows (i.e. if any of its parents has
	//     the WS_EX_COMPOSITED style specified).
	// Returns:
	//     TRUE if the preview implementation is WS_EX_COMPOSITED compatible.
	//-----------------------------------------------------------------------
	virtual BOOL SupportsCompositedParent() const = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Activate preview.
	//-----------------------------------------------------------------------
	virtual void Activate() = 0;
};

//===========================================================================
// Summary:
//     Interface implemented by the preview host control.
// See Also:
//     IXTPPreview, IXTPPreviewProvider
//===========================================================================
struct IXTPPreviewHost
{
	//-----------------------------------------------------------------------
	// Summary:
	//     Handles object destruction.
	//-----------------------------------------------------------------------
	virtual ~IXTPPreviewHost()
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Obtains the background color used by the host control.
	// Returns:
	//     The background color used by the host control.
	//-----------------------------------------------------------------------
	virtual COLORREF GetBackgroundColor() const = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Obtains the foreground color used by the host control.
	// Returns:
	//     The foreground color used by the host control.
	//-----------------------------------------------------------------------
	virtual COLORREF GetForegroundColor() const = 0;
};

//===========================================================================
// Summary:
//     Interface to be implemented by a preview provider.
// See Also:
//     IXTPPreviewHost, IXTPPreview, CXTPPreviewProviderBase
//===========================================================================
struct IXTPPreviewProvider
{
	//-----------------------------------------------------------------------
	// Summary:
	//     Handles object destruction.
	//-----------------------------------------------------------------------
	virtual ~IXTPPreviewProvider()
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called by the host control when it first binds the provider.
	// Parameters:
	//     host - Preview host control interface reference.
	//-----------------------------------------------------------------------
	virtual void BindHost(IXTPPreviewHost& host) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Obtains the list of file extensions supported by the associated preview
	//     implementation. The list is a semicolon separated string of extensions
	//     without periods (e.g. "doc;docx;xls;xlsx"). The return value can also be
	//     NULL which means that the preview implementation is capable of extracting
	//     previews from all supplied file types.
	// Returns:
	//     The list of supported extensions, or NULL to indicate ability to preview
	//     all file types.
	//-----------------------------------------------------------------------
	virtual LPCTSTR GetDefaultFileAssociations() const = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a preview object for the specified file.
	// Parameters:
	//     pFilePath - File path value for which the preview is to be created.
	//     hwndParent - Preview parent window handle.
	//     stream - Data stream that contains file data to be previewed.
	//     pImpliedFileName - An implied name of the file associated with the stream object, only
	//     extension part is important for identiying the format of the data to be previewed.
	// Returns:
	//     A valid pointer to the new preview object created, or NULL if it was not
	//     possible to create a preview object.
	//-----------------------------------------------------------------------
	virtual IXTPPreview* CreatePreview(LPCTSTR pFilePath, HWND hwndParent) = 0;
	virtual IXTPPreview* CreatePreview(
		IStream& stream, LPCTSTR pImpliedFileName,
		HWND hwndParent) = 0; // <combine IXTPPreviewProvider::CreatePreview@LPCTSTR@HWND>
};

class CXTPPreviewCtrl;

//=======================================================================
// Summary:
//     Base class for a preview provider which implements common behavior
//     to be exposed by a preview provider.
// See Also:
//     IXTPPreviewProvider
//=======================================================================
class _XTP_EXT_CLASS CXTPPreviewProviderBase : public IXTPPreviewProvider
{
	friend class CXTPPreviewCtrl;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Handles object construction.
	//-----------------------------------------------------------------------
	CXTPPreviewProviderBase();

	//-----------------------------------------------------------------------
	// Summary:
	//     The method is called by the host preview control when the provider
	//     is initially bound to the host.
	// Parameters:
	//     host - Host interface.
	//-----------------------------------------------------------------------
	void BindHost(IXTPPreviewHost& host);

	//-----------------------------------------------------------------------
	// Summary:
	//     Obtains the preview host interface that the provider is bound to.
	// Returns:
	//     Preview host interface.
	//-----------------------------------------------------------------------
	IXTPPreviewHost& GetHost() const;

private:
	mutable IXTPPreviewHost* m_pHost;
};

AFX_INLINE CXTPPreviewProviderBase::CXTPPreviewProviderBase()
	: m_pHost(NULL)
{
}

AFX_INLINE void CXTPPreviewProviderBase::BindHost(IXTPPreviewHost& host)
{
	m_pHost = &host;
}

AFX_INLINE IXTPPreviewHost& CXTPPreviewProviderBase::GetHost() const
{
	ASSERT(NULL != m_pHost);
	return *m_pHost;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPPREVIEWABSTRACT_H__)
