#include "BP_Net.h"
#include <iostream>
#include <fstream>

using namespace std;

//Getdata获取数据类
class Getdata//从文件获取数据类
{
public:
    // 获得txt文件中准备的数据
    std::vector<double> getFileData(const char* fileName);
};

vector<double> Getdata::getFileData(const char* fileName) {
    vector<double> res;

    ifstream input(fileName);
    if (!input) {
        return res;
    }

    string buff;
    char* ptr = NULL;
    while (getline(input, buff)) {
        char* datas = (char*)buff.c_str();//datas=输入的字符串转换为的字符数组
        const char* spilt = ",";
        // strtok字符串拆分函数
        char* data = strtok_s(datas, spilt, &ptr);//每当遇见 , 就把前面切下来放入data

        while (data != NULL) {
            // atof是stdlib头文件下转化字符串为数字的函数
            res.push_back(atof(data));//atof将字符串转化为double类型
            // NULL代表从上次没拆分完地方继续拆
            data = strtok_s(NULL, spilt, &ptr);
        }
    }

    input.close();
    return res;
}

vector<Sample> getTrianData();           // 从文件获取训练数据 没获取到直接退出
vector<Sample> getTestData();            // 从文件获取测试数据 没获取到直接退出


vector<Sample> getTrianData() {//Iris-setosa=101  Iris-versicolor=102 Iris-virginica=103
    Getdata util;
    vector<double> trainData = util.getFileData("F://C++//irisff.data");
    //cout << trainData.size() << endl;
    if (trainData.size() == 0) {
        cout << "载入训练数据失败！" << endl;
        exit(0);
    }

    int groups = trainData.size() / 5;//4个特征值,1个名称,名称用101，102，103进行分别,groups个样本
    // 创建样本数据
    Sample trainInOut[200];//样本数组

    // 把vector设置给样本Sample
    for (int i = 0, index = 0; i < groups; i++) {
        for (int j = 0; j < 5; j++) {
            if (j % 5 != 4) {//将特征值放入in
                trainInOut[i].in.push_back(trainData[index++]);
            }
            else {//将名字放入out
                trainInOut[i].out.push_back(trainData[index++]);
            }
        }
    }

    // 初始化录入的个数据
    return vector<Sample>(trainInOut, trainInOut + groups);
}

vector<Sample> getTestData() {//测试数据
    Getdata util;
    vector<double> testData = util.getFileData("F://C++//iristest.txt");
    //cout << testData.size() << endl;
    if (testData.size() == 0) {
        cout << "载入测试数据失败！" << endl;
        exit(0);
    }
    int groups = testData.size() / 4;//几个特征值
    // 创建测试数据
    Sample testInOut[200];//groups

    for (int i = 0, index = 0; i < groups; i++) {
        for (int j = 0; j < 4; j++) {
            testInOut[i].in.push_back(testData[index++]);
        }
    }

    // 初始化数据
    return vector<Sample>(testInOut, testInOut + groups);
}

int main()
{
	BP_Net bp;
    vector<Sample> trainspl = { { {0.8,0.6,0.5,0.2}, {1,0,0} } };
	bp.InputSample({ {0.8,0.6,0.5,0.2}, {1,0,0} });
    bp.Train(trainspl);
	return 0;
}