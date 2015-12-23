#include<iostream>
#include<opencv2\opencv.hpp>
#include"HazeRemove.h"

using namespace std;
using namespace cv;

void TestImg();
void TestvIdeo();
int main()
{
	TestImg();
	//TestvIdeo();
}

void TestImg()
{
	const float w0 = 0.95;
	Mat img = imread("TestImg//fog1.jpg");
	Mat gray;
	cvtColor(img, gray, CV_BGR2GRAY);


	HazeRemove hazeRomove(img,7,w0);
	hazeRomove.getDarkChannelPrior();
	hazeRomove.getEstimatedTransmissionMap();

	//hazeRomove.guidedFilter(gray, hazeRomove.estimatedTransmissionMap, 20, 0.001);
	//imshow("guidedfilterTransmissionMap", hazeRomove.guidedfilterTransmissionMap);
	//waitKey(5);
	imshow("estimatedTransmissionMap", hazeRomove.estimatedTransmissionMap);
	waitKey(5);
	hazeRomove.getTransmissionMap();
	imshow("TransmissionMap", hazeRomove.transmissionMap);
	waitKey(5);

	//hazeRomove.getTransmissionMap(true);
	Mat estimatedHazeRemoveImg = hazeRomove.getHazeRemoveImg(hazeRomove.estimatedTransmissionMap);
	Mat transmissionMapHazeRemove = hazeRomove.getHazeRemoveImg(hazeRomove.transmissionMap);
	//Mat filterGuidedHazeRemoveImg = hazeRomove.getHazeRemoveImg(hazeRomove.guidedfilterTransmissionMap);


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
	//imshow("filterGuidedHazeRemoveImg", filterGuidedHazeRemoveImg);
	imshow("tansmissionMapHazeRemove", transmissionMapHazeRemove);
	waitKey();
}

void TestvIdeo()
{
	VideoCapture cap;
	Mat frame;
	const int radius = 7;
	
	cap.open("D:\\JY\\JY_TrainingSamples\\changshu data\\goodLight.avi");
	if (!cap.isOpened())
	{
		cout << "can not open the video!" << endl;
		exit(-1);
	}

	while (cap.read(frame))
	{
		int start = cvGetTickCount();
		HazeRemove hazeRemove(frame, radius, 0.95);
		hazeRemove.getDarkChannelPrior();
		//hazeRemove.getTransmissionMap();
		//hazeRemove.getEstimatedTransmissionMap();
		//Mat defog = hazeRemove.getHazeRemoveImg(hazeRemove.estimatedTransmissionMap);
		imshow("defog", hazeRemove.darkChannelImg);
		waitKey(5);
		imshow("frame", frame);
		waitKey(5);
		int end = cvGetTickCount();
		float time = (float)(end - start) / (cvGetTickFrequency() * 1000000);
		cout << "process time:" << time << endl;
	}
}