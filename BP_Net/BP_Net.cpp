#include "BP_Net.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
int BP_Net::Compare()
{
	double maxn = 0, maxm = 0;
	int weizhi, weizhi1;
	for (int i = 0; i < output.size(); i++)
	{
		if (maxn < output[i].value) {
			maxn = output[i].value;
			weizhi = i;
		}
		if (maxm < output[i].y) {
			maxm = output[i].y;
			weizhi1 = i;
		}
	}
	if (weizhi == weizhi1) return 1;
	return 0;
}

void BP_Net::Train(vector<Sample> sampleGroup)
{
	for (int c = 0; c < Epoch; c++)
	{
		int trainsize = sampleGroup.size();
		for (int i = 0; i <= 100; i++) {
			int a1 = rand() % trainsize;
			int a2 = rand() % trainsize;
			swap(sampleGroup[a1], sampleGroup[a2]);
		}
		for (auto ite = sampleGroup.begin(); ite != sampleGroup.end(); ite++)
		{
			InputSample(*ite);
			Forward();
			Backward();
			Update();
		}
	}
}

void BP_Net::Evaluate(vector<Sample> sampleGroup)
{
	int accsum = 0;
	int sumnum = sampleGroup.size();
	for (auto ite = sampleGroup.begin(); ite != sampleGroup.end(); ite++)
	{
		InputSample(*ite);
		Forward();
		if (Compare()) {
			accsum++;
		}
	}
	cout << "acc_rate:" << accsum*1.0 / sumnum << endl;
}

void BP_Net::Predict(vector<Sample> sampleGroup)
{
	for (auto ite = sampleGroup.begin(); ite != sampleGroup.end(); ite++)
	{
		InputSample(*ite);
		Forward();
		//Output();
	}
}

void BP_Net::Forward()
{
	for (int layer = 0; layer < hide.size(); layer++)
	{
		if (layer == 0)
		{
			for (int i = 0; i < hide[layer].size(); i++)
			{
				double sum = 0;
				for (int j = 0; j < input.size(); j++)
					sum += input[j].value * input[j].weight[i];
				hide[layer][i].value = sigmoid(sum + hide[layer][i].bias);
			}
		}
		else
		{
			for (int i = 0; i < hide[layer].size(); i++)
			{
				double sum = 0;
				for (int j = 0; j < hide[layer - 1].size(); j++)
					sum += hide[layer - 1][j].value * hide[layer - 1][j].weight[i];
				hide[layer][i].value = sigmoid(sum + hide[layer][i].bias);
			}
		}
	}
	for (int i = 0; i < output.size(); i++)
	{
		double sum = 0;
		for (int j = 0; j < hide[hide.size() - 1].size(); j++)
			sum += hide[hide.size() - 1][j].value * hide[hide.size() - 1][j].weight[i];
		output[i].value = sigmoid(sum + output[i].bias);
	}
}
void BP_Net::Backward()
{
	for (int i = 0; i < output.size(); i++)
	{
		output[i].delta = (output[i].value - output[i].y) * output[i].value * (1 - output[i].value);
	}
	for (int i = hide.size() - 1; i >= 0; i--)
	{
		if (i == hide.size() - 1)
		{
			for (int j = hide[i].size() - 1; j >= 0; j--)
			{
				double sum = 0;
				for (int k = 0; k < output.size(); k++)
					sum += hide[i][j].weight[k] * output[k].delta;
				hide[i][j].delta = hide[i][j].value * (1 - hide[i][j].value) * sum;
			}
		}
		else
		{
			for (int j = hide[i].size() - 1; j >= 0; j--)
			{
				double sum = 0;
				for (int k = 0; k < hide[i+1].size(); k++)
					sum += hide[i][j].weight[k] * hide[i+1][k].delta;
				hide[i][j].delta = hide[i][j].value * (1 - hide[i][j].value) * sum;
			}
		}
	}
}
void BP_Net::Update()
{
	for (int i = 0; i < input.size(); i++)
		for (int j = 0; j < hide[0].size(); j++)
			input[i].weight[j] -= LR * input[i].value * hide[0][j].delta;

	for (int layer = 0; layer < hide.size(); layer++)
	{
		if (layer == hide.size()-1)
		{
			for (int i = 0; i < hide[layer].size(); i++)
			{
				for (int j = 0; j < hide[layer][i].weight.size(); j++)
					hide[layer][i].weight[j] -= LR * hide[layer][i].value * output[j].delta;
				hide[layer][i].bias -= LR * hide[layer][i].delta;
			}
		}
		else
		{
			for (int i = 0; i < hide[layer].size(); i++)
			{
				for (int j = 0; j < hide[layer][i].weight.size(); j++)
					hide[layer][i].weight[j] -= LR * hide[layer][i].value * hide[layer + 1][j].delta;
				hide[layer][i].bias -= LR * hide[layer][i].delta;
			}
		}
	}
}
BP_Net::BP_Net(int innum,int hidnum,int layer,int outnum)
{
	srand((unsigned)time(0));
	for (int i = 0; i < innum; i++)
	{
		inNode node;
		node.value = 0;
		for (int j = 0; j < hidnum; j++)
			node.weight.push_back((2.0 * (double)rand() / RAND_MAX) - 1);
		input.push_back(node);
	}
	for (int k = 0; k < layer; k++)
	{
		if (k == layer - 1)
		{
			vector<hidNode> hid;
			for (int i = 0; i < hidnum; i++)
			{
				hidNode node;
				node.value = 0;
				node.bias = 0;
				for (int j = 0; j < outnum; j++)
					node.weight.push_back((2.0 * (double)rand() / RAND_MAX) - 1);
				hid.push_back(node);
			}
			hide.push_back(hid);
		}
		else
		{
			vector<hidNode> hid;
			for (int i = 0; i < hidnum; i++)
			{
				hidNode node;
				node.value = 0;
				node.bias = 0;
				for (int j = 0; j < hidnum; j++)
					node.weight.push_back((2.0 * (double)rand() / RAND_MAX) - 1);
				hid.push_back(node);
			}
			hide.push_back(hid);
		}
	}
	for (int i = 0; i < outnum; i++)
	{
		outNode node;
		node.bias = 0;
		node.value = 0;
		output.push_back(node);
	}
}
void BP_Net::InputSample(Sample s)
{
	for (int i = 0; i < s.in.size(); i++)
	{
		input[i].value = s.in[i];
	}
	for (int i = 0; i < s.out.size(); i++)
	{
		output[i].y = s.out[i];
	}
}