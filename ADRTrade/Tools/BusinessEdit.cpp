#include "stdafx.h"
#include "BusinessEdit.h"

IMPLEMENT_DYNAMIC(CBusinessEdit, CEdit)

CBusinessEdit::CBusinessEdit()
{
	m_sBusinessField = "";
}

CBusinessEdit::~CBusinessEdit()
{
}


BEGIN_MESSAGE_MAP(CBusinessEdit, CEdit)
END_MESSAGE_MAP()




