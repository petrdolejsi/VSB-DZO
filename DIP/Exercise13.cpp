#include "stdafx.h"

int exercise13()
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
	cv::Mat result = cv::Mat::zeros(rows, cols, CV_8UC1);
	CentralDifference(input, resultCentral);

	viewImage(resultCentral, "Differenced image");

	for (int y = 1; y < resultCentral.rows - 1; y++)
	{
		for (int x = 1; x < resultCentral.cols - 1; x++)
		{
			double X = (resultCentral.at<uchar>(x - 1, y) - resultCentral.at<uchar>(x + 1, y)) * 0.5;
			double Y = (resultCentral.at<uchar>(x, y - 1) - resultCentral.at<uchar>(x, y + 1)) * 0.5;
			int alpha = atan2(Y, X);

			int a = tan(alpha);

			if (alpha >= 0 && alpha < PI / 2)
			{
				double tempA = a * resultCentral.at<uchar>(x + 1, y + 1) + (1 - a) * (resultCentral.at<uchar>(x + 1, y));
				double tempB = a * resultCentral.at<uchar>(x - 1, y - 1) + (1 - a) * (resultCentral.at<uchar>(x - 1, y));
				if (tempA < resultCentral.at<uchar>(x, y) && resultCentral.at<uchar>(x, y) < tempB)
				{
					result.at<uchar>(x, y) = resultCentral.at<uchar>(x, y);
				}
			}
			else if (alpha >= PI / 2 && alpha < PI)
			{
				double tempA = a * resultCentral.at<uchar>(x - 1, y) + (1 - a) * (resultCentral.at<uchar>(x, y + 1));
				double tempB = a * resultCentral.at<uchar>(x + 1, y) + (1 - a) * (resultCentral.at<uchar>(x, y - 1));
				if (tempA < resultCentral.at<uchar>(x, y) && resultCentral.at<uchar>(x, y) < tempB)
				{
					result.at<uchar>(x, y) = resultCentral.at<uchar>(x, y);
				}
			}
			else if (alpha >= PI && alpha < (3/2)*PI)
			{
				double tempA = a * resultCentral.at<uchar>(x, y - 1) + (1 - a) * (resultCentral.at<uchar>(x - 1, y));
				double tempB = a * resultCentral.at<uchar>(x, y + 1) + (1 - a) * (resultCentral.at<uchar>(x + 1, y));
				if (tempA < resultCentral.at<uchar>(x, y) && resultCentral.at<uchar>(x, y) < tempB)
				{
					result.at<uchar>(x, y) = resultCentral.at<uchar>(x, y);
				}
			}
			else if (alpha >= (3 / 2)*PI && alpha < 2*PI)
			{
				double tempA = a * resultCentral.at<uchar>(x + 1, y) + (1 - a) * (resultCentral.at<uchar>(X, y - 1));
				double tempB = a * resultCentral.at<uchar>(x - 1, y) + (1 - a) * (resultCentral.at<uchar>(x, y + 1));
				if (tempA < resultCentral.at<uchar>(x, y) && resultCentral.at<uchar>(x, y) < tempB)
				{
					result.at<uchar>(x, y) = resultCentral.at<uchar>(x, y);
				}
			}
		}
	}

	viewImage(result, "Result image");

	cv::waitKey(0);

	return 0;
}