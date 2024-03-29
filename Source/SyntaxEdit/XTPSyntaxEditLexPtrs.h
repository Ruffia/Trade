// XTPSyntaxEditLexPtrs.h
//
// (c)1998-2023 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME SYNTAX EDIT LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPSYNTAXEDITLEXPTRS_H__)
#	define __XTPSYNTAXEDITLEXPTRS_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPSyntaxEditTextIterator;

////////////////////////////////////////////////////////////////////////////
namespace XTPSyntaxEditLexAnalyser
{
//===========================================================================
typedef CXTPSyntaxEditTextIterator CTextIter;

//===========================================================================
class CXTPSyntaxEditLexVariant;
class CXTPSyntaxEditLexTextSchema;
class CXTPSyntaxEditLexParser;
class CXTPSyntaxEditLexParserSchemaOptions;
class CXTPSyntaxEditLexClass;
class CXTPSyntaxEditLexClass_file;
class CXTPSyntaxEditLexTextBlock;

typedef CXTPSmartPtrInternalT<CXTPSyntaxEditLexVariant> CXTPSyntaxEditLexVariantPtr;
typedef CXTPSmartPtrInternalT<CXTPSyntaxEditLexClass> CXTPSyntaxEditLexClassPtr;
typedef CXTPSmartPtrInternalT<CXTPSyntaxEditLexTextBlock> CXTPSyntaxEditLexTextBlockPtr;
typedef CXTPSmartPtrInternalT<CXTPSyntaxEditLexParser> CXTPSyntaxEditLexParserPtr;

typedef CXTPInternalCollectionT<CXTPSyntaxEditLexTextBlock> CXTPSyntaxEditLexTextBlockPtrArray;
typedef CXTPInternalCollectionT<CXTPSyntaxEditLexVariant> CXTPSyntaxEditLexVariantPtrArray;
typedef CXTPInternalCollectionT<CXTPSyntaxEditLexClass> CXTPSyntaxEditLexClassPtrArray;

typedef CXTPSmartPtrInternalT<CXTPSyntaxEditLexVariantPtrArray> CXTPSyntaxEditLexVariantPtrArrayPtr;

} // END namespace XTPSyntaxEditLexAnalyser

//////////////////////////////////////////////////////////////////////////////

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSYNTAXEDITLEXPTRS_H__)
