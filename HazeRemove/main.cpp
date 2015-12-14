#include<iostream>
#include<opencv2\opencv.hpp>
#include"HazeRemove.h"

using namespace std;
using namespace cv;

int main()
{
	const float w0 = 0.95;
	Mat img = imread("TestImg//test1.jpg");
	HazeRemove hazeRomove(img,7,w0);
	hazeRomove.getDarkChannelPrior();
	hazeRomove.getTransmissionMap(true);
	hazeRomove.getEstimatedTransmissionMap(true);
	hazeRomove.getHazeRemoveImg();

	imshow("src", img);
	waitKey(5);
	imshow("darkChanel", hazeRomove.darkChannelImg);
	waitKey(5);
	imshow("transmissionMap", hazeRomove.transmissionMap);
	waitKey(5);
	imshow("estimatedTransmissionMap", hazeRomove.estimatedTransmissionMap);
	waitKey(5);
	imshow("hazeRemoveImg", hazeRomove.hazeRemoveImg);
	waitKey();
}