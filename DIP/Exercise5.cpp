#include "stdafx.h"

int exercise5()
{
	cv::Mat source = cv::imread("images/lena64.png", CV_LOAD_IMAGE_GRAYSCALE);

	if (source.empty()) 
	{
		printf("Unable to read input file (%s, %d).", __FILE__, __LINE__);
	}	

	int imageHeight = 300;
	int imageWidth = 300;

	int M = source.rows;
	int N = source.cols;

	double sqrtMN_d = 1.0 / sqrt(M * N);
	double sumR = 0, sumI = 0;
	double M_d = 1.0 / M, N_d = 1.0 / N;

	cv::Mat matrix = cv::Mat(M, N, CV_64FC2);

	FurierFunction(source, matrix);

	// phase -------------------------------

	cv::Mat destPhase = cv::Mat(M, N, CV_64FC1);

	for (int m = 0; m < M; m++) 
	{
		for (int n = 0; n < N; n++) 
		{
			cv::Vec2d F = matrix.at<cv::Vec2d>(m, n);
			destPhase.at<double>(m, n) = atan(F[1] / F[0]);
		}
	}

	viewImage(destPhase, "Phase");

	// power -------------------------------

	cv::Mat destPower = cv::Mat(M, N, CV_64FC1);
	
	int hRows = destPower.rows / 2;
	int hCols = destPower.cols / 2;

	for (int m = 0; m < M; m++) 
	{
		for (int n = 0; n < N; n++) 
		{
			cv::Vec2d F = matrix.at<cv::Vec2d>(m, n);
			double amp = sqrt(pow(F[0],2) + pow(F[1],2));
			destPower.at<double>(m, n) = log(pow(amp,2));
		}
	}

	cv::normalize(destPower, destPower, 0.0, 1.0, CV_MINMAX);

	viewImage(destPower, "Power");

	cvWaitKey();

	return 0;
}