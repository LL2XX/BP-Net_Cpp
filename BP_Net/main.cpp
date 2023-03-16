#include "BP_Net.h"
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

vector<Sample> trainInOut;//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
vector<Sample> testInOut;//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

//Getdataï¿½ï¿½È¡ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
class Getdata//ï¿½ï¿½ï¿½Ä¼ï¿½ï¿½ï¿½È¡ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
{
public:
    // ï¿½ï¿½ï¿½txtï¿½Ä¼ï¿½ï¿½ï¿½×¼ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
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
        char* datas = (char*)buff.c_str();//datas=ÊäÈëµÄ×Ö·û´®×ª»»ÎªµÄ×Ö·ûÊý×é
        const char* spilt = ",";
        // strtok×Ö·û´®²ð·Öº¯Êý
        const char* data = strtok_s(datas, spilt, &ptr);//Ã¿µ±Óö¼û , ¾Í°ÑÇ°ÃæÇÐÏÂÀ´·ÅÈëdata

        while (data != NULL) {
            if (strcmp(data, "Iris-setosa") == 0) {
                data = "0";
            }
            if (strcmp(data, "Iris-versicolor") == 0) {
                data = "1";
            }
            if (strcmp(data, "Iris-virginica") == 0) {
                data = "2";
            }
            // atofÊÇstdlibÍ·ÎÄ¼þÏÂ×ª»¯×Ö·û´®ÎªÊý×ÖµÄº¯Êý
            res.push_back(atof(data));//atof½«×Ö·û´®×ª»¯ÎªdoubleÀàÐÍ
            // NULL´ú±í´ÓÉÏ´ÎÃ»²ð·ÖÍêµØ·½¼ÌÐø²ð
            data = strtok_s(NULL, spilt, &ptr);
        }
    }

    input.close();
    return res;
}

double normalization(double min, double max, double val) {
    return ((val - min) / (max - min));
}

void getData() {//Iris-setosa=101  Iris-versicolor=102 Iris-virginica=103
    Sample train[200];
    Sample test[200];
    Getdata util;
    vector<double> trainData = util.getFileData("iris.data");
    //cout << trainData.size() << endl;
    if (trainData.size() == 0) {
        cout << "Error" << endl;
        exit(0);
    }

    int groups = trainData.size() / 5;//4ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Öµ,1ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½,ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½101ï¿½ï¿½102ï¿½ï¿½103ï¿½ï¿½ï¿½Ð·Ö±ï¿½,groupsï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
    // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

    double min[4], max[4];
    for (int i = 0; i < 4; i++)
    {
        min[i] = 10;
        max[i] = 0;
    }
    int index;
    int t1, t2;
    // ï¿½ï¿½vectorï¿½ï¿½ï¿½Ã¸ï¿½ï¿½ï¿½ï¿½ï¿½Sample
    for (int i = 0, index = 0, t1 = 0, t2 = 0; i < groups; i++) {
        if (i % 10 != 7) {
            for (int j = 0; j < 5; j++) {
                if (j % 5 != 4) {
                    train[t1].in.push_back(trainData[index]);
                    if (max[j] < trainData[index])max[j] = trainData[index];
                    if (min[j] > trainData[index])min[j] = trainData[index];
                    index++;
                }

                else {
                    if (trainData[index] == 0.0)
                    {
                        train[t1].out.push_back(1);
                        train[t1].out.push_back(0);
                        train[t1].out.push_back(0);
                    }
                    else if (trainData[index] == 1.0)
                    {
                        train[t1].out.push_back(0);
                        train[t1].out.push_back(1);
                        train[t1].out.push_back(0);
                    }
                    else if (trainData[index] == 2.0)
                    {
                        train[t1].out.push_back(0);
                        train[t1].out.push_back(0);
                        train[t1].out.push_back(1);
                    }
                    index++;
                }
            }
            t1++;
        }
        else {
            for (int j = 0; j < 5; j++) {
                if (j % 5 != 4) {
                    test[t2].in.push_back(trainData[index]);
                    if (max[j] < trainData[index])max[j] = trainData[index];
                    if (min[j] > trainData[index])min[j] = trainData[index];
                    index++;
                }

                else {//ï¿½ï¿½ï¿½ï¿½ï¿½Ö·ï¿½ï¿½ï¿½out
                    if (trainData[index] == 0.0)
                    {
                        test[t2].out.push_back(1);
                        test[t2].out.push_back(0);
                        test[t2].out.push_back(0);
                    }
                    else if (trainData[index] == 1.0)
                    {
                        test[t2].out.push_back(0);
                        test[t2].out.push_back(1);
                        test[t2].out.push_back(0);
                    }
                    else if (trainData[index] == 2.0)
                    {
                        test[t2].out.push_back(0);
                        test[t2].out.push_back(0);
                        test[t2].out.push_back(1);
                    }
                    index++;
                }
            }
            t2++;
        }
    }
    for (int i = 0; i < groups - 15; i++) {//å½’ä¸€åŒ?
        for (int j = 0; j < 5; j++) {
            if (j % 5 != 4) {
                train[i].in[j] = normalization(min[j], max[j], train[i].in[j]);
            }
        }
    }
    for (int i = 0; i < 15; i++) {//å½’ä¸€åŒ?
        for (int j = 0; j < 5; j++) {
            if (j % 5 != 4) {
                test[i].in[j] = normalization(min[j], max[j], test[i].in[j]);
            }
        }
    }
    for (int i = 0; i < 135; i++)
        trainInOut.push_back(train[i]);
    for (int i = 0; i < 15; i++)
        testInOut.push_back(test[i]);
    // ï¿½ï¿½Ê¼ï¿½ï¿½Â¼ï¿½ï¿½Ä¸ï¿½ï¿½ï¿½ï¿½ï¿?
}

int main()
{
	BP_Net bp;
    getData();
    bp.Train(trainInOut);
    bp.Evaluate(testInOut);
	return 0;
}