#include"judge.h"

static double svmCoef[NumOfHogFeature+1]={0}; //用于存放检测子的数组
extern float HogFeature[NumOfHogFeature];

void getDetector(string detectorName)
/*用于获得文件中的检测子*/
{
	ifstream hogFile(detectorName);//包含hog检测子的文件
	if(!hogFile){
		printf("hogFile can't open file");
		exit(1);
	}
	for( int i=0; i<NumOfHogFeature+1; i++ )
		hogFile>>svmCoef[i];
}

bool judge()
/* 该函数判断窗口中是否包含行人*/
{
	double sum = 0;
	int i = 0;
	for (i = 0; i<NumOfHogFeature; i += 4){
		sum += svmCoef[i] * HogFeature[i] + svmCoef[i + 1] * HogFeature[i + 1]
			+ svmCoef[i + 2] * HogFeature[i + 2] + svmCoef[i + 3] * HogFeature[i + 3];
	}
	for (; i<NumOfHogFeature; i++)
		sum += svmCoef[i] * HogFeature[i];
	sum += svmCoef[NumOfHogFeature];

	if (sum > JUDGE_THRESHOLD)
		return true;
	else
		return false;
}