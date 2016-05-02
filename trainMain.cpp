#include"common.h"
#ifndef IS_TEST_CODE
#ifdef IS_TRAIN

#include"calHog.h"
#include"judge.h"
#include"getHardFromNeg.h"

class MySVM : public CvSVM{
public:
	//获得SVM的决策函数中的alpha数组  
	double * get_alpha_vector(){
		return this->decision_func->alpha;
	}
	//获得SVM的决策函数中的rho参数,即偏移量  
	float get_rho(){
		return this->decision_func->rho;
	}
};

extern uchar imgBin[MAX_SIZE_OF_INPUT_IMAGE];//用于存放计算的一幅图像的所有像素所属的bin的区间
extern float imgMag[MAX_SIZE_OF_INPUT_IMAGE];//用于存放计算的一幅图像的所有像素的梯度的幅度值
extern float HogFeature[NumOfHogFeature];

#define PosSamNO 2416    //正样本个数  
#define NegSamNO 12180    //负样本个数 
#define TestPosSamNO 2416 
#define TestNegSamNO 12180

#define INCLUDE_HARD true
#define HardExampleNO 27946   //-1

#define TRAIN true
#define GET_HARD false

string posTrainPath = "D:\\trainPics\\pos\\";
string negTrainPath = "D:\\trainPics\\neg\\";
string hardTrainPath = "D:\\trainPics\\hard\\";

string posNameTxt = "D:\\trainPics\\pos\\pos.txt";
string negNameTxt = "D:\\trainPics\\neg\\neg.txt";
string hardNameTxt = "D:\\trainPics\\hard\\hard.txt";

string detectorName = "D:\\testPics\\detectorFile\\HOGDetector3.2_neg_and_hard_3.txt";

int main()
{
	int DescriptorDim;
	MySVM svm;
	unsigned char *pImg;
	assert((TRAIN && !GET_HARD) || (!TRAIN && GET_HARD));//两个有且只能有一个为真
	//若TRAIN为true，重新训练分类器  
	if (TRAIN)
	{
		string ImgName;
		ifstream finPos(posNameTxt);
		ifstream finNeg(negNameTxt);

		Mat sampleFeatureMat = Mat::zeros(PosSamNO + NegSamNO + HardExampleNO, NumOfHogFeature, CV_32FC1);//所有训练样本的特征向量组成的矩阵，行数等于所有样本的个数，列数等于HOG描述子维数      
		Mat sampleLabelMat = Mat::zeros(PosSamNO + NegSamNO + HardExampleNO, 1, CV_32FC1);//训练样本的类别向量，行数等于所有样本的个数，列数等于1；1表示有人，-1表示无人  

		//依次读取正样本图片，生成HOG描述子  
		for (int num = 0; num < PosSamNO && getline(finPos, ImgName); num++){
			cout << "处理：" << ImgName << endl;
			ImgName = posTrainPath + ImgName;
			Mat src = imread(ImgName, 0);
			src = src(Rect(16, 16, 64, 128));
			assert(src.data != NULL&&src.rows == winR&&src.cols == winC);

			pImg = Mat2ImgPointer(src);
			calImgBin(pImg, winR, winC);
			calHog(0, 0);
			free(pImg);

			//将计算好的HOG描述子复制到样本特征矩阵sampleFeatureMat  
			for (int i = 0; i < NumOfHogFeature; i++)
				sampleFeatureMat.at<float>(num, i) = HogFeature[i];//第num个样本的特征向量中的第i个元素  
			sampleLabelMat.at<float>(num, 0) = 1;//正样本类别为1，有人 
		}

		//依次读取负样本图片，生成HOG描述子  
		for (int num = 0; num < NegSamNO && getline(finNeg, ImgName); num++){
			cout << "处理：" << ImgName << endl;
			ImgName = negTrainPath + ImgName;
			Mat src = imread(ImgName, 0);
			assert(src.data != NULL&&src.rows == winR&&src.cols == winC);

			pImg = Mat2ImgPointer(src);
			calImgBin(pImg, winR, winC);
			calHog(0, 0);
			free(pImg);

			//将计算好的HOG描述子复制到样本特征矩阵sampleFeatureMat  
			for (int i = 0; i < NumOfHogFeature; i++)
				sampleFeatureMat.at<float>(num + PosSamNO, i) = HogFeature[i];//第PosSamNO+num个样本的特征向量中的第i个元素  
			sampleLabelMat.at<float>(num + PosSamNO, 0) = -1;//负样本类别为-1，无人  
		}

		//处理HardExample负样本 
		if (INCLUDE_HARD && HardExampleNO > 0){
			ifstream finHardExample(hardNameTxt);
			//依次读取HardExample负样本图片，生成HOG描述子  
			for (int num = 0; num < HardExampleNO && getline(finHardExample, ImgName); num++)
			{
				cout << "处理：" << ImgName << endl;
				ImgName = hardTrainPath + ImgName;
				Mat src = imread(ImgName, 0);
				assert(src.data != NULL&&src.rows == winR&&src.cols == winC);

				pImg = Mat2ImgPointer(src);
				calImgBin(pImg, winR, winC);
				calHog(0, 0);
				free(pImg);

				//将计算好的HOG描述子复制到样本特征矩阵sampleFeatureMat  
				for (int i = 0; i < NumOfHogFeature; i++)
					sampleFeatureMat.at<float>(num + PosSamNO + NegSamNO, i) = HogFeature[i];//第PosSamNO+num个样本的特征向量中的第i个元素  
				sampleLabelMat.at<float>(num + PosSamNO + NegSamNO, 0) = -1;//负样本类别为-1，无人  
			}
		}

		//训练SVM分类器  		 
		CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 1000, FLT_EPSILON);//迭代终止条件，当迭代满1000次或误差小于FLT_EPSILON时停止迭代 	 
		CvSVMParams param(CvSVM::C_SVC, CvSVM::LINEAR, 0, 1, 0, 0.01, 0, 0, 0, criteria);//SVM参数：SVM类型为C_SVC；线性核函数；松弛因子C=0.01 
		//CvSVMParams param(CvSVM::C_SVC, CvSVM::RBF, 0, 1, 0, 1, 0, 0, 0, criteria);
		cout << "开始训练SVM分类器" << endl;
		svm.train(sampleFeatureMat, sampleLabelMat, Mat(), Mat(), param);//训练分类器   
		svm.save("SVM_HOG_INARA.xml");//将训练好的SVM模型保存为xml文件
		svm.load("SVM_HOG_INARA.xml");
		DescriptorDim = svm.get_var_count();//特征向量的维数，即HOG描述子的维数  
		int supportVectorNum = svm.get_support_vector_count();//支持向量的个数   
		Mat alphaMat = Mat::zeros(1, supportVectorNum, CV_32FC1);//alpha向量，长度等于支持向量个数  
		Mat supportVectorMat = Mat::zeros(supportVectorNum, DescriptorDim, CV_32FC1);//支持向量矩阵  
		Mat resultMat = Mat::zeros(1, DescriptorDim, CV_32FC1);//alpha向量乘以支持向量矩阵的结果  
		//将支持向量的数据复制到supportVectorMat矩阵中  
		for (int i = 0; i < supportVectorNum; i++){
			const float * pSVData = svm.get_support_vector(i);//返回第i个支持向量的数据指针  
			for (int j = 0; j < DescriptorDim; j++){
				supportVectorMat.at<float>(i, j) = pSVData[j];
			}
		}
		//将alpha向量的数据复制到alphaMat中  
		double * pAlphaData = svm.get_alpha_vector();//返回SVM的决策函数中的alpha向量  
		for (int i = 0; i < supportVectorNum; i++){
			alphaMat.at<float>(0, i) = pAlphaData[i];
		}
		resultMat = -1 * alphaMat * supportVectorMat;
		vector<float> myDetector;
		//将resultMat中的数据复制到数组myDetector中  
		for (int i = 0; i < DescriptorDim; i++){
			myDetector.push_back(resultMat.at<float>(0, i));
		}
		//最后添加偏移量rho，得到检测子  
		myDetector.push_back(svm.get_rho());
		cout << "检测子维数：" << myDetector.size() << endl;

		//保存检测子参数到文件  
		ofstream fout(detectorName);
		for (int i = 0; i < myDetector.size(); i++){
			fout << myDetector[i] << endl;
		}
		cout << "训练完成" << endl;


		//if (TRAIN){
		cout << "是否察看分类结果的好坏:是(y),否(n)" << endl;
		string s; cin >> s;
		if (s != "y"&&s != "Y")
			return 0;
		else
		{
			/**************察看分类结果的好坏******************/
			getDetector(detectorName);

			string imgName;
			ifstream filePos(posNameTxt);
			ifstream fileNeg(negNameTxt);
			int posCount = 0;
			int negCount = 0;

			//POS
			for (int num = 0; num < TestPosSamNO && getline(filePos, imgName); num++)
			{
				cout << "处理：" << imgName << endl;
				imgName = posTrainPath + imgName;
				Mat src = imread(imgName, 0);
				src = src(Rect(16, 16, 64, 128));
				assert(src.data != NULL&&src.rows == winR&&src.cols == winC);

				pImg = Mat2ImgPointer(src);
				calImgBin(pImg, winR, winC);
				calHog(0, 0);
				free(pImg);

				if (judge())
					posCount++;
				//_sleep(100);
			}

			//NEG
			for (int num = 0; num < TestNegSamNO && getline(fileNeg, imgName); num++)
			{
				cout << "处理：" << imgName << endl;
				imgName = negTrainPath + imgName;
				Mat src = imread(imgName, 0);
				assert(src.data != NULL&&src.rows == winR&&src.cols == winC);

				pImg = Mat2ImgPointer(src);
				calImgBin(pImg, winR, winC);
				calHog(0, 0);
				free(pImg);

				if (!judge())
					negCount++;
			}
			cout << "TestPosSamNO - posCount: " << TestPosSamNO << "-" << posCount
				<< "= " << TestPosSamNO - posCount << endl;

			cout << "TestNegSamNO - negCount: " << TestNegSamNO << "-" << negCount
				<< "= " << TestNegSamNO - negCount << endl;
		}

	}
	else if (GET_HARD){
		getNegFromHard(detectorName);
	}

	system("pause");
	return 0;
}

#endif
#endif