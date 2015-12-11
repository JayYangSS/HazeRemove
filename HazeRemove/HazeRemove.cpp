#include "HazeRemove.h"
using namespace std;

HazeRemove::HazeRemove(Mat img,int radius)
{
	srcImg = img;
	darkChannelImg.create(srcImg.rows, srcImg.cols, CV_8UC1);
	patchRadius = radius;
}


HazeRemove::~HazeRemove()
{
}

void HazeRemove::getDarkChannelPrior()
{
	int rows = srcImg.rows;
	int cols = srcImg.cols;
	int nchannels = srcImg.channels();
	Mat tmpMat=darkChannelImg.clone();

	//if the image is continus, transform it into 1-dimension to make the operation faster
	if (srcImg.isContinuous())
	{
		cols = cols*rows;
		rows = 1;
	}
	for (int i = 0; i < rows; i++)
	{
		uchar* srcData = srcImg.ptr<uchar>(i);
		uchar* tmpData = tmpMat.ptr<uchar>(i);
		for (int j = 0; j < cols; j++)
		{
			int colPos = j*nchannels;
			*tmpData = *srcData;
			//get the mimum value of R,G,B in each pixel
			for (int p = 1; p < nchannels; p++)
			{
				if (*tmpData>*(srcData + p))
					*tmpData = *(srcData + p);
			}
			tmpData++;
			srcData = srcData + nchannels;
		}
	}

	//minmum filter in the patch
	cols = tmpMat.cols;
	rows = tmpMat.rows;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			//set the patch size and position
			Rect rect;
			if (j - patchRadius < 0)
			{
				rect.x = 0;
				rect.width = j + patchRadius;
				if (i - patchRadius < 0)
				{
					rect.y = 0;
					rect.height = i + patchRadius;
				}
				else{
					rect.y = i - patchRadius;
					rect.height = i + patchRadius>=rows ? patchRadius + rows - i  : 2 * patchRadius + 1;
				}
			}
			else if (i - patchRadius <= 0)
			{
				rect.y = 0;
				rect.height = i + patchRadius;
				rect.x = j - patchRadius;
				rect.width = j + patchRadius>=cols ? patchRadius + cols - j  : 2 * patchRadius + 1;
			}
			else if (i + patchRadius >= rows)
			{
				rect.y = i - patchRadius;
				rect.height = patchRadius + rows - i ;
				rect.x = j - patchRadius;
				rect.width = j + patchRadius>=cols ? patchRadius + cols - j : 2 * patchRadius + 1;
			}
			else if (j + patchRadius >= cols)
			{
				rect.x = j - patchRadius;
				rect.width = j + patchRadius>=cols ? patchRadius + cols - j  : 2 * patchRadius + 1;
				rect.y = i - patchRadius;
				rect.height = 2 * patchRadius + 1;
			}
			else{
				rect.x = j - patchRadius;
				rect.y = i - patchRadius;
				rect.height = 2 * patchRadius + 1;
				rect.width = 2 * patchRadius + 1;
			}

			//find the minimum in patch
			Mat patchImg = tmpMat(rect);
			double minimum=0;
			minMaxLoc(patchImg,&minimum);

			//set the minimum to each pixel in the patch 
			for (int patch_y = rect.y; patch_y < rect.y+rect.height; patch_y++)
			{
				uchar* darkPriorData = darkChannelImg.ptr<uchar>(patch_y);
				for (int patch_x = rect.x; patch_x < rect.x+rect.width; patch_x++)
				{
					*(darkPriorData+patch_x) = (int)minimum;
				}
			}
		
		}
	}


	imshow("tmpMat", tmpMat);
	waitKey();
	imshow("darkChanel", darkChannelImg);
	waitKey();
}

void HazeRemove::getGlobalAtmosphericLight()
{

}