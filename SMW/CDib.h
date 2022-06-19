#pragma once
#include <wtypes.h>
#include <afx.h>

class RGBBYTE
{
public:
	BYTE b;
	BYTE g;
	BYTE r;

	// 연산자 오버로딩
	RGBBYTE& operator = (const RGBBYTE& pixel);
	RGBBYTE& operator = (const COLORREF& pixel);
	RGBBYTE& operator = (const BYTE& pixel);

	int operator==(const RGBBYTE& pixel);
	int operator!=(const RGBBYTE& pixel);
};

class CDib
{
protected:
	LONG m_nWidth; // 이미지의 가로크기 (픽셀단위)
	LONG m_nHeight; // 이미지의 세로크기 (픽셀단위)
	WORD m_nBitCount; // 픽셀 당 비트 수
	DWORD m_nDibSize; // 비트맵 파일의 전체 크기 
	// (BITMAPINFOHEADER + 색상테이블 + 픽셀데이터)
	BYTE* m_pDib; // Dib 내용을 담고 있는 버퍼의 시작 주소
	BYTE** m_pPtr; // 그레이스케일 이미지의 2D 버퍼 시작점
	RGBBYTE** m_pRgbPtr; // 트루컬러 이미지의 2D 버퍼 시작점

public:
	// 생성자 & 소멸자
	CDib();
	CDib(const CDib& dib);
	~CDib();

	// 이미지를 생성 & 소멸
	BOOL CreateGrayImage(int nWidth, 
		int nHeight, BYTE value = 255);
	BOOL CreateRGBImage(int nWidth, 
		int nHeight, COLORREF value = 0x00ffffff);

	// 파일입출력
	BOOL Load(LPCTSTR lpszFileName);
	BOOL Save(LPCTSTR lpszFileName);
	BOOL LoadBMP(LPCTSTR lpszFileName);
	BOOL SaveBMP(LPCTSTR lpszFileName);

	// 이미지 정보 반환 함수들
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

	// BITMAPINFOHEADER 시작 포인터
	LPBITMAPINFO GetBitmapInfoAddr() { 
		return (LPBITMAPINFO)m_pDib; }

	// 팔레트 크기 반환
	int GetPalleteNums();

	// 픽셀 데이터의 시작포인터반환
	LPVOID GetDibBitsAddr();

	// 그리기 함수
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

	// 치환 연산자 재정의
	CDib& operator = (const CDib& dib);
	// CDib a; CDib b; a = b;

	// 비트맵 복사
	BOOL Copy(CDib* pDib);

	// 클립보드를 이용한 복사하기와 붙여넣기
	BOOL CopyToClipboard();
	BOOL PasteFromClipboard();

protected:
	// 비트맵 데이터 제어 함수
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

