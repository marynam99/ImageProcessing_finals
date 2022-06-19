// CHistogramDlg.cpp : implementation file
//

#include "pch.h"
#include "SMWColor.h"
#include "afxdialogex.h"
#include "CHistogramDlg.h"

#include "CDib.h"
#include "SMWColorDoc.h"
#include "SMWColorView.h"
#include "DibEnhancement.h"


// CHistogramDlg dialog

IMPLEMENT_DYNAMIC(CHistogramDlg, CDialogEx)

CHistogramDlg::CHistogramDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HISTOGRAM, pParent)
{

}

CHistogramDlg::~CHistogramDlg()
{
}

void CHistogramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CHistogramDlg::SetImage(CDib& dib)
{
	float histo[256] = { 0.f, };

	if (dib.GetBitCount() == 8)
	{
		DibHistogram(dib, histo);
		NormalizeHist(histo, m_Histogram);
	}
	if (dib.GetBitCount() == 24)
	{
		isColor = true;
		DibHistogram(dib, histo, 0);
		NormalizeHist(histo, m_HistogramRGB[0]);
		DibHistogram(dib, histo, 1);
		NormalizeHist(histo, m_HistogramRGB[1]);
		DibHistogram(dib, histo, 2);
		NormalizeHist(histo, m_HistogramRGB[2]);
	}
}


BEGIN_MESSAGE_MAP(CHistogramDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CHistogramDlg message handlers


void CHistogramDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialogEx::OnPaint() for painting messages

	register int i;
	int* src = m_Histogram;
	/*if (isColor) {
		src = m_HistogramRGB[0];
	}*/

	if (isColor)
	{
		// 히스토그램 박스 출력
		/*dc.MoveTo(20, 30);
		dc.LineTo(20, 130);
		dc.LineTo(275, 130);
		dc.LineTo(275, 30);*/

		// R
		src = m_HistogramRGB[0];
		for (i = 0; i < 256; i++)
		{
			dc.SelectStockObject(DC_PEN);
			dc.SetDCPenColor(RGB(i, 0, 0));
			dc.MoveTo(20 + i, 130);
			dc.LineTo(20 + i, 130 - src[i]);
		}
		// 빨간색에 대한 막대
		for (i = 0; i < 256; i++)
		{
			dc.SelectStockObject(DC_PEN);
			dc.SetDCPenColor(RGB(i, 0, 0));

			dc.MoveTo(20 + i, 140);
			dc.LineTo(20 + i, 155);
		}

		// G
		src = m_HistogramRGB[1];
		for (i = 0; i < 256; i++)
		{
			dc.SelectStockObject(DC_PEN);
			dc.SetDCPenColor(RGB(0, i, 0));
			dc.MoveTo(20 + i, 130 + 130);
			dc.LineTo(20 + i, 130 + 130 - src[i]);
		}
		// G에 대한 막대
		for (i = 0; i < 256; i++)
		{
			dc.SelectStockObject(DC_PEN);
			dc.SetDCPenColor(RGB(0, i, 0));

			dc.MoveTo(20 + i, 140 + 130);
			dc.LineTo(20 + i, 155 + 130);
		}

		// B
		src = m_HistogramRGB[2];
		for (i = 0; i < 256; i++)
		{
			dc.SelectStockObject(DC_PEN);
			dc.SetDCPenColor(RGB(0, 0, i));
			dc.MoveTo(20 + i, 130 + 260);
			dc.LineTo(20 + i, 130 + 260 - src[i]);
		}
		// B에 대한 막대
		for (i = 0; i < 256; i++)
		{
			dc.SelectStockObject(DC_PEN);
			dc.SetDCPenColor(RGB(0, 0, i));

			dc.MoveTo(20 + i, 140 + 260);
			dc.LineTo(20 + i, 155 + 260);
		}
	}
	else
	{
		// 히스토그램 박스 출력
		dc.MoveTo(20, 30);
		dc.LineTo(20, 130);
		dc.LineTo(275, 130);
		dc.LineTo(275, 30);


		// 각 그래이스케일에 해당하는 히스토그램 출력
		for (i = 0; i < 256; i++)
		{
			dc.MoveTo(20 + i, 130);
			dc.LineTo(20 + i, 130 - src[i]);
		}

		// 그레이스케일 레벨 출력
		for (i = 0; i < 256; i++)
		{
			dc.SelectStockObject(DC_PEN);
			dc.SetDCPenColor(RGB(i, i, i));

			dc.MoveTo(20 + i, 140);
			dc.LineTo(20 + i, 155);
		}
	}

}


void CHistogramDlg::NormalizeHist(float src[256], int* dest)
{
	// TODO: Add your implementation code here.
	register int i;
	float max_value = src[0];
	for (i = 1; i < 256; i++)
	{
		if (src[i] > max_value)
		{
			max_value = src[i];
		}
	}

	for (i = 0; i < 256; i++)
	{
		dest[i] = (int)(src[i] * 100 / max_value);
	}
}
