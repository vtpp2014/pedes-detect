#ifndef _MULTIDECT_
#define _MULTIDECT

#pragma once

#include"common.h"
#include"calHog.h"
#include"judge.h"
#include"circlePerson.h"

void detectMultiScale(
    Mat &src,
	uint imgR,          uint imgC,
	uchar winStrideR,   uchar winStrideC,
	float scale);

#endif