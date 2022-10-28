// FillButton.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CustomButton.h"

// CCustomButton

IMPLEMENT_DYNAMIC(CCustomButton, CButton)
CCustomButton::CCustomButton():m_bDownColor(FALSE),m_bUpColor(FALSE),
   m_nWordSize(10),m_BKID(0)
{
}

CCustomButton::~CCustomButton()
{
}


void CCustomButton::SetDownColor(COLORREF color)
{
	m_DownColor = color;
	m_bDownColor = TRUE;//��ɫ�Ƿ����ñ�־λ
}


void CCustomButton::SetUpColor(COLORREF color)
{
	m_UpColor = color;
	m_bUpColor = TRUE;
}


void CCustomButton::setBit(UINT ID)
{
	m_BKID = ID;
}



void CCustomButton::setWordSize(int wordsize)
{
	m_nWordSize = wordsize;
}


void CCustomButton::setWordColor(COLORREF wordColor = RGB(255, 255, 255))
{
	m_TextColor = wordColor;
}


void CCustomButton::_brushBit(CRect rect,CDC *pDc,UINT ID)
{
	CDC   dcBmp;             //���岢����һ���ڴ��豸����
	dcBmp.CreateCompatibleDC(pDc);             //����������DC
	CBitmap   bmpBackground;
	bmpBackground.LoadBitmap(ID);    //������Դ��ͼƬ
	BITMAP   m_bitmap;                         //ͼƬ����               
	bmpBackground.GetBitmap(&m_bitmap);       //��ͼƬ����λͼ��
	//��λͼѡ����ʱ�ڴ��豸����
	CBitmap  *pbmpOld = dcBmp.SelectObject(&bmpBackground);
	//���ú�����ʾͼƬStretchBlt��ʾ��״�ɱ�
	pDc->SetStretchBltMode(HALFTONE);
	pDc->StretchBlt(0, 0, rect.Width(), rect.Height(), &dcBmp, 0, 0, m_bitmap.bmWidth, m_bitmap.bmHeight, SRCCOPY);
}


// ���ִ���
void CCustomButton::_setWord(CRect rect, CDC* pDc, int nWordSize)
{
	CString str;
	GetWindowText(str);//��ȡ����
	CFont font;
	font.CreatePointFont(nWordSize, "΢���ź�");//���ÿؼ����ִ�С������
	pDc->SelectObject(&font);
	pDc->SetTextColor(m_TextColor);//����������ɫ
	// dc.SetBkColor(RGB(255, 0, 0));//�������ֱ�����ɫ
	pDc->SetBkMode(TRANSPARENT);//�������ֱ���͸��
	pDc->DrawText(str, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_TABSTOP);//��������
}

void CCustomButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	// TODO:  ������Ĵ����Ի���ָ����
	CDC dc;
	dc.Attach(lpDIS->hDC);//�õ����Ƶ��豸����CDC
	//��ť�����
	{
		dc.SelectStockObject(NULL_BRUSH);//ѡ��ťĬ��ɫ
		dc.Rectangle(&lpDIS->rcItem);//������ť��
	}
	CString str;
	CRect rect = lpDIS->rcItem;
	GetClientRect(&rect);//�õ��ͻ������� //ScreenToClient();����Ļ����ת��Ϊ�ͻ�������

	//���ر���ͼƬ���ж��Ƿ���ر���ͼƬ��û����ʹ��Ĭ�ϱ���
	if (m_BKID != 0) 
		_brushBit(rect, &dc, m_BKID);

	//��ť����ʱ�ı䱳��ɫ
	if (lpDIS->itemState&ODS_SELECTED)
	{
		if (m_bDownColor)
		{
			CBrush brush(m_DownColor);
			dc.FillRect(&(lpDIS->rcItem), &brush);//���û�ˢbrush�������ο�
		}
	}
	else//����ť���������ߵ���ʱ�ı䱳��ɫ
	{
		if (m_bUpColor)
		{
			CBrush brush(m_UpColor);
			dc.FillRect(&(lpDIS->rcItem), &brush);//���û�ˢbrush�������ο�
		}
	}


	//ѡ���˱��ؼ�,�����߿�
	if ((lpDIS->itemState&ODS_SELECTED) && (lpDIS->itemAction &(ODA_SELECT | ODA_DRAWENTIRE)))
	{
		//���ð�ť�߿��밴ťɫ�෴
		COLORREF fc = RGB(255 - GetRValue(m_UpColor), 255 - GetGValue(m_UpColor), 255 - GetBValue(m_UpColor));
		CBrush brush(fc);
		dc.FrameRect(&(lpDIS->rcItem), &brush);//�û�ˢbrush�������α߿�
	}


	//���Ƶ�ѡ��״̬����,ȥ���߿�
	//if (!(lpDIS->itemState &ODS_SELECTED) && (lpDIS->itemAction & ODA_SELECT))
	//{
	//	CBrush brush(m_UpColor);
	//	dc.FrameRect(&lpDIS->rcItem, &brush);
	//}

	//���ִ����������֣�
	_setWord(rect, &dc, m_nWordSize);
	dc.Detach();
}


BEGIN_MESSAGE_MAP(CCustomButton, CButton)
END_MESSAGE_MAP()



// CFillButton ��Ϣ�������


