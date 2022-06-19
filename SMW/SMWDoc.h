
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

	// ��Ʈ�÷��� �̹�����
	unsigned char* m_BitPlane0;
	unsigned char* m_BitPlane1;
	unsigned char* m_BitPlane2;
	unsigned char* m_BitPlane3;
	unsigned char* m_BitPlane4;
	unsigned char* m_BitPlane5;
	unsigned char* m_BitPlane6;
	unsigned char* m_BitPlane7;

	// ������׷� ����
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
	// �ȼ� ���� ����� ���ϴ� �Լ�
	void OnImageprocessingAddoperation();
	// �ȼ� ������ ����� ���� �Լ�
	void OnImageprocessingSuboperation();
	// �̹��� ����
	void OnImageprocessingInverseoperation();
	// ����ȭ
	void OnImageprocessingBinarization();
	// ���������Լ�
	void OnImageprocessingGammacorrection();
	// ��Ʈ�÷��� �и�
	void OnImageprocessingBitplane();
	// ��� ��Ʈ�÷����� �ð�ȭ
	void OnImageprocessingBitplaneall();
private:
	// ��Ʈ�÷��� �и�
	unsigned char* SplitBitPlane(unsigned char mask, int position);
public:
	// ������׷� �׸���
	void OnImageprocessingHistogram();
	// ������׷� �Լ�
	unsigned char* OnHistogram(bool is_input);
	// ������׷� ��Ʈ��Ī
	void OnHistogramHistogramstretching();
	// ���� �� Ž�� �Լ�
	void OnHistogramEndinsearch();
	// ������׷� �����������̼�
	void OnHistogramHistogramequalization();
	// ������׷� ��Ī
	void OnHistogramHistogrammatching();
	void OnContrastStretching();
	void OnDissolve();
};
