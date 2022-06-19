
// SMWDoc.h : interface of the CSMWDoc class
//


#pragma once


class CSMWDoc : public CDocument
{
protected: // create from serialization only
	CSMWDoc() noexcept;
	DECLARE_DYNCREATE(CSMWDoc)

// Attributes
public:
	int m_width;
	int m_height;
	int m_size;

	int m_Re_width;
	int m_Re_height;
	int m_Re_size;

	unsigned char* m_InputImage;
	unsigned char* m_OutputImage;

	// 비트플레인 이미지들
	unsigned char* m_BitPlane0;
	unsigned char* m_BitPlane1;
	unsigned char* m_BitPlane2;
	unsigned char* m_BitPlane3;
	unsigned char* m_BitPlane4;
	unsigned char* m_BitPlane5;
	unsigned char* m_BitPlane6;
	unsigned char* m_BitPlane7;

	// 히스토그램 변수
	int m_HIST[256];
	unsigned char m_Scale_HIST[256];
	int m_input_hist_size;
	int m_output_hist_size;
	unsigned char* m_InputHistogram;
	unsigned char* m_OutputHistogram;
	double m_Sum_Of_HIST[256];

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CSMWDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	// 픽셀 값에 상수를 더하는 함수
	void OnImageprocessingAddoperation();
	// 픽셀 값에서 상수를 빼는 함수
	void OnImageprocessingSuboperation();
	// 이미지 반전
	void OnImageprocessingInverseoperation();
	// 이진화
	void OnImageprocessingBinarization();
	// 감마보정함수
	void OnImageprocessingGammacorrection();
	// 비트플레인 분리
	void OnImageprocessingBitplane();
	// 모든 비트플레인을 시각화
	void OnImageprocessingBitplaneall();
private:
	// 비트플레인 분리
	unsigned char* SplitBitPlane(unsigned char mask, int position);
public:
	// 히스토그램 그리기
	void OnImageprocessingHistogram();
	// 히스토그램 함수
	unsigned char* OnHistogram(bool is_input);
	// 히스토그램 스트래칭
	void OnHistogramHistogramstretching();
	// 엔드 인 탐색 함수
	void OnHistogramEndinsearch();
	// 히스토그램 이퀄라이제이션
	void OnHistogramHistogramequalization();
	// 히스토그램 매칭
	void OnHistogramHistogrammatching();
	void OnContrastStretching();
	void OnDissolve();
};
