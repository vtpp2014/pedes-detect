#include"getHardFromNeg.h"

static string negForHardPath = "D:\\trainPics\\negPicsForHardExamples\\";
static string negForHardNameTxt = "D:\\trainPics\\negPicsForHardExamples\\negForHard.txt";

void getNegFromHard(string detectorName){
	getDetector(detectorName);
	ifstream fileNeg(negForHardNameTxt);
	string imgName;
	unsigned char *pImg;
	for (int num = 0; getline(fileNeg, imgName); num++){
		cout << "´¦Àí£º" << imgName << endl;
		imgName = negForHardPath + imgName;
		Mat src = imread(imgName, 0);
		assert(src.data != NULL);
		detectMultiScale(src, src.rows, src.cols, WIN_STRIDE_R, WIN_STRIDE_C, SCALE);
	}

}