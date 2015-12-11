#pragma once
#include<opencv2\opencv.hpp>
using namespace cv;
class HazeRemove
{
private:
	int patchRadius;
	int globalAtmosphericLight[3];
	static const float T0;
public:
	Mat srcImg;
	Mat darkChannelImg;
	Mat transmissionMap;
	Mat estimatedTransmissionMap;
	HazeRemove(Mat img,int radius);
	~HazeRemove();
	void getDarkChannelPrior();
	void getTransmissionMap(bool isUseAverage=true);
	void getEstimatedTransmissionMap(bool isUseAverage);
	void getGlobalAtmosphericLight(bool isUseAverage);
	void getHazeRemoveImg();
};


typedef struct{
	int posX;
	int posY;
	int val;
}Pixel;
