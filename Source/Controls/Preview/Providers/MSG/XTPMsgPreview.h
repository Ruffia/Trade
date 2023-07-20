// XTPMsgPreview.h : interface for the CXTPMsgPreview class.
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

#if !defined(__XTPMSGPREVIEW_H__)
#	define __XTPMSGPREVIEW_H__

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPMsgPreviewDialog;
class CXTPMsgPreviewProvider;

class _XTP_EXT_CLASS CXTPMsgPreview : public IXTPPreview
{
public:
	CXTPMsgPreview();
	~CXTPMsgPreview();

private:
	virtual BOOL IsReady() const;
	virtual BOOL IsAvailable() const;
	virtual BOOL HasOwnNavigator() const;
	virtual UINT GetNumberOfPages() const;
	virtual SIZE GetPreferredPageSize() const;
	virtual void GoToPage(UINT page);
	virtual void Draw(HDC dc, SIZE clientSize, RECT updateRect);
	virtual BOOL OnHostWndMsg(LPMSG msg, LRESULT* result);
	virtual BOOL SupportsCompositedParent() const;
	virtual void Activate();

private:
	BOOL m_bCreated;
	BOOL m_bLoadFinished;
	BOOL m_bLoadSuccess;
	CXTPMsgPreviewDialog* m_MsgDialog;

private:
	typedef struct _TEXTFIEDS
	{
		CString m_To;
		CString m_Date;
		CString m_Subject;
		CString m_CC;
		CString m_BCC;
		CString m_Attachments;
		CString m_Body;

		CString m_ReceiverName;
		CString m_SenderName;
		CString m_SenderMail;
		CString m_ReceiverMail;
	} TextFields;

	class CStatStg
	{
	public:
		STATSTG m_statStg;
		~CStatStg()
		{
			CoTaskMemFree(m_statStg.pwcsName);
		}
	};

	friend class CXTPMsgPreviewProvider;
	BOOL CreateMsgDialog(HWND parent);
	BOOL LoadMsg(LPCTSTR pFilePath);
	BOOL LoadMsg(IStream* pStream, LPCTSTR pImpliedFileName);
	static BOOL MustRead(CString& tag);
	static BOOL ReadStream(CStatStg& stg, CString& val, CString type, IStorage* pStorage);
	static void PlaceValue(TextFields& fields, CString val, CString tag);
	static BOOL ReadAttachmentStorage(TextFields& fields, LPOLESTR storageName, IStorage* pStorage);
	void FillFields(TextFields fields);
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPMSGPREVIEW_H__)

//}}AFX_CODEJOCK_PRIVATE
