#include "stdafx.h"
#include "Field.h"

IMPLEMENT_FACTORY(CField,CField,string,"CField")
CField::CField()
{
	m_pFieldDesc = NULL;
	m_pValue = new FieldValue;
}


CField::~CField()
{
	if(m_pValue)
	{
		delete m_pValue;
		m_pValue = NULL;
	}

	m_pFieldDesc = NULL;
}


void CField::Create()
{

}