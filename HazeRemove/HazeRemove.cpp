#include "HazeRemove.h"
using namespace std;

int compareMyType(const void * a, const void * b)
{
	if (((Pixel*)a)->val <  ((Pixel*)b)->val) return 1;
	if (((Pixel*)a)->val == ((Pixel*)b)->val) return 0;
	if (((Pixel*)a)->val >((Pixel*)b)->val) return -1;
}

const float HazeRemove::T0 = 0.1;

HazeRemove::HazeRemove(Mat img, int radius)
{
	srcImg = img;
	darkChannelImg.create(srcImg.rows, srcImg.cols, CV_8UC1);
	transmissionMap.create(srcImg.rows, srcImg.cols, CV_32F);
	estimatedTransmissionMap.create(srcImg.rows, srcImg.cols, CV_32F);
	patchRadius = radius;
	for (int i = 0; i < 3; i++)
	{
		globalAtmosphericLight[i] = 0;
	}
	
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
}

void HazeRemove::getTransmissionMap(bool isUseAverage)
{
	/*
	static float brightes_ratio = 0.001;
	static int rows = darkChannelImg.rows;
	static int cols = darkChannelImg.cols;
	 
	int brightestPixelNum = (int)(brightes_ratio*rows*cols);
	int count = 0;
	vector<Pixel> pixelContainer;
	if (darkChannelImg.isContinuous())
	{
		cols = cols*rows;
		rows = 1;
	}
	for (int i = 0; i < rows; i++)
	{
		uchar* srcData = darkChannelImg.ptr<uchar>(i);
		for (int j = 0; j < cols; j++)
		{
			Pixel p;
			p.posX = j;
			p.posY = i;
			p.val = *(srcData+j);
			if (count < brightestPixelNum)
			{
				pixelContainer.push_back(p);
				count++;
			}
			else{
				qsort(&pixelContainer[0], pixelContainer.size(), sizeof(Pixel), compareMyType);
				if (p.val>pixelContainer[brightestPixelNum-1].val)
					pixelContainer[brightestPixelNum-1] = p;
			}
		}
	}



	//get the globalAtmosphericLight in each channel
	float average[3] = { 0, 0, 0 };
	float max[3] = { 0, 0, 0 };
	int nChannels = srcImg.channels();
	for (int c = 0; c < nChannels; c++)
	{
		int sum = 0;
		for (int iter = 0; iter < brightestPixelNum; iter++)
		{
			int x = pixelContainer[iter].posX;
			int y = pixelContainer[iter].posY;
			uchar* fogImgData = srcImg.ptr<uchar>(y);
			sum += *(fogImgData + nChannels*y + c);
			if (max[c] < *(fogImgData + nChannels*y + c))
				max[c] = *(fogImgData + nChannels*y + c);
		}
		average[c] = (float)sum / (float)brightestPixelNum;
		if (isUseAverage)
			globalAtmosphericLight[c] = average[c];
		else
			globalAtmosphericLight[c] = max[c];
	}
	*/
	
	getGlobalAtmosphericLight(isUseAverage);
	const int rows = darkChannelImg.rows;
	const int cols = darkChannelImg.cols;
	const int nChannels = srcImg.channels();
	//get the transmission map
	for (int i = 0; i < rows; i++)
	{
		uchar* pdata = srcImg.ptr<uchar>(i);
		float* transmissinData = transmissionMap.ptr<float>(i);
		for (int j = 0; j < cols; j++)
		{
			float BVal = (float)*(pdata + j*nChannels);
			float GVal = (float)*(pdata + j*nChannels + 1);
			float RVal = (float)*(pdata + j*nChannels + 2);
			float tmp = BVal / globalAtmosphericLight[0] < GVal / globalAtmosphericLight[1] ? (BVal / globalAtmosphericLight[0]) : (GVal / globalAtmosphericLight[1]);
			float minVal = tmp < RVal / globalAtmosphericLight[2] ? tmp : (RVal / globalAtmosphericLight[2]);
			*transmissinData++ = (1 - minVal <= T0) ? T0 : (1 - minVal);//minVal may be larger than 1
		}
	}
}

void HazeRemove::getGlobalAtmosphericLight(bool isUseAverage)
{
	const float brightes_ratio = 0.001;
	int rows = darkChannelImg.rows;
	int cols = darkChannelImg.cols;

	int brightestPixelNum = (int)(brightes_ratio*rows*cols);
	int count = 0;
	vector<Pixel> pixelContainer;
	if (darkChannelImg.isContinuous())
	{
		cols = cols*rows;
		rows = 1;
	}
	for (int i = 0; i < rows; i++)
	{
		uchar* srcData = darkChannelImg.ptr<uchar>(i);
		for (int j = 0; j < cols; j++)
		{
			Pixel p;
			p.posX = j;
			p.posY = i;
			p.val = *(srcData + j);
			if (count < brightestPixelNum)
			{
				pixelContainer.push_back(p);
				count++;
			}
			else{
				qsort(&pixelContainer[0], pixelContainer.size(), sizeof(Pixel), compareMyType);
				if (p.val>pixelContainer[brightestPixelNum - 1].val)
					pixelContainer[brightestPixelNum - 1] = p;
			}
		}
	}

	//get the globalAtmosphericLight in each channel
	float average[3] = { 0, 0, 0 };
	float max[3] = { 0, 0, 0 };
	int nChannels = srcImg.channels();
	for (int c = 0; c < nChannels; c++)
	{
		int sum = 0;
		for (int iter = 0; iter < brightestPixelNum; iter++)
		{
			int x = pixelContainer[iter].posX;
			int y = pixelContainer[iter].posY;
			uchar* fogImgData = srcImg.ptr<uchar>(y);
			sum += *(fogImgData + nChannels*y + c);
			if (max[c] < *(fogImgData + nChannels*y + c))
				max[c] = *(fogImgData + nChannels*y + c);
		}
		average[c] = (float)sum / (float)brightestPixelNum;
		if (isUseAverage)
			globalAtmosphericLight[c] = average[c];
		else
			globalAtmosphericLight[c] = max[c];
	}
}

void HazeRemove::getEstimatedTransmissionMap(bool isUseAverage)
{
	getGlobalAtmosphericLight(isUseAverage);
	const int rows = darkChannelImg.rows;
	const int cols = darkChannelImg.cols;

	//get the transmission map
	for (int i = 0; i < rows; i++)
	{
		uchar* pdata = darkChannelImg.ptr<uchar>(i);
		float* transmissinData = estimatedTransmissionMap.ptr<float>(i);
		for (int j = 0; j < cols; j++)
		{
			float Val = (float)*(pdata + j);
			//get the max
			float maxValOfGlobalAtmosphericLight = (globalAtmosphericLight[0] < globalAtmosphericLight[1]) ? globalAtmosphericLight[1] : globalAtmosphericLight[0];
			maxValOfGlobalAtmosphericLight = (maxValOfGlobalAtmosphericLight < globalAtmosphericLight[2]) ? globalAtmosphericLight[2] : maxValOfGlobalAtmosphericLight;

			float normalizedVal = Val / maxValOfGlobalAtmosphericLight;
			*transmissinData++ = (1 - normalizedVal <= T0) ? T0 : (1 - normalizedVal);//minVal may be larger than 1
		}
	}
}

void HazeRemove::getHazeRemoveImg()
{

}