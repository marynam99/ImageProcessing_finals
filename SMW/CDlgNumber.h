#pragma once
#include "afxdialogex.h"


// CDlgNumber dialog

class CDlgNumber : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgNumber)

public:
	CDlgNumber(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgNumber();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// ����Ʈ��Ʈ�ѿ��� �Է¹��� ����
	double m_InputNumber;
	double m_InputNumber2;
//	double m_InputNumber3;
//	double m_InputNumber4;
	double m_InputNumber3;
	double m_InputNumber4;
};
