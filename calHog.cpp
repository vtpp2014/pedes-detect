#include "calHog.h"

static const uchar CellNumR = BlockR/CellR;  //һ������а���Cell����Ŀ
static const uchar CellNumC = BlockC/CellC; 
static const uchar NumberOfBlockC = (winC - BlockC) / BlockStrideC + 1; //һ�����ڵ�һ�а�����Block����Ŀ
static const uchar NumberOfBlockR = (winR - BlockR) / BlockStrideR + 1;
static const uchar NumOfBlockFeature = CellNumR*CellNumC*Bin;//һ�����������
static uint  NumOfHogFeatureTest = NumberOfBlockR*NumberOfBlockC*NumOfBlockFeature; //һ�������ڵ��ܵ�������

static float CellHog[Bin] = {0};         //hog feature of a cell
static float BlockHog[CellNumC*CellNumR*Bin] = {0}; //hog feature of a block


/*�����ļ�Ҳʹ�����涨��ı���*/
float HogFeature[NumOfHogFeature] = { 0 };     //all hog frature


extern intHistType intHist[802][802][Bin];

void calHog(uint startR, uint startC)
/*�ú������������ص���ݶȵķ���
�ͽǶ�ֵ�����ڵ�Bin��֪���������hog��������*/
{
	assert(BlockR % CellR == 0 && BlockC % CellC == 0 &&
		(winC - BlockC) % BlockStrideC == 0 &&
		(winR - BlockR) % BlockStrideR == 0);
	assert(NumOfHogFeatureTest == NumOfHogFeature);

	uchar br, bc, cc, cr; // br,bc �ֱ��ʾ�к����ϵڼ���Block�� cc��cr����Cell��
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

	/*��ȡhog����*/
	for (br = 0; br < NumberOfBlockR; br++){ //y�����ϵ�br��block
		for (bc = 0; bc < NumberOfBlockC; bc++) //x�����ϵ�bc��block
		{
			cellCountBin = 0;
			byOfs = br*BlockStrideR + startR;
			bxOfs = bc*BlockStrideC + startC;
			for (cr = 1; cr <= CellNumR; cr++){ //��cr��cell
				for (cc = 1; cc <= CellNumC; cc++) //��cc��cell
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

			/*��һ��һ��block�ڵ�hog����*/
			l2Norm(BlockHog, NumOfBlockFeature);
			/*��ȡ���е�Hog����*/
			memcpy(HogFeature + blockCount, BlockHog, sizeof(BlockHog));
			blockCount += NumOfBlockFeature;
		}
	}
}
