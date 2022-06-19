// CDlgNumber.cpp : implementation file
//

#include "pch.h"
#include "SMW.h"
#include "afxdialogex.h"
#include "CDlgNumber.h"


// CDlgNumber dialog

IMPLEMENT_DYNAMIC(CDlgNumber, CDialogEx)

CDlgNumber::CDlgNumber(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_InputNumber(0)
	, m_InputNumber2(0)
	, m_InputNumber3(0)
	, m_InputNumber4(0)
{

}

CDlgNumber::~CDlgNumber()
{
}

void CDlgNumber::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_InputNumber);
	DDX_Text(pDX, IDC_EDIT2, m_InputNumber2);
	// DDX_Text(pDX, IDC_EDIT3, m_InputNumber3);
	//  DDX_Text(pDX, IDC_EDIT4, m_InputNumber3);
	DDX_Text(pDX, IDC_EDIT3, m_InputNumber3);
	DDX_Text(pDX, IDC_EDIT4, m_InputNumber4);
}


BEGIN_MESSAGE_MAP(CDlgNumber, CDialogEx)
END_MESSAGE_MAP()


// CDlgNumber message handlers
