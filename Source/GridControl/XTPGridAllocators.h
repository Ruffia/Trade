// XTPGridAllocators.h
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
#if !defined(__XTPGRIDALLOCATORS_H__)
#	define __XTPGRIDALLOCATORS_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//{{AFX_CODEJOCK_PRIVATE

XTP_DECLARE_HEAP_ALLOCATOR_(CXTPGridDataAllocator, _XTP_EXT_CLASS)
XTP_DECLARE_HEAP_ALLOCATOR_(CXTPGridRowAllocator, _XTP_EXT_CLASS)

// to allocate in the app default heap
XTP_DECLARE_HEAP_ALLOCATOR_(CXTPGridAllocatorDefault, _XTP_EXT_CLASS)

XTP_DECLARE_BATCH_ALLOC_OBJ_DATA_(CXTPGridRow_BatchData, _XTP_EXT_CLASS);
XTP_DECLARE_BATCH_ALLOC_OBJ_DATA_(CXTPGridGroupRow_BatchData, _XTP_EXT_CLASS);

//}}AFX_CODEJOCK_PRIVATE

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPGRIDALLOCATORS_H__)