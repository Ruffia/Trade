#pragma once
#include <string>
using namespace std;
#include "Export.h"

//������������
class EXPORT_CALSS CTradeDayPrimaryData
{
protected:
	CTradeDayPrimaryData();

public:
	static CTradeDayPrimaryData& Instance();


public:
	string  m_strTradeDay;
	//����
	string  m_strFutureContractName_LastTime;  //�ϴθ���ʱ��¼��FutureContractName
	string  m_strFutureContractName;           //�ϴθ���ʱ��¼��FutureContractName
	bool    m_bNeed2UpdateFutureContractName;

};