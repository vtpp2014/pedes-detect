#include "circlePerson.h"

static bool similarRect(const Rect& r1, const Rect& r2, float weight);
int getRoot(int x, int *father);
void  merge(int x, int y, int *father);

vector<Rect> mergeRect(const vector<Rect>& _vec, int groupThreshold, float weight){

	int i, j, N = (int)_vec.size();
	const Rect* vec = &_vec[0];
	vector<Rect> rectFinalRes;

	int *father = (int*)calloc(N, sizeof(int));
	
	// init
	for (i = 0; i < N; ++i)
		father[i] = i;

	//if similar, merge.
	for (i = 0; i < N; ++i){
		for (j = i + 1; j < N; j++){
			if (similarRect(vec[i], vec[j], weight))
				merge(i, j, father);
		}
	}

	//һ�����ٸ���
	int nclass = 0;
	int *className = (int *)calloc(N,sizeof(int));
	for (int i = 0; i < N; i++){
		if (father[i] == i){
			className[nclass++] = i;
		}
	}

	Rect *vecRst;
	int *rectCount;
	vecRst = (Rect*)calloc(nclass, sizeof(Rect));
	rectCount = (int*)calloc(nclass, sizeof(int));

	int k = 0;

	/*��������ͬ�����ֵ�����������ֵ����������groupThreshold����*/
	for (i = 0; i < nclass; i++){
		rectCount[k] = 0;
		memset(&vecRst[k], 0, sizeof(Rect));
		for (j = 0; j < N; j++){
			if (getRoot(j,father) == className[i]){
				rectCount[k]++;
				vecRst[k].x += vec[j].x;
				vecRst[k].y += vec[j].y;
				vecRst[k].width += vec[j].width;
				vecRst[k].height += vec[j].height;
			}
		}
		if (rectCount[k] >= groupThreshold){
			vecRst[k].x /= rectCount[k];
			vecRst[k].y /= rectCount[k];
			vecRst[k].width /= rectCount[k];
			vecRst[k].height /= rectCount[k];
			k++;
		}
	}

	/*��Ƕ�ڴ���ο��ڲ���С���ο���˵�,
     ���ʣ�µľ��ο�Ϊ����Ľ��*/
	for (i = 0; i < k; i++){
		for (j = 0; j < k; j++){
			if (i != j && vecRst[i].x >= vecRst[j].x && vecRst[i].y >= vecRst[j].y &&
				vecRst[i].x + vecRst[i].width <= vecRst[j].x + vecRst[j].width  &&
				vecRst[i].y + vecRst[i].height <= vecRst[j].y + vecRst[j].height)
				break;
		}
		if (j == k)
			rectFinalRes.push_back(vecRst[i]);
	}

	free(father);
	free(vecRst);
	free(rectCount);
	return rectFinalRes;

	///*��Ƕ�ڴ���ο��ڲ���С���ο���˵�,
	//���ʣ�µľ��ο�Ϊ����Ľ��*/
	//for (int i = 0; i < rectRst2.size(); i++)
	//{
	//	Rect r = rectRst2[i];

	//	int j = 0;
	//	for (; j < rectRst2.size(); j++)
	//	if (i != j &&
	//		r.x >= rectRst2[j].x  &&
	//		r.y >= rectRst2[j].y  &&
	//		r.x + r.width <= rectRst2[j].x + rectRst2[j].width  &&
	//		r.y + r.height <= rectRst2[j].y + rectRst2[j].height)
	//		break;
	//	if (j == rectRst2.size())
	//		rectFinalRst.push_back(r);
	//}

}


bool similarRect(const Rect& r1, const Rect& r2, float weight)
/*�ú����ж��������ο��Ƿ����ƣ����Ƿ���Ի���Ϊͬһ��*/
{
	// deltaΪ��С�����eps*0.5��
	double eps = weight;
	double delta = eps*(std::min(r1.width, r2.width) + std::min(r1.height, r2.height))*0.5;
	// ������ε��ĸ������λ�ò��С��delta�����ʾ���Ƶľ���
	return std::abs(r1.x - r2.x) <= delta &&
		std::abs(r1.y - r2.y) <= delta &&
		std::abs(r1.x + r1.width - r2.x - r2.width) <= delta &&
		std::abs(r1.y + r1.height - r2.y - r2.height) <= delta;
}

int getRoot(int x, int *father)
{
	//return father[x]==x? x:getRoot(father[x]);
	/*���鼯��һ���Ż�������·��ѹ���������ڲ��鼯ִ�в�ѯʱ�Ծ����ĵ���С���ƽ�����ķ�����
	ʹ���´β�ѯʱ����Ҫ�����м�ڵ㣬��������*/
	if (father[x] != x) father[x] = getRoot(father[x], father);
	return father[x];
}

void  merge(int x, int y, int *father)
{
	int rx = getRoot(x, father);
	int ry = getRoot(y, father);
	if (rx != ry)
		father[ry] = x;
}