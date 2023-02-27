#pragma once
#include <string>
using namespace std;
#include "Export.h"

//�� LongPeriodicAnalyze �������FutureContractName ��,��������
//��Ҫ���� FutureContractName �ĵط�
//1.MinorCycleAnalyze
//2.Future_DailyTraceEvidence
//3.Future_DailyTraceConflict
#define Place2UpdateFutureContractName 3

enum  ESynchronizeBusiness
{
	UI_MinorCycleAnalyze,
	UI_DailyTraceEvidence,
	UI_DailyTraceConflict,
};

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
	//�� m_bNeed2UpdateFutureContractName ֵΪtrueʱ�� 
	// m_Synchronize[X] = true ��ʾ��ͬ��, m_Synchronize[X] = false ��ʾδͬ��
	bool    m_Synchronize[Place2UpdateFutureContractName];  

};