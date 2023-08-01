#pragma once
#include <string>
using namespace std;
#include "Field.h"

class CGridViewField:public CField
{
public:
	CGridViewField();

	~CGridViewField();

     //Create Field object based on m_pFieldDesc
     virtual void Create();

private:
	string m_strDisplayValue;
	string m_strControlType;
	int m_nTextColor;
};