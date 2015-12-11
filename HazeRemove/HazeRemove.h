#pragma once
#include<opencv2\opencv.hpp>
using namespace cv;
class HazeRemove
{
private:
	Mat srcImg;
	Mat darkChannelImg;
	int patchRadius;
	int globalAtmosphericLight;
public:
	HazeRemove(Mat img,int radius);
	~HazeRemove();
	void getDarkChannelPrior();
	void getGlobalAtmosphericLight();
};

