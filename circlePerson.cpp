#include"circlePerson.h"

#ifndef IS_TRAIN
void circlePerson(Mat &src, vector<Rect> found)
/*该函数对所有得到的矩形框进行处理并最终框出行人
其中的条件编译是为了对利用mergeRect函数进行矩形框
合并后的前后结果进行对照。*/
{
	vector<Rect> foundFiltered;

#ifdef RECT_UNMERGED_RESULT
	/*不对举行框作任何处理，用来作为对照*/
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

	/*合并矩形框*/
	foundFiltered = mergeRect(found, GROUP_THRESHOLD, 0.2);

	//画矩形框
	for (int i = 0; i < foundFiltered.size(); i++){
		Rect r = foundFiltered[i];

#ifndef RECT_UNMERGED_RESULT
		//因为hog检测出的矩形框比实际人体框要稍微大些, 所以这里需要将矩形框缩小
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
		Mat hardExampleImg = src(r);//从原图上截取矩形框大小的图片  
		resize(hardExampleImg, hardExampleImg, Size(64, 128));//将剪裁出来的图片缩放为64*128大小 
		sprintf(saveName, "D:\\trainPics\\hard\\hard%d.jpg", hardExampleCount++);//生成hard example图片的文件名  
		imwrite(saveName, hardExampleImg);//保存文件  
	}
}


#endif