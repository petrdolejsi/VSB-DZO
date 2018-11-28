#include "stdafx.h"	

int exercise7noise(const char* sourceText, int radiusLow, int radiusHigh)
{
	cv::Mat source = cv::imread(sourceText, CV_LOAD_IMAGE_GRAYSCALE);
	
	if (source.empty())
	{
		printf("Unable to read input file (%s, %d).", __FILE__, __LINE__);
	}
	
	cv::Mat filter_lowPass, filter_highPass;

	filter_lowPass = cv::Mat::zeros(64, 64, CV_8UC1);
	filter_highPass = cv::Mat::ones(64, 64, CV_8UC1) * 255;

	int M = source.rows;
	int N = source.cols;

	cv::circle(filter_lowPass, cv::Point(M / 2, N / 2), radiusLow, 255, -1);
	cv::circle(filter_highPass, cv::Point(M / 2, N / 2), radiusHigh, 0, -1);

	viewImage(filter_lowPass, "Circle");
	

	cv::Mat matrixLow = cv::Mat(M, N, CV_64FC2);
	cv::Mat matrixHigh = cv::Mat(M, N, CV_64FC2);

	FurierFunction(source, matrixLow);

	for (int k = 0; k < M; k++) 
	{
		for (int l = 0; l < N; l++) 
		{
			uchar pixelLow = filter_lowPass.at<uchar>(k,l);

			if (pixelLow == 0) {
				matrixLow.at<cv::Vec2d>(k, l) = cv::Vec2d(0, 0);
			}
		}
	}

	cv::Mat lena_noise_cleared_low = cv::Mat(M, N, CV_64FC1), lena_noise_cleared_high = cv::Mat(M, N, CV_64FC1);

	FurierInvertedFunction(matrixLow, lena_noise_cleared_low);

	cvWaitKey();

	FurierFunction(source, matrixHigh);

	for (int k = 0; k < M; k++)
	{
		for (int l = 0; l < N; l++)
		{
			uchar pixelHigh = filter_highPass.at<uchar>(k, l);

			if (pixelHigh == 0) {
				matrixHigh.at<cv::Vec2d>(k, l) = cv::Vec2d(0, 0);
			}
		}
	}

	viewImage(filter_highPass, "Circle");

	FurierInvertedFunction(matrixHigh, lena_noise_cleared_high);

	cvWaitKey();

	return 0;
}

int exercise7bars(const char* sourceText, int radiusLow, int radiusHigh)
{
	cv::Mat source = cv::imread(sourceText, CV_LOAD_IMAGE_GRAYSCALE);

	if (source.empty())
	{
		printf("Unable to read input file (%s, %d).", __FILE__, __LINE__);
	}

	cv::Mat filter_bars;

	filter_bars = cv::Mat::ones(64, 64, CV_8UC1) * 255;

	int M = source.rows;
	int N = source.cols;

	for (int i = 0; i < 64; i++) 
	{
		if (i > 30 && i < 34) continue;
		filter_bars.at<uchar>(32, i) = 0;
	}

	viewImage(filter_bars, "Bars");

	cv::Mat matrix = cv::Mat(M, N, CV_64FC2);

	FurierFunction(source, matrix);

	for (int k = 0; k < M; k++)
	{
		for (int l = 0; l < N; l++)
		{
			uchar pixelLow = filter_bars.at<uchar>(k, l);

			if (pixelLow == 0) 
			{
				matrix.at<cv::Vec2d>(k, l) = cv::Vec2d(0, 0);
			}
		}
	}

	cv::Mat cleared = cv::Mat(M, N, CV_64FC1);

	FurierInvertedFunction(matrix, cleared);

	cvWaitKey();

	return 0;
}