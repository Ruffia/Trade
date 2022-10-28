#pragma once


// CCustomButton
// 定制化按钮控件
class CCustomButton : public CButton
{
	DECLARE_DYNAMIC(CCustomButton)

public:
	CCustomButton();
	virtual ~CCustomButton();

public:
	//三种颜色分别为文字，Button Down的背景颜色，Button Up的背景颜色
	COLORREF m_TextColor, m_DownColor, m_UpColor;

	//Button Down的背景颜色，Button Up的背景颜色标志位
	bool m_bDownColor, m_bUpColor;

	//文字大小
	int m_nWordSize;

	//图片ID
	UINT m_BKID;

public:
	//设置Button Down的背景颜色
	void SetDownColor(COLORREF color);
	//设置Button Up的背景颜色
	void SetUpColor(COLORREF color);
	//设置背景图片
	void setBit(UINT ID);
	//设置文字大小，默认为10号
	void setWordSize(int wordsize);
	//设置文字颜色默认为黑色
	void setWordColor(COLORREF wordColor);

protected:
	// 文字处理
	void _setWord(CRect rect, CDC* pDc, int size);
	//绘制背景图片
	void _brushBit(CRect rect, CDC *pDc, UINT ID);
	//重载DrawItem
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);

protected:
	DECLARE_MESSAGE_MAP()
};


