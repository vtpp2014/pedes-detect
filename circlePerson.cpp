#include"circlePerson.h"

#ifndef IS_TRAIN
void circlePerson(Mat &src, vector<Rect> found)
/*�ú��������еõ��ľ��ο���д������տ������
���е�����������Ϊ�˶�����mergeRect�������о��ο�
�ϲ����ǰ�������ж��ա�*/
{
	vector<Rect> foundFiltered;

#ifdef RECT_UNMERGED_RESULT
	/*���Ծ��п����κδ���������Ϊ����*/
	Mat srcTemp = src.clone();
	for (int i = 0; i < found.size(); i++){
		Rect r = found[i];
		rectangle(srcTemp, r, Scalar(255), 1);
	}
	string winName = "unprocessed image";
	namedWindow(winName);
	imshow(winName, srcTemp);
	waitKey();
	//destroyWindow(winName);
#endif

	/*�ϲ����ο�*/
	foundFiltered = mergeRect(found, GROUP_THRESHOLD, 0.2);

	//�����ο�
	for (int i = 0; i < foundFiltered.size(); i++){
		Rect r = foundFiltered[i];

#ifndef RECT_UNMERGED_RESULT
		//��Ϊhog�����ľ��ο��ʵ�������Ҫ��΢��Щ, ����������Ҫ�����ο���С
		r.x += cvRound(r.width*0.1);
		r.width = cvRound(r.width*0.8);
		r.y += cvRound(r.height*0.07);
		r.height = cvRound(r.height*0.8);
#endif

		rectangle(src, r, Scalar(255), 2);
	}
}

#else

static int hardExampleCount = 1;
void circlePerson(Mat &src, vector<Rect> found){
	for (int i = 0; i < found.size(); i++){
		char saveName[256];
		Rect r = found[i];
		Mat hardExampleImg = src(r);//��ԭͼ�Ͻ�ȡ���ο��С��ͼƬ  
		resize(hardExampleImg, hardExampleImg, Size(64, 128));//�����ó�����ͼƬ����Ϊ64*128��С 
		sprintf(saveName, "D:\\trainPics\\hard\\hard%d.jpg", hardExampleCount++);//����hard exampleͼƬ���ļ���  
		imwrite(saveName, hardExampleImg);//�����ļ�  
	}
}


#endif