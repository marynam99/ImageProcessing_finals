#pragma once
#include "afxdialogex.h"


// CHistogramDlg dialog

class CHistogramDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistogramDlg)

public:
	CHistogramDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CHistogramDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HISTOGRAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	bool isColor = false;
	int m_Histogram[256];
	int m_HistogramRGB[3][256];

	void SetImage(CDib& dib);
	afx_msg void OnPaint();
	void NormalizeHist(float src[256], int* dest);
};
