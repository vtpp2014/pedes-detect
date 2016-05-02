#ifndef _SETTING_H_
#define _SETTING_H_
#pragma once

//winR,winC分别为窗口的行和列大小,
//CellR、CellC为cell的高和宽，BlockR、BlockC为block的高和宽，
//BlockStrideC,BlockStrideR分别为块步长的高和宽,Bin为箱子数目

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
#define MAX_SIZE_OF_INPUT_IMAGE (MAX_ROWS_OF_INPUT_IMAGE*MAX_COLS_OF_INPUT_IMAGE)   //输入图片的最大尺寸


#define JUDGE_THRESHOLD 0.f  //判断是否为行人的临界值

//#define RECT_UNMERGED_RESULT      //是否显示未合并矩形框的图像(定义表示显示)，以便与合并后的进行对比
#define GROUP_THRESHOLD 2  //保留类所需的矩形框的最小数目，小于该值的类视为误判并舍弃

//#define NDEBUG //是否禁用assert调试
//#define IS_TEST_CODE //是否测试代码，如果定义IS_TEST_CODE表示在test.cpp中测试代码
#define IS_TRAIN  //是否训练代码，与IS_TEST_CODE不可同时define

typedef float imgMagType;
typedef double intHistType;

#endif 