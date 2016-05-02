#ifndef _CALHOG_
#define _CALHOG_

#pragma once

#include"common.h"

////winR,winC分别为窗口的行和列大小,
////CellR、CellC为cell的高和宽，BlockR、BlockC为block的高和宽，
////BlockStrideC,BlockStrideR分别为块步长的高和宽,Bin为箱子数目
//
//#define winR  128   
//#define winC  64 
//#define BlockR  16  
//#define BlockC  16
//#define CellR  8
//#define CellC  8
//#define BlockStrideR  8  // can't be 0
//#define BlockStrideC  8  // can't be 0 
//#define Bin  9
// 
//#define NumOfHogFeature 3780


void calHog(uint startR, uint startC);
void calImgBin(const uchar *src, uint imgR, uint imgC);
void calImgBin2(const uchar *src, uint imgR, uint imgC);
void constructIntegralHist(const uchar *imgBin, const imgMagType *imgMag, uint imgR, uint imgC);
#endif