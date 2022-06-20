#pragma once
#include "afxdialogex.h"


// CFilterDlg 대화 상자

class CFilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFilterDlg)

public:
	CFilterDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CFilterDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_CBoxFilter;
	CEdit m_EditKSize;
	CEdit m_EditSigma;
	CSpinButtonCtrl m_SpinKSize;
	CSpinButtonCtrl m_SpinSigma;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombo1();

	int m_FilterType = 0;
	int m_FilterSize = 3;
	int m_FilterSize2 = 3;
	FLOAT m_sigma = 1.;
	FLOAT m_sigma2 = 1.;
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnChangeEdit1();
	CEdit m_EditSigma2;
	CSpinButtonCtrl m_SpinSigma2;
	afx_msg void OnDeltaposSpinSigma2(NMHDR* pNMHDR, LRESULT* pResult);
	CEdit m_EditKSize2;
	CSpinButtonCtrl m_SpinKSize2;
	afx_msg void OnDeltaposSpinKsize2(NMHDR* pNMHDR, LRESULT* pResult);
};