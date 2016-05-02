#include"calHog.h"

intHistType intHist[MAX_ROWS_OF_INPUT_IMAGE + 1][MAX_COLS_OF_INPUT_IMAGE + 1][Bin] = { 0 };

void constructIntegralHist(const uchar *imgBin, const imgMagType *imgMag, uint imgR, uint imgC){

	int i, j, k, pos;
	memset(intHist, 0, sizeof(intHist));
	//第一列的
	for (j = 0+1; j < imgR+1; j++){
		intHist[j][1][imgBin[(j-1)*imgC]] = imgMag[(j-1)*imgC];
	}
	//每一行的行累加值
	for (j = 0+1; j < imgR+1; j++){
		for (i = 1+1; i < imgC+1; i++){
			memcpy(&intHist[j][i], &intHist[j][i - 1], sizeof(intHistType)*Bin);
			pos = (j-1)*imgC + i-1;
			intHist[j][i][imgBin[pos]] += imgMag[pos];
		}
	}

	//积分图像值
	for (i = 0+1; i < imgC+1; i++){
		for (j = 1+1; j < imgR+1; j++){
			for (k = 0; k < Bin; k++){
				intHist[j][i][k] += intHist[j - 1][i][k];
			}
		}
	}
}


//void constructIntegralHist(const uchar *imgBin, const imgMagType *imgMag, uint imgR, uint imgC){
//	assert(imgR < 801 && imgC < 801);
//	int i, j, k, pos;
//	memset(intHist, 0, sizeof(intHist));
//	//第一列的
//	for (j = 0; j < imgR; j++){
//		intHist[j][0][imgBin[j*imgC]] = imgMag[j*imgC];
//	}
//	//每一行的行累加值
//	for (j = 0; j < imgR; j++){
//		for (i = 1; i < imgC; i++){
//			memcpy(&intHist[j][i], &intHist[j][i - 1], sizeof(intHistType)*Bin);
//			pos = j*imgC + i;
//			intHist[j][i][imgBin[pos]] += imgMag[pos];
//		}
//	}
//
//	//积分图像值
//	for (i = 0; i < imgC; i++){
//		for (j = 1; j < imgR; j++){
//			for (k = 0; k < Bin; k++){
//				intHist[j][i][k] += intHist[j - 1][i][k];
//			}
//		}
//	}
//}