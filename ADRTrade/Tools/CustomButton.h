#pragma once


// CCustomButton
// ���ƻ���ť�ؼ�
class CCustomButton : public CButton
{
	DECLARE_DYNAMIC(CCustomButton)

public:
	CCustomButton();
	virtual ~CCustomButton();

public:
	//������ɫ�ֱ�Ϊ���֣�Button Down�ı�����ɫ��Button Up�ı�����ɫ
	COLORREF m_TextColor, m_DownColor, m_UpColor;

	//Button Down�ı�����ɫ��Button Up�ı�����ɫ��־λ
	bool m_bDownColor, m_bUpColor;

	//���ִ�С
	int m_nWordSize;

	//ͼƬID
	UINT m_BKID;

public:
	//����Button Down�ı�����ɫ
	void SetDownColor(COLORREF color);
	//����Button Up�ı�����ɫ
	void SetUpColor(COLORREF color);
	//���ñ���ͼƬ
	void setBit(UINT ID);
	//�������ִ�С��Ĭ��Ϊ10��
	void setWordSize(int wordsize);
	//����������ɫĬ��Ϊ��ɫ
	void setWordColor(COLORREF wordColor);

protected:
	// ���ִ���
	void _setWord(CRect rect, CDC* pDc, int size);
	//���Ʊ���ͼƬ
	void _brushBit(CRect rect, CDC *pDc, UINT ID);
	//����DrawItem
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);

protected:
	DECLARE_MESSAGE_MAP()
};


