#include "pch.h"
#include "CDib.h"

#define DIB_HEADER_MARKER ((WORD) ('M' << 8) | 'B')

CDib::CDib()
{
    m_nWidth = 0;
    m_nHeight = 0;
    m_nBitCount = 0;
    m_nDibSize = 0;
    m_pDib = NULL;
    m_pPtr = NULL;
    m_pRgbPtr = NULL;
}

CDib::CDib(const CDib& dib)
{
    m_nWidth = 0;
    m_nHeight = 0;
    m_nBitCount = 0;
    m_nDibSize = 0;
    m_pDib = NULL;
    m_pPtr = NULL;
    m_pRgbPtr = NULL;

    if (dib.m_pDib != NULL)
    {
        m_pDib = new BYTE[dib.m_nDibSize];
        memcpy(m_pDib, dib.m_pDib, dib.m_nDibSize);
    }
    initDib();
}

CDib::~CDib()
{
    Destroy();
}

BOOL CDib::CreateGrayImage(int nWidth, int nHeight, BYTE value)
{
    // 이미 객체가 있다면 삭제한다
    if (m_pDib) { Destroy(); }
    int nBitCount = 8;
    // 실제 영상 데이터의 크기 계산(바이트 단위)
    DWORD dwSizeImage = nHeight * (DWORD)((nWidth * nBitCount / 8 + 3) & ~3);

    // 메모리 공간 할당
    m_pDib = new BYTE[sizeof(BITMAPINFOHEADER) 
        + sizeof(RGBQUAD) * (1 << nBitCount) + dwSizeImage];
    if (m_pDib == NULL) { return FALSE; }

    // 헤더의 필드 값 설정
    LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib;
    lpbmi->biSize = sizeof(BITMAPINFOHEADER);
    lpbmi->biWidth = nWidth;
    lpbmi->biHeight = nHeight;
    lpbmi->biPlanes = 1;
    lpbmi->biBitCount = nBitCount;
    lpbmi->biCompression = BI_RGB;
    lpbmi->biSizeImage = dwSizeImage;
    
    lpbmi->biXPelsPerMeter = 0;
    lpbmi->biYPelsPerMeter = 0;
    lpbmi->biClrUsed = 0;
    lpbmi->biClrImportant = 0;

    // 색상 팔레트를 그레이스케일로 초기화
    RGBQUAD* pPal = (RGBQUAD*)((BYTE*)m_pDib + sizeof(BITMAPINFOHEADER));
    for (int i = 0; i < 256; i++)
    {
        pPal->rgbBlue   = (BYTE)i;
        pPal->rgbGreen  = (BYTE)i;
        pPal->rgbRed    = (BYTE)i;
        pPal->rgbReserved = 0;
        pPal++;
    }

    // 멤버 변수 값 설정
    initDib();

    // 픽셀 데이터 초기화
    BYTE* pData = (BYTE*)GetDibBitsAddr();
    memset(pData, value, dwSizeImage);
    return TRUE;
}

BOOL CDib::CreateRGBImage(int nWidth, int nHeight, COLORREF value)
{
    // 이미 객체가 있다면 삭제한다
    if (m_pDib) { Destroy(); }
    int nBitCount = 24;
    // 실제 영상 데이터의 크기 계산(바이트 단위)
    DWORD dwSizeImage = nHeight * (DWORD)((nWidth * nBitCount / 8 + 3) & ~3);

    // 메모리 공간 할당
    m_pDib = new BYTE[sizeof(BITMAPINFOHEADER) + dwSizeImage];
    if (m_pDib == NULL) { return FALSE; }

    // 헤더의 필드 값 설정
    LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib;
    lpbmi->biSize = sizeof(BITMAPINFOHEADER);
    lpbmi->biWidth = nWidth;
    lpbmi->biHeight = nHeight;
    lpbmi->biPlanes = 1;
    lpbmi->biBitCount = nBitCount;
    lpbmi->biCompression = BI_RGB;
    lpbmi->biSizeImage = dwSizeImage;

    lpbmi->biXPelsPerMeter = 0;
    lpbmi->biYPelsPerMeter = 0;
    lpbmi->biClrUsed = 0;
    lpbmi->biClrImportant = 0;

    // 멤버 변수 값 설정
    initDib();

    // 픽셀 데이터 초기화
    register int i, j;
    for (j = 0; j < m_nHeight; j++)
    {
        for (i = 0; i < m_nWidth; i++)
        {
            // RGBBYTE 클래스의 = 연산자 오버로딩 사용
            m_pRgbPtr[j][i] = value;
        }
    }

    return TRUE;
}

void CDib::Destroy()
{
    if (m_pDib)
    {
        delete[] m_pDib;
        m_pDib = NULL;
    }

    FreePtr();

    m_nWidth = 0;
    m_nHeight = 0;
    m_nBitCount = 0;
    m_nDibSize = 0;
}

BOOL CDib::CopyToClipboard()
{
    return 0;
}

BOOL CDib::PasteFromClipboard()
{
    return 0;
}

BOOL CDib::initDib()
{
    LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib;

    // 이미지의 가로크기, 세로크기, 픽셀 당 비트수, DIB 크기에 대한 멤버변수 설정
    m_nWidth = lpbmi->biWidth;
    m_nHeight = lpbmi->biHeight;
    m_nBitCount = lpbmi->biBitCount;

    DWORD dwSizeImage = m_nHeight *
        (DWORD)((m_nWidth * m_nBitCount / 8 + 3) & ~3);
    m_nDibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * GetPalleteNums()
        + dwSizeImage;

    // m_pPtr 또는 m_pRgbPtr 멤버 변수 설정
    if (!AllocPtr()) { return FALSE; }
    return TRUE;
}

BOOL CDib::AllocPtr()
{
    if (m_nBitCount == 8)
    {
        if (m_pPtr) { FreePtr(); }
        m_pPtr = new BYTE * [m_nHeight];
        if (!m_pPtr) { return FALSE; }

        int nRWidth = ((m_nWidth * m_nBitCount / 8 + 3) & ~3);
        BYTE* pData = (BYTE*)GetDibBitsAddr();
        for (int i = 0; i < m_nHeight; i++)
        {
            m_pPtr[i] = (BYTE*)(pData + (m_nHeight - i - 1) * nRWidth);
        }
    }
    else if (m_nBitCount == 24)
    {
        if (m_pRgbPtr) { FreePtr(); }
        m_pRgbPtr = new RGBBYTE * [m_nHeight];
        if (!m_pRgbPtr) { return FALSE; }

        int nRWidth = ((m_nWidth * m_nBitCount / 8 + 3) & ~3);
        BYTE* pData = (BYTE*)GetDibBitsAddr();
        for (int i = 0; i < m_nHeight; i++)
        {
            m_pRgbPtr[i] = (RGBBYTE*)(pData + (m_nHeight - i - 1) * nRWidth);
        }
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}

void CDib::FreePtr()
{
    if (m_pPtr != NULL)
    {
        delete[] m_pPtr;
        m_pPtr = NULL;
    }
    if (m_pRgbPtr != NULL)
    {
        delete[] m_pRgbPtr;
        m_pRgbPtr = NULL;
    }
}

BOOL CDib::Load(LPCTSTR lpszFileName)
{
    CString filetype;
    filetype = lpszFileName;
    filetype.MakeUpper();

    if (filetype.Right(4) == ".BMP")
    {
        return LoadBMP(lpszFileName);
    }
    else
    {
        return FALSE;
    }
}

BOOL CDib::Save(LPCTSTR lpszFileName)
{
    CString filetype;
    filetype = lpszFileName;
    filetype.MakeUpper();

    if (filetype.Right(4) == ".BMP")
    {
        return SaveBMP(lpszFileName);
    }
    else
    {
        return FALSE;
    }
}

BOOL CDib::LoadBMP(LPCTSTR lpszFileName)
{
    CFile file;
    UINT nFileSize, nDibSize;
    BITMAPFILEHEADER bmfh;
    LPBITMAPINFOHEADER lpbmi;

    // 읽기 모드로 파일을 열기
    if (!file.Open(lpszFileName,
        CFile::modeRead | CFile::shareDenyWrite))
    {
        return FALSE;
    }

    // bmp 파일 크기 계산 (바이트단위)
    nFileSize = (UINT)file.GetLength();

    // 파일헤더 읽기
    if (file.Read((LPSTR) & bmfh, 
        sizeof(BITMAPFILEHEADER)) != 
        sizeof(BITMAPFILEHEADER))
    {
        file.Close();
        return FALSE;
    }

    // bmp 파일임을 나타내는 "BM" 마커가 있는지 확인
    if (bmfh.bfType != DIB_HEADER_MARKER)
    {
        file.Close();
        return FALSE;
    }

    // packed-dib 메모리 블럭 크기 계산
    nDibSize = nFileSize - sizeof(bmfh);

    // 현재 설정된 CDib 객체가 있다면 삭제
    if (m_pDib != NULL) { Destroy(); }

    // 계산된 크기만큼 메모리 할당
    m_pDib = new BYTE[nDibSize];
    if (m_pDib == NULL) 
    {
        file.Close();
        return FALSE;
    }

    // 파일로부터 packed-dib 크기만큼 읽기
    if (file.Read(m_pDib, nDibSize) != nDibSize)
    {
        delete[] m_pDib;
        m_pDib = NULL;
        file.Close();
        return FALSE;
    }

    // m_pDib는 DIB를 담고있는 버퍼의 시작 주소
    // CDib 클래스의 멤버 변수 값 설정
    if (!initDib())
    {
        delete[] m_pDib;
        m_pDib = NULL;
        file.Close();
        return FALSE;
    }

    // 파일 닫기
    file.Close();
    return TRUE;
}

BOOL CDib::SaveBMP(LPCTSTR lpszFileName)
{
    CFile file;
    BITMAPFILEHEADER bmfh;

    // DIB 객체를 포함하고 있지 않으면 FALSE
    if (m_pDib == NULL) { return FALSE; }

    // 쓰기 모드로 파일 열기
    if (!file.Open(lpszFileName,
        CFile::modeCreate |
        CFile::modeWrite |
        CFile::shareDenyWrite))
    {
        return FALSE;
    }

    // BITMAPFILEHEADER 변수 설정
    bmfh.bfType = DIB_HEADER_MARKER; // 항상 "BM"
    bmfh.bfSize = (DWORD)sizeof(BITMAPFILEHEADER) + m_nDibSize;
    bmfh.bfReserved1 = 0;
    bmfh.bfReserved2 = 0;
    bmfh.bfOffBits = (DWORD)(sizeof(BITMAPFILEHEADER) +
        sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) +
        GetPalleteNums());

    file.Write((LPSTR)&bmfh, sizeof(bmfh));

    // 데이터 쓰기
    file.Write(m_pDib, m_nDibSize);

    file.Close();
    return TRUE;
}

int CDib::GetPalleteNums()
{
    switch (m_nBitCount)
    {
    case 1: return 2;
    case 2: return 16;
    case 8: return 256;
    default: return 0;
    }
}

LPVOID CDib::GetDibBitsAddr()
{
    LPBITMAPINFOHEADER lpbmi;
    LPVOID lpDibBits;

    lpbmi = (LPBITMAPINFOHEADER)m_pDib;
    lpDibBits = (LPVOID)((BYTE*)m_pDib + lpbmi->biSize
        + sizeof(RGBQUAD) * GetPalleteNums());

    return lpDibBits;
}

void CDib::Draw(HDC hDC)
{
    Draw(hDC, 0, 0);
}

void CDib::Draw(HDC hDC, int x, int y)
{
    if (m_pDib == NULL) { return; }

    LPBITMAPINFO lpbi = (LPBITMAPINFO)m_pDib;
    void* lpBits = (void*)GetDibBitsAddr();

    ::SetDIBitsToDevice(
        hDC,        // DC 객체
        x, y,       // 그려질 위치 좌표
        m_nWidth,   // 이미지 가로
        m_nHeight,  // 이미지 세로
        0, 0, 0,    // SrcX, SrcY, nStartScan
        m_nHeight,  // nNumScans
        lpBits,
        lpbi,
        DIB_RGB_COLORS);
}

void CDib::Draw(HDC hDC, int dx, int dy, int dw, int dh, DWORD dwRop)
{
    if (m_pDib == NULL) { return; }

    LPBITMAPINFO lpbi = (LPBITMAPINFO)m_pDib;
    LPSTR lpDIBBits = (LPSTR)GetDibBitsAddr();

    ::StretchDIBits(
        hDC,
        dx, dy,
        dw, dh,
        0, 0,
        m_nWidth,
        m_nHeight,
        lpDIBBits,
        lpbi,
        DIB_RGB_COLORS,
        SRCCOPY);
}

void CDib::Draw(HDC hDC, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, DWORD dwRop)
{
    if (m_pDib == NULL) { return; }

    LPBITMAPINFO lpbi = (LPBITMAPINFO)m_pDib;
    LPSTR lpDIBBits = (LPSTR)GetDibBitsAddr();

    ::StretchDIBits(
        hDC,
        dx, dy,
        dw, dh,
        sx, sy,
        sw, sh,
        lpDIBBits,
        lpbi,
        DIB_RGB_COLORS,
        SRCCOPY);
}

CDib& CDib::operator=(const CDib& dib)
{
    // 재귀 검사
    if (this == &dib) { return *this; }

    // dib가 비어있는 CDib 객체라면 자신도 비운다.
    if (dib.m_pDib == NULL)
    {
        Destroy();
        return *this;
    }

    // 현재 설정된 CDib 객체가 있다면 삭제
    if (m_pDib) { Destroy(); }

    // (BITMAPINFOHEADER + 픽셀 데이터) 공간 할당
    m_pDib = new BYTE[dib.m_nDibSize];

    // 내용 복사
    memcpy(m_pDib, dib.m_pDib, dib.m_nDibSize);

    // 멤버 변수 초기화
    initDib();

    return *this;
}

BOOL CDib::Copy(CDib* pDib)
{
    // pDib가 비어있는 CDib 객체를 가리키고 있다면 자신도 비운다.
    if (!pDib->IsValid()) 
    { 
        Destroy(); 
        return TRUE; 
    }

    // 현재 설정된 CDib 객체가 있다면 삭제
    if (m_pDib) { Destroy(); }

    // DIB를 담을 메모리 공간 할당
    DWORD dwDibSize = pDib->GetDibSize();
    m_pDib = new BYTE[dwDibSize];
    if (m_pDib == NULL) { return FALSE; }

    // 복사
    BYTE* pbi = (BYTE*)pDib->GetBitmapInfoAddr();
    memcpy(m_pDib, pbi, dwDibSize);

    // 멤버 변수 초기화
    initDib();
    return TRUE;
}

RGBBYTE& RGBBYTE::operator=(const RGBBYTE& pixel)
{
    // // O: insert return statement here
    if (this == &pixel)
        return *this;
    this->r = pixel.r;
    this->g = pixel.g;
    this->b = pixel.b;
    return *this;
}

RGBBYTE& RGBBYTE::operator=(const COLORREF& pixel)
{
    // // O: insert return statement here
    r = (BYTE)(pixel);
    g = (BYTE)((WORD)(pixel) >> 8);
    b = (BYTE)((pixel) >> 16);
    return *this;
}

RGBBYTE& RGBBYTE::operator=(const BYTE& pixel)
{
    // // O: insert return statement here
    r = pixel;
    g = pixel;
    b = pixel;
    return *this;
}

int RGBBYTE::operator==(const RGBBYTE& pixel)
{
    if (r == pixel.r && g == pixel.g && b == pixel.b)
        return 1;
    else return 0;
}

int RGBBYTE::operator!=(const RGBBYTE& pixel)
{
    if (r != pixel.r || g != pixel.g || b != pixel.b)
        return 1;
    else return 0;
}
