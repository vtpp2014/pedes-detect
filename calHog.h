#ifndef _CALHOG_
#define _CALHOG_

#pragma once

#include"common.h"

////winR,winC�ֱ�Ϊ���ڵ��к��д�С,
////CellR��CellCΪcell�ĸߺͿ�BlockR��BlockCΪblock�ĸߺͿ�
////BlockStrideC,BlockStrideR�ֱ�Ϊ�鲽���ĸߺͿ�,BinΪ������Ŀ
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