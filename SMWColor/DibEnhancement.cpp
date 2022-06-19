#include "pch.h"
#include "DibEnhancement.h"

void DibHistogram(CDib& dib, float histo[256], int rgb)
{
	register int i, j;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	int temp[256];
	memset(temp, 0, sizeof(int) * 256);

	if (dib.GetBitCount() == 8)
	{
		BYTE** ptr = dib.GetPtr();
		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				temp[ptr[j][i]]++;
			}
		}
	}
	else if (dib.GetBitCount() == 24)
	{
		RGBBYTE** ptr = dib.GetRGBPtr();
		if (rgb == 0)
		{
			for (j = 0; j < h; j++)
			{
				for (i = 0; i < w; i++)
				{
					temp[ptr[j][i].r]++;
				}
			}
		}
		if (rgb == 1)
		{
			for (j = 0; j < h; j++)
			{
				for (i = 0; i < w; i++)
				{
					temp[ptr[j][i].g]++;
				}
			}
		}
		if (rgb == 2)
		{
			for (j = 0; j < h; j++)
			{
				for (i = 0; i < w; i++)
				{
					temp[ptr[j][i].b]++;
				}
			}
		}
	}

	float area = (float)w * h;
	for (i = 0; i < 256; i++)
	{
		histo[i] = temp[i] / area;
	}
}

void RGB2YCrCb(CDib& dib, float** Y, float** Cr, float** Cb) 
{
	register int i, j;

	int w = dib.GetWidth();
	int h = dib.GetHeight();
	RGBBYTE** ptr = dib.GetRGBPtr();

	float R = 0.;
	float G = 0.;
	float B = 0.;


	for (j = 0; j < h; j++) 
	{
		for (i = 0; i < w; i++)
		{
			R = ptr[j][i].r / 255.;
			G = ptr[j][i].g / 255.;
			B = ptr[j][i].b / 255.;

			Y[j][i] = 0.299 * R + 0.587 * G + 0.114 * B;
			Cr[j][i] = 0.500 * R - 0.419 * G - 0.081 * B;
			Cb[j][i] = -0.16 * R - 0.331 * G + 0.500 * B;

		}
	}
}

void YCrCb2RGB(CDib& dib, float** Y, float** Cr, float** Cb)
{
	register int i, j;
	int w = dib.GetWidth();
	int h = dib.GetHeight();

	float R = 0.;
	float G = 0.;
	float B = 0.;

	RGBBYTE** ptr = dib.GetRGBPtr();

	for (j = 0; j < h; j++)
	{
		for (i = 0; i < w; i++)
		{
			R = 1.000 * Y[j][i] + 1.402 * Cr[j][i] + 0.000 * Cb[j][i];
			ptr[j][i].r = limit(R * 255);

			G = 1.000 * Y[j][i] - 0.714 * Cr[j][i] - 0.344 * Cb[j][i];
			ptr[j][i].g = limit(G * 255);

			B = 1.000 * Y[j][i] + 0.000 * Cr[j][i] + 1.772 * Cb[j][i];
			ptr[j][i].b = limit(B * 255);
		}
	}
}

FLOAT** Alloc2DMem(int h, int w)
{
	FLOAT** temp;
	int i, j;

	temp = new FLOAT * [h];
	for (i = 0; i < h; i++)
	{
		temp[i] = new FLOAT[w];
	}
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			temp[i][j] = 0.0;
		}
	}
	return temp;
}

/// <summary>
/// Same logic as 'Embossing' in 24 bit
/// </summary>
/// <param name="dib"></param>
/// <param name="Mask">Double pointer for filtering mask(value defined)</param>
/// <param name="ksize">Filter size selected from dialog</param>
void ConvolutionProcessColor(CDib& dib, FLOAT** Mask, int ksize)
{
	register int i, j, n, m;

	// ksize ������ Ȧ���ϱ� �ϳ� ����(��� ��) 2�� ������ �Ͱ� ����
	int margin = ksize / 2;  

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	RGBBYTE** ptr = dib.GetRGBPtr();

	FLOAT** Y = Alloc2DMem(h, w);
	FLOAT** Cr = Alloc2DMem(h, w);
	FLOAT** Cb = Alloc2DMem(h, w);

	// �÷����� ����
	RGB2YCrCb(dib, Y, Cr, Cb);

	// �Ǽ� �������� �ӽ� ����
	FLOAT** Y_margin = Alloc2DMem(h + ksize - 1, w + ksize - 1);
	FLOAT** Y_result = Alloc2DMem(h, w);

	// �Է� �̹��� �Ǽ������� ����, zero-padding
	for (j = 0; j < h; j++)
	{
		for (i = 0; i < w; i++)
		{
			Y_margin[j + margin][i + margin] = (FLOAT)Y[j][i]; 
		}
	}

	// convolution
	FLOAT sum = 0.;
	for (j = 0; j < h; j++)
	{
		for (i = 0; i < w; i++)
		{
			for (n = 0; n < ksize; n++)
			{
				for (m = 0; m < ksize; m++)
				{
					sum += Mask[n][m] * Y_margin[j + n][i + m];
				}
			}
			// ��� ���ۿ� ����
			Y_result[j][i] = sum; // ��� ���۴� padding ���Ե� ũ�Ⱑ �ƴ϶� ������ �Ȱ��� ũ���̹Ƿ� index ����
			sum = 0;
		}
	}
	
	// convolution ��� clipping
	for (j = 0; j < h; j++)
	{
		for (i = 0; i < w; i++)
		{
			/*Y_result[j][i] = limit(Y_result[j][i] + avg,
				(FLOAT)0., (FLOAT)1.);*/
			Y_result[j][i] = limit(Y_result[j][i],
				(FLOAT)0., (FLOAT)1.);
		}
	}

	// �÷����� ����
	YCrCb2RGB(dib, Y_result, Cr, Cb);

	delete[] Mask;
	delete[] Y;
	delete[] Cr;
	delete[] Cb;
	delete[] Y_margin;
	delete[] Y_result;
}

void ConvolutionProcessColorRGB(CDib& dib, FLOAT** Mask, int ksize)
{
	register int i, j, n, m, ch;

	int margin = ksize / 2;  

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	RGBBYTE** ptr = dib.GetRGBPtr();

	// �Ǽ� �������� �ӽ� ����
	FLOAT** buf[3];
	FLOAT** RGB[3];
	for (ch = 0; ch < 3; ch++)
	{
		buf[ch] = Alloc2DMem(h, w);
		RGB[ch] = Alloc2DMem(h + ksize - 1, w + ksize - 1);
	}

	// �Է� �̹��� �Ǽ������� ����, zero-padding
	for (j = 0; j < h; j++)
	{
		for (i = 0; i < w; i++)
		{
			RGB[0][j + margin][i + margin] = (FLOAT)ptr[j][i].r / 255; 
			RGB[1][j + margin][i + margin] = (FLOAT)ptr[j][i].g / 255;
			RGB[2][j + margin][i + margin] = (FLOAT)ptr[j][i].b / 255;
		}
	}

	// convolution
	for (ch = 0; ch < 3; ch++)
	{
		FLOAT sum = 0.;
		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				for (n = 0; n < ksize; n++)
				{
					for (m = 0; m < ksize; m++)
					{
						sum += Mask[n][m] * RGB[ch][j + n][i + m];
					}
				}

				// ��� ���ۿ� ����
				buf[ch][j][i] = sum; // ��� ���۴� padding ���Ե� ũ�Ⱑ �ƴ϶� ������ �Ȱ��� ũ���̹Ƿ� index ����
				sum = 0.;
			}
		}

		// convolution ��� clipping
		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				/*Y_result[j][i] = limit(Y_result[j][i] + avg,
					(FLOAT)0., (FLOAT)1.);*/
				buf[ch][j][i] = limit(buf[ch][j][i]*255., 0., 255.);
			}
		}
	}

	// ����
	for (j = 0; j < h; j++)
	{
		for (i = 0; i < w; i++)
		{
			ptr[j][i].r = (BYTE)buf[0][j][i];
			ptr[j][i].g = (BYTE)buf[1][j][i];
			ptr[j][i].b = (BYTE)buf[2][j][i];
		}
	}

	// ���� �Ҵ� ����
	for (ch = 0; ch < 3; ch++)
	{
		delete[] buf[ch];
		delete[] RGB[ch];
	}
}