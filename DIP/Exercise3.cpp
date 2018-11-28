#include "stdafx.h"

int exercise3()
{
	cv::Mat original = cv::imread("images/lena.png", CV_LOAD_IMAGE_COLOR); // load color image from file system to Mat variable, this will be loaded using 8 bits (uchar)

	if (original.empty()) 
	{
		printf("Unable to read input file (%s, %d).", __FILE__, __LINE__);
	}

	cv::imshow("LENA original", original);

	cv::Mat box(original.rows, original.cols, CV_8UC3);

	const int size1 = 3;
	int mask1 [size1][size1]= { {1,1,1},{1,1,1},{1,1,1} };

	int halfSize1 = size1 / 2;

	for (int y = halfSize1; y < original.rows - halfSize1; y++) 
	{
		for (int x = halfSize1; x < original.cols - halfSize1; x++) 
		{
			double r=0, g=0, b=0;
			for (int i = 0; i < size1; i++)
			{
				for (int j = 0; j < size1; j++)
				{
					cv::Vec3b value = original.at<cv::Vec3b>(y + i - halfSize1, x + j - halfSize1);
					r = r + value[0] * mask1[i][j];
					g = g + value[1] * mask1[i][j];
					b = b + value[2] * mask1[i][j];
				}
			}
			r = r / 9.0;
			g = g / 9.0;
			b = b / 9.0;
			cv::Vec3b value = 0;
			value[0] = r;
			value[1] = g;
			value[2] = b;
			box.at<cv::Vec3b>(y, x) = value;
		}
	}

	cv::imshow("Box", box);

	// -----------------------------------------

	cv::Mat gaussian3(original.rows, original.cols, CV_8UC3);

	const int size2 = 3;
	int mask2[size2][size2] = { { 1,2,1 },{ 2,4,2 },{ 1,2,1 } };

	int halfSize2 = size2 / 2;

	for (int y = halfSize2; y < original.rows - halfSize2; y++) 
	{
		for (int x = halfSize2; x < original.cols - halfSize2; x++) 
		{
			double r = 0, g = 0, b = 0;
			for (int i = 0; i < size2; i++)
			{
				for (int j = 0; j < size2; j++)
				{
					cv::Vec3b value = original.at<cv::Vec3b>(y + i - halfSize2, x + j - halfSize2);
					r = r + value[0] * mask2[i][j];
					g = g + value[1] * mask2[i][j];
					b = b + value[2] * mask2[i][j];
				}
			}
			r = r / 16.0;
			g = g / 16.0;
			b = b / 16.0;
			cv::Vec3b value = 0;
			value[0] = r;
			value[1] = g;
			value[2] = b;
			gaussian3.at<cv::Vec3b>(y, x) = value;
		}
	}

	cv::imshow("Gaussian 3x3", gaussian3);

	// -----------------------------------------

	cv::Mat gaussian5(original.rows, original.cols, CV_8UC3);

	const int size3 = 5;
	int mask3[size3][size3] = { { 1,4,6,4,1 },{ 4,16,24,16,4 },{ 6,24,36,24,6 },{ 4,16,24,16,4 },{ 1,4,6,4,1 } };

	int halfSize3 = size3 / 2;

	for (int y = halfSize3; y < original.rows - halfSize3; y++) 
	{
		for (int x = halfSize3; x < original.cols - halfSize3; x++) 
		{
			double r = 0, g = 0, b = 0;
			for (int i = 0; i < size3; i++)
			{
				for (int j = 0; j < size3; j++)
				{
					cv::Vec3b value = original.at<cv::Vec3b>(y + i - halfSize3, x + j - halfSize3);
					r = r + value[0] * mask3[i][j];
					g = g + value[1] * mask3[i][j];
					b = b + value[2] * mask3[i][j];
				}
			}
			r = r / 256.0;
			g = g / 256.0;
			b = b / 256.0;
			cv::Vec3b value = 0;
			value[0] = r;
			value[1] = g;
			value[2] = b;
			gaussian5.at<cv::Vec3b>(y, x) = value;
		}
	}

	cv::imshow("Gaussian 5x5", gaussian5);
	
	cv::waitKey(0); // wait until keypressed

	return 0;
}