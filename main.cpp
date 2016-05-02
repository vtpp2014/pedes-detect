#include"common.h"
#ifndef IS_TEST_CODE
#ifndef IS_TRAIN
#include"calHog.h"
#include"detectMultiScale.h"
#include"circlePerson.h"

string hogDetectorFileName = "D:\\testPics\\detectorFile\\HOGDetector3.2_neg_hard_2.txt";   //������ļ�·��
string testPicsFilePath = "D:\\testPics\\test.txt";  //�������ͼƬ��Ϣ���ļ���·��
string testPicsPath = "D:\\testPics\\";  //����ͼƬ·��
//string testPicsFilePath = "D:\\testPics\\INRIA_pos_huge_test\\INRIA_pos_huge_test.txt";  //�������ͼƬ��Ϣ���ļ���·��
//string testPicsPath = "D:\\testPics\\INRIA_pos_huge_test\\";  //����ͼƬ·��


int main()
{
	Mat src;
	string imgName;
	getDetector(hogDetectorFileName);//���ļ��л�ü����

	ifstream fileTest(testPicsFilePath);
	if (!fileTest){
		printf("file fileTest is NULL");
		system("pause");
		exit(1);
	}

	string winName = "i";
	while (getline(fileTest, imgName) && !imgName.empty()){
		cout << "testing:  " << imgName << endl;
		imgName = testPicsPath + imgName;
	
		src = imread(imgName,0);
		assert(src.data != NULL);
		cout <<"size: "<< src.rows << " x " << src.cols << endl;

		double duration;
		duration = static_cast<double>(getTickCount());

		detectMultiScale(src, src.rows, src.cols, WIN_STRIDE_R, WIN_STRIDE_C, SCALE);

		duration = static_cast<double>(getTickCount()) - duration;
		duration /= getTickFrequency();
		cout << "detectMultiScale duration time: " << duration << "s" << endl;
		cout << endl;

		//imwrite("ImgDetected.jpg",src);
		namedWindow(winName);
		imshow(winName, src);
		waitKey();
		//destroyWindow(winName);
		//winName += "i";
	}

	fileTest.close();

	system("pause");
	return 0;
}
#endif
#endif
