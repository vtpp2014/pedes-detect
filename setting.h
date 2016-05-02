#ifndef _SETTING_H_
#define _SETTING_H_
#pragma once

//winR,winC�ֱ�Ϊ���ڵ��к��д�С,
//CellR��CellCΪcell�ĸߺͿ�BlockR��BlockCΪblock�ĸߺͿ�
//BlockStrideC,BlockStrideR�ֱ�Ϊ�鲽���ĸߺͿ�,BinΪ������Ŀ

#define winR  128   
#define winC  64 
#define BlockR  16  
#define BlockC  16
#define CellR  8
#define CellC  8
#define BlockStrideR  8  // can't be 0
#define BlockStrideC  8  // can't be 0 
#define Bin  9
#define WIN_STRIDE_R 8 
#define WIN_STRIDE_C 8
#define SCALE      1.2f
#define NumOfHogFeature  (Bin*(BlockR/CellR)*(BlockC/CellC)*((winC-BlockC)/BlockStrideC+1)*((winR-BlockR)/BlockStrideR+1))


#define MAX_ROWS_OF_INPUT_IMAGE 2000
#define MAX_COLS_OF_INPUT_IMAGE 2000
#define MAX_SIZE_OF_INPUT_IMAGE (MAX_ROWS_OF_INPUT_IMAGE*MAX_COLS_OF_INPUT_IMAGE)   //����ͼƬ�����ߴ�


#define JUDGE_THRESHOLD 0.f  //�ж��Ƿ�Ϊ���˵��ٽ�ֵ

//#define RECT_UNMERGED_RESULT      //�Ƿ���ʾδ�ϲ����ο��ͼ��(�����ʾ��ʾ)���Ա���ϲ���Ľ��жԱ�
#define GROUP_THRESHOLD 2  //����������ľ��ο����С��Ŀ��С�ڸ�ֵ������Ϊ���в�����

//#define NDEBUG //�Ƿ����assert����
//#define IS_TEST_CODE //�Ƿ���Դ��룬�������IS_TEST_CODE��ʾ��test.cpp�в��Դ���
#define IS_TRAIN  //�Ƿ�ѵ�����룬��IS_TEST_CODE����ͬʱdefine

typedef float imgMagType;
typedef double intHistType;

#endif 