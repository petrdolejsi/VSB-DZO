#include "stdafx.h"

int exercise12()
{
	cv::Mat input = cv::imread("images/lena.png", CV_LOAD_IMAGE_GRAYSCALE);

	if (input.empty())
	{
		printf("Unable to read input file (%s, %d).", __FILE__, __LINE__);
	}

	viewImage(input, "Input image");

	int rows = input.rows;
	int cols = input.cols;

	cv::Mat resultCentral = cv::Mat::zeros(rows, cols, CV_8UC1);
	cv::Mat resultSobel = cv::Mat::zeros(rows, cols, CV_8UC1);
	CentralDifference(input, resultCentral);
	Sobel(input, resultSobel);

	viewImage(resultCentral, "Result image with central difference");
	viewImage(resultSobel, "Result image with sobel");

	cv::waitKey(0);

	return 0;
}