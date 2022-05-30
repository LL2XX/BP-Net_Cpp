#pragma once
#include <vector>
#include <cmath>
#include <string>
#define LR 0.01
#define Epoch 10000
#define Batch 64
using namespace std;

typedef struct inNode {
	double value;
	vector<double> weight;
} inNode; //�����ÿһ���ڵ�

typedef struct hidNode {
	double value, delta, bias;
	vector<double> weight;
} hidNode;

typedef struct outNode {
	double value, delta, bias, y;
} outNode; //�����ÿһ���ڵ�

typedef struct sample {
	vector<double> in, out;
} Sample;

class BP_Net {
	vector<inNode> input; //һ����������������
	vector<outNode> output; //һ���������������
	vector<vector<hidNode>> hide;
	double sigmoid(double z) { return 1 / (1 + exp(-z)); }
public:
	BP_Net(int innum = 4, int hidnum = 5, int layer = 2, int outnum = 3);
	void InputSample(Sample);//�����ļ�
	void Backward();
	void Forward();
	void Update();
	void Train(vector<Sample> sampleGroup);
	void Predict(vector<Sample> sampleGroup);
	void Output();
};

