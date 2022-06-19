// CFilterDlg.cpp: 구현 파일
//

#include "pch.h"
#include "SMWColor.h"
#include "afxdialogex.h"
#include "CFilterDlg.h"


// CFilterDlg 대화 상자

IMPLEMENT_DYNAMIC(CFilterDlg, CDialogEx)

CFilterDlg::CFilterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CFilterDlg::~CFilterDlg()
{
}

void CFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_CBoxFilter);
	DDX_Control(pDX, IDC_EDIT1, m_EditKSize);
	DDX_Control(pDX, IDC_SPIN1, m_SpinKSize);
	DDX_Control(pDX, IDC_SPIN4, m_SpinKSize2);
	DDX_Control(pDX, IDC_EDIT2, m_EditSigma);
	DDX_Control(pDX, IDC_EDIT3, m_EditSigma2);
	DDX_Control(pDX, IDC_SPIN2, m_SpinSigma);
	DDX_Control(pDX, IDC_SPIN3, m_SpinSigma2);
}


BEGIN_MESSAGE_MAP(CFilterDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CFilterDlg::OnCbnSelchangeCombo1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CFilterDlg::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CFilterDlg::OnDeltaposSpin2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CFilterDlg::OnDeltaposSpin3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &CFilterDlg::OnDeltaposSpin4)

	ON_EN_CHANGE(IDC_EDIT1, &CFilterDlg::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CFilterDlg::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &CFilterDlg::OnEnChangeEdit3)
END_MESSAGE_MAP()


// CFilterDlg 메시지 처리기


BOOL CFilterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	// combo 박스에 순서대로 들어갈 것 
	m_CBoxFilter.AddString(_T("Blurring (Average Filter)"));
	m_CBoxFilter.AddString(_T("Blurring (Gaussian Smoothing)"));
	m_CBoxFilter.AddString(_T("Blurring (Sharpening)"));
	m_CBoxFilter.AddString(_T("Blurring (Sharpening2)"));
	m_CBoxFilter.AddString(_T("Blurring (Unsharp)"));
	m_CBoxFilter.AddString(_T("Blurring (High Frequency Band Pass Filter)"));
	m_CBoxFilter.AddString(_T("Blurring (HighBoost)"));
	m_CBoxFilter.AddString(_T("Blurring (LoG)"));
	m_CBoxFilter.AddString(_T("Blurring (DoG)"));

	m_CBoxFilter.SetCurSel(0); //디폴트로 리스트 중 첫번쨰

	m_EditKSize.SetWindowTextW(_T("3"));
	m_SpinKSize.SetRange(0, 5);
	m_SpinKSize.SetPos(0);

	m_EditKSize2.SetWindowTextW(_T("3"));
	m_SpinKSize2.SetRange(0, 5);
	m_SpinKSize2.SetPos(0);

	m_EditSigma.SetWindowTextW(_T("1"));
	m_SpinSigma.SetRange(1, 10);
	m_SpinSigma.SetPos(0);

	m_EditSigma2.SetWindowTextW(_T("1"));
	m_SpinSigma2.SetRange(1, 10);
	m_SpinSigma2.SetPos(0);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CFilterDlg::OnCbnSelchangeCombo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_FilterType = m_CBoxFilter.GetCurSel(); // 콤보 박스에 선택된 인덱스값 저장
}


void CFilterDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int ival = pNMUpDown->iPos + pNMUpDown->iDelta;
	ival = ival <= 0 ? 0 : ival;

	m_FilterSize = 3 + ival + 2;

	CString sVal;
	sVal.Format(_T("%d\n"), m_FilterSize);
	m_EditKSize.SetWindowTextW(sVal);


	*pResult = 0;
}


void CFilterDlg::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString sVal;
	sVal.Format(_T("%d\n"), pNMUpDown->iPos);
	m_EditSigma.SetWindowTextW(sVal);

	if (m_FilterType == 1)
	{
		m_sigma = (FLOAT)pNMUpDown->iPos;
	}

	*pResult = 0;
}


void CFilterDlg::OnDeltaposSpin3(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString sVal;
	sVal.Format(_T("%d\n"), pNMUpDown->iPos);
	m_EditSigma2.SetWindowTextW(sVal);

	if (m_FilterType == 9)
	{
		m_sigma2 = (FLOAT)pNMUpDown->iPos;
	}
	*pResult = 0;
}

void CFilterDlg::OnDeltaposSpin4(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int ival = pNMUpDown->iPos + pNMUpDown->iDelta;
	ival = ival <= 0 ? 0 : ival;

	m_FilterSize2 = 3 + ival + 2;

	CString sVal;
	sVal.Format(_T("%d\n"), m_FilterSize);
	m_EditKSize2.SetWindowTextW(sVal);

	*pResult = 0;
}

void CFilterDlg::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CFilterDlg::OnEnChangeEdit2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CFilterDlg::OnEnChangeEdit3()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


