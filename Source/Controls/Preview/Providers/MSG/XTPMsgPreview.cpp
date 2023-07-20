// XTPMsgPreview.cpp : implementation of the CXTPMsgPreview class.
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
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceManager.h"

#include "Controls/Preview/XTPPreviewAbstract.h"
#include "Controls/Preview/Providers/MSG/XTPMsgPreviewDialog.h"
#include "Controls/Preview/Providers/MSG/XTPMsgPreview.h"
#include "Controls/Preview/Providers/MSG/XTPMsgPreviewProvider.h"

#include "Controls/Resource.h"

#include "Common/XTPSystemHelpers.h"

#include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"
#include <comdef.h>
#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#define XTP_SUBJECT_TAG _T("0037")
#define XTP_SUBJECT_PREFIX_TAG _T("003D")
#define XTP_RECEIVER_DISPLAY_NAME_TAG _T("0040")
#define XTP_SENDER_DISPLAY_NAME_TAG _T("0042")
#define XTP_RECEIVER_NAME_TAG _T("0044")
#define XTP_AUTHOR_DISPLAY_NAME_TAG _T("004D")
#define XTP_SENDER_R_MAIL_TAG _T("0065")
#define XTP_AUTHOR_MAIL_TAG _T("0067")
#define XTP_TOPIC_TAG _T("0070")
#define XTP_RECEIVER_MAIL_TAG _T("0076")
#define XTP_HEADER_TAG _T("007D")
#define XTP_SENDER_NAME_TAG _T("0C1A")
#define XTP_SENDER_MAIL_TAG _T("0C1F")
#define XTP_BCC_TAG _T("0E02")
#define XTP_CC_TAG _T("0E03")
#define XTP_TO_TAG _T("0E04")
#define XTP_BODY_TAG _T("1000")

#define XTP_UNICODE_TAG _T("001F")
#define XTP_STRING8_TAG _T("001E")

#define XTP_DATE_TAG _T("Date:")
#define XTP_CR_TAG _T("\r\n")

#define XTP_FILE_NAME_TAG _T("3707")
#define XTP_ATTACH_TAG _T("__attach_")

CXTPMsgPreview::CXTPMsgPreview()
	: m_bCreated(FALSE)
	, m_bLoadFinished(FALSE)
	, m_bLoadSuccess(FALSE)
{
	m_MsgDialog = new CXTPMsgPreviewDialog();
}

CXTPMsgPreview::~CXTPMsgPreview()
{
	if (m_bCreated)
	{
		m_MsgDialog->DestroyWindow();
		m_bCreated = FALSE;
	}

	delete m_MsgDialog;
}

BOOL CXTPMsgPreview::IsReady() const
{
	return m_bLoadFinished;
}

BOOL CXTPMsgPreview::IsAvailable() const
{
	return m_bLoadSuccess;
}

BOOL CXTPMsgPreview::HasOwnNavigator() const
{
	return 0;
}

UINT CXTPMsgPreview::GetNumberOfPages() const
{
	return 0;
}

SIZE CXTPMsgPreview::GetPreferredPageSize() const
{
	return CSize(100, 100);
}

void CXTPMsgPreview::GoToPage(UINT page)
{
	UNREFERENCED_PARAMETER(page);
}

void CXTPMsgPreview::Draw(HDC dc, SIZE clientSize, RECT updateRect)
{
	UNREFERENCED_PARAMETER(dc);
	UNREFERENCED_PARAMETER(clientSize);
	UNREFERENCED_PARAMETER(updateRect);
}

BOOL CXTPMsgPreview::OnHostWndMsg(LPMSG msg, LRESULT* result)
{
	UNREFERENCED_PARAMETER(result);

	if ((msg->message == WM_WINDOWPOSCHANGING) || (msg->message == WM_WINDOWPOSCHANGED))
	{
		WINDOWPOS* pWinPos = (WINDOWPOS*)msg->lParam;
		m_MsgDialog->SetWindowPos(NULL, 0, 0, pWinPos->cx, pWinPos->cy,
								  SWP_NOACTIVATE | SWP_NOZORDER);
		m_MsgDialog->UpdateWindow();
	}

	return FALSE;
}

BOOL CXTPMsgPreview::SupportsCompositedParent() const
{
	return TRUE;
}

void CXTPMsgPreview::Activate()
{
	if (NULL != m_MsgDialog && ::IsWindow(*m_MsgDialog))
		m_MsgDialog->SetFocus();
}

BOOL CXTPMsgPreview::MustRead(CString& tag)
{
	return ((tag.CompareNoCase(XTP_SUBJECT_PREFIX_TAG) == 0)
			|| (tag.CompareNoCase(XTP_SUBJECT_TAG) == 0)
			|| (tag.CompareNoCase(XTP_RECEIVER_DISPLAY_NAME_TAG) == 0)
			|| (tag.CompareNoCase(XTP_SENDER_DISPLAY_NAME_TAG) == 0)
			|| (tag.CompareNoCase(XTP_RECEIVER_NAME_TAG) == 0)
			|| (tag.CompareNoCase(XTP_SENDER_R_MAIL_TAG) == 0)
			|| (tag.CompareNoCase(XTP_TOPIC_TAG) == 0)
			|| (tag.CompareNoCase(XTP_RECEIVER_MAIL_TAG) == 0)
			|| (tag.CompareNoCase(XTP_HEADER_TAG) == 0)
			|| (tag.CompareNoCase(XTP_SENDER_NAME_TAG) == 0)
			|| (tag.CompareNoCase(XTP_SENDER_MAIL_TAG) == 0)
			|| (tag.CompareNoCase(XTP_BCC_TAG) == 0) || (tag.CompareNoCase(XTP_CC_TAG) == 0)
			|| (tag.CompareNoCase(XTP_TO_TAG) == 0) || (tag.CompareNoCase(XTP_BODY_TAG) == 0));
}

BOOL CXTPMsgPreview::ReadStream(CStatStg& stg, CString& val, CString type, IStorage* pStorage)
{
	IStreamPtr pPropertyStream;

	HRESULT hr = pStorage->OpenStream(stg.m_statStg.pwcsName, 0, STGM_READ | STGM_SHARE_EXCLUSIVE,
									  0, &pPropertyStream);
	if (FAILED(hr))
		return FALSE;

	CArray<char, char> buffer;
	buffer.SetSize(XTPToIntPtr(stg.m_statStg.cbSize.LowPart + 2));

	DWORD rcb;
	hr = pPropertyStream->Read(&buffer[0], stg.m_statStg.cbSize.LowPart, &rcb);
	if (FAILED(hr))
		return FALSE;

	buffer[XTPToIntPtr(rcb)]	 = 0;
	buffer[XTPToIntPtr(rcb) + 1] = 0;

	if (type.CompareNoCase(XTP_STRING8_TAG) == 0)
	{
		val = XTP_CA2CT(&buffer[0]);
		return TRUE;
	}

	if (type.CompareNoCase(XTP_UNICODE_TAG) == 0)
	{
		val = XTP_CW2CT(&buffer[0]);
		return TRUE;
	}

	return FALSE;
}

void CXTPMsgPreview::PlaceValue(TextFields& fields, CString val, CString tag)
{
	if (tag.CompareNoCase(XTP_SUBJECT_TAG) == 0)
	{
		fields.m_Subject = val;
		return;
	}

	if (tag.CompareNoCase(XTP_SUBJECT_PREFIX_TAG) == 0)
	{
		if (!fields.m_Subject.IsEmpty())
		{
			fields.m_Subject += _T(", ");
		}

		fields.m_Subject += val;
		return;
	}

	if (tag.CompareNoCase(XTP_RECEIVER_DISPLAY_NAME_TAG) == 0)
	{
		fields.m_ReceiverName = val;
		return;
	}

	if (tag.CompareNoCase(XTP_SENDER_DISPLAY_NAME_TAG) == 0)
	{
		fields.m_SenderName = val;
		return;
	}

	if (tag.CompareNoCase(XTP_RECEIVER_NAME_TAG) == 0)
	{
		if (fields.m_ReceiverName.IsEmpty())
			fields.m_ReceiverName = val;

		return;
	}

	if (tag.CompareNoCase(XTP_SENDER_R_MAIL_TAG) == 0)
	{
		fields.m_SenderMail = val;
		return;
	}

	if (tag.CompareNoCase(XTP_TOPIC_TAG) == 0)
	{
		fields.m_Subject += _T(" (");
		fields.m_Subject += val;
		fields.m_Subject += _T(" )");
		return;
	}

	if (tag.CompareNoCase(XTP_RECEIVER_MAIL_TAG) == 0)
	{
		fields.m_ReceiverMail = val;
		return;
	}

	if (tag.CompareNoCase(XTP_HEADER_TAG) == 0)
	{
		int startNext = 5, found = val.Find(XTP_DATE_TAG);
		if (0 <= found)
		{
			startNext += found;
			found = val.Find(XTP_CR_TAG, startNext);
			if (0 <= found)
			{
				while (startNext < found && isspace(val.GetAt(startNext)))
				{
					++startNext;
				}

				while (startNext < found && isspace(val.GetAt(found - 1)))
				{
					--found;
				}

				int count	 = found - startNext;
				fields.m_Date = val.Mid(startNext, count);
			}
		}

		return;
	}

	if (tag.CompareNoCase(XTP_SENDER_NAME_TAG) == 0)
	{
		if (fields.m_SenderName.IsEmpty())
		{
			fields.m_SenderName = val;
		}

		return;
	}

	if (tag.CompareNoCase(XTP_SENDER_MAIL_TAG) == 0)
	{
		fields.m_SenderMail = val;
		return;
	}

	if (tag.CompareNoCase(XTP_BCC_TAG) == 0)
	{
		fields.m_BCC = val;
		return;
	}

	if (tag.CompareNoCase(XTP_CC_TAG) == 0)
	{
		fields.m_CC = val;
		return;
	}

	if (tag.CompareNoCase(XTP_TO_TAG) == 0)
	{
		fields.m_To = val;
		return;
	}

	if (tag.CompareNoCase(XTP_BODY_TAG) == 0)
	{
		fields.m_Body = val;
		return;
	}
}

BOOL CXTPMsgPreview::ReadAttachmentStorage(TextFields& fields, LPOLESTR storageName,
										   IStorage* pStorage)
{
	IStoragePtr pAttachmentStorage;
	HRESULT hr = pStorage->OpenStorage(storageName, 0, STGM_READ | STGM_SHARE_EXCLUSIVE, 0, 0,
									   &pAttachmentStorage);
	if (FAILED(hr))
		return FALSE;

	IEnumSTATSTGPtr pAttachmentStgEnum;
	hr = pAttachmentStorage->EnumElements(0, NULL, 0, &pAttachmentStgEnum);
	if (FAILED(hr))
		return FALSE;

	CStatStg attachStatstg;
	hr = pAttachmentStgEnum->Next(1, &attachStatstg.m_statStg, NULL);
	if (FAILED(hr))
		return FALSE;

	while (hr != S_FALSE)
	{
		CString attachTagName(attachStatstg.m_statStg.pwcsName);
		attachTagName		 = attachTagName.Right(8);
		CString propertyType = attachTagName.Right(4);
		attachTagName		 = attachTagName.Left(4);
		if ((attachStatstg.m_statStg.type == STGTY_STREAM)
			&& (attachTagName.CompareNoCase(XTP_FILE_NAME_TAG) == 0))
		{
			CString fileName;
			if (!ReadStream(attachStatstg, fileName, propertyType, pAttachmentStorage))
				return FALSE;

			if (!fields.m_Attachments.IsEmpty())
				fields.m_Attachments += _T("; ");

			fields.m_Attachments += fileName;
		}

		hr = pAttachmentStgEnum->Next(1, &attachStatstg.m_statStg, NULL);
		if (FAILED(hr))
			return FALSE;
	}

	return TRUE;
}

void CXTPMsgPreview::FillFields(TextFields fields)
{
	if (fields.m_To.IsEmpty())
	{
		if (!fields.m_ReceiverName.IsEmpty())
			fields.m_To = fields.m_ReceiverName;

		if (!fields.m_ReceiverMail.IsEmpty())
		{
			if (!fields.m_To.IsEmpty())
				fields.m_To += _T(" ");

			fields.m_To += fields.m_ReceiverMail;
		}
	}

	CString sFrom;
	if (!fields.m_SenderName.IsEmpty())
		sFrom = fields.m_SenderName;

	if (!fields.m_SenderMail.IsEmpty())
	{
		if (!fields.m_SenderName.IsEmpty())
		{
			sFrom += _T(" (");
			fields.m_SenderMail += _T(")");
		}

		sFrom += fields.m_SenderMail;
	}

	m_MsgDialog->SetSubject(fields.m_Subject);
	m_MsgDialog->SetTo(fields.m_To);
	m_MsgDialog->SetFrom(sFrom);
	m_MsgDialog->SetAttachment(fields.m_Attachments);
	m_MsgDialog->SetDate(fields.m_Date);
	m_MsgDialog->SetCC(fields.m_CC);
	m_MsgDialog->SetBCC(fields.m_BCC);
	m_MsgDialog->SetBody(fields.m_Body);
}

BOOL CXTPMsgPreview::CreateMsgDialog(HWND parent)
{
	WNDCLASS wc;
	VERIFY(XTPLoadWindowClass(xtpWindowClassRichEdit, xtpWindowClassVersionExact,
							  xtpRichEditWindowClassVersion20, &wc));

	CWnd* pHost = CWnd::FromHandle(parent);
	ASSERT_VALID(pHost);

	m_bCreated = m_MsgDialog->Create(pHost);
	VERIFY(m_bCreated);
	if (!m_bCreated)
	{
		TRACE(_T("Unable to create MSG preview dialog\n"));
		return FALSE;
	}

	CXTPClientRect rcHost(pHost);
	int w = rcHost.Width();
	int h = rcHost.Height();
	m_MsgDialog->SetWindowPos(NULL, rcHost.left, rcHost.top, w, h,
							  SWP_NOACTIVATE | SWP_SHOWWINDOW | SWP_NOZORDER);
	m_MsgDialog->UpdateWindow();

	CRichEditCtrl richEdit;
	richEdit.SubclassDlgItem(XTP_IDC_MSG_BODY_RICHEDIT, m_MsgDialog);
	richEdit.SetTargetDevice(NULL, 0);
	richEdit.UnsubclassWindow();

	return m_bCreated;
}

BOOL CXTPMsgPreview::LoadMsg(LPCTSTR pFilePath)
{
	return LoadMsg(NULL, pFilePath);
}

BOOL CXTPMsgPreview::LoadMsg(IStream* pStream, LPCTSTR pImpliedFileName)
{
	m_bLoadFinished = FALSE;
	m_bLoadSuccess  = FALSE;

	HRESULT hr;
	IStoragePtr ipStorage;
	if (NULL != pStream)
	{
		ILockBytesPtr pLockBytes;
		hr = pStream->QueryInterface(IID_ILockBytes, reinterpret_cast<LPVOID*>(&pLockBytes));
		if (FAILED(hr))
			return FALSE;

		hr = StgOpenStorageOnILockBytes(pLockBytes, NULL,
										STGM_DIRECT | STGM_READ | STGM_SHARE_EXCLUSIVE, NULL, 0,
										&ipStorage);
		if (FAILED(hr))
			return FALSE;
	}
	else
	{
		_bstr_t path(pImpliedFileName);
		hr = StgIsStorageFile(path);
		if (FAILED(hr))
			return FALSE;

		hr = StgOpenStorage(path, NULL, STGM_DIRECT | STGM_READ | STGM_SHARE_EXCLUSIVE, NULL, 0,
							&ipStorage);
		if (FAILED(hr))
			return FALSE;
	}

	IEnumSTATSTGPtr pStgEnum;
	hr = ipStorage->EnumElements(0, NULL, 0, &pStgEnum);
	if (FAILED(hr))
		return FALSE;

	CStatStg currentStatStg;
	hr = pStgEnum->Next(1, &currentStatStg.m_statStg, NULL);
	if (FAILED(hr))
		return FALSE;

	TextFields textFields;
	CString sFrom, sTo, sSubject, sReceiverName, sReceiverMail, sSenderName, sSenderMail, sAttach;
	while (hr != S_FALSE)
	{
		CString storageName(currentStatStg.m_statStg.pwcsName);
		storageName.MakeLower();
		CString propertyTag  = storageName.Right(8);
		CString propertyType = propertyTag.Right(4);
		propertyTag			 = propertyTag.Left(4);
		if ((currentStatStg.m_statStg.type == STGTY_STREAM) && MustRead(propertyTag))
		{
			CString propertyValue;
			if (!ReadStream(currentStatStg, propertyValue, propertyType, ipStorage))
				return FALSE;

			PlaceValue(textFields, propertyValue, propertyTag);
		}
		else if ((currentStatStg.m_statStg.type == STGTY_STORAGE)
				 && (storageName.Find(XTP_ATTACH_TAG) >= 0))
		{
			if (!ReadAttachmentStorage(textFields, currentStatStg.m_statStg.pwcsName, ipStorage))
				return FALSE;
		}

		hr = pStgEnum->Next(1, &currentStatStg.m_statStg, NULL);
		if (FAILED(hr))
			return FALSE;
	}

	m_bLoadFinished = TRUE;
	FillFields(textFields);
	m_bLoadSuccess = TRUE;

	return TRUE;
}

#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
