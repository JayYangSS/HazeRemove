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
	system("wait");
}