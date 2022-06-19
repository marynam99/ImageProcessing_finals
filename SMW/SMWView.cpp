
// SMWView.cpp : implementation of the CSMWView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SMW.h"
#endif

#include "SMWDoc.h"
#include "SMWView.h"
#include "CDib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSMWView

IMPLEMENT_DYNCREATE(CSMWView, CView)

BEGIN_MESSAGE_MAP(CSMWView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSMWView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_IMAGEPROCESSING_ADDOPERATION, &CSMWView::OnImageprocessingAddoperation)
	ON_COMMAND(ID_IMAGEPROCESSING_SUBOPERATION, &CSMWView::OnImageprocessingSuboperation)
	ON_COMMAND(ID_IMAGEPROCESSING_INVERSEOPERATION, &CSMWView::OnImageprocessingInverseoperation)
	ON_COMMAND(ID_IMAGEPROCESSING_BINARIZATION, &CSMWView::OnImageprocessingBinarization)
	ON_COMMAND(ID_IMAGEPROCESSING_GAMMACORRECTION, &CSMWView::OnImageprocessingGammacorrection)
	ON_COMMAND(ID_IMAGEPROCESSING_BITPLANE, &CSMWView::OnImageprocessingBitplane)
	ON_COMMAND(ID_IMAGEPROCESSING_BITPLANEALL, &CSMWView::OnImageprocessingBitplaneall)
	ON_COMMAND(ID_IMAGEPROCESSING_HISTOGRAM, &CSMWView::OnImageprocessingHistogram)
	ON_COMMAND(ID_HISTOGRAM_HISTOGRAMSTRETCHING, &CSMWView::OnHistogramHistogramstretching)
	ON_COMMAND(ID_HISTOGRAM_ENDINSEARCH, &CSMWView::OnHistogramEndinsearch)
	ON_COMMAND(ID_HISTOGRAM_HISTOGRAMEQUALIZATION, &CSMWView::OnHistogramHistogramequalization)
	ON_COMMAND(ID_HISTOGRAM_HISTOGRAMMATCHING, &CSMWView::OnHistogramHistogrammatching)
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_32784, &CSMWView::On32784)
	ON_COMMAND(ID_32785, &CSMWView::On32785)
END_MESSAGE_MAP()

// CSMWView construction/destruction

CSMWView::CSMWView() noexcept
{
	// TODO: add construction code here

}

CSMWView::~CSMWView()
{
}

BOOL CSMWView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSMWView drawing

void CSMWView::OnDraw(CDC* pDC)
{
	CSMWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	CRect viewRect;
	GetClientRect(viewRect);

	int height = viewRect.Height();
	int width = viewRect.Width();

	CDC memDC;
	CBitmap bit;
	memDC.CreateCompatibleDC(pDC);
	bit.CreateCompatibleBitmap(pDC, width, height);
	memDC.SelectObject(&bit);
	memDC.Rectangle(0, 0, width, height);
	
	// 입력 이미지를 화면에 출력
	unsigned char R, G, B;
	for (int i = 0; i < pDoc->m_height; i++)
	{
		for (int j = 0; j < pDoc->m_width; j++)
		{
			R = G = B = pDoc->m_InputImage[i * pDoc->m_width + j];
			memDC.SetPixel(j + 5, i + 5, RGB(R, G, B));
		}
	}

	if (!is_bitplaneall)
	{
		// 출력 이미지를 화면에 출력
		for (int i = 0; i < pDoc->m_Re_height; i++)
		{
			for (int j = 0; j < pDoc->m_Re_width; j++)
			{
				R = G = B = pDoc->m_OutputImage[i * pDoc->m_Re_width + j];
				memDC.SetPixel(j + 5, i + 35 + pDoc->m_Re_height, RGB(R, G, B));
			}
		}

		// 입력 이미지의 히스토그램 결과
		for (int i = 0; i < pDoc->m_input_hist_size + 20; i++)
		{
			for (int j = 0; j < pDoc->m_input_hist_size; j++)
			{
				R = G = B = pDoc->m_InputHistogram[i * pDoc->m_input_hist_size + j];
				memDC.SetPixel(j + 10 + pDoc->m_width, i + 5, RGB(R, G, B));
			}
		}

		// 출력 이미지의 히스토그램 결과
		for (int i = 0; i < pDoc->m_output_hist_size + 20; i++)
		{
			for (int j = 0; j < pDoc->m_output_hist_size; j++)
			{
				R = G = B = pDoc->m_OutputHistogram[i * pDoc->m_output_hist_size + j];
				memDC.SetPixel(
					j + 20 + pDoc->m_Re_width, 
					i + 35 + pDoc->m_Re_height, 
					RGB(R, G, B));
			}
		}
	}
	else
	{
		// 비트플레인 이미지를 화면에 출력
		for (int i = 0; i < pDoc->m_Re_height; i++)
		{
			for (int j = 0; j < pDoc->m_Re_width; j++)
			{
				R = G = B = pDoc->m_BitPlane7[i * pDoc->m_Re_width + j];
				memDC.SetPixel(j + pDoc->m_width + 10, i + 5, RGB(R, G, B));
			}
		}

		for (int i = 0; i < pDoc->m_Re_height; i++)
		{
			for (int j = 0; j < pDoc->m_Re_width; j++)
			{
				R = G = B = pDoc->m_BitPlane6[i * pDoc->m_Re_width + j];
				memDC.SetPixel(j + pDoc->m_width * 2 + 20, i + 5, RGB(R, G, B));
			}
		}

		for (int i = 0; i < pDoc->m_Re_height; i++)
		{
			for (int j = 0; j < pDoc->m_Re_width; j++)
			{
				R = G = B = pDoc->m_BitPlane5[i * pDoc->m_Re_width + j];
				memDC.SetPixel(j + pDoc->m_width * 3 + 30, i + 5, RGB(R, G, B));
			}
		}

		for (int i = 0; i < pDoc->m_Re_height; i++)
		{
			for (int j = 0; j < pDoc->m_Re_width; j++)
			{
				R = G = B = pDoc->m_BitPlane4[i * pDoc->m_Re_width + j];
				memDC.SetPixel(j + pDoc->m_width * 4 + 40, i + 5, RGB(R, G, B));
			}
		}

		for (int i = 0; i < pDoc->m_Re_height; i++)
		{
			for (int j = 0; j < pDoc->m_Re_width; j++)
			{
				R = G = B = pDoc->m_BitPlane3[i * pDoc->m_Re_width + j];
				memDC.SetPixel(j + pDoc->m_width + 10,
					i + pDoc->m_height + 15, 
					RGB(R, G, B));
			}
		}

		for (int i = 0; i < pDoc->m_Re_height; i++)
		{
			for (int j = 0; j < pDoc->m_Re_width; j++)
			{
				R = G = B = pDoc->m_BitPlane2[i * pDoc->m_Re_width + j];
				memDC.SetPixel(j + pDoc->m_width * 2 + 20,
					i + pDoc->m_height + 15,
					RGB(R, G, B));
			}
		}

		for (int i = 0; i < pDoc->m_Re_height; i++)
		{
			for (int j = 0; j < pDoc->m_Re_width; j++)
			{
				R = G = B = pDoc->m_BitPlane1[i * pDoc->m_Re_width + j];
				memDC.SetPixel(j + pDoc->m_width * 3 + 30,
					i + pDoc->m_height + 15,
					RGB(R, G, B));
			}
		}

		for (int i = 0; i < pDoc->m_Re_height; i++)
		{
			for (int j = 0; j < pDoc->m_Re_width; j++)
			{
				R = G = B = pDoc->m_BitPlane0[i * pDoc->m_Re_width + j];
				memDC.SetPixel(j + pDoc->m_width * 4 + 40,
					i + pDoc->m_height + 15,
					RGB(R, G, B));
			}
		}
	}

	// 결과 출력
	pDC->BitBlt(0, 0, width, height, &memDC, 0, 0, SRCCOPY);

	// 메모리 해제
	memDC.DeleteDC();
	bit.DeleteObject();
}


// CSMWView printing


void CSMWView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CSMWView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSMWView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSMWView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CSMWView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSMWView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSMWView diagnostics

#ifdef _DEBUG
void CSMWView::AssertValid() const
{
	CView::AssertValid();
}

void CSMWView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSMWDoc* CSMWView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSMWDoc)));
	return (CSMWDoc*)m_pDocument;
}
#endif //_DEBUG


// CSMWView message handlers


void CSMWView::OnImageprocessingAddoperation()
{
	// TODO: Add your command handler code here
	is_bitplaneall = false;
	CSMWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnImageprocessingAddoperation();
	Invalidate(TRUE);
}


void CSMWView::OnImageprocessingSuboperation()
{
	// TODO: Add your command handler code here
	is_bitplaneall = false;
	CSMWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnImageprocessingSuboperation();
	Invalidate(TRUE);
}


void CSMWView::OnImageprocessingInverseoperation()
{
	// TODO: Add your command handler code here
	is_bitplaneall = false;
	CSMWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnImageprocessingInverseoperation();
	Invalidate(TRUE);
}


void CSMWView::OnImageprocessingBinarization()
{
	// TODO: Add your command handler code here
	is_bitplaneall = false;
	CSMWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnImageprocessingBinarization();
	Invalidate(TRUE);
}


void CSMWView::OnImageprocessingGammacorrection()
{
	// TODO: Add your command handler code here
	is_bitplaneall = false;
	CSMWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnImageprocessingGammacorrection();
	Invalidate(TRUE);
}


void CSMWView::OnImageprocessingBitplane()
{
	// TODO: Add your command handler code here
	is_bitplaneall = false;
	CSMWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnImageprocessingBitplane();
	Invalidate(TRUE);
}


void CSMWView::OnImageprocessingBitplaneall()
{
	// TODO: Add your command handler code here
	is_bitplaneall = true;
	CSMWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnImageprocessingBitplaneall();
	Invalidate(TRUE);
}


void CSMWView::OnImageprocessingHistogram()
{
	// TODO: Add your command handler code here
}


void CSMWView::OnHistogramHistogramstretching()
{
	// TODO: Add your command handler code here
	is_bitplaneall = false;
	CSMWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnHistogramHistogramstretching();
	Invalidate(TRUE);
}


void CSMWView::OnHistogramEndinsearch()
{
	is_bitplaneall = false;
	CSMWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnHistogramEndinsearch();
	Invalidate(TRUE);
}


void CSMWView::OnHistogramHistogramequalization()
{
	is_bitplaneall = false;
	CSMWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnHistogramHistogramequalization();
	Invalidate(TRUE);
}


void CSMWView::OnHistogramHistogrammatching()
{
	is_bitplaneall = false;
	CSMWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnHistogramHistogrammatching();
	Invalidate(TRUE);
}


void CSMWView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CFile file;
	BITMAPFILEHEADER bmfh;
	DWORD dwFileSize, dwDibSize;
	BYTE* pDib = NULL;
	file.Open(_T("K:\\실습영상\\BMP영상\\lena.bmp"),
		CFile::modeRead | CFile::shareDenyWrite, NULL);
	dwFileSize = (DWORD)file.GetLength();
	dwDibSize = dwFileSize - sizeof(BITMAPFILEHEADER);
	pDib = new BYTE[dwDibSize];
	file.Read(&bmfh, sizeof(BITMAPFILEHEADER));
	file.Read(pDib, dwDibSize);
	file.Close();

	BITMAPINFOHEADER* lpbmih = (BITMAPINFOHEADER*)pDib;
	int w = lpbmih->biWidth;
	int h = lpbmih->biHeight;
	int c = lpbmih->biBitCount;

	BYTE* lpvBits = NULL;
	if (c == 24)
	{
		lpvBits = (BYTE*)pDib + sizeof(BITMAPINFOHEADER);
	}
	else
	{
		lpvBits = (BYTE*)pDib + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << c);
	}

	CClientDC dc(this);
	::SetDIBitsToDevice(dc.m_hDC, point.x, point.y, w, h, 0, 0, 0, h,
		lpvBits, (BITMAPINFO*)pDib, DIB_RGB_COLORS);
	delete[] pDib;

	CView::OnRButtonDown(nFlags, point);
}


void CSMWView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CDib dib;
	dib.Load(_T("K:\\실습영상\\BMP영상\\lena.bmp"));

	CClientDC dc(this);
	dib.Draw(dc.m_hDC, point.x, point.y);

	CView::OnLButtonDown(nFlags, point);
}


void CSMWView::On32784()
{
	is_bitplaneall = false;
	CSMWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnContrastStretching();
	Invalidate(TRUE);
}


void CSMWView::On32785()
{
	is_bitplaneall = false;
	CSMWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnDissolve();
	Invalidate(TRUE);
}
