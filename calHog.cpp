#include "calHog.h"

static const uchar CellNumR = BlockR/CellR;  //一个块的列包含Cell的数目
static const uchar CellNumC = BlockC/CellC; 
static const uchar NumberOfBlockC = (winC - BlockC) / BlockStrideC + 1; //一个窗口的一行包含的Block的数目
static const uchar NumberOfBlockR = (winR - BlockR) / BlockStrideR + 1;
static const uchar NumOfBlockFeature = CellNumR*CellNumC*Bin;//一个块的特征数
static uint  NumOfHogFeatureTest = NumberOfBlockR*NumberOfBlockC*NumOfBlockFeature; //一个窗口内的总的特征数

static float CellHog[Bin] = {0};         //hog feature of a cell
static float BlockHog[CellNumC*CellNumR*Bin] = {0}; //hog feature of a block


/*其他文件也使用下面定义的变量*/
float HogFeature[NumOfHogFeature] = { 0 };     //all hog frature


extern intHistType intHist[802][802][Bin];

void calHog(uint startR, uint startC)
/*该函数用于在像素点的梯度的幅度
和角度值所属于的Bin已知的情况下求hog特征向量*/
{
	assert(BlockR % CellR == 0 && BlockC % CellC == 0 &&
		(winC - BlockC) % BlockStrideC == 0 &&
		(winR - BlockR) % BlockStrideR == 0);
	assert(NumOfHogFeatureTest == NumOfHogFeature);

	uchar br, bc, cc, cr; // br,bc 分别表示行和列上第几个Block； cc、cr则是Cell的
	uchar bin = 0;  uint i = 0;
	//uchar pixelCount = 0;
	uint cellCountBin = 0; //cellCount*Bin
	uint blockCount = 0;
	//uint bccb = 0; uchar y, x;	
	int byOfs, bxOfs, xOfs, yOfs, k;

	int pos1R, pos1C;
	int pos2R, pos2C;
	int pos3R, pos3C;
	int pos4R, pos4C;

	/*获取hog特征*/
	for (br = 0; br < NumberOfBlockR; br++){ //y方向上第br个block
		for (bc = 0; bc < NumberOfBlockC; bc++) //x方向上第bc个block
		{
			cellCountBin = 0;
			byOfs = br*BlockStrideR + startR;
			bxOfs = bc*BlockStrideC + startC;
			for (cr = 1; cr <= CellNumR; cr++){ //第cr个cell
				for (cc = 1; cc <= CellNumC; cc++) //第cc个cell
				{
					yOfs = byOfs + (cr - 1)*CellR;
					xOfs = bxOfs + (cc - 1)*CellC;

					pos1R = yOfs; pos1C = xOfs;
					pos2R = yOfs; pos2C = xOfs + CellC;
					pos3R = yOfs + CellR; pos3C = xOfs;
					pos4R = yOfs + CellR; pos4C = xOfs + CellC;

					for (k = 0; k < Bin; k++){
						CellHog[k] = intHist[pos1R][pos1C][k] + intHist[pos4R][pos4C][k] -
							intHist[pos2R][pos2C][k] - intHist[pos3R][pos3C][k];
						assert(CellHog[k]>=0);
					}
			
					// merge cellHog to blockHog
					memcpy(BlockHog + cellCountBin, CellHog, sizeof(CellHog));
					cellCountBin += Bin;
				}
			}
			/*for (int i = 0; i < NumOfBlockFeature; i++){
				cout << BlockHog[i] << " " << endl;
			}
			cout << endl;*/

			/*归一化一个block内的hog特征*/
			l2Norm(BlockHog, NumOfBlockFeature);
			/*获取所有的Hog特征*/
			memcpy(HogFeature + blockCount, BlockHog, sizeof(BlockHog));
			blockCount += NumOfBlockFeature;
		}
	}
}
