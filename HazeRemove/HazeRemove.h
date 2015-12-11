#pragma once
#include<opencv2\opencv.hpp>
using namespace cv;
class HazeRemove
{
private:
	Mat srcImg;
	Mat darkChannelImg;
	Mat transmissionMap;
	int patchRadius;
	int globalAtmosphericLight[3];
public:
	HazeRemove(Mat img,int radius);
	~HazeRemove();
	void getDarkChannelPrior();
	void getGlobalAtmosphericLight(bool isUseAverage=true);
};


typedef struct{
	int posX;
	int posY;
	int val;
}Pixel;
