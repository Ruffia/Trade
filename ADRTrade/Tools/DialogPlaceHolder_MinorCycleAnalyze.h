#pragma once
#include "DialogPlaceHolder.h"

class CDialogTabItem_MinorCycleAnalyze : public CDialogPlaceHolder
{
	DECLARE_DYNAMIC(CDialogTabItem_MinorCycleAnalyze)
public:
	CDialogTabItem_MinorCycleAnalyze(CWnd* pParent = NULL);

	void SetItem(const string& sItemName)
	{
		m_strItemName = sItemName;
	}

	string GetItemName()
	{
		return m_strItemName;
	}

protected:
	string m_strItemName_Old;  //保存修改页签名称之前的名称
	string m_strItemName;
};