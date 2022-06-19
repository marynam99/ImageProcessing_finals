
// SMWDoc.cpp : implementation of the CSMWDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SMW.h"
#include "CDlgNumber.h"
#endif

#include "SMWDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSMWDoc

IMPLEMENT_DYNCREATE(CSMWDoc, CDocument)

BEGIN_MESSAGE_MAP(CSMWDoc, CDocument)
END_MESSAGE_MAP()


// CSMWDoc construction/destruction

CSMWDoc::CSMWDoc() noexcept
{
	// TODO: add one-time construction code here

}

CSMWDoc::~CSMWDoc()
{
}

BOOL CSMWDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CSMWDoc serialization

void CSMWDoc::Serialize(CArchive& ar)
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
void CSMWDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CSMWDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CSMWDoc::SetSearchContent(const CString& value)
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

// CSMWDoc diagnostics

#ifdef _DEBUG
void CSMWDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSMWDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSMWDoc commands


BOOL CSMWDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  Add your specialized creation code here
	CFile File; // 파일 객체 선언

	File.Open(lpszPathName, CFile::modeRead | CFile::typeBinary);

	if (File.GetLength() == 32 * 32) // 이미지 크기 = 32 * 32
	{
		this->m_height = 32;
		m_width = 32;
	}
	else if (File.GetLength() == 64 * 64)
	{
		this->m_height = 64;
		m_width = 64;
	}
	else if (File.GetLength() == 128 * 128)
	{
		this->m_height = 128;
		m_width = 128;
	}
	else if (File.GetLength() == 256 * 256)
	{
		this->m_height = 256;
		m_width = 256;
	}
	else if (File.GetLength() == 512 * 512)
	{
		this->m_height = 512;
		m_width = 512;
	}
	else if (File.GetLength() == 640 * 480)
	{
		this->m_height = 480;
		m_width = 640;
	}
	else if (File.GetLength() == 176 * 144)
	{
		this->m_height = 144;
		m_width = 176;
	}
	else if (File.GetLength() == 176 * 216)
	{
		this->m_height = 216;
		m_width = 176;
	}
	else
	{
		AfxMessageBox(L"지원되지 않는 이미지 크기입니다.",
			MB_OK | MB_ICONEXCLAMATION);
		// 정의되지 않은 이미지 크기는 에러 처리
		return 0;
	}

	m_size = m_width * m_height; // raw 영상의 크기
	m_InputImage = new unsigned char[m_size]; // 메모리 할당
	for (int i = 0; i < m_size; i++) // 메모리를 초기화
	{
		m_InputImage[i] = 255;
	}
	File.Read(m_InputImage, m_size); // 이미지 파일을 읽어서 변수에 저장
	File.Close(); // 파일 닫기

	m_InputHistogram = this->OnHistogram(true);

	return TRUE;
}


// 픽셀 값에 상수를 더하는 함수
void CSMWDoc::OnImageprocessingAddoperation()
{
	// TODO: Add your implementation code here.
	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;
	m_OutputImage = new unsigned char[m_Re_size];

	CDlgNumber dlg;
	if (dlg.DoModal() == IDOK)
	{
		int const_num = (int)dlg.m_InputNumber;
		for (int i = 0; i < m_Re_size; i++)
		{
			int temp = m_InputImage[i] + const_num;
			if (temp > 255)
			{
				m_OutputImage[i] = (temp % 255) - 1;
			}
			else
			{
				m_OutputImage[i] = temp;
			}
		}
	}
	m_OutputHistogram = this->OnHistogram(false);
}


// 픽셀 값에서 상수를 빼는 함수
void CSMWDoc::OnImageprocessingSuboperation()
{
	// TODO: Add your implementation code here.
	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;
	m_OutputImage = new unsigned char[m_Re_size];

	CDlgNumber dlg;
	if (dlg.DoModal() == IDOK)
	{
		int const_num = (int)dlg.m_InputNumber;
		for (int i = 0; i < m_Re_size; i++)
		{
			int temp = m_InputImage[i] - const_num;
			if (temp < 0)
			{
				m_OutputImage[i] = 0;
			}
			else
			{
				m_OutputImage[i] = temp;
			}
		}
	}
	m_OutputHistogram = this->OnHistogram(false);
}


// 이미지 반전
void CSMWDoc::OnImageprocessingInverseoperation()
{
	// TODO: Add your implementation code here.
	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_width * m_height;
	m_OutputImage = new unsigned char[m_Re_size];

	for (int i = 0; i < m_height; i++)
	{
		for (int j = 0; j < m_width; j++)
		{
			m_OutputImage[i * m_height + j] = 
				255 - m_InputImage[i * m_width + j];
		}
	}
	m_OutputHistogram = this->OnHistogram(false);
}


// 이진화
void CSMWDoc::OnImageprocessingBinarization()
{
	// TODO: Add your implementation code here.
	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;
	m_OutputImage = new unsigned char[m_Re_size];

	CDlgNumber dlg;
	if (dlg.DoModal() == IDOK)
	{
		int const_num = (int)dlg.m_InputNumber;
		for (int i = 0; i < m_Re_size; i++)
		{
			int temp = m_InputImage[i];
			if (temp < const_num)
			{
				m_OutputImage[i] = 0;
			}
			else
			{
				m_OutputImage[i] = 255;
			}
		}
	}
	m_OutputHistogram = this->OnHistogram(false);
}


// 감마보정함수
void CSMWDoc::OnImageprocessingGammacorrection()
{
	// TODO: Add your implementation code here.
	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;
	m_OutputImage = new unsigned char[m_Re_size];

	CDlgNumber dlg;
	if (dlg.DoModal() == IDOK)
	{
		double gamma = dlg.m_InputNumber;
		for (int i = 0; i < m_Re_size; i++)
		{
			int temp = pow(m_InputImage[i], 1./gamma);
			if (temp > 255)
			{
				m_OutputImage[i] = 255;
			}
			else if (temp < 0)
			{
				m_OutputImage[i] = 0;
			}
			else
			{
				m_OutputImage[i] = temp;
			}
		}
	}
	m_OutputHistogram = this->OnHistogram(false);
}


// 비트플레인 분리
void CSMWDoc::OnImageprocessingBitplane()
{
	// TODO: Add your implementation code here.
	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;
	m_OutputImage = new unsigned char[m_Re_size];

	unsigned char mask = 0x01;

	CDlgNumber dlg;
	if (dlg.DoModal() == IDOK)
	{
		int position = (int)dlg.m_InputNumber;
		mask <<= position;
		for (int i = 0; i < m_Re_size; i++)
		{
			unsigned char pel = m_InputImage[i];
			if ((pel & mask) == pow(2, position))
			{
				m_OutputImage[i] = 255;
			}
			else
			{
				m_OutputImage[i] = 0;
			}
		}
	}
}


// 모든 비트플레인을 시각화
void CSMWDoc::OnImageprocessingBitplaneall()
{
	// TODO: Add your implementation code here.
	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	unsigned char mask = 0x01;
	int position = 0;

	m_BitPlane0 = SplitBitPlane(mask, position);

	mask <<= 1;
	position += 1;
	m_BitPlane1 = SplitBitPlane(mask, position);

	mask <<= 1;
	position += 1;
	m_BitPlane2 = SplitBitPlane(mask, position);

	mask <<= 1;
	position += 1;
	m_BitPlane3 = SplitBitPlane(mask, position);

	mask <<= 1;
	position += 1;
	m_BitPlane4 = SplitBitPlane(mask, position);

	mask <<= 1;
	position += 1;
	m_BitPlane5 = SplitBitPlane(mask, position);

	mask <<= 1;
	position += 1;
	m_BitPlane6 = SplitBitPlane(mask, position);

	mask <<= 1;
	position += 1;
	m_BitPlane7 = SplitBitPlane(mask, position);
}


// 비트플레인 분리
unsigned char* CSMWDoc::SplitBitPlane(unsigned char mask, int position)
{
	// TODO: Add your implementation code here.
	unsigned char* bitPlane = new unsigned char[m_Re_size];
	for (int i = 0; i < m_Re_size; i++)
	{
		unsigned char pel = m_InputImage[i];
		if ((pel & mask) == pow(2, position))
		{
			bitPlane[i] = 255;
		}
		else
		{
			bitPlane[i] = 0;
		}
	}
	return bitPlane;
}


// 히스토그램 그리기
void CSMWDoc::OnImageprocessingHistogram()
{
	// TODO: Add your implementation code here.
	
}


// 히스토그램 함수
unsigned char* CSMWDoc::OnHistogram(bool is_input)
{
	// TODO: Add your implementation code here.
	m_input_hist_size = 256;
	unsigned char* image = m_InputImage;
	unsigned char* result = m_InputHistogram;
	int IMG_size = m_size;

	if (!is_input)
	{
		m_output_hist_size = 256;
		image = m_OutputImage;
		result = m_OutputHistogram;
		IMG_size = m_Re_size;
	}

	unsigned char LOW = 0, HIGH = 255;
	double MAX, MIN, DIF;
	int value;

	// 초기화
	for (int i = 0; i < m_input_hist_size; i++)
	{
		m_HIST[i] = LOW;
	}

	// 빈도 조사
	for (int i = 0; i < IMG_size; i++)
	{
		value = (int)image[i];
		m_HIST[value]++;
	}

	// 정규화
	MAX = m_HIST[0];
	MIN = m_HIST[0];

	for (int i = 0; i < m_input_hist_size; i++)
	{
		if (MIN > m_HIST[i])
		{
			MIN = m_HIST[i];
		}
		if (MAX < m_HIST[i])
		{
			MAX = m_HIST[i];
		}
	}

	DIF = MAX - MIN;

	// 정규화
	for (int i = 0; i < m_input_hist_size; i++)
	{
		m_Scale_HIST[i] = (unsigned char)((m_HIST[i] - MIN) * HIGH / DIF);
	}

	// 출력
	int HIST_IMG_size = (m_input_hist_size * m_input_hist_size) +
		(m_input_hist_size * 20);

	result = new unsigned char[HIST_IMG_size];

	//하얀색 이미지로 초기화
	for (int i = 0; i < HIST_IMG_size; i++)
	{
		result[i] = 255;
	}
	// 히스토그램 부분은 검은색으로 표시
	for (int i = 0; i < m_input_hist_size; i++)
	{
		for (int j = 0; j < m_Scale_HIST[i]; j++)
		{
			result[m_input_hist_size * (m_input_hist_size - j - 1) + i] = 0;
		}
	}

	// 히스토그램 하단에 픽셀 범위를 표시
	for (int i = m_input_hist_size + 5; i < m_input_hist_size + 20; i++)
	{
		for (int j = 0; j < m_input_hist_size; j++)
		{
			result[m_input_hist_size * i + j] = j;
		}
	}
	return result;
}


// 히스토그램 스트래칭
void CSMWDoc::OnHistogramHistogramstretching()
{
	// TODO: Add your implementation code here.
	m_Re_width = m_width;
	m_Re_height = m_height;
	m_Re_size = m_Re_width * m_Re_height;

	unsigned char LOW = 0, HIGH = 255;
	unsigned char MIN = m_InputImage[0];
	unsigned char MAX = m_InputImage[0];

	// 최대, 최소 픽셀 값 탐색
	for (int i = 0; i < m_size; i++)
	{
		if (MIN > m_InputImage[i])
		{
			MIN = m_InputImage[i];
		}
		if (MAX < m_InputImage[i])
		{
			MAX = m_InputImage[i];
		}
	}

	// 출력 버퍼 할당
	m_OutputImage = new unsigned char[m_Re_size];

	// 스트래칭
	for (int i = 0; i < m_size; i++)
	{
		m_OutputImage[i] = (unsigned char)((m_InputImage[i] - MIN) * HIGH / (MAX - MIN));
	}

	m_OutputHistogram = this->OnHistogram(false);
}


// 엔드 인 탐색 함수
void CSMWDoc::OnHistogramEndinsearch()
{
	// 결과 이미지 크기
	m_Re_width = m_width;
	m_Re_height = m_height;
	m_Re_size = m_Re_width * m_Re_height;

	// 변수
	int i = 0;
	unsigned char LOW = 0, HIGH = 255;
	unsigned char MIN = 0, MAX = 255;

	// 메모리 할당
	m_OutputImage = new unsigned char[m_Re_size];

	CDlgNumber dlg;
	if (dlg.DoModal() == IDOK)
	{
		MIN = (unsigned char)dlg.m_InputNumber;
	}
	if (dlg.DoModal() == IDOK)
	{
		MAX = (unsigned char)dlg.m_InputNumber;
	}

	// 스트래칭
	for (int i = 0; i < m_size; i++)
	{
		unsigned char val = m_InputImage[i];
		if (val >= MAX)
		{
			m_OutputImage[i] = HIGH;
		}
		else if (val <= MIN)
		{
			m_OutputImage[i] = LOW;
		}
		else
		{
			m_OutputImage[i] = (unsigned char)(
				(m_InputImage[i] - MIN) * HIGH / (MAX - MIN));
		}
	}
	m_OutputHistogram = this->OnHistogram(false);
}


// 히스토그램 이퀄라이제이션
void CSMWDoc::OnHistogramHistogramequalization()
{
	m_Re_width = m_width;
	m_Re_height = m_height;
	m_Re_size = m_Re_height * m_Re_width;

	int i, value;
	unsigned char LOW = 0, HIGH = 255, Temp;
	double SUM = 0.0;
	

	// 히스토그램은 이미 있음
	// 누적 히스토그램만 생성
	for (i = 0; i < 256; i++)
	{
		SUM += m_HIST[i];
		m_Sum_Of_HIST[i] = SUM;
	}

	m_OutputImage = new unsigned char[m_Re_size];

	for (i = 0; i < m_size; i++)
	{
		Temp = m_InputImage[i];
		auto val = m_Sum_Of_HIST[Temp] * HIGH / m_size;
		m_OutputImage[i] = (unsigned char)(val);
	}
	m_OutputHistogram = this->OnHistogram(false);
}


// 히스토그램 매칭
void CSMWDoc::OnHistogramHistogrammatching()
{
	// 1. 입력 영상에 히스토그램 계산
	// 2. 누적분포함수 (CDF) 를 구한다
	// 3. 대상 이미지의 히스토그램 계산
	// 4. 대상 이미지의 CDF 계산
	// 5. 대상 이미지의 히스토그램 역변환을 계산
	// 6. 입력 이미지에 대해 평활화 (equalization) 수행
	// 7. 결과 영상의 CDF 에 대한 역변환 값 계산
	// 8. 필요한 경우 clipping 수행

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	int top = 255;
	int bottom = top - 1;

	m_OutputImage = new unsigned char[m_Re_size];
	unsigned char* m_Org_Temp = new unsigned char[m_size];

	// 대상 이미지 로드
	unsigned char* m_DTEMP = NULL;
	CFile File;
	CFileDialog OpenDlg(TRUE);
	if (OpenDlg.DoModal() == IDOK)
	{
		File.Open(OpenDlg.GetPathName(), CFile::modeRead);
		if (File.GetLength() == (unsigned)m_size)
		{
			m_DTEMP = new unsigned char[m_size];
			File.Read(m_DTEMP, m_size);
			File.Close();
		}
		else
		{
			AfxMessageBox(L"Image size not matched");
			return;
		}
	}

	// 초기화
	int i = 0;
	unsigned char LOW = 0, HIGH = 255;
	double m_DHIST[256];
	unsigned char m_TABLE[256];
	for (i = 0; i < 256; i++)
	{
		//m_HIST[i] = LOW;
		m_DHIST[i] = LOW;
		m_TABLE[i] = LOW;
	}

	// 빈도 조사 - 다음에 계속...
	int Dvalue = 0;
	for (i = 0; i < m_size; i++)
	{
		Dvalue = (int)m_DTEMP[i];
		m_DHIST[Dvalue]++;
	}

	// 누적히스토그램 계산
	double SUM = 0.0;
	double DSUM = 0.0;
	double m_Sum_Of_DHIST[256];
	for (i = 0; i < 256; i++)
	{
		SUM += m_HIST[i];
		m_Sum_Of_HIST[i] = SUM;

		DSUM += m_DHIST[i];
		m_Sum_Of_DHIST[i] = DSUM;
	}

	// Equalization
	unsigned char Temp;
	for (i = 0; i < m_size; i++)
	{
		Temp = m_InputImage[i];
		m_Org_Temp[i] = (unsigned char)(m_Sum_Of_HIST[Temp] * HIGH / m_size);
	}

	// 정규화
	double DMIN = m_Sum_Of_DHIST[0];
	double DMAX = m_Sum_Of_DHIST[255];
	unsigned char m_Sum_Of_ScHIST[256];
	for (i = 0; i < 256; i++)
	{
		m_Sum_Of_ScHIST[i] =
			(unsigned char)((m_Sum_Of_DHIST[i] - DMIN) * HIGH / (DMAX - DMIN));
	}
	
	// 룩업테이블 생성
	while (TRUE)
	{
		for (i = m_Sum_Of_ScHIST[bottom]; i < m_Sum_Of_ScHIST[top]; i++)
		{
			m_TABLE[i] = top;
		}
		top = bottom;
		bottom = bottom - 1;
		if (bottom < -1)
		{
			break;
		}
	}

	// 결과이미지 생성
	int DADD = 0;
	for (i = 0; i < m_size; i++)
	{
		DADD = (int)m_Org_Temp[i];
		m_OutputImage[i] = m_TABLE[DADD];
	}
	m_OutputHistogram = this->OnHistogram(false);
}


void CSMWDoc::OnContrastStretching()
{
	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;
	m_OutputImage = new unsigned char[m_Re_size];

	unsigned char r1 = 0;
	unsigned char r2 = 0;

	unsigned char s1 = 0;
	unsigned char s2 = 0;

	CDlgNumber dlg;
	if (dlg.DoModal() == IDOK)
	{
		r1 = (unsigned char)dlg.m_InputNumber;
		r2 = (unsigned char)dlg.m_InputNumber2;
		s1 = (unsigned char)dlg.m_InputNumber3;
		s2 = (unsigned char)dlg.m_InputNumber4;
	}

	int temp = 0;
	for (int i = 0; i < m_Re_size; i++)
	{
		temp = m_InputImage[i];
		if (0 <= temp && temp < r1)
		{
			m_OutputImage[i] = (unsigned char)s1 / r1 * temp;
		}
		else if (r1 <= temp && temp < r2)
		{
			m_OutputImage[i] = (unsigned char)((s2 - s1) / (r2 - r1) * (temp - r1)) + s1;
		}
		else
		{
			m_OutputImage[i] = (unsigned char)((255 - s2) / (255 - r2) * (temp - r2)) + s2;
		}

	}
	m_OutputHistogram = this->OnHistogram(false);
}


void CSMWDoc::OnDissolve()
{
	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;
	m_OutputImage = new unsigned char[m_Re_size];

	double alpha = 0.;

	CDlgNumber dlg;
	if (dlg.DoModal() == IDOK)
	{
		alpha = dlg.m_InputNumber;
	}
	// 대상 이미지 로드
	unsigned char* m_DTEMP = NULL;
	CFile File;
	CFileDialog OpenDlg(TRUE);
	if (OpenDlg.DoModal() == IDOK)
	{
		File.Open(OpenDlg.GetPathName(), CFile::modeRead);
		if (File.GetLength() == (unsigned)m_size)
		{
			m_DTEMP = new unsigned char[m_size];
			File.Read(m_DTEMP, m_size);
			File.Close();
		}
		else
		{
			AfxMessageBox(L"Image size not matched");
			return;
		}
	}


	int temp1 = 0;
	int temp2 = 0;
	for (int i = 0; i < m_Re_size; i++)
	{
		temp1 = m_InputImage[i];
		temp2 = m_DTEMP[i];
		unsigned char temp = (unsigned char)(alpha * temp1 + (1 - alpha) * temp2);
		m_OutputImage[i] = temp;

	}
	m_OutputHistogram = this->OnHistogram(false);
}
