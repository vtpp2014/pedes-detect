
#ifndef _CIRCLEPERSON_
#define _CIRCLEPERSON_

#include"common.h"

void circlePerson(Mat &src,vector<Rect> found);
vector<Rect> mergeRect(const vector<Rect>& _vec, int groupThreshold, float weight);

#endif