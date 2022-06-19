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
    // �̹� ��ü�� �ִٸ� �����Ѵ�
    if (m_pDib) { Destroy(); }
    int nBitCount = 8;
    // ���� ���� �������� ũ�� ���(����Ʈ ����)
    DWORD dwSizeImage = nHeight * (DWORD)((nWidth * nBitCount / 8 + 3) & ~3);

    // �޸� ���� �Ҵ�
    m_pDib = new BYTE[sizeof(BITMAPINFOHEADER) 
        + sizeof(RGBQUAD) * (1 << nBitCount) + dwSizeImage];
    if (m_pDib == NULL) { return FALSE; }

    // ����� �ʵ� �� ����
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

    // ���� �ȷ�Ʈ�� �׷��̽����Ϸ� �ʱ�ȭ
    RGBQUAD* pPal = (RGBQUAD*)((BYTE*)m_pDib + sizeof(BITMAPINFOHEADER));
    for (int i = 0; i < 256; i++)
    {
        pPal->rgbBlue   = (BYTE)i;
        pPal->rgbGreen  = (BYTE)i;
        pPal->rgbRed    = (BYTE)i;
        pPal->rgbReserved = 0;
        pPal++;
    }

    // ��� ���� �� ����
    initDib();

    // �ȼ� ������ �ʱ�ȭ
    BYTE* pData = (BYTE*)GetDibBitsAddr();
    memset(pData, value, dwSizeImage);
    return TRUE;
}

BOOL CDib::CreateRGBImage(int nWidth, int nHeight, COLORREF value)
{
    // �̹� ��ü�� �ִٸ� �����Ѵ�
    if (m_pDib) { Destroy(); }
    int nBitCount = 24;
    // ���� ���� �������� ũ�� ���(����Ʈ ����)
    DWORD dwSizeImage = nHeight * (DWORD)((nWidth * nBitCount / 8 + 3) & ~3);

    // �޸� ���� �Ҵ�
    m_pDib = new BYTE[sizeof(BITMAPINFOHEADER) + dwSizeImage];
    if (m_pDib == NULL) { return FALSE; }

    // ����� �ʵ� �� ����
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

    // ��� ���� �� ����
    initDib();

    // �ȼ� ������ �ʱ�ȭ
    register int i, j;
    for (j = 0; j < m_nHeight; j++)
    {
        for (i = 0; i < m_nWidth; i++)
        {
            // RGBBYTE Ŭ������ = ������ �����ε� ���
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

    // �̹����� ����ũ��, ����ũ��, �ȼ� �� ��Ʈ��, DIB ũ�⿡ ���� ������� ����
    m_nWidth = lpbmi->biWidth;
    m_nHeight = lpbmi->biHeight;
    m_nBitCount = lpbmi->biBitCount;

    DWORD dwSizeImage = m_nHeight *
        (DWORD)((m_nWidth * m_nBitCount / 8 + 3) & ~3);
    m_nDibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * GetPalleteNums()
        + dwSizeImage;

    // m_pPtr �Ǵ� m_pRgbPtr ��� ���� ����
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

    // �б� ���� ������ ����
    if (!file.Open(lpszFileName,
        CFile::modeRead | CFile::shareDenyWrite))
    {
        return FALSE;
    }

    // bmp ���� ũ�� ��� (����Ʈ����)
    nFileSize = (UINT)file.GetLength();

    // ������� �б�
    if (file.Read((LPSTR) & bmfh, 
        sizeof(BITMAPFILEHEADER)) != 
        sizeof(BITMAPFILEHEADER))
    {
        file.Close();
        return FALSE;
    }

    // bmp �������� ��Ÿ���� "BM" ��Ŀ�� �ִ��� Ȯ��
    if (bmfh.bfType != DIB_HEADER_MARKER)
    {
        file.Close();
        return FALSE;
    }

    // packed-dib �޸� �� ũ�� ���
    nDibSize = nFileSize - sizeof(bmfh);

    // ���� ������ CDib ��ü�� �ִٸ� ����
    if (m_pDib != NULL) { Destroy(); }

    // ���� ũ�⸸ŭ �޸� �Ҵ�
    m_pDib = new BYTE[nDibSize];
    if (m_pDib == NULL) 
    {
        file.Close();
        return FALSE;
    }

    // ���Ϸκ��� packed-dib ũ�⸸ŭ �б�
    if (file.Read(m_pDib, nDibSize) != nDibSize)
    {
        delete[] m_pDib;
        m_pDib = NULL;
        file.Close();
        return FALSE;
    }

    // m_pDib�� DIB�� ����ִ� ������ ���� �ּ�
    // CDib Ŭ������ ��� ���� �� ����
    if (!initDib())
    {
        delete[] m_pDib;
        m_pDib = NULL;
        file.Close();
        return FALSE;
    }

    // ���� �ݱ�
    file.Close();
    return TRUE;
}

BOOL CDib::SaveBMP(LPCTSTR lpszFileName)
{
    CFile file;
    BITMAPFILEHEADER bmfh;

    // DIB ��ü�� �����ϰ� ���� ������ FALSE
    if (m_pDib == NULL) { return FALSE; }

    // ���� ���� ���� ����
    if (!file.Open(lpszFileName,
        CFile::modeCreate |
        CFile::modeWrite |
        CFile::shareDenyWrite))
    {
        return FALSE;
    }

    // BITMAPFILEHEADER ���� ����
    bmfh.bfType = DIB_HEADER_MARKER; // �׻� "BM"
    bmfh.bfSize = (DWORD)sizeof(BITMAPFILEHEADER) + m_nDibSize;
    bmfh.bfReserved1 = 0;
    bmfh.bfReserved2 = 0;
    bmfh.bfOffBits = (DWORD)(sizeof(BITMAPFILEHEADER) +
        sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) +
        GetPalleteNums());

    file.Write((LPSTR)&bmfh, sizeof(bmfh));

    // ������ ����
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
        hDC,        // DC ��ü
        x, y,       // �׷��� ��ġ ��ǥ
        m_nWidth,   // �̹��� ����
        m_nHeight,  // �̹��� ����
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
    // ��� �˻�
    if (this == &dib) { return *this; }

    // dib�� ����ִ� CDib ��ü��� �ڽŵ� ����.
    if (dib.m_pDib == NULL)
    {
        Destroy();
        return *this;
    }

    // ���� ������ CDib ��ü�� �ִٸ� ����
    if (m_pDib) { Destroy(); }

    // (BITMAPINFOHEADER + �ȼ� ������) ���� �Ҵ�
    m_pDib = new BYTE[dib.m_nDibSize];

    // ���� ����
    memcpy(m_pDib, dib.m_pDib, dib.m_nDibSize);

    // ��� ���� �ʱ�ȭ
    initDib();

    return *this;
}

BOOL CDib::Copy(CDib* pDib)
{
    // pDib�� ����ִ� CDib ��ü�� ����Ű�� �ִٸ� �ڽŵ� ����.
    if (!pDib->IsValid()) 
    { 
        Destroy(); 
        return TRUE; 
    }

    // ���� ������ CDib ��ü�� �ִٸ� ����
    if (m_pDib) { Destroy(); }

    // DIB�� ���� �޸� ���� �Ҵ�
    DWORD dwDibSize = pDib->GetDibSize();
    m_pDib = new BYTE[dwDibSize];
    if (m_pDib == NULL) { return FALSE; }

    // ����
    BYTE* pbi = (BYTE*)pDib->GetBitmapInfoAddr();
    memcpy(m_pDib, pbi, dwDibSize);

    // ��� ���� �ʱ�ȭ
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
