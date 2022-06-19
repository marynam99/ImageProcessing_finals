// CScaling.cpp: 구현 파일
//

#include "pch.h"
#include "SMWColor.h"
#include "afxdialogex.h"
#include "CScalingDlg.h"


// CScaling 대화 상자

IMPLEMENT_DYNAMIC(CScalingDlg, CDialogEx)

CScalingDlg::CScalingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

CScalingDlg::~CScalingDlg()
{
}

void CScalingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_EditScale);
}


BEGIN_MESSAGE_MAP(CScalingDlg, CDialogEx)
END_MESSAGE_MAP()


// CScaling 메시지 처리기
