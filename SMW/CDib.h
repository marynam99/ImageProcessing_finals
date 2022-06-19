#pragma once
#include <wtypes.h>
#include <afx.h>

class RGBBYTE
{
public:
	BYTE b;
	BYTE g;
	BYTE r;

	// ������ �����ε�
	RGBBYTE& operator = (const RGBBYTE& pixel);
	RGBBYTE& operator = (const COLORREF& pixel);
	RGBBYTE& operator = (const BYTE& pixel);

	int operator==(const RGBBYTE& pixel);
	int operator!=(const RGBBYTE& pixel);
};

class CDib
{
protected:
	LONG m_nWidth; // �̹����� ����ũ�� (�ȼ�����)
	LONG m_nHeight; // �̹����� ����ũ�� (�ȼ�����)
	WORD m_nBitCount; // �ȼ� �� ��Ʈ ��
	DWORD m_nDibSize; // ��Ʈ�� ������ ��ü ũ�� 
	// (BITMAPINFOHEADER + �������̺� + �ȼ�������)
	BYTE* m_pDib; // Dib ������ ��� �ִ� ������ ���� �ּ�
	BYTE** m_pPtr; // �׷��̽����� �̹����� 2D ���� ������
	RGBBYTE** m_pRgbPtr; // Ʈ���÷� �̹����� 2D ���� ������

public:
	// ������ & �Ҹ���
	CDib();
	CDib(const CDib& dib);
	~CDib();

	// �̹����� ���� & �Ҹ�
	BOOL CreateGrayImage(int nWidth, 
		int nHeight, BYTE value = 255);
	BOOL CreateRGBImage(int nWidth, 
		int nHeight, COLORREF value = 0x00ffffff);

	// ���������
	BOOL Load(LPCTSTR lpszFileName);
	BOOL Save(LPCTSTR lpszFileName);
	BOOL LoadBMP(LPCTSTR lpszFileName);
	BOOL SaveBMP(LPCTSTR lpszFileName);

	// �̹��� ���� ��ȯ �Լ���
	LONG GetHeight() { return m_nHeight; }
	LONG GetWidth() { return m_nWidth; }
	WORD GetBitCount() { return m_nBitCount; }
	DWORD GetDibSize() { return m_nDibSize; }
	BOOL IsValid() { return (m_pDib != NULL); }
	BYTE** GetPtr() { 
		ASSERT(m_nBitCount == 8);
		ASSERT(m_pPtr != NULL);
		return m_pPtr;
	}
	RGBBYTE** GetRGBPtr() {
		ASSERT(m_nBitCount == 24);
		ASSERT(m_pRgbPtr != NULL);
		return m_pRgbPtr;
	}

	// BITMAPINFOHEADER ���� ������
	LPBITMAPINFO GetBitmapInfoAddr() { 
		return (LPBITMAPINFO)m_pDib; }

	// �ȷ�Ʈ ũ�� ��ȯ
	int GetPalleteNums();

	// �ȼ� �������� ���������͹�ȯ
	LPVOID GetDibBitsAddr();

	// �׸��� �Լ�
	void Draw(HDC hDC);
	void Draw(HDC hDC, int x, int y);
	void Draw(HDC hDC, 
		int dx, int dy, 
		int dw, int dh, 
		DWORD dwRop = SRCCOPY);
	void Draw(HDC hDC, 
		int dx, int dy, 
		int dw, int dh, 
		int sx, int sy, 
		int sw, int sh, 
		DWORD dwRop=SRCCOPY);

	// ġȯ ������ ������
	CDib& operator = (const CDib& dib);
	// CDib a; CDib b; a = b;

	// ��Ʈ�� ����
	BOOL Copy(CDib* pDib);

	// Ŭ�����带 �̿��� �����ϱ�� �ٿ��ֱ�
	BOOL CopyToClipboard();
	BOOL PasteFromClipboard();

protected:
	// ��Ʈ�� ������ ���� �Լ�
	BOOL initDib();
	BOOL AllocPtr();
	void FreePtr();

	void Destroy();
};

template<typename T>
inline void swap(T& lha, T& rha)
{
	T tmp = lha;
	lha = rha;
	rha = tmp;
}

template<typename T>
inline void limit(const T& value)
{
	return ((value > 255) ? 255 : ((value < 0) ? 0 : value))
}

template<typename T>
inline void limit(const T& value,
	const T& lower, const T& upper)
{
	T tmp = value;
	if (value > upper)
	{
		tmp = upper;
	}
	else
	{
		if (value < lower)
		{
			tmp = lower;
		}
	}
	return tmp;
}

