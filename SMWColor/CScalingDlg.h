#pragma once
#include "afxdialogex.h"


// CScaling 대화 상자

class CScalingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CScalingDlg)

public:
	CScalingDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CScalingDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	// 몇 배 줄이거나 늘릴 지 설정
	int m_EditScale;
};
