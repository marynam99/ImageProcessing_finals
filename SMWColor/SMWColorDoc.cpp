
// SMWColorDoc.cpp : implementation of the CSMWColorDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SMWColor.h"
#endif

#include "SMWColorDoc.h"
#include "DibEnhancement.h"
#include "CFilterDlg.h"
#include "CScalingDlg.h"

#include <propkey.h>

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSMWColorDoc

IMPLEMENT_DYNCREATE(CSMWColorDoc, CDocument)

BEGIN_MESSAGE_MAP(CSMWColorDoc, CDocument)
END_MESSAGE_MAP()


// CSMWColorDoc construction/destruction

CSMWColorDoc::CSMWColorDoc() noexcept
{
	// TODO: add one-time construction code here

}

CSMWColorDoc::~CSMWColorDoc()
{
}

BOOL CSMWColorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	BOOL bSuccess = TRUE;
	if (theApp.m_pNewDib != NULL)
	{
		m_Dib.Copy(theApp.m_pNewDib);
		theApp.m_pNewDib = NULL;
	}
	else
	{
		bSuccess = FALSE;
	}

	return bSuccess;
}




// CSMWColorDoc serialization

void CSMWColorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CSMWColorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CSMWColorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CSMWColorDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CSMWColorDoc diagnostics

#ifdef _DEBUG
void CSMWColorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSMWColorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSMWColorDoc commands


BOOL CSMWColorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  Add your specialized creation code here
	m_Dib.Load(lpszPathName);

	return TRUE;
}


BOOL CSMWColorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: Add your specialized code here and/or call the base class

	//return CDocument::OnSaveDocument(lpszPathName);
	return m_Dib.Save(lpszPathName);
}


// 영상 반전
void CSMWColorDoc::OnPixelprocessingInverse(CDib& dib)
{
	// TODO: Add your implementation code here.
	register int i, j;

	int w = dib.GetWidth();
	int h = dib.GetHeight();
	if (dib.GetBitCount() == 8)
	{
		BYTE** ptr = dib.GetPtr();
		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				ptr[j][i] = (255 - ptr[j][i]);
			}
		}
	}
	else if (dib.GetBitCount() == 24)
	{
		RGBBYTE** ptr = dib.GetRGBPtr();
		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				ptr[j][i].r = (255 - ptr[j][i].r);
				ptr[j][i].g = (255 - ptr[j][i].g);
				ptr[j][i].b = (255 - ptr[j][i].b);
			}
		}
	}
}


void CSMWColorDoc::OnHistogramprocessingHistogramstretching(CDib& dib)
{
	// TODO: 여기에 구현 코드 추가.
	register int i, j;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	// 그레이스케일 이미지일 때 히스토그램 스트레칭
	if (dib.GetBitCount() == 8)
	{
		BYTE** ptr = dib.GetPtr();
		unsigned char LOW = 0, HIGH = 255;
		unsigned char MIN = ptr[0][0];
		unsigned char MAX = ptr[0][0];
		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				if (MIN > ptr[j][i])
				{
					MIN = ptr[j][i];
				}
				if (MAX < ptr[j][i])
				{
					MAX = ptr[j][i];
				}
			}
		}
		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				ptr[j][i] = (unsigned char)((ptr[j][i] - MIN) * HIGH / (MAX - MIN));
			}
		}

	}
	else if (dib.GetBitCount() == 24)
	{
		RGBBYTE** ptr = dib.GetRGBPtr();

		unsigned char LOW = 0, HIGH = 255;
		unsigned char MIN = ptr[0][0].r;
		unsigned char MAX = ptr[0][0].r;

		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				if (MIN > ptr[j][i].r)
				{
					MIN = ptr[j][i].r;
				}
				if (MAX < ptr[j][i].r)
				{
					MAX = ptr[j][i].r;
				}
			}
		}
		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				ptr[j][i].r = (unsigned char)((ptr[j][i].r - MIN) * HIGH / (MAX - MIN));
			}
		}
		//G channel
		MIN = ptr[0][0].g;
		MAX = ptr[0][0].g;

		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				if (MIN > ptr[j][i].g)
				{
					MIN = ptr[j][i].g;
				}
				if (MAX < ptr[j][i].g)
				{
					MAX = ptr[j][i].g;
				}
			}
		}
		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				ptr[j][i].g = (unsigned char)((ptr[j][i].g - MIN) * HIGH / (MAX - MIN));
			}
		}

		//B channel
		MIN = ptr[0][0].b;
		MAX = ptr[0][0].b;

		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				if (MIN > ptr[j][i].b)
				{
					MIN = ptr[j][i].b;
				}
				if (MAX < ptr[j][i].b)
				{
					MAX = ptr[j][i].b;
				}
			}
		}
		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				ptr[j][i].b = (unsigned char)((ptr[j][i].b - MIN) * HIGH / (MAX - MIN));
			}
		}
	}
}


void CSMWColorDoc::OnColormodelRgb2yuv(CDib& dib)
{
	// TODO: 여기에 구현 코드 추가.
	register int j, i; 
	int w = dib.GetWidth();
	int h = dib.GetHeight();

	if (dib.GetBitCount() == 8)
	{

	}
	else if (dib.GetBitCount() == 24)
	{
		RGBBYTE** ptr = dib.GetRGBPtr();
		unsigned char Y = 0;
		unsigned char Cb = 0;
		unsigned char Cr = 0;

		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				Y = 0.299 * ptr[j][i].r + 0.587 * ptr[j][i].g + 0.114 * ptr[j][i].b;
				if (Y > 255) { Y = 255; }
				Cr = 0.500 * ptr[j][i].r - 0.419 * ptr[j][i].g - 0.081 * ptr[j][i].b;
				if (Cr > 255) { Cr = 255; }
				Cb = -0.16 * ptr[j][i].r  - 0.331 * ptr[j][i].g + 0.500 * ptr[j][i].b;
				if (Cb > 255) { Cb = 255; }

				ptr[j][i].r = Y;
				ptr[j][i].g = Cr;
				ptr[j][i].b = Cb;
			}
		}
	}
}


void CSMWColorDoc::OnHistogramprocessingHistogramstretchingycbcr(CDib& dib)
{
	// TODO: 여기에 구현 코드 추가.
	register int j, i;
	int w = dib.GetWidth();
	int h = dib.GetHeight();

	if (dib.GetBitCount() == 8)
	{
		BYTE** ptr = dib.GetPtr();
		unsigned char LOW = 0, HIGH = 255;
		unsigned char MIN = ptr[0][0];
		unsigned char MAX = ptr[0][0];

		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				if (MIN > ptr[j][i])
				{
					MIN = ptr[j][i];
				}
				if (MAX < ptr[j][i])
				{
					MAX = ptr[j][i];
				}
			}
		}
		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				ptr[j][i] = (unsigned char)((ptr[j][i] - MIN) 
					* HIGH / (MAX - MIN));
			}
		}
	}
	else if (dib.GetBitCount() == 24)
	{
		float** Y = new float* [h];
		float** Cr = new float* [h];
		float** Cb = new float* [h];

		for (j = 0; j < h; j++)
		{
			Y[j] = new float[w];
			Cr[j] = new float[w];
			Cb[j] = new float[w];
		}
		
		RGB2YCrCb(dib, Y, Cr, Cb);

		RGBBYTE** ptr = dib.GetRGBPtr();
		float LOW = 0., HIGH = 1.;
		float MIN = Y[0][0];
		float MAX = Y[0][0];

		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				if (MIN > Y[j][i])
				{
					MIN = Y[j][i];
				}
				if (MAX < Y[j][i])
				{
					MAX = Y[j][i];
				}
			}
		}
		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				Y[j][i] = (Y[j][i] - MIN) * HIGH / (MAX - MIN);
			}
		}

		YCrCb2RGB(dib, Y, Cr, Cb);

		delete[] Y;
		delete[] Cr;
		delete[] Cb;
		
	}
}


void CSMWColorDoc::OnHistogramprocessingHistogrammatching(CDib& dib)
{
	// TODO: 여기에 구현 코드 추가.
	register int j, i;
	int w = dib.GetWidth();
	int h = dib.GetHeight();

	if (dib.GetBitCount() == 8)
	{
		CDib TEMP;
		CFileDialog OpenDlg(TRUE);
		if (OpenDlg.DoModal() == IDOK)
		{
			TEMP.Load(OpenDlg.GetPathName());
			if (TEMP.GetBitCount() != 8 ||
				TEMP.GetWidth()    != w || 
				TEMP.GetHeight()   != h)
			{
				AfxMessageBox(L"Images not matched");
					return;
			}
		}

		BYTE** ptr_orig = dib.GetPtr();
		BYTE** ptr_targ = TEMP.GetPtr();

		FLOAT HIST_Orig[256] = { 0, };
		FLOAT HIST_Targ[256] = { 0, };

		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				HIST_Orig[ptr_orig[j][i]]++;
				HIST_Targ[ptr_targ[j][i]]++;
			}
		}

		FLOAT SUM_HIST_Orig[256] = { 0, };
		FLOAT SUM_HIST_Targ[256] = { 0, };

		FLOAT sum_orig = 0.;
		FLOAT sum_targ = 0.;
		for (i = 0; i < 256; i++)
		{
			sum_orig += HIST_Orig[i];
			sum_targ += HIST_Targ[i];

			SUM_HIST_Orig[i] = sum_orig;
			SUM_HIST_Targ[i] = sum_targ;
		}

		// Equalization
		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				// Normalization
				ptr_orig[j][i] = (BYTE)
					(SUM_HIST_Orig[ptr_orig[j][i]] * 255)
					/ (w * h);
			}
		}

		// Scaling
		BYTE SUM_ScHIST[256];

		FLOAT MIN = SUM_HIST_Targ[0];
		FLOAT MAX = SUM_HIST_Targ[255];
		for (i = 0; i < 255; i++)
		{
			SUM_ScHIST[i] = (BYTE)
				((SUM_HIST_Targ[i] - MIN) * 255
					/ (MAX - MIN));
		}

		// Table
		BYTE TABLE[256] = { 0, };
		int top = 255;
		int bottom = top - 1;
		while (TRUE)
		{
			for (i = SUM_ScHIST[bottom]; i <= SUM_ScHIST[top]; i++)
			{
				TABLE[i] = top;
			}
			top = bottom;
			bottom = bottom - 1;
			if (bottom < -1)
			{
				break;
			}
		}
		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				ptr_orig[j][i] = TABLE[ptr_orig[j][i]];
			}
		}
	}
}

/* 
	기말 범위
*/
void CSMWColorDoc::OnRegionprocessingEmbossing(CDib& dib)
{
	// TODO: 여기에 구현 코드 추가.
	register int i, j, n, m;
	int w = dib.GetWidth();
	int h = dib.GetHeight();

	//마스크 선택 - 계수의 합 0
	/*FLOAT Mask[3][3] = {
		{0.,0.,0.},
		{0.,1.,0.},
		{0.,0.,0.}
	};*/

	/*FLOAT Mask[3][3] = {
		{1.,1.,1.},
		{1.,-8.,1.},
		{1.,1.,1.}
	};*/
	FLOAT Mask[3][3] = {	// blur
		{1 / 9.,1 / 9.,1 / 9.},
		{1 / 9.,1 / 9.,1 / 9.},
		{1 / 9.,1 / 9.,1 / 9.}
	};

	if (dib.GetBitCount() == 8)
	{
		BYTE** ptr = dib.GetPtr();

		//실수 스케일의 임시 버퍼
		FLOAT** temp_in = Alloc2DMem(h + 2, w + 2); // 0으로 초기화: zero padding
		FLOAT** temp_out = Alloc2DMem(h, w);

		// 입력 이미지 실수값으로 복사 zero padding
		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				temp_in[j + 1][i + 1] = (FLOAT)ptr[j][i]; // padding하는 픽셀 빼고 복사
			}
		}

		// convolution
		FLOAT sum = 0.;
		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				for (n = 0; n < 3; n++)
				{
					for (m = 0; m < 3; m++)
					{
						sum += Mask[n][m] * temp_in[j + n][i + m];
					}
				}

				// 결과 버퍼에 저장
				temp_out[j][i] = sum; // 결과 버퍼는 padding 포함된 크기가 아니라 원본과 똑같은 크기이므로 index 동일
				sum = 0;
			}
		}

			// 평균 계산
			sum = 0.;
			for (j = 0; j < h; j++)
			{
				for (i = 0; i < w; i++)
				{
					sum += ptr[j][i];
				}
			}
			FLOAT avg = sum / ((FLOAT)w * h);

			// 컨볼루션 결과 clipping
			for (j = 0; j < h; j++)
			{
				for (i = 0; i < w; i++)
				{
					//temp_out[j][i] = limit(temp_out[j][i] + avg);
					temp_out[j][i] = limit(temp_out[j][i]);
				}
			}
			for (j = 0; j < h; j++)
			{
				for (i = 0; i < w; i++)
				{
					ptr[j][i] = (BYTE)temp_out[j][i];
				}
			}
			delete[] temp_in;
			delete[] temp_out;

	}

	else if (dib.GetBitCount() == 24)
	{
		RGBBYTE** ptr = dib.GetRGBPtr();

		FLOAT** Y = Alloc2DMem(h, w);
		FLOAT** Cr = Alloc2DMem(h, w);
		FLOAT** Cb = Alloc2DMem(h, w);

		// 컬러공간 변경
		RGB2YCrCb(dib, Y, Cr, Cb);

		FLOAT** Y_margin = Alloc2DMem(h + 2, w + 2);
		FLOAT** Y_result = Alloc2DMem(h, w);

		// 입력 이미지 실수값으로 복사, zero-padding
		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				Y_margin[j + 1][i + 1] = (FLOAT)Y[j][i];
			}
		}

		// convolution
		FLOAT sum = 0.;
		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				for (n = 0; n < 3; n++)
				{
					for (m = 0; m < 3; m++)
					{
						sum += Mask[n][m] * Y_margin[j + n][i + m];
					}
				}

				// 결과 버퍼에 저장
				Y_result[j][i] = sum; // 결과 버퍼는 padding 포함된 크기가 아니라 원본과 똑같은 크기이므로 index 동일
				sum = 0;
			}
		}
		// 평균 계산
		sum = 0.;
		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				sum += Y[j][i];
			}
		}
		FLOAT avg = sum / ((FLOAT)w * h);

		// 컨볼루션 결과 clipping
		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				/*Y_result[j][i] = limit(Y_result[j][i] + avg,
					(FLOAT)0., (FLOAT)1.);*/
				Y_result[j][i] = limit(Y_result[j][i],
					(FLOAT)0., (FLOAT)1.);
			}
		}

		// 컬러공간 변경
		YCrCb2RGB(dib, Y_result, Cr, Cb);

		delete[] Y;
		delete[] Cr;
		delete[] Cb;
		delete[] Y_margin;
		delete[] Y_result;
	}
}

void CSMWColorDoc::OnRegionprocessingSelectfilter(CDib& dib)
{
	// TODO: 여기에 구현 코드 추가.
	CFilterDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		int FilterSize = dlg.m_FilterSize;
		int FilterSize2 = dlg.m_FilterSize2;
		int FilterType = dlg.m_FilterType;
		FLOAT Sigma  = dlg.m_sigma;
		FLOAT Sigma2 = dlg.m_sigma2;
		switch (FilterType)
		{
		case 0:
			AverageFiltering(dib, FilterSize);
			break;
		case 1:
			GaussianBlurring(dib, FilterSize, Sigma);
			break;
		case 2:
			SharpeningFiltering(dib, FilterSize);
			break;
		case 3:
			Sharpening2Filtering(dib, FilterSize);
			break;
		case 4:
			Unsharp(dib, FilterSize);
			break;
		case 5:
			HighFrequencyBandPassFilter(dib, FilterSize);
			break;
		case 6:
			HighBoost(dib, FilterSize);
			break;
		case 7:
			LoG(dib, FilterSize, Sigma);
			break;
		case 8:
			DoG(dib, FilterSize, FilterSize2, Sigma, Sigma2);
			break;
		case 9:
			break;
		}
	}
}

void CSMWColorDoc::AverageFiltering(CDib& dib, int ksize)
{
	register int i, j;

	// 마스크 생성
	FLOAT** mask = Alloc2DMem(ksize, ksize); //margin 때문에 호출
	for (i = 0; i < ksize; i++)
	{
		for (j = 0; j < ksize; j++)
		{
			mask[i][j] = 1. / ((FLOAT)ksize * ksize);
		}
	}

	// 그레이 스케일
	if (dib.GetBitCount() == 8)
	{

	}
	//컬러
	else if (dib.GetBitCount() == 24)
	{
		ConvolutionProcessColor(dib, mask, ksize);
	}
}

void CSMWColorDoc::GaussianBlurring(CDib& dib, int ksize, FLOAT sigma) // kernel size, sigma 커지면 blurring 더 많이 됨
{	// 결과가 average blurring에 비해 자연스러움
	register int i, j, n, m;
	int w = dib.GetWidth();
	int h = dib.GetHeight();
	int margin = ksize / 2;

	// 마스크 생성
	FLOAT** mask = Alloc2DMem(ksize, ksize);
	//FLOAT sigma = 1.;
	FLOAT r, s = 2. * sigma * sigma;
	FLOAT sum = 0.;

	for (i = -margin; i <= margin; i++)
	{
		for (j = -margin; j <= margin; j++)
		{
			r = sqrt(i * i + j * j);
			mask[i + margin][j + margin] =
				(exp(-(r * r) / s)) / (M_PI * s); // 이 연산만 가지고는 mask 값이 1 넘음
			sum += mask[i + margin][j + margin];
		}
	}
	// 여기까지 gaussian 분포를 하고 나면 결과가 알맞지 않음
	
	// Normalization: 전체 합이 1이 되도록 각 값을 sum으로 나눔
	for (i = 0; i < ksize; i++)
	{
		for (j = 0; j < ksize; j++)
		{
			mask[i][j] /= sum;
		}
	}

	// grayscale
	if (dib.GetBitCount() == 8)
	{

	}
	// 컬러
	else if (dib.GetBitCount() == 24)
	{
		ConvolutionProcessColor(dib, mask, ksize);
	}
}

void CSMWColorDoc::SharpeningFiltering(CDib& dib, int ksize)
{
	register int i, j;
	int w = dib.GetWidth();
	int h = dib.GetHeight();
	int margin = ksize / 2;

	// 마스크 생성
	FLOAT** mask = Alloc2DMem(ksize, ksize);
	for (i = 0; i < ksize; i++)
	{
		for (j = 0; j < ksize; j++)
		{
			mask[i][j] = -1.;
			if (i == margin && j == margin) // 좌표가 가운데면 ksize^2이고 나머지는 -1
			{
				mask[i][i] = ((FLOAT)ksize * ksize);
			}
		}
	}

	// grayscale
	if (dib.GetBitCount() == 8)
	{

	}
	// 컬러
	else if (dib.GetBitCount() == 24)
	{
		ConvolutionProcessColor(dib, mask, ksize);
	}
}

void CSMWColorDoc::Sharpening2Filtering(CDib& dib, int ksize)
{
	// TODO: 여기에 구현 코드 추가.
	register int i, j;
	int w = dib.GetWidth();
	int h = dib.GetHeight();
	int margin = ksize / 2;

	// 마스크 생성
	FLOAT** mask = Alloc2DMem(ksize, ksize);
	for (i = 0; i < ksize; i++)
	{
		for (j = 0; j < ksize; j++)
		{
			if ((i + j) % 2 == 1) { mask[i][j] = -1.; }
			else if ((i + j) % 2 == 0) { mask[i][j] = 0.; }

			if (i == margin && j == margin) { mask[i][i] = (ksize - 1) * (ksize - 1) + 1; }
		}
	}

	// grayscale
	if (dib.GetBitCount() == 8)
	{

	}
	// 컬러
	else if (dib.GetBitCount() == 24)
	{
		ConvolutionProcessColor(dib, mask, ksize);
	}
}

/// <summary>
/// 원 영상과 저주파 통과 필터링 결과의 차이값
/// </summary>
void CSMWColorDoc::Unsharp(CDib& dib, int ksize)
{
	// TODO: 여기에 구현 코드 추가.
	register int i, j, m, n, ch;
	int h = dib.GetHeight();
	int w = dib.GetWidth();

	if (dib.GetBitCount() == 24)
	{
		RGBBYTE** ptr = dib.GetRGBPtr();
		FLOAT** temp[3];
		FLOAT** blur[3];

		for (ch = 0; ch < 3; ch++)
		{
			temp[ch] = Alloc2DMem(h, w);
			blur[ch] = Alloc2DMem(h, w);
		}

		for (i = 0; i < h; i++)
		{
			for (j = 0; j < w; j++)
			{
				temp[0][i][j] = ptr[i][j].r;
				temp[1][i][j] = ptr[i][j].g;
				temp[2][i][j] = ptr[i][j].b;
			}
		}
		GaussianBlurring(dib, 3, 1);
		ptr = dib.GetRGBPtr();
		for (i = 0; i < h; i++)
		{
			for (j = 0; j < w; j++)
			{
				blur[0][i][j] = ptr[i][j].r;
				blur[1][i][j] = ptr[i][j].g;
				blur[2][i][j] = ptr[i][j].b;
			}
		}
		for (i = 0; i < h; i++)
		{
			for (j = 0; j < w; j++)
			{
				ptr[i][j].r = abs(temp[0][i][j] - blur[0][i][j]);
				ptr[i][j].g = abs(temp[1][i][j] - blur[1][i][j]);
				ptr[i][j].b = abs(temp[2][i][j] - blur[2][i][j]);
			}
		}
		for (ch = 0; ch < 3; ch++)
		{
			delete[] blur[ch];
			delete[] temp[ch];
		}
	}
}

void CSMWColorDoc::HighFrequencyBandPassFilter(CDib& dib, int ksize)
{
	register int i, j;
	int w = dib.GetWidth();
	int h = dib.GetHeight();
	int margin = ksize / 2;

	// 마스크 생성
	FLOAT** mask = Alloc2DMem(ksize, ksize);
	for (i = 0; i < ksize; i++)
	{
		for (j = 0; j < ksize; j++)
		{
			mask[i][j] = -1. / ((FLOAT)ksize*ksize);
			if (i == margin && j == margin) // 좌표가 가운데면 ksize^2이고 나머지는 -1
			{
				mask[i][i] = ((FLOAT)ksize * ksize - 1)
					/ ((FLOAT)ksize * ksize);
			}
		}
	}

	// grayscale
	if (dib.GetBitCount() == 8)
	{

	}
	// 컬러
	else if (dib.GetBitCount() == 24)
	{
		ConvolutionProcessColorRGB(dib, mask, ksize);
	}
	delete[] mask;
}

void CSMWColorDoc::HighBoost(CDib& dib, int ksize)
{
	// TODO: 여기에 구현 코드 추가.
	register int i, j, ch;
	int h = dib.GetHeight();
	int w = dib.GetWidth();
	FLOAT alpha = 1.;
	int k = 1;

	if (dib.GetBitCount() == 24)
	{
		RGBBYTE** ptr = dib.GetRGBPtr();
		FLOAT** temp[3];
		FLOAT** unsharp[3];

		for (ch = 0; ch < 3; ch++)
		{
			temp[ch] = Alloc2DMem(h, w);
			unsharp[ch] = Alloc2DMem(h, w);
		}

		for (i = 0; i < h; i++)
		{
			for (j = 0; j < w; j++)
			{
				temp[0][i][j] = ptr[i][j].r;
				temp[1][i][j] = ptr[i][j].g;
				temp[2][i][j] = ptr[i][j].b;
			}
		}
		Unsharp(dib, 3);
		ptr = dib.GetRGBPtr();
		for (i = 0; i < h; i++)
		{
			for (j = 0; j < w; j++)
			{
				unsharp[0][i][j] = ptr[i][j].r;
				unsharp[1][i][j] = ptr[i][j].g;
				unsharp[2][i][j] = ptr[i][j].b;
			}
		}
		// convolution 결과 clipping
		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				/*Y_result[j][i] = limit(Y_result[j][i] + avg,
					(FLOAT)0., (FLOAT)1.);*/
				temp[0][i][j] = limit(alpha * temp[0][i][j] + k * unsharp[0][i][j] * 1., 0., 255.);
				temp[1][i][j] = limit(alpha * temp[1][i][j] + k * unsharp[1][i][j] * 1., 0., 255.);
				temp[2][i][j] = limit(alpha * temp[2][i][j] + k * unsharp[2][i][j] * 1., 0., 255.);
			}
		}
		for (i = 0; i < h; i++)
		{
			for (j = 0; j < w; j++)
			{
				ptr[i][j].r = temp[0][i][j];
				ptr[i][j].g = temp[1][i][j];
				ptr[i][j].b = temp[2][i][j];
			}
		}
		for (ch = 0; ch < 3; ch++)
		{
			delete[] temp[ch];
			delete[] unsharp[ch];
		}
	}
}

void CSMWColorDoc::LoG(CDib& dib, int ksize, FLOAT sigma)
{
	// TODO: 여기에 구현 코드 추가.
	GaussianBlurring(dib, ksize, sigma);
	OnDifferenceLaplacian(dib);
}

void CSMWColorDoc::DoG(CDib& dib, int ksize, int ksize2, FLOAT sigma, FLOAT sigma2)
{
	// TODO: 여기에 구현 코드 추가.
	register int i, j;
	int w = dib.GetWidth();
	int h = dib.GetHeight();

	/*ksize = 3;
	int ksize2 = 5;
	FLOAT sigma1 = 3.2;
	FLOAT sigma2 = 2.0;*/


	if (dib.GetBitCount() == 8) {

	}
	else if (dib.GetBitCount() == 24) {

		CDib ndib(dib);
		GaussianBlurring(dib, ksize2, sigma);
		GaussianBlurring(ndib, ksize, sigma2);

		RGBBYTE** ptr = dib.GetRGBPtr();
		RGBBYTE** ptr2 = ndib.GetRGBPtr();

		for (i = 0; i < h; i++) {
			for (j = 0; j < w; j++) {
				ptr[i][j].r = limit(ptr[i][j].r - ptr2[i][j].r);
				ptr[i][j].g = limit(ptr[i][j].g - ptr2[i][j].g);
				ptr[i][j].b = limit(ptr[i][j].b - ptr2[i][j].b);
			}
		}

	}
}

/// <summary>
/// detects horizontal edge
/// </summary>
void CSMWColorDoc::OnDifferenceHorizontal(CDib& dib)
{
	// TODO: 여기에 구현 코드 추가.
	register int i, j;
	int ksize = 3;
	int margin = ksize / 2;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	// 마스크 생성
	FLOAT** mask = Alloc2DMem(ksize, ksize);
	mask[1][0] = -1.;
	mask[1][1] = 1.;

	// grayscale
	if (dib.GetBitCount() == 8)
	{

	}
	// 컬러
	else if (dib.GetBitCount() == 24)
	{
		ConvolutionProcessColorRGB(dib, mask, ksize);
	}
	delete[] mask;
}

void CSMWColorDoc::OnDifferenceVertical(CDib& dib)
{
	// TODO: 여기에 구현 코드 추가.
	register int i, j;
	int ksize = 3;
	int margin = ksize / 2;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	// 마스크 생성
	FLOAT** mask = Alloc2DMem(ksize, ksize);
	mask[0][1] = -1.;
	mask[1][1] = 1.;

	// grayscale
	if (dib.GetBitCount() == 8)
	{

	}
	// 컬러
	else if (dib.GetBitCount() == 24)
	{
		ConvolutionProcessColorRGB(dib, mask, ksize);
	}
	delete[] mask;
}

/// <summary>
/// 유사 연산자: 주변 픽셀과의 차이 중에서 가장 큰 것을 엣지로 정의한다
/// </summary>
void CSMWColorDoc::OnDifferenceHomogeneousoperator(CDib& dib) 
{
	// TODO: 여기에 구현 코드 추가.
	register int i, j, n, m, ch;
	int ksize = 3;
	int margin = ksize / 2;  // 가운데 칸 제외하고 절반

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	if (dib.GetBitCount() == 24)
	{
		RGBBYTE** ptr = dib.GetRGBPtr();

		// 실수 스케일의 임시 버퍼
		FLOAT** buf[3];
		FLOAT** RGB[3];
		for (ch = 0; ch < 3; ch++)
		{
			buf[ch] = Alloc2DMem(h, w);
			RGB[ch] = Alloc2DMem(h + ksize - 1, w + ksize - 1);
		}

		// 입력 이미지 실수값으로 복사, zero-padding
		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				RGB[0][j + margin][i + margin] = (FLOAT)ptr[j][i].r ;
				RGB[1][j + margin][i + margin] = (FLOAT)ptr[j][i].g ;
				RGB[2][j + margin][i + margin] = (FLOAT)ptr[j][i].b ;
			}
		}
		// convolution
		for (ch = 0; ch < 3; ch++)
		{
			for (j = 0; j < h; j++)
			{
				for (i = 0; i < w; i++)
				{
					FLOAT max = 0.;
					for (n = 0; n < ksize; n++)
					{
						for (m = 0; m < ksize; m++)
						{
							if (max < abs(RGB[ch][j + ksize / 2][i + ksize / 2] - RGB[ch][j + n][i + m]))
							{
								max = abs(RGB[ch][j + ksize / 2][i + ksize / 2] - RGB[ch][j + n][i + m]);
							}
						}
					}
					// 결과 버퍼에 저장
					buf[ch][j][i] = max;
				}
			}
			// convolution 결과 clipping
			for (j = 0; j < h; j++)
			{
				for (i = 0; i < w; i++)
				{
					/*Y_result[j][i] = limit(Y_result[j][i] + avg,
						(FLOAT)0., (FLOAT)1.);*/
					buf[ch][j][i] = limit(buf[ch][j][i] * 1., 0., 255.);
				}
			}
		}

		// 복사
		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				ptr[j][i].r = (BYTE)buf[0][j][i];
				ptr[j][i].g = (BYTE)buf[1][j][i];
				ptr[j][i].b = (BYTE)buf[2][j][i];
			}
		}

		// 동적 할당 해제
		for (ch = 0; ch < 3; ch++)
		{
			delete[] buf[ch];
			delete[] RGB[ch];
		}
	}
}

/// <summary>
/// 차 연산자: 대각선, 상하좌우로 총 네 번의 뺄셈 연산만 하면 됨
/// </summary>
/// <param name="dib"></param>
void CSMWColorDoc::OnDifferenceDifferenceoperator(CDib& dib)
{
	// TODO: 여기에 구현 코드 추가.
	register int i, j, m, n, ch;
	int h = dib.GetHeight();
	int w = dib.GetWidth();
	int ksize = 3;
	int margin = ksize / 2;

	if (dib.GetBitCount() == 24)
	{
		RGBBYTE** ptr = dib.GetRGBPtr();

		// buf: 연산 결과 저장
		// RGB: 초기 rgb 값 저장 후 연산의 대상
		FLOAT** buf[3];
		FLOAT** RGB[3];
		for (ch = 0; ch < 3; ch++)
		{
			buf[ch] = Alloc2DMem(h, w);
			RGB[ch] = Alloc2DMem(h + ksize - 1, w + ksize - 1);
		}
		for (i = 0; i < h; i++) {
			for (j = 0; j < w; j++) {
				RGB[0][i + margin][j + margin] = ptr[i][j].r;
				RGB[1][i + margin][j + margin] = ptr[i][j].g;
				RGB[2][i + margin][j + margin] = ptr[i][j].b;
			}
		}

		for (ch = 0; ch < 3; ch++)
		{
			for (i = 0; i < h; i++)
			{
				for (j = 0; j < w; j++)
				{
					FLOAT max = 0;
					for (m = 0; m < ksize + 1; m++)
					{
						if (m < ksize) {
							if (max < abs(RGB[ch][i + m][j + m] - RGB[ch][i + (2 - m)][j + (2 - m)]))
							{
								max = abs(RGB[ch][i + m][j + m] - RGB[ch][i + (2 - m)][j + (2 - m)]);
							}
						}
						else {
							if (max < abs(RGB[ch][i + 1][j + 2] - RGB[ch][i + 1][j]))
							{
								max = abs(RGB[ch][i + 1][j + 2] - RGB[ch][i + 1][j]);
							}
						}
					}
					buf[ch][i][j] = max;
				}
			}
			// convolution 결과 clipping
			for (i = 0; i < h; i++)
			{
				for (j = 0; j < w; j++)
				{
					/*Y_result[j][i] = limit(Y_result[j][i] + avg,
						(FLOAT)0., (FLOAT)1.);*/
					buf[ch][i][j] = limit(buf[ch][i][j] * 1., 0., 255.);
				}
			}
		}
		for (i = 0; i < h; i++)
		{
			for (j = 0; j < w; j++)
			{
				ptr[i][j].r = buf[0][i][j];
				ptr[i][j].g = buf[1][i][j];
				ptr[i][j].b = buf[2][i][j];
			}
		}
		for (ch = 0; ch < 3; ch++)
		{
			delete[] buf[ch];
			delete[] RGB[ch];
		}
	}
}

void CSMWColorDoc::OnDifferenceLaplacian(CDib& dib)
{
	// TODO: 여기에 구현 코드 추가.
	register int i, j;
	int ksize = 3;
	int margin = ksize / 2;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	// 마스크 생성
	FLOAT** mask = Alloc2DMem(ksize, ksize);
	for (i = 0; i < ksize; i++)
	{
		for (j = 0; j < ksize; j++)
		{
			// 가운데만 ksize^2-1이고 그 외에는 모두 -1
			/*mask[i][j] = -1.;
			if (i == margin && j == margin)
			{
				mask[i][i] = ((FLOAT)ksize * ksize - 1);
			}*/

			if ((i + j) % 2 == 1) { mask[i][j] = -1.; }
			else if ((i + j) % 2 == 0) { mask[i][j] = 0.; }

			if (i == margin && j == margin) { mask[i][i] = (ksize - 1) * (ksize - 1); }
		}
	}
	if (dib.GetBitCount() == 24)
	{
		ConvolutionProcessColorRGB(dib, mask, ksize);
	}
}

void CSMWColorDoc::OnDifferenceSobel(CDib& dib)
{
	// TODO: 여기에 구현 코드 추가.
	register int i, j, n, m, ch;
	int ksize = 3;
	int margin = ksize / 2;  // 2로 나누는 이유: 가운데 칸의 위 아래로 반(-0.5개)가 남음. 소수점 어차피 버림

	int w = dib.GetWidth();
	int h = dib.GetHeight();


	// 실수 스케일의 임시 버퍼
	FLOAT** mask_hor = Alloc2DMem(ksize, ksize);
	FLOAT** mask_ver = Alloc2DMem(ksize, ksize);
	
	mask_hor[0][0] = -1.;
	mask_hor[0][2] = 1.;
	mask_hor[1][0] = -2.;
	mask_hor[1][2] = 2.;
	mask_hor[2][0] = -1.;
	mask_hor[2][2] = 1.;

	mask_ver[0][0] = 1.;
	mask_ver[0][1] = 2.;
	mask_ver[0][2] = 1.;
	mask_ver[2][0] = -1.;
	mask_ver[2][1] = -2.;
	mask_ver[2][2] = -1.;


	// grayscale
	if (dib.GetBitCount() == 8)
	{

	}
	else if (dib.GetBitCount() == 24)
	{
		RGBBYTE** ptr = dib.GetRGBPtr();

		// 실수 스케일의 임시 버퍼
		FLOAT** buf[3];
		FLOAT** RGB[3];
		for (ch = 0; ch < 3; ch++)
		{
			buf[ch] = Alloc2DMem(h, w);
			RGB[ch] = Alloc2DMem(h + ksize - 1, w + ksize - 1);
		}

		// 입력 이미지 실수값으로 복사, zero-padding
		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				RGB[0][j + margin][i + margin] = (FLOAT)ptr[j][i].r / 255; //embossing에서 [i+1][j+1]인 이유는 mask를 3*3으로 고정했었기 때문
				RGB[1][j + margin][i + margin] = (FLOAT)ptr[j][i].g / 255;
				RGB[2][j + margin][i + margin] = (FLOAT)ptr[j][i].b / 255;
			}
		}

		// convolution
		for (ch = 0; ch < 3; ch++)
		{
			for (j = 0; j < h; j++)
			{
				for (i = 0; i < w; i++)
				{

					FLOAT hor = 0.;
					FLOAT ver = 0.;
					for (n = 0; n < ksize; n++)
					{
						for (m = 0; m < ksize; m++)
						{
							hor += mask_hor[n][m] * RGB[ch][j + n][i + m];
							ver += mask_ver[n][m] * RGB[ch][j + n][i + m];
						}
					}

					// 결과 버퍼에 저장
					buf[ch][j][i] = abs(hor)+abs(ver);
				}
			}
			// convolution 결과 clipping
			for (j = 0; j < h; j++)
			{
				for (i = 0; i < w; i++)
				{
					/*Y_result[j][i] = limit(Y_result[j][i] + avg,
						(FLOAT)0., (FLOAT)1.);*/
					buf[ch][j][i] = limit(buf[ch][j][i] * 255., 0., 255.);
				}
			}
		}

		// 복사
		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				ptr[j][i].r = (BYTE)buf[0][j][i];
				ptr[j][i].g = (BYTE)buf[1][j][i];
				ptr[j][i].b = (BYTE)buf[2][j][i];
			}
		}

		// 동적 할당 해제
		for (ch = 0; ch < 3; ch++)
		{
			delete[] buf[ch];
			delete[] RGB[ch];
		}
	}
}

void CSMWColorDoc::OnGeometrictransformationSelecttransf(CDib& dib)
{
	// TODO: 여기에 구현 코드 추가.
}

void CSMWColorDoc::OnGeometrictransformationTranslation(CDib& dib)
{
	// TODO: 여기에 구현 코드 추가.
	register int i, j;
	int w = dib.GetWidth();
	int h = dib.GetHeight();

	

}

void CSMWColorDoc::OnGeometrictransformationRotation(CDib& dib)
{
	// TODO: 여기에 구현 코드 추가.
	register int i, j;
	int w = dib.GetWidth();
	int h = dib.GetHeight();

	int h_center = h / 2;
	int w_center = w / 2;
	int degree = 45;

	int h_new, w_new;

	FLOAT radian = (FLOAT)degree * M_PI / 180.0;

	if (dib.GetBitCount() == 8) {}
	else if (dib.GetBitCount() == 24)
	{
		RGBBYTE** ptr = dib.GetRGBPtr();
		FLOAT** temp_r = Alloc2DMem(h, w);
		FLOAT** temp_g = Alloc2DMem(h, w);
		FLOAT** temp_b = Alloc2DMem(h, w);

		for (i = 0; i < h; i++)
		{
			for (j = 0; j < w; j++)
			{
				h_new = (int)((i - h_center) * cos(radian) - (j - w_center) * sin(radian) + h_center); // i = x
				w_new = (int)((i - h_center) * sin(radian) + (j - w_center) * cos(radian) + w_center); // j = y

				if (h_new < 0 || h_new >= h ||
					w_new < 0 || w_new >= w)
				{
					temp_r[i][j] = 0;
					temp_g[i][j] = 0;
					temp_b[i][j] = 0;
				}
				else
				{
					temp_r[i][j] = ptr[h_new][w_new].r;
					temp_g[i][j] = ptr[h_new][w_new].g;
					temp_b[i][j] = ptr[h_new][w_new].b;
				}
			}
		}

		for (i = 0; i < h; i++)
		{
			for (j = 0; j < w; j++)
			{
				ptr[i][j].r = temp_r[i][j];
				ptr[i][j].g = temp_g[i][j];
				ptr[i][j].b = temp_b[i][j];
			}
		}

		delete[] temp_r;
		delete[] temp_g;
		delete[] temp_b;
	}
}

void CSMWColorDoc::OnGeometrictransformationRotation2(CDib& dib)
{
	// TODO: 여기에 구현 코드 추가.
	register int i, j;
	int w = dib.GetWidth();
	int h = dib.GetHeight();

	int h_center = h / 2;
	int w_center = w / 2;
	int degree = 45;

	int h_new, w_new;
	FLOAT radian = (FLOAT)degree * M_PI / 180.0;
	int W = h * sin(radian) + w * cos(radian);
	int H = h * cos(radian) + w * sin(radian);

	if (dib.GetBitCount() == 8) {}
	else if (dib.GetBitCount() == 24)
	{
		RGBBYTE** ptr = dib.GetRGBPtr();
		FLOAT** temp_r = Alloc2DMem(H, W);
		FLOAT** temp_g = Alloc2DMem(H, W);
		FLOAT** temp_b = Alloc2DMem(H, W);

		for (i = 0; i < H; i++)
		{
			for (j = 0; j < W; j++)
			{
				//h_new = (int)(i * cos(radian) - j * sin(radian) + h * sin(radian)); // 딱 맞음
				//w_new = (int)(i * sin(radian) + j * cos(radian) - h * sin(radian));
				h_new = (int)(i * cos(radian) - (j - h * sin(radian)) * sin(radian)); 
				w_new = (int)(i * sin(radian) + (j - h * sin(radian)) * cos(radian));

				if (h_new < 0 || h_new >= h ||
					w_new < 0 || w_new >= w)
				{
					temp_r[i][j] = 0;
					temp_g[i][j] = 0;
					temp_b[i][j] = 0;
				}
				else
				{
					temp_r[i][j] = ptr[h_new][w_new].r;
					temp_g[i][j] = ptr[h_new][w_new].g;
					temp_b[i][j] = ptr[h_new][w_new].b;
				}
			}
		}
		dib.CreateRGBImage(W, H);
		ptr = dib.GetRGBPtr();

		for (i = 0; i < H; i++)
		{
			for (j = 0; j < W; j++)
			{
				ptr[i][j].r = temp_r[i][j];
				ptr[i][j].g = temp_g[i][j];
				ptr[i][j].b = temp_b[i][j];
			}
		}

		delete[] temp_r;
		delete[] temp_g;
		delete[] temp_b;
	}
}

void CSMWColorDoc::OnScalingNearest(CDib& dib)
{
	// TODO: 여기에 구현 코드 추가.
	register int i, j;
	int w = dib.GetWidth();
	int h = dib.GetWidth();
	int h_new, w_new, scale;

	CScalingDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		scale = dlg.m_EditScale;
		
		h_new = int(scale * h);
		w_new = int(scale * h);
	}
	if (dib.GetBitCount() == 24)
	{
		RGBBYTE** ptr = dib.GetRGBPtr();
		FLOAT** temp_r = Alloc2DMem(h_new, w_new);
		FLOAT** temp_g = Alloc2DMem(h_new, w_new);
		FLOAT** temp_b = Alloc2DMem(h_new, w_new);

		for (i = 0; i < h_new; i++)
		{
			for (j = 0; j < w_new; j++)
			{
				temp_r[i][j] = ptr[i / scale][j / scale].r;
				temp_g[i][j] = ptr[i / scale][j / scale].g;
				temp_b[i][j] = ptr[i / scale][j / scale].b;
			}
		}

		// 원래 이미지와 결과 이미지의 해상도가 다르기 때문에 새로 선언
		dib.CreateRGBImage(w_new, h_new);
		ptr = dib.GetRGBPtr();

		for (i = 0; i < h_new; i++)
		{
			for (j = 0; j < w_new; j++)
			{
				ptr[i][j].r = temp_r[i][j];
				ptr[i][j].g = temp_g[i][j];
				ptr[i][j].b = temp_b[i][j];
			}
		}
		delete[] temp_r;
		delete[] temp_g;
		delete[] temp_b;
	}

}

void CSMWColorDoc::OnScalingBilinear(CDib& dib)
{
	// TODO: 여기에 구현 코드 추가.
	register int i, j;
	int w = dib.GetWidth();
	int h = dib.GetWidth();
	int h_new, w_new, scale;

	CScalingDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		scale = dlg.m_EditScale;

		h_new = int(scale * h);
		w_new = int(scale * h);
	}
	if (dib.GetBitCount() == 24)
	{
		RGBBYTE** ptr = dib.GetRGBPtr();
		FLOAT** temp_r = Alloc2DMem(h_new, w_new);
		FLOAT** temp_g = Alloc2DMem(h_new, w_new);
		FLOAT** temp_b = Alloc2DMem(h_new, w_new);

		for (i = 0; i < h_new; i++)
		{
			for (j = 0; j < w_new; j++)
			{
				temp_r[i][j] = ptr[i / scale][j / scale].r;
				temp_g[i][j] = ptr[i / scale][j / scale].g;
				temp_b[i][j] = ptr[i / scale][j / scale].b;
			}
		}

		// 원래 이미지와 결과 이미지의 해상도가 다르기 때문에 새로 선언
		dib.CreateRGBImage(w_new, h_new);
		ptr = dib.GetRGBPtr();

		for (i = 0; i < h_new; i++)
		{
			for (j = 0; j < w_new; j++)
			{
				ptr[i][j].r = temp_r[i][j];
				ptr[i][j].g = temp_g[i][j];
				ptr[i][j].b = temp_b[i][j];
			}
		}
		delete[] temp_r;
		delete[] temp_g;
		delete[] temp_b;
	}
}

void CSMWColorDoc::OnScalingAliasing(CDib& dib)
{
	// TODO: 여기에 구현 코드 추가.
	register int i, j;
	int w = dib.GetWidth();
	int h = dib.GetHeight();
	int h_new, w_new, scale;

	CScalingDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		scale = dlg.m_EditScale;

		h_new = h / scale;
		w_new = w / scale;
	}
	if (dib.GetBitCount() == 24)
	{
		RGBBYTE** ptr = dib.GetRGBPtr();

		FLOAT** temp_r = Alloc2DMem(h_new, w_new);
		FLOAT** temp_g = Alloc2DMem(h_new, w_new);
		FLOAT** temp_b = Alloc2DMem(h_new, w_new);

		AverageFiltering(dib, scale);
		
		for (i = 0; i < h_new; i++)
		{
			for (j = 0; j < w_new; j++)
			{
				temp_r[i][j] = ptr[i * scale][j * scale].r;
				temp_g[i][j] = ptr[i * scale][j * scale].g;
				temp_b[i][j] = ptr[i * scale][j * scale].b;
			}
		}
		dib.CreateRGBImage(w_new, h_new);
		ptr = dib.GetRGBPtr();
		for (i = 0; i < h_new; i++)
		{
			for (j = 0; j < w_new; j++)
			{
				ptr[i][j].r = temp_r[i][j];
				ptr[i][j].g = temp_g[i][j];
				ptr[i][j].b = temp_b[i][j];
			}
		}

	}
}

void CSMWColorDoc::OnSecondderivativeLog(CDib& dib)
{
	// TODO: 여기에 구현 코드 추가.

}

void CSMWColorDoc::OnMorphologicalprocessingErosion(CDib& dib)
{
	// TODO: 여기에 구현 코드 추가.
	register int i, j, m, n;
	int h = dib.GetHeight();
	int w = dib.GetWidth();
	int ksize = 3;
	int margin = ksize / 2;

	FLOAT** mask = Alloc2DMem(ksize, ksize);
	for (i = 0; i < ksize; i++)
	{
		for (j = 0; j < ksize; j++)
		{
			mask[i][j] = 255.;
		}
	}

	FLOAT** buf = Alloc2DMem(h + ksize * 2, w + ksize * 2);

	FLOAT** Y  = Alloc2DMem(h, w);
	FLOAT** Cr = Alloc2DMem(h, w);
	FLOAT** Cb = Alloc2DMem(h, w);
	RGB2YCrCb(dib, Y, Cr, Cb);
	RGBBYTE** ptr = dib.GetRGBPtr();

	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			buf[i + margin][j + margin] = Y[i][j] * 255.;
		}
	}

	// 영상 이진화
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			FLOAT val = buf[i + margin][j + margin];
			if (val < 128) { buf[i + margin][j + margin] = 0.; }
			else { buf[i + margin][j + margin] = 255.; }
		}
	}

	int sum;
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			sum = 0.;
			for (m = 0; m < ksize; m++)
			{
				for (n = 0; n < ksize; n++)
				{
					if (mask[m][n] == buf[i + m][j + n]) {
						sum += 1;
					}
				}
			}
			if (sum == ksize * ksize)
			{
				ptr[i][j].r = 255.;
				ptr[i][j].g = 255.;
				ptr[i][j].b = 255.;
			}
			else {
				ptr[i][j].r = 0.;
				ptr[i][j].g = 0.;
				ptr[i][j].b = 0.;
			}
		}
	}
	delete[] buf;
}

void CSMWColorDoc::OnMorphologicalprocessingDilation(CDib& dib)
{
	// TODO: 여기에 구현 코드 추가.
	register int i, j, m, n;
	int h = dib.GetHeight();
	int w = dib.GetWidth();
	int ksize = 3;
	int margin = ksize / 2;

	FLOAT** mask = Alloc2DMem(ksize, ksize);

	FLOAT** buf = Alloc2DMem(h + ksize * 2, w + ksize * 2);

	FLOAT** Y = Alloc2DMem(h, w);
	FLOAT** Cr = Alloc2DMem(h, w);
	FLOAT** Cb = Alloc2DMem(h, w);
	RGB2YCrCb(dib, Y, Cr, Cb);
	RGBBYTE** ptr = dib.GetRGBPtr();

	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			buf[i + margin][j + margin] = Y[i][j] * 255.;
		}
	}

	// 영상 이진화
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			FLOAT val = buf[i + margin][j + margin];
			if (val < 128) { buf[i + margin][j + margin] = 0.; }
			else { buf[i + margin][j + margin] = 255.; }
		}
	}

	int sum;
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			sum = 0.;
			for (m = 0; m < ksize; m++)
			{
				for (n = 0; n < ksize; n++)
				{
					if (mask[m][n] == buf[i + m][j + n]) {
						sum += 1;
					}
				}
			}
			if (sum == ksize * ksize)
			{
				ptr[i][j].r = 0.;
				ptr[i][j].g = 0.;
				ptr[i][j].b = 0.;
			}
			else {
				ptr[i][j].r = 255.;
				ptr[i][j].g = 255.;
				ptr[i][j].b = 255.;
			}
		}
		delete[] Y;
		delete[] Cr;
		delete[] Cb;
	}
	delete[] mask;
	delete[] buf;
}

void CSMWColorDoc::OnMorphologicalprocessingLogicaloperation(CDib& dib)

{
	// TODO: 여기에 구현 코드 추가.
	register int i, j, ch;
	int h = dib.GetHeight();
	int w = dib.GetWidth();
	RGBBYTE** ptr = dib.GetRGBPtr();
	FLOAT** temp_r = Alloc2DMem(h, w);
	FLOAT** temp_g = Alloc2DMem(h, w);
	FLOAT** temp_b = Alloc2DMem(h, w);
	// 컬러공간 변경
	//RGB2YCrCb(dib, Y, Cr, Cb);


	FLOAT* mTemp = NULL;
	CDib TEMP;
	CFileDialog OpenDlg(TRUE);
	if (OpenDlg.DoModal() == IDOK)
	{
		TEMP.Load(OpenDlg.GetPathName());
	}
	BYTE** ptr_targ = TEMP.GetPtr();

	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			/*
			좀 더 간단하게
			temp_r[i][j] = ptr[i][j].r * (ptr_targ[i][j] / 255);
			temp_g[i][j] = ptr[i][j].g * (ptr_targ[i][j] / 255);
			temp_b[i][j] = ptr[i][j].b * (ptr_targ[i][j] / 255);
			*/
			if (ptr[i][j].r && (ptr_targ[i][j])) { temp_r[i][j] = ptr[i][j].r; }
			else { temp_r[i][j] = 0; }

			if (ptr[i][j].g && (ptr_targ[i][j])) { temp_g[i][j] = ptr[i][j].g; }
			else { temp_g[i][j] = 0; }

			if (ptr[i][j].b && (ptr_targ[i][j])) { temp_b[i][j] = ptr[i][j].b; }
			else { temp_b[i][j] = 0; }
		}
	}

	CDib TEMP2= m_Dib;
	CFileDialog OpenDlg2(TRUE);
	if (OpenDlg2.DoModal() == IDOK)
	{
		TEMP2.Load(OpenDlg2.GetPathName());
	}
	RGBBYTE** ptr_targ2 = TEMP2.GetRGBPtr();
	FLOAT** temp2[3];
	for (ch = 0; ch < 3; ch++)
	{
		temp2[ch] = Alloc2DMem(h, w);
	}
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			if (temp_r[i][j] && (ptr_targ2[i][j].r) || ptr_targ[i][j]) { temp2[0][i][j] = temp_r[i][j]; }
			else { temp2[0][i][j] = ptr_targ2[i][j].r; }

			if (temp_g[i][j] && (ptr_targ2[i][j].g) || ptr_targ[i][j]) { temp2[1][i][j] = temp_g[i][j]; }
			else { temp2[1][i][j] = ptr_targ2[i][j].g; }

			if (temp_b[i][j] && (ptr_targ2[i][j].b) || ptr_targ[i][j]) { temp2[2][i][j] = temp_b[i][j]; }
			else { temp2[2][i][j] = ptr_targ2[i][j].b; }
		}
	}

	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			ptr[i][j].r = temp2[0][i][j];
			ptr[i][j].g = temp2[1][i][j];
			ptr[i][j].b = temp2[2][i][j];
		}
	}
	delete[] temp_r;
	delete[] temp_g;
	delete[] temp_b;
	for (ch = 0; ch < 3; ch++)
	{
		delete[] temp2[ch];
	}

}
