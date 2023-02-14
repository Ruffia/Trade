#pragma once
#include <string>
#include <map>
using namespace std;

class CBusinessCheckBox : public CButton
{
	DECLARE_DYNAMIC(CBusinessCheckBox)

public:
	CBusinessCheckBox();
	virtual ~CBusinessCheckBox();

public:
	string m_sBusinessField;

protected:
	DECLARE_MESSAGE_MAP()
};


