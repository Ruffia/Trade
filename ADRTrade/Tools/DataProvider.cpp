#include "stdafx.h"
#include "DataProvider.h"
#include "DialogPlaceHolderDB.h"

IDataProvider::IDataProvider():m_sBusiness(""),m_pHolder(NULL)
{
	m_ds.Clear();
}

void IDataProvider::Attach(CDialogPlaceHolderBusiness* pHolder)
{
	m_pHolder = pHolder;
}