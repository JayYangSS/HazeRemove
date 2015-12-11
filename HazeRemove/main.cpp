#include<iostream>
#include<opencv2\opencv.hpp>
#include"HazeRemove.h"

using namespace std;
using namespace cv;

int main()
{
	Mat img = imread("test.jpg");
	HazeRemove hazeRomove(img,7);
	hazeRomove.getDarkChannelPrior();
	hazeRomove.getTransmissionMap(true);
	hazeRomove.getEstimatedTransmissionMap(true);

	imshow("src", img);
	waitKey(5);
	imshow("darkChanel", hazeRomove.darkChannelImg);
	waitKey(5);
	imshow("transmissionMap", hazeRomove.transmissionMap);
	waitKey(5);
	imshow("estimatedTransmissionMap", hazeRomove.estimatedTransmissionMap);
	waitKey();
}