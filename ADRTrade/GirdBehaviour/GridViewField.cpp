#include "stdafx.h"
#include "Factory.h"
#include "GridViewField.h"
#include "GridFieldMeta.h"

IMPLEMENT_FACTORY(CField,CGridViewField,string,"CGridViewField")
CGridViewField::CGridViewField()
{
	m_strDisplayValue = "";
	m_strControlType = "";
	m_nTextColor = 0;
}


CGridViewField::~CGridViewField()
{
}


void CGridViewField::Create()
{

}