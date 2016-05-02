#include "detectMultiScale.h"

//extern uchar imgBin[MAX_SIZE_OF_INPUT_IMAGE]; 
//extern float imgMag[MAX_SIZE_OF_INPUT_IMAGE]; 

void detectMultiScale(
	Mat &src, uint imgR, uint imgC,
	uchar winStrideR, uchar winStrideC,
	float scale)
	/*�������ڽ��жര�ڼ�⣻
	img ԭͼ��imgR��imgC ͼ��ߡ���
	winStrideR�� winStrideC ���������з��򲽽���С��
	scale�����ű���*/
{
	assert(scale > 1);
	unsigned char *img = Mat2ImgPointer(src);

	uchar zoomNumR = log((float)imgR / winR) / log(scale) ;
	uchar zoomNumC = log((float)imgC / winC) / log(scale) ;
	uchar zoomNum = zoomNumR > zoomNumC ? zoomNumC : zoomNumR; //�������Ŵ���
	printf("zoomNum:%d\n",zoomNum);

	uchar zi = 0, jr = 0, jc = 0;
	uchar wr = 0, wc = 0;
	uint afterZoomR, afterZoomC; //���ź�ĸߺͿ�
	uchar *afterZoomImg;  //���ź��ͼƬ
	uchar winStepNumR, winStepNumC;  // �С��з���Ĵ��ڲ�����

	uint startR, startC;             //������ͼ���е��С���λ��
	//int startOfs,jw,ji;
	//uchar winBin[winR*winC]; //��Ŵ�������ÿ�����ص�������bin������
	//float winMag[winR*winC]; //��Ŵ��ڵ��ݶȵķ���ֵ

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

		winStepNumR = (afterZoomR - winR) / winStrideR + 1; //tip���������������������������
		winStepNumC = (afterZoomC - winC) / winStrideC + 1;
		for (wr = 0; wr < winStepNumR; wr++){
			for (wc = 0; wc < winStepNumC; wc++){

				startR = wr*winStrideR;
				startC = wc*winStrideC;
				calHog(startR, startC);
				callCount++;

				if (judge()){ //����жϽ��Ϊ�棬����Ϊ������
					y = (startR + 0.5)*imgR / afterZoomR - 0.5;
					x = (startC + 0.5)*imgC / afterZoomC - 0.5;
					height = winR*pow(scale, (float)zi);
					width = winC*pow(scale, (float)zi);
					Rect rect(x, y, width, height);
					found.push_back(rect); // ���������˵ľ��ο򱣴���found��
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
		circlePerson(src, found); // �����еõ��ľ��ο���д������տ������
}
