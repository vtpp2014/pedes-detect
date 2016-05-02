#include "mylib.h"

/*��һ������������ΪL2norm*/
void l2Norm(float *p, int length){
	int i = 0;
	float sum = 0;
	for (i = 0; i < length; i++){
		sum += p[i] * p[i];
	}
	sum = sqrt(sum);
	for (i = 0; i < length; i++)
		p[i] /= sum+0.001;
}


/*����ͼ�������ú��������������һ��Ӱ��*/
/*��һ�ַ������Լӿ�ú����ļ��㣬��ʱ������*/
void imResize(
	const uchar *img, uchar *afterZoomImg,
	uint imgR, uint imgC,
	uint afterZoomR, uint afterZoomC)
{
	uint r, c;
	float posR, posC;
	float y, x;
	uint posRInt, posCInt;
	uint indLu, indLd, indRu, indRd;
	for (r = 0; r < afterZoomR; r++){
		for (c = 0; c < afterZoomC; c++)
		{
			posR = (r + 0.5)*imgR / (float)afterZoomR - 0.5; //�Ӽ�0.5
			posC = (c + 0.5)*imgC / (float)afterZoomC - 0.5;
			posRInt = (uint)posR;  posCInt = (uint)posC;
			y = posR - posRInt;   x = posC - posCInt;
			indLu = posRInt*imgC + posCInt;  indRu = indLu + 1;
			indLd = indLu + imgC;  indRd = indLd + 1;
			afterZoomImg[c + r*afterZoomC] = img[indLu] * (1 - x)*(1 - y) +
				img[indLd] * y*(1 - x) + img[indRu] * (1 - y)*x + img[indRd] * x*y;
		}
	}
}

unsigned char *Mat2ImgPointer(const Mat &src){
	/*�ú�����Mat��������ȡ����ת��Ϊһ��һά�����ݣ�
	�����src�����ǵ�ͨ�����Һ�����������Ϊunsigned char*/
	assert(src.channels() == 1);
	int row = src.rows;
	int col = src.cols;
	int i, j;
	unsigned char *pImg = (unsigned char*)malloc(sizeof(unsigned char)*row*col);
	assert(pImg != NULL);
	for (j = 0; j < row; j++){
		for (i = 0; i < col; i++){
			pImg[j*col + i] = src.at<unsigned char>(j, i);
		}
	}
	return pImg;//ע�ⷵ������Ϊunsigned char
}


Mat ImgPointer2Mat(const unsigned char *pImg, int imgR, int imgC){
	/*��һ��һά������ת��ΪMat,Mat��ͨ����Ϊ1*/
	Mat mat(Size(imgC, imgR), CV_8UC1);
	int i, j;
	for (j = 0; j < imgR; j++){
		for (i = 0; i < imgC; i++){
			mat.at<unsigned char>(j, i) = pImg[j*imgC + i];
		}
	}
	return mat;
}

//float sum; // this variable is used for nomalization
/*sum =0;
for (i = 0; i<NumOfBlockFeature; i++)
{
sum += BlockHog[i]*BlockHog[i];
}
sum = sqrt(sum);
for (i = 0; i<NumOfBlockFeature; i++)
{
BlockHog[i] = BlockHog[i]/(sum+0.001);
}*/