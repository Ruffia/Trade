#pragma  once
#include "../Tools/DataProvider.h"
#include "Factory.h"


class DataProvider_DailyMacroEnvironmentAnalyze : public IDataProvider
{
public:
	//�����ݿ��м�������
	virtual void LoadData(CDataSet& ds);

	//��UI�ϵ����ݱ��浽���ݿ���
	virtual void UpdateUI2DB(CDataSet& ds);

	//���ݴ����ݿ��ѯ�õ������ݼ�¼ֵ����UI�ؼ�����ʾ
	virtual void UpdateDB2UI(CDataSet& ds);
};