#include "stdafx.h"
#include "BusinessCheckBox.h"

IMPLEMENT_DYNAMIC(CBusinessCheckBox, CComboBox)

CBusinessCheckBox::CBusinessCheckBox()
{
	m_sBusinessField = "";
}

CBusinessCheckBox::~CBusinessCheckBox()
{
}


BEGIN_MESSAGE_MAP(CBusinessCheckBox, CButton)
END_MESSAGE_MAP()




