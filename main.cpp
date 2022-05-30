#include "BP_Net.h"
#include <iostream>
#include <fstream>

using namespace std;

//Getdata��ȡ������
class Getdata//���ļ���ȡ������
{
public:
    // ���txt�ļ���׼��������
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
        char* datas = (char*)buff.c_str();//datas=������ַ���ת��Ϊ���ַ�����
        const char* spilt = ",";
        // strtok�ַ�����ֺ���
        char* data = strtok_s(datas, spilt, &ptr);//ÿ������ , �Ͱ�ǰ������������data

        while (data != NULL) {
            // atof��stdlibͷ�ļ���ת���ַ���Ϊ���ֵĺ���
            res.push_back(atof(data));//atof���ַ���ת��Ϊdouble����
            // NULL������ϴ�û�����ط�������
            data = strtok_s(NULL, spilt, &ptr);
        }
    }

    input.close();
    return res;
}

vector<Sample> getTrianData();           // ���ļ���ȡѵ������ û��ȡ��ֱ���˳�
vector<Sample> getTestData();            // ���ļ���ȡ�������� û��ȡ��ֱ���˳�


vector<Sample> getTrianData() {//Iris-setosa=101  Iris-versicolor=102 Iris-virginica=103
    Getdata util;
    vector<double> trainData = util.getFileData("F://C++//irisff.data");
    //cout << trainData.size() << endl;
    if (trainData.size() == 0) {
        cout << "����ѵ������ʧ�ܣ�" << endl;
        exit(0);
    }

    int groups = trainData.size() / 5;//4������ֵ,1������,������101��102��103���зֱ�,groups������
    // ������������
    Sample trainInOut[200];//��������

    // ��vector���ø�����Sample
    for (int i = 0, index = 0; i < groups; i++) {
        for (int j = 0; j < 5; j++) {
            if (j % 5 != 4) {//������ֵ����in
                trainInOut[i].in.push_back(trainData[index++]);
            }
            else {//�����ַ���out
                trainInOut[i].out.push_back(trainData[index++]);
            }
        }
    }

    // ��ʼ��¼��ĸ�����
    return vector<Sample>(trainInOut, trainInOut + groups);
}

vector<Sample> getTestData() {//��������
    Getdata util;
    vector<double> testData = util.getFileData("F://C++//iristest.txt");
    //cout << testData.size() << endl;
    if (testData.size() == 0) {
        cout << "�����������ʧ�ܣ�" << endl;
        exit(0);
    }
    int groups = testData.size() / 4;//��������ֵ
    // ������������
    Sample testInOut[200];//groups

    for (int i = 0, index = 0; i < groups; i++) {
        for (int j = 0; j < 4; j++) {
            testInOut[i].in.push_back(testData[index++]);
        }
    }

    // ��ʼ������
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