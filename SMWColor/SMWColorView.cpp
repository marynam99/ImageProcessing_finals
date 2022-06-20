
// SMWColorView.cpp : implementation of the CSMWColorView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SMWColor.h"
#endif

#include "SMWColorDoc.h"
#include "SMWColorView.h"

#include "CHistogramDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSMWColorView

IMPLEMENT_DYNCREATE(CSMWColorView, CScrollView)

BEGIN_MESSAGE_MAP(CSMWColorView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSMWColorView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_PIXELPROCESSING_INVERSE, &CSMWColorView::OnPixelprocessingInverse)
	ON_COMMAND(ID_HISTOGRAM_DRAW, &CSMWColorView::OnHistogramDraw)
	ON_COMMAND(ID_HISTOGRAMPROCESSING_HISTOGRAMSTRETCHING, &CSMWColorView::OnHistogramprocessingHistogramstretching)
	ON_COMMAND(ID_COLORMODEL_RGB2YUV, &CSMWColorView::OnColormodelRgb2yuv)
	ON_COMMAND(ID_HISTOGRAMPROCESSING_HISTOGRAMSTRETCHINGYCBCR, &CSMWColorView::OnHistogramprocessingHistogramstretchingycbcr)
	ON_COMMAND(ID_HISTOGRAMPROCESSING_HISTOGRAMMATCHING, &CSMWColorView::OnHistogramprocessingHistogrammatching)
	ON_COMMAND(ID_REGIONPROCESSING_EMBOSSING, &CSMWColorView::OnRegionprocessingEmbossing)
	ON_COMMAND(ID_REGIONPROCESSING_SELECTFILTER, &CSMWColorView::OnRegionprocessingSelectfilter)
	ON_COMMAND(ID_DIFFERENCE_HORIZONTAL, &CSMWColorView::OnDifferenceHorizontal)
	ON_COMMAND(ID_DIFFERENCE_VERTICAL, &CSMWColorView::OnDifferenceVertical)
	ON_COMMAND(ID_DIFFERENCE_HOMOGENEOUSOPERATOR, &CSMWColorView::OnDifferenceHomogeneousoperator)
	ON_COMMAND(ID_DIFFERENCE_LAPLACIAN, &CSMWColorView::OnDifferenceLaplacian)
	ON_COMMAND(ID_DIFFERENCE_SOBEL, &CSMWColorView::OnDifferenceSobel)
	ON_COMMAND(ID_GEOMETRICTRANSFORMATION_TRANSLATION, &CSMWColorView::OnGeometrictransformationTranslation)
	ON_COMMAND(ID_GEOMETRICTRANSFORMATION_ROTATION, &CSMWColorView::OnGeometrictransformationRotation)
	ON_COMMAND(ID_SCALING_NEAREST, &CSMWColorView::OnScalingNearest)
	ON_COMMAND(ID_SCALING_BILINEAR, &CSMWColorView::OnScalingBilinear)
	ON_COMMAND(ID_SCALING_ALIASING, &CSMWColorView::OnScalingAliasing)
	ON_COMMAND(ID_GEOMETRICTRANSFORMATION_SELECTTRANSF, &CSMWColorView::OnGeometrictransformationSelecttransf)
	ON_COMMAND(ID_DIFFERENCE_DIFFERENCEOPERATOR, &CSMWColorView::OnDifferenceDifferenceoperator)
	ON_COMMAND(ID_SECONDDERIVATIVE_LOG, &CSMWColorView::OnSecondderivativeLog)
	ON_COMMAND(ID_MORPHOLOGICALPROCESSING_LOGICALOPERATION, &CSMWColorView::OnMorphologicalprocessingLogicaloperation)
	ON_COMMAND(ID_MORPHOLOGICALPROCESSING_EROSION, &CSMWColorView::OnMorphologicalprocessingErosion)
	ON_COMMAND(ID_MORPHOLOGICALPROCESSING_DILATION, &CSMWColorView::OnMorphologicalprocessingDilation)
	ON_COMMAND(ID_GEOMETRICTRANSFORMATION_ROTATION2, &CSMWColorView::OnGeometrictransformationRotation2)
END_MESSAGE_MAP()

// CSMWColorView construction/destruction

CSMWColorView::CSMWColorView() noexcept
{
	// TODO: add construction code here

}

CSMWColorView::~CSMWColorView()
{
}

BOOL CSMWColorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

// CSMWColorView drawing

void CSMWColorView::OnDraw(CDC* pDC)
{
	CSMWColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	if (pDoc->m_Dib.IsValid())
	{
		pDoc->m_Dib.Draw(pDC->m_hDC);
	}
}

void CSMWColorView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	CSMWColorDoc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid())
	{
		sizeTotal.cx = pDoc->m_Dib.GetWidth();
		sizeTotal.cy = pDoc->m_Dib.GetHeight();
	}
	else
	{
		sizeTotal.cx = sizeTotal.cy = 100;
	}
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit(TRUE);
}


// CSMWColorView printing


void CSMWColorView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CSMWColorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSMWColorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSMWColorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CSMWColorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSMWColorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSMWColorView diagnostics

#ifdef _DEBUG
void CSMWColorView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CSMWColorView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CSMWColorDoc* CSMWColorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSMWColorDoc)));
	return (CSMWColorDoc*)m_pDocument;
}
#endif //_DEBUG


// CSMWColorView message handlers


void CSMWColorView::OnPixelprocessingInverse()
{
	// TODO: Add your command handler code here
	CSMWColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDib dib = pDoc->m_Dib;
	pDoc->OnPixelprocessingInverse(dib);
	AfxNewImage(dib);
}


void CSMWColorView::OnHistogramDraw()
{
	// TODO: Add your command handler code here
	CSMWColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CHistogramDlg dlg;

	dlg.SetImage(pDoc->m_Dib);
	dlg.DoModal();
}


void CSMWColorView::OnHistogramprocessingHistogramstretching()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSMWColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDib dib = pDoc->m_Dib;
	pDoc->OnHistogramprocessingHistogramstretching(dib);
	AfxNewImage(dib);
}


void CSMWColorView::OnColormodelRgb2yuv()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSMWColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDib dib = pDoc->m_Dib;
	pDoc->OnColormodelRgb2yuv(dib);
	AfxNewImage(dib);
}


void CSMWColorView::OnHistogramprocessingHistogramstretchingycbcr()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSMWColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDib dib = pDoc->m_Dib;
	pDoc->OnHistogramprocessingHistogramstretchingycbcr(dib);
	AfxNewImage(dib);
}


void CSMWColorView::OnHistogramprocessingHistogrammatching()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSMWColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDib dib = pDoc->m_Dib;
	pDoc->OnHistogramprocessingHistogrammatching(dib);
	AfxNewImage(dib);
}


void CSMWColorView::OnRegionprocessingEmbossing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSMWColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDib dib = pDoc->m_Dib;
	pDoc->OnRegionprocessingEmbossing(dib);
	AfxNewImage(dib);
}


void CSMWColorView::OnRegionprocessingSelectfilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSMWColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDib dib = pDoc->m_Dib;
	pDoc->OnRegionprocessingSelectfilter(dib);
	AfxNewImage(dib);
}

void CSMWColorView::OnDifferenceHorizontal()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSMWColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDib dib = pDoc->m_Dib;
	pDoc->OnDifferenceHorizontal(dib);
	AfxNewImage(dib);
}


void CSMWColorView::OnDifferenceVertical()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSMWColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDib dib = pDoc->m_Dib;
	pDoc->OnDifferenceVertical(dib);
	AfxNewImage(dib);
}


void CSMWColorView::OnDifferenceHomogeneousoperator()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSMWColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDib dib = pDoc->m_Dib;
	pDoc->OnDifferenceHomogeneousoperator(dib);
	AfxNewImage(dib);
}


void CSMWColorView::OnDifferenceLaplacian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSMWColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDib dib = pDoc->m_Dib;
	pDoc->OnDifferenceLaplacian(dib);
	AfxNewImage(dib);
}


void CSMWColorView::OnDifferenceSobel()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSMWColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDib dib = pDoc->m_Dib;
	pDoc->OnDifferenceSobel(dib);
	AfxNewImage(dib);
}


void CSMWColorView::OnGeometrictransformationTranslation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSMWColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDib dib = pDoc->m_Dib;
	pDoc->OnGeometrictransformationTranslation(dib);
	AfxNewImage(dib);
}


void CSMWColorView::OnGeometrictransformationRotation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSMWColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDib dib = pDoc->m_Dib;
	pDoc->OnGeometrictransformationRotation(dib);
	AfxNewImage(dib);
}


void CSMWColorView::OnScalingNearest()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSMWColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDib dib = pDoc->m_Dib;
	pDoc->OnScalingNearest(dib);
	AfxNewImage(dib);
}


void CSMWColorView::OnScalingBilinear()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSMWColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDib dib = pDoc->m_Dib;
	pDoc->OnScalingBilinear(dib);
	AfxNewImage(dib);
}


void CSMWColorView::OnScalingAliasing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSMWColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDib dib = pDoc->m_Dib;
	pDoc->OnScalingAliasing(dib);
	AfxNewImage(dib);
}


void CSMWColorView::OnGeometrictransformationSelecttransf()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSMWColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDib dib = pDoc->m_Dib;
	pDoc->OnGeometrictransformationSelecttransf(dib);
	AfxNewImage(dib);
}


void CSMWColorView::OnDifferenceDifferenceoperator()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSMWColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDib dib = pDoc->m_Dib;
	pDoc->OnDifferenceDifferenceoperator(dib);
	AfxNewImage(dib);
}


void CSMWColorView::OnSecondderivativeLog()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSMWColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDib dib = pDoc->m_Dib;
	pDoc->OnSecondderivativeLog(dib);
	AfxNewImage(dib);
}

void CSMWColorView::OnMorphologicalprocessingErosion()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSMWColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDib dib = pDoc->m_Dib;
	pDoc->OnMorphologicalprocessingErosion(dib);
	AfxNewImage(dib);
}


void CSMWColorView::OnMorphologicalprocessingDilation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSMWColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDib dib = pDoc->m_Dib;
	pDoc->OnMorphologicalprocessingDilation(dib);
	AfxNewImage(dib);
}

void CSMWColorView::OnMorphologicalprocessingLogicaloperation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSMWColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDib dib = pDoc->m_Dib;
	pDoc->OnMorphologicalprocessingLogicaloperation(dib);
	AfxNewImage(dib);
}



void CSMWColorView::OnGeometrictransformationRotation2()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSMWColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDib dib = pDoc->m_Dib;
	pDoc->OnGeometrictransformationRotation2(dib);
	AfxNewImage(dib);
}
