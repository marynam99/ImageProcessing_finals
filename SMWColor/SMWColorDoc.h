
// SMWColorDoc.h : interface of the CSMWColorDoc class
//


#pragma once
#include "CDib.h"

class CSMWColorDoc : public CDocument
{
protected: // create from serialization only
	CSMWColorDoc() noexcept;
	DECLARE_DYNCREATE(CSMWColorDoc)

// Attributes
public:
	CDib m_Dib;

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
	virtual ~CSMWColorDoc();
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
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	// 영상 반전
	void OnPixelprocessingInverse(CDib& dib);
	void OnHistogramprocessingHistogramstretching(CDib& dib);
	void OnColormodelRgb2yuv(CDib& dib);
	void OnHistogramprocessingHistogramstretchingycbcr(CDib& dib);
	void OnHistogramprocessingHistogrammatching(CDib& dib);
	void OnRegionprocessingEmbossing(CDib& dib);
	void OnRegionprocessingSelectfilter(CDib& dib);

	void AverageFiltering(CDib& dib, int ksize);
	void GaussianBlurring(CDib& dib, int ksize, FLOAT sigma);
	void SharpeningFiltering(CDib& dib, int ksize);
	void Sharpening2Filtering(CDib& dib, int ksize);
	void Unsharp(CDib& dib, int ksize);
	void HighFrequencyBandPassFilter(CDib& dib, int ksize);
	void HighBoost(CDib& dib, int ksize);
	void LoG(CDib& dib, int ksize, FLOAT sigma);
	void DoG(CDib& dib, int ksize, FLOAT sigma, FLOAT sigma2);
	void OnDifferenceHorizontal(CDib& dib);
	void OnDifferenceVertical(CDib& dib);
	void OnDifferenceHomogeneousoperator(CDib& dib);
	void OnDifferenceLaplacian(CDib& dib);
	void OnDifferenceSobel(CDib& dib);
	void OnGeometrictransformationTranslation(CDib& dib);
	void OnGeometrictransformationRotation(CDib& dib);
	void OnScalingNearest(CDib& dib);
	void OnScalingBilinear(CDib& dib);
	void OnScalingAliasing(CDib& dib);
	void OnGeometrictransformationSelecttransf(CDib& dib);
	void OnDifferenceDifferenceoperator(CDib& dib);
	void OnSecondderivativeLog(CDib& dib);
	void OnMorphologicalprocessingLogicaloperation(CDib& dib);
	void OnMorphologicalprocessingErosion(CDib& dib);
	void OnMorphologicalprocessingDilation(CDib& dib);
};
