#pragma  once
#include <string>
using namespace std;
#include "DataSet.h"
class CDialogPlaceHolderBusiness;

class IDataProvider
{
public:
	IDataProvider();

	void Attach(CDialogPlaceHolderBusiness* pHolder);

	//�����ݿ��м�������
	virtual void LoadData(CDataSet& ds) = 0;

	//��UI�ϵ����ݱ��浽���ݿ���
	virtual void UpdateUI2DB(CDataSet& ds) = 0;

	//���ݴ����ݿ��ѯ�õ������ݼ�¼ֵ����UI�ؼ�����ʾ
	virtual void UpdateDB2UI(CDataSet& ds) = 0;

public:
	string m_sBusiness;                          //ҵ����
	CDataSet m_ds;                               //���ݼ�
	CDialogPlaceHolderBusiness*     m_pHolder;   //������UI
};