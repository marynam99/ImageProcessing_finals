#pragma once
#include "CDib.h"

void DibHistogram(CDib& dib, float histo[256], int rgb = 0);
void RGB2YCrCb(CDib& dib, float** Y, float** Cr, float** Cb);
void YCrCb2RGB(CDib& dib, float** Y, float** Cr, float** Cb);
FLOAT** Alloc2DMem(int h, int w);

void ConvolutionProcessColor(CDib& dib, FLOAT** Mask, int ksize);
void ConvolutionProcessColorRGB(CDib& dib, FLOAT** Mask, int ksize);