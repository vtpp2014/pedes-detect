
#ifndef _MYLIB_H_
#define _MYLIB_H_
#pragma once

#include"common.h"

void l2Norm(float *p, int length);

void imResize(
	const uchar *img, uchar *afterZoomImg,
	uint imgR, uint imgC,
	uint afterZoomR, uint afterZoomC);

unsigned char *Mat2ImgPointer(const Mat &src);

Mat ImgPointer2Mat(const unsigned char *pImg, int imgR, int imgC);



#endif