#pragma once
#include <string>
using namespace std;
#include "Export.h"

//�� LongPeriodicAnalyze �������FutureContractName ��,��������
//��Ҫ���� FutureContractName �ĵط�
//1.MinorCycleAnalyze
//2.Future_DailyTraceEvidence
#define Place2UpdateFutureContractName 2

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
	int     m_nPlace2UpdateFutureContractName;  //��Ҫ����FutureContractName �ĵط�

};