#include<iostream>
#include<opencv2\opencv.hpp>
#include"HazeRemove.h"

using namespace std;
using namespace cv;

int main()
{
	const float w0 = 0.95;
	Mat img = imread("TestImg//fog1.jpg");
	Mat gray;
	cvtColor(img, gray, CV_BGR2GRAY);


	HazeRemove hazeRomove(img,7,w0);
	hazeRomove.getDarkChannelPrior();
	hazeRomove.getEstimatedTransmissionMap(true);
	
	hazeRomove.guidedFilter(gray, hazeRomove.estimatedTransmissionMap, 20, 0.001);
	imshow("guidedfilterTransmissionMap", hazeRomove.guidedfilterTransmissionMap);
	waitKey(5);
	imshow("estimatedTransmissionMap", hazeRomove.estimatedTransmissionMap);
	waitKey(5);

	//hazeRomove.getTransmissionMap(true);
	Mat estimatedHazeRemoveImg = hazeRomove.getHazeRemoveImg(hazeRomove.estimatedTransmissionMap);
	Mat filterGuidedHazeRemoveImg = hazeRomove.getHazeRemoveImg(hazeRomove.guidedfilterTransmissionMap);
	

	imshow("src", img);
	waitKey(5);
	imshow("darkChanel", hazeRomove.darkChannelImg);
	waitKey(5);
	//imshow("transmissionMap", hazeRomove.transmissionMap);
	//waitKey(5);
	imshow("estimatedTransmissionMap", hazeRomove.estimatedTransmissionMap);
	waitKey(5);
	imshow("estimatedHazeRemoveImg", estimatedHazeRemoveImg);
	waitKey(5);
	imshow("filterGuidedHazeRemoveImg", filterGuidedHazeRemoveImg);
	waitKey();
}