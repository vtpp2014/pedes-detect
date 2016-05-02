#include "detectMultiScale.h"

//extern uchar imgBin[MAX_SIZE_OF_INPUT_IMAGE]; 
//extern float imgMag[MAX_SIZE_OF_INPUT_IMAGE]; 

void detectMultiScale(
	Mat &src, uint imgR, uint imgC,
	uchar winStrideR, uchar winStrideC,
	float scale)
	/*函数用于进行多窗口检测；
	img 原图像，imgR、imgC 图像高、宽，
	winStrideR、 winStrideC 窗口在行列方向步进大小，
	scale是缩放比例*/
{
	assert(scale > 1);
	unsigned char *img = Mat2ImgPointer(src);

	uchar zoomNumR = log((float)imgR / winR) / log(scale) ;
	uchar zoomNumC = log((float)imgC / winC) / log(scale) ;
	uchar zoomNum = zoomNumR > zoomNumC ? zoomNumC : zoomNumR; //窗口缩放次数
	printf("zoomNum:%d\n",zoomNum);

	uchar zi = 0, jr = 0, jc = 0;
	uchar wr = 0, wc = 0;
	uint afterZoomR, afterZoomC; //缩放后的高和宽
	uchar *afterZoomImg;  //缩放后的图片
	uchar winStepNumR, winStepNumC;  // 行、列方向的窗口步进数

	uint startR, startC;             //窗口在图像中的行、列位置
	//int startOfs,jw,ji;
	//uchar winBin[winR*winC]; //存放窗口里面每个像素点所属的bin的区间
	//float winMag[winR*winC]; //存放窗口的梯度的幅度值

	int trueCount = 0;
	int callCount = 0;
	unsigned int y, x, height, width;//Rect parameters
	vector<Rect> found;

	afterZoomImg = (uchar*)calloc(imgR*imgC, sizeof(uchar));
	assert(afterZoomImg != NULL);
	for (zi = 0; zi <= zoomNum; zi++){
		afterZoomR = imgR / pow(scale, (float)zi);
		afterZoomC = imgC / pow(scale, (float)zi);
		if (zi == 0)
			memcpy(afterZoomImg, img, sizeof(uchar)*afterZoomR*afterZoomC);
		else{
			afterZoomImg = (uchar*)realloc(afterZoomImg,sizeof(uchar)*afterZoomR*afterZoomC);
			assert(afterZoomImg != NULL);
			imResize(img, afterZoomImg, imgR, imgC, afterZoomR, afterZoomC);
		}

		calImgBin(afterZoomImg, afterZoomR, afterZoomC);

		winStepNumR = (afterZoomR - winR) / winStrideR + 1; //tip：整数除以整数，结果还是整数
		winStepNumC = (afterZoomC - winC) / winStrideC + 1;
		for (wr = 0; wr < winStepNumR; wr++){
			for (wc = 0; wc < winStepNumC; wc++){

				startR = wr*winStrideR;
				startC = wc*winStrideC;
				calHog(startR, startC);
				callCount++;

				if (judge()){ //如果判断结果为真，则认为有行人
					y = (startR + 0.5)*imgR / afterZoomR - 0.5;
					x = (startC + 0.5)*imgC / afterZoomC - 0.5;
					height = winR*pow(scale, (float)zi);
					width = winC*pow(scale, (float)zi);
					Rect rect(x, y, width, height);
					found.push_back(rect); // 将包含行人的矩形框保存在found中
					trueCount++;
				}
			}
		}

		
	}

	
	free(img);
	free(afterZoomImg);

	cout << "trueCount: " << trueCount << endl;
	cout << "callCount: " << callCount << endl;

	if (trueCount>0)
		circlePerson(src, found); // 对所有得到的矩形框进行处理并最终框出行人
}
