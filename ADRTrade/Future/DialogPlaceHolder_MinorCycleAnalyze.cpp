#include "stdafx.h"
#include "DialogPlaceHolder_MinorCycleAnalyze.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CDialogTabItem_MinorCycleAnalyze, CDialogPlaceHolder)
CDialogTabItem_MinorCycleAnalyze::CDialogTabItem_MinorCycleAnalyze(CWnd* pParent /*=NULL*/)
	: CDialogPlaceHolder(pParent)
{
	m_strItemName = "";
}

