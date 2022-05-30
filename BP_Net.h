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
} inNode; //输入层每一个节点

typedef struct hidNode {
	double value, delta, bias;
	vector<double> weight;
} hidNode;

typedef struct outNode {
	double value, delta, bias, y;
} outNode; //输出层每一个节点

typedef struct sample {
	vector<double> in, out;
} Sample;

class BP_Net {
	vector<inNode> input; //一个样本的所有输入
	vector<outNode> output; //一个样本的所有输出
	vector<vector<hidNode>> hide;
	double sigmoid(double z) { return 1 / (1 + exp(-z)); }
public:
	BP_Net(int innum = 4, int hidnum = 5, int layer = 2, int outnum = 3);
	void InputSample(Sample);//读入文件
	void Backward();
	void Forward();
	void Update();
	void Train(vector<Sample> sampleGroup);
	void Predict(vector<Sample> sampleGroup);
	void Output();
};

