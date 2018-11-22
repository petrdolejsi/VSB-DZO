// DIP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int exercise2()
{
	cv::Mat original = cv::imread("images/moon.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	if (original.empty()) 
	{
		printf("Unable to read input file (%s, %d).", __FILE__, __LINE__);
	}

	cv::imshow("Original", original);
	cv::Mat gammaEdit(original.rows, original.cols, CV_8UC1);

	double min, max;
	cv::minMaxLoc(original, &min, &max);

	uchar max_8UC = max * 255;
	uchar min_8UC = min * 255;

	for (int y = 0; y < original.rows; y++) 
	{
		for (int x = 0; x < original.cols; x++) 
		{
			gammaEdit.at<uchar>(y, x) = (original.at<uchar>(y, x) - min) / (max - min) * 255;
		}
	}

	cv::imshow("Moon gamma", gammaEdit);

	cv::Mat contractEdit(original.rows, original.cols, CV_8UC1);

	for (int y = 0; y < contractEdit.rows; y++) 
	{
		for (int x = 0; x < contractEdit.cols; x++) 
		{
			for (int c = 0; c < 3; c++) {

				double l = (double)original.at<uchar>(y, x) / 255.0;
				double gamma = 1 / 2.2;
				double cor = pow(l, gamma) * 255.0;
				contractEdit.at<uchar>(y, x) = cor;
			}
		}
	}

	cv::imshow("Moon contract", contractEdit);

	cv::waitKey(0);

	return 0;
}