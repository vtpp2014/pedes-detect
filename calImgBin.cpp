#include"calHog.h"

uchar imgBin[MAX_SIZE_OF_INPUT_IMAGE] = {0};//���ڴ�ż����һ��ͼ�����������������bin������
imgMagType imgMag[MAX_SIZE_OF_INPUT_IMAGE] = { 0 };//���ڴ�ż����һ��ͼ����������ص��ݶȵķ���ֵ

static int ysub[MAX_SIZE_OF_INPUT_IMAGE];
static int xsub[MAX_SIZE_OF_INPUT_IMAGE];

void calImgBin(const uchar *src, uint imgR, uint imgC)
/*�ú�������������ص��ݶȵķ��Ⱥ������ڵ�Bin����*/
{
	assert(imgR*imgC <= MAX_SIZE_OF_INPUT_IMAGE);
	uint y = 0, x = 0;
	uint temp = 0;
	float angTemp;

	//gradient of x orientation,���ø�����䡣
	for (y = 0; y < imgR; y++)
	for (x = 1; x < imgC - 1; x++)
	{
		temp = y*imgC + x;
		xsub[temp] = src[temp + 1] - src[temp - 1];
	}
	for (y = 0; y < imgR; y++)
	{
		temp = y*imgC;
		xsub[temp] = src[temp + 1] - src[temp];
		xsub[temp + imgC - 1] = src[temp + imgC - 1] - src[temp + imgC - 2];
	}

	//gradient of y orientation�����ø�����䡣
	for (y = 1; y < imgR - 1; y++)
	for (x = 0; x < imgC; x++)
	{
		temp = y*imgC + x; //���y*imgC��ֵ�����ٳ˷��������
		ysub[temp] = src[temp + imgC] - src[temp - imgC];
	}
	for (x = 0; x < imgC; x++)
	{
		temp = imgC*(imgR - 1);
		ysub[x] = src[imgC + x] - src[x];
		ysub[temp + x] = src[temp + x] - src[temp - imgC + x];
	}

	for (y = 0; y < imgR; y++)
	for (x = 0; x < imgC; x++)
	{
		temp = y*imgC + x;
		angTemp = atan(ysub[temp] / (xsub[temp] + 0.001)) + 1.5708; //angle of gradient 3.1416/2 = 1.5708
		imgMag[temp] = sqrt((float)(ysub[temp] * ysub[temp] + xsub[temp] * xsub[temp])); //magnitude of gradient
		imgBin[temp] = (uchar)(angTemp * 2.864); //�����ڵ�bin������ 9/3.1416 = 2.8648
		assert(imgBin[temp] < 9);
	}
	constructIntegralHist(imgBin, imgMag, imgR, imgC);
}
