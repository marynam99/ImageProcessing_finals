
// SMWView.h : interface of the CSMWView class
//

#pragma once


class CSMWView : public CView
{
	bool is_bitplaneall = false;

protected: // create from serialization only
	CSMWView() noexcept;
	DECLARE_DYNCREATE(CSMWView)

// Attributes
public:
	CSMWDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CSMWView();
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
	afx_msg void OnImageprocessingAddoperation();
	afx_msg void OnImageprocessingSuboperation();
	afx_msg void OnImageprocessingInverseoperation();
	afx_msg void OnImageprocessingBinarization();
	afx_msg void OnImageprocessingGammacorrection();
	afx_msg void OnImageprocessingBitplane();
	afx_msg void OnImageprocessingBitplaneall();
	afx_msg void OnImageprocessingHistogram();
	afx_msg void OnHistogramHistogramstretching();
	afx_msg void OnHistogramEndinsearch();
	afx_msg void OnHistogramHistogramequalization();
	afx_msg void OnHistogramHistogrammatching();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void On32784();
	afx_msg void On32785();
};

#ifndef _DEBUG  // debug version in SMWView.cpp
inline CSMWDoc* CSMWView::GetDocument() const
   { return reinterpret_cast<CSMWDoc*>(m_pDocument); }
#endif

