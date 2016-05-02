#include"common.h"
#ifndef IS_TEST_CODE
#ifdef IS_TRAIN

#include"calHog.h"
#include"judge.h"
#include"getHardFromNeg.h"

class MySVM : public CvSVM{
public:
	//���SVM�ľ��ߺ����е�alpha����  
	double * get_alpha_vector(){
		return this->decision_func->alpha;
	}
	//���SVM�ľ��ߺ����е�rho����,��ƫ����  
	float get_rho(){
		return this->decision_func->rho;
	}
};

extern uchar imgBin[MAX_SIZE_OF_INPUT_IMAGE];//���ڴ�ż����һ��ͼ�����������������bin������
extern float imgMag[MAX_SIZE_OF_INPUT_IMAGE];//���ڴ�ż����һ��ͼ����������ص��ݶȵķ���ֵ
extern float HogFeature[NumOfHogFeature];

#define PosSamNO 2416    //����������  
#define NegSamNO 12180    //���������� 
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
	assert((TRAIN && !GET_HARD) || (!TRAIN && GET_HARD));//��������ֻ����һ��Ϊ��
	//��TRAINΪtrue������ѵ��������  
	if (TRAIN)
	{
		string ImgName;
		ifstream finPos(posNameTxt);
		ifstream finNeg(negNameTxt);

		Mat sampleFeatureMat = Mat::zeros(PosSamNO + NegSamNO + HardExampleNO, NumOfHogFeature, CV_32FC1);//����ѵ������������������ɵľ��������������������ĸ�������������HOG������ά��      
		Mat sampleLabelMat = Mat::zeros(PosSamNO + NegSamNO + HardExampleNO, 1, CV_32FC1);//ѵ����������������������������������ĸ�������������1��1��ʾ���ˣ�-1��ʾ����  

		//���ζ�ȡ������ͼƬ������HOG������  
		for (int num = 0; num < PosSamNO && getline(finPos, ImgName); num++){
			cout << "����" << ImgName << endl;
			ImgName = posTrainPath + ImgName;
			Mat src = imread(ImgName, 0);
			src = src(Rect(16, 16, 64, 128));
			assert(src.data != NULL&&src.rows == winR&&src.cols == winC);

			pImg = Mat2ImgPointer(src);
			calImgBin(pImg, winR, winC);
			calHog(0, 0);
			free(pImg);

			//������õ�HOG�����Ӹ��Ƶ�������������sampleFeatureMat  
			for (int i = 0; i < NumOfHogFeature; i++)
				sampleFeatureMat.at<float>(num, i) = HogFeature[i];//��num�����������������еĵ�i��Ԫ��  
			sampleLabelMat.at<float>(num, 0) = 1;//���������Ϊ1������ 
		}

		//���ζ�ȡ������ͼƬ������HOG������  
		for (int num = 0; num < NegSamNO && getline(finNeg, ImgName); num++){
			cout << "����" << ImgName << endl;
			ImgName = negTrainPath + ImgName;
			Mat src = imread(ImgName, 0);
			assert(src.data != NULL&&src.rows == winR&&src.cols == winC);

			pImg = Mat2ImgPointer(src);
			calImgBin(pImg, winR, winC);
			calHog(0, 0);
			free(pImg);

			//������õ�HOG�����Ӹ��Ƶ�������������sampleFeatureMat  
			for (int i = 0; i < NumOfHogFeature; i++)
				sampleFeatureMat.at<float>(num + PosSamNO, i) = HogFeature[i];//��PosSamNO+num�����������������еĵ�i��Ԫ��  
			sampleLabelMat.at<float>(num + PosSamNO, 0) = -1;//���������Ϊ-1������  
		}

		//����HardExample������ 
		if (INCLUDE_HARD && HardExampleNO > 0){
			ifstream finHardExample(hardNameTxt);
			//���ζ�ȡHardExample������ͼƬ������HOG������  
			for (int num = 0; num < HardExampleNO && getline(finHardExample, ImgName); num++)
			{
				cout << "����" << ImgName << endl;
				ImgName = hardTrainPath + ImgName;
				Mat src = imread(ImgName, 0);
				assert(src.data != NULL&&src.rows == winR&&src.cols == winC);

				pImg = Mat2ImgPointer(src);
				calImgBin(pImg, winR, winC);
				calHog(0, 0);
				free(pImg);

				//������õ�HOG�����Ӹ��Ƶ�������������sampleFeatureMat  
				for (int i = 0; i < NumOfHogFeature; i++)
					sampleFeatureMat.at<float>(num + PosSamNO + NegSamNO, i) = HogFeature[i];//��PosSamNO+num�����������������еĵ�i��Ԫ��  
				sampleLabelMat.at<float>(num + PosSamNO + NegSamNO, 0) = -1;//���������Ϊ-1������  
			}
		}

		//ѵ��SVM������  		 
		CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 1000, FLT_EPSILON);//������ֹ��������������1000�λ����С��FLT_EPSILONʱֹͣ���� 	 
		CvSVMParams param(CvSVM::C_SVC, CvSVM::LINEAR, 0, 1, 0, 0.01, 0, 0, 0, criteria);//SVM������SVM����ΪC_SVC�����Ժ˺������ɳ�����C=0.01 
		//CvSVMParams param(CvSVM::C_SVC, CvSVM::RBF, 0, 1, 0, 1, 0, 0, 0, criteria);
		cout << "��ʼѵ��SVM������" << endl;
		svm.train(sampleFeatureMat, sampleLabelMat, Mat(), Mat(), param);//ѵ��������   
		svm.save("SVM_HOG_INARA.xml");//��ѵ���õ�SVMģ�ͱ���Ϊxml�ļ�
		svm.load("SVM_HOG_INARA.xml");
		DescriptorDim = svm.get_var_count();//����������ά������HOG�����ӵ�ά��  
		int supportVectorNum = svm.get_support_vector_count();//֧�������ĸ���   
		Mat alphaMat = Mat::zeros(1, supportVectorNum, CV_32FC1);//alpha���������ȵ���֧����������  
		Mat supportVectorMat = Mat::zeros(supportVectorNum, DescriptorDim, CV_32FC1);//֧����������  
		Mat resultMat = Mat::zeros(1, DescriptorDim, CV_32FC1);//alpha��������֧����������Ľ��  
		//��֧�����������ݸ��Ƶ�supportVectorMat������  
		for (int i = 0; i < supportVectorNum; i++){
			const float * pSVData = svm.get_support_vector(i);//���ص�i��֧������������ָ��  
			for (int j = 0; j < DescriptorDim; j++){
				supportVectorMat.at<float>(i, j) = pSVData[j];
			}
		}
		//��alpha���������ݸ��Ƶ�alphaMat��  
		double * pAlphaData = svm.get_alpha_vector();//����SVM�ľ��ߺ����е�alpha����  
		for (int i = 0; i < supportVectorNum; i++){
			alphaMat.at<float>(0, i) = pAlphaData[i];
		}
		resultMat = -1 * alphaMat * supportVectorMat;
		vector<float> myDetector;
		//��resultMat�е����ݸ��Ƶ�����myDetector��  
		for (int i = 0; i < DescriptorDim; i++){
			myDetector.push_back(resultMat.at<float>(0, i));
		}
		//������ƫ����rho���õ������  
		myDetector.push_back(svm.get_rho());
		cout << "�����ά����" << myDetector.size() << endl;

		//�������Ӳ������ļ�  
		ofstream fout(detectorName);
		for (int i = 0; i < myDetector.size(); i++){
			fout << myDetector[i] << endl;
		}
		cout << "ѵ�����" << endl;


		//if (TRAIN){
		cout << "�Ƿ�쿴�������ĺû�:��(y),��(n)" << endl;
		string s; cin >> s;
		if (s != "y"&&s != "Y")
			return 0;
		else
		{
			/**************�쿴�������ĺû�******************/
			getDetector(detectorName);

			string imgName;
			ifstream filePos(posNameTxt);
			ifstream fileNeg(negNameTxt);
			int posCount = 0;
			int negCount = 0;

			//POS
			for (int num = 0; num < TestPosSamNO && getline(filePos, imgName); num++)
			{
				cout << "����" << imgName << endl;
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
				cout << "����" << imgName << endl;
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