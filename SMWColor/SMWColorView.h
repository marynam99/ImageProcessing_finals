
// SMWColorView.h : interface of the CSMWColorView class
//

#pragma once


class CSMWColorView : public CScrollView
{
protected: // create from serialization only
	CSMWColorView() noexcept;
	DECLARE_DYNCREATE(CSMWColorView)

// Attributes
public:
	CSMWColorDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CSMWColorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPixelprocessingInverse();
	afx_msg void OnHistogramDraw();
	afx_msg void OnHistogramprocessingHistogramstretching();
	afx_msg void OnColormodelRgb2yuv();
	afx_msg void OnHistogramprocessingHistogramstretchingycbcr();
	afx_msg void OnHistogramprocessingHistogrammatching();
	afx_msg void OnRegionprocessingEmbossing();
	afx_msg void OnRegionprocessingSelectfilter();
	afx_msg void OnDifferenceHorizontal();
	afx_msg void OnDifferenceVertical();
	afx_msg void OnDifferenceHomogeneousoperator();
	afx_msg void OnDifferenceLaplacian();
	afx_msg void OnDifferenceSobel();
	afx_msg void OnGeometrictransformationTranslation();
	afx_msg void OnGeometrictransformationRotation();
	afx_msg void OnScalingNearest();
	afx_msg void OnScalingBilinear();
	afx_msg void OnScalingAliasing();
	afx_msg void OnGeometrictransformationSelecttransf();
	afx_msg void OnDifferenceDifferenceoperator();
	afx_msg void OnSecondderivativeLog();
	afx_msg void OnMorphologicalprocessingLogicaloperation();
	afx_msg void OnMorphologicalprocessingErosion();
	afx_msg void OnMorphologicalprocessingDilation();
};

#ifndef _DEBUG  // debug version in SMWColorView.cpp
inline CSMWColorDoc* CSMWColorView::GetDocument() const
   { return reinterpret_cast<CSMWColorDoc*>(m_pDocument); }
#endif

