#pragma once
#include<opencv2\opencv.hpp>
using namespace cv;
class HazeRemove
{
private:
	int patchRadius;
	int globalAtmosphericLight[3];
	float w0;
	static const float T0;
public:
	Mat srcImg;
	Mat darkChannelImg;
	Mat transmissionMap;
	Mat guidedfilterTransmissionMap;
	Mat estimatedTransmissionMap;
	//Mat hazeRemoveImg;
	HazeRemove(Mat img,int radius,float coffecient);
	~HazeRemove();
	void getDarkChannelPrior();
	void getTransmissionMap(bool isUseAverage=true);
	void getEstimatedTransmissionMap(bool isUseAverage);
	void getGlobalAtmosphericLight(bool isUseAverage);
	Mat getHazeRemoveImg(Mat transmissionMat);
	Mat guidedFilter(cv::Mat I, cv::Mat p, int r, double eps);
};


typedef struct{
	int posX;
	int posY;
	int val;
}Pixel;
