#include "stdafx.h"

struct RLDUserData
{
	cv::Mat & source;
	cv::Mat & output;
	int T1;
	int T2;

	RLDUserData(const int T1, const int T2, cv::Mat & source, cv::Mat & output) : T1(T1), T2(T2), source(source), output(output)
	{

	}
};

void thresholding(cv::Mat & source, cv::Mat & output, double T1 = 10.0, double T2 = 10.0)
{
	for (int y = 1; y < source.rows - 1; y++)
	{
		for (int x = 1; x < source.cols - 1; x++)
		{
			if (source.at<uchar>(x, y) >= T2)
			{
				output.at<uchar>(x, y) = 255;
			}
			else
			{
				output.at<uchar>(x, y) = 0;
			}
		}
	}

	for (int y = 1; y < source.rows - 1; y++)
	{
		for (int x = 1; x < source.cols - 1; x++)
		{
			if (output.at<uchar>(x, y) != 255 && source.at<uchar>(x, y) > T1)
			{
				if (source.at<uchar>(x + 1, y) >= T2)
				{
					output.at<uchar>(x, y) = 255;
				}
				else if (source.at<uchar>(x - 1, y) >= T2)
				{
					output.at<uchar>(x, y) = 255;
				}
				else if (source.at<uchar>(x, y + 1) >= T2)
				{
					output.at<uchar>(x, y) = 255;
				}
				else if (source.at<uchar>(x, y - 1) >= T2)
				{
					output.at<uchar>(x, y) = 255;
				}
				else
				{
					output.at<uchar>(x, y) = 0;
				}
			}
		}
	}
}

void apply_rld13(int id, void * user_data)
{
	RLDUserData *rld_user_data13 = (RLDUserData*)user_data;

	thresholding(rld_user_data13->source, rld_user_data13->output, rld_user_data13->T1, rld_user_data13->T2);
	viewImage(rld_user_data13->output, "Result image after thresholding", 600, 600);
}

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

	cv::Mat result = cv::Mat::zeros(rows, cols, CV_8UC1);

	cv::Mat resultCentral = cv::Mat::zeros(rows, cols, CV_8UC1);
	cv::Mat resultTemp = cv::Mat::zeros(rows, cols, CV_8UC1);

	CentralDifference(input, resultCentral);

	viewImage(resultCentral, "Differenced image");

	for (int y = 1; y < resultCentral.rows - 1; y++)
	{
		for (int x = 1; x < resultCentral.cols - 1; x++)
		{
			double X = (resultCentral.at<uchar>(x - 1, y) - resultCentral.at<uchar>(x + 1, y)) * 0.5;
			double Y = (resultCentral.at<uchar>(x, y - 1) - resultCentral.at<uchar>(x, y + 1)) * 0.5;
			double alpha = atan2(Y, X);

			double a = tan(alpha);

			if (alpha >= 0 && alpha < PI / 2)
			{
				double tempA = a * resultCentral.at<uchar>(x + 1, y + 1) + (1 - a) * (resultCentral.at<uchar>(x + 1, y));
				double tempB = a * resultCentral.at<uchar>(x - 1, y - 1) + (1 - a) * (resultCentral.at<uchar>(x - 1, y));
				if (tempA < resultCentral.at<uchar>(x, y) && resultCentral.at<uchar>(x, y) < tempB)
				{
					resultTemp.at<uchar>(x, y) = resultCentral.at<uchar>(x, y);
				}
			}
			else if (alpha >= PI / 2 && alpha < PI)
			{
				double tempA = a * resultCentral.at<uchar>(x - 1, y) + (1 - a) * (resultCentral.at<uchar>(x, y + 1));
				double tempB = a * resultCentral.at<uchar>(x + 1, y) + (1 - a) * (resultCentral.at<uchar>(x, y - 1));
				if (tempA < resultCentral.at<uchar>(x, y) && resultCentral.at<uchar>(x, y) < tempB)
				{
					resultTemp.at<uchar>(x, y) = resultCentral.at<uchar>(x, y);
				}
			}
			else if (alpha >= PI && alpha < (3.0 / 2.0) * PI)
			{
				double tempA = a * resultCentral.at<uchar>(x, y - 1) + (1 - a) * (resultCentral.at<uchar>(x - 1, y));
				double tempB = a * resultCentral.at<uchar>(x, y + 1) + (1 - a) * (resultCentral.at<uchar>(x + 1, y));
				if (tempA < resultCentral.at<uchar>(x, y) && resultCentral.at<uchar>(x, y) < tempB)
				{
					resultTemp.at<uchar>(x, y) = resultCentral.at<uchar>(x, y);
				}
			}
			else if (alpha >= (3.0 / 2.0)*PI && alpha <= 2 * PI)
			{
				double tempA = a * resultCentral.at<uchar>(x + 1, y) + (1 - a) * (resultCentral.at<uchar>(X, y - 1));
				double tempB = a * resultCentral.at<uchar>(x - 1, y) + (1 - a) * (resultCentral.at<uchar>(x, y + 1));
				if (tempA < resultCentral.at<uchar>(x, y) && resultCentral.at<uchar>(x, y) < tempB)
				{
					resultTemp.at<uchar>(x, y) = resultCentral.at<uchar>(x, y);
				}
			}
		}
	}

	RLDUserData rld_user_data13(10.0, 30.0, resultTemp, result);
	apply_rld13(0, (void*)&rld_user_data13);

	viewImage(resultTemp, "Result image before thresholding");

	viewImage(result, "Result image after thresholding", 600, 600);
	cv::createTrackbar("T1", "Result image after thresholding", &rld_user_data13.T1, 50, apply_rld13, &rld_user_data13);
	cv::createTrackbar("T2", "Result image after thresholding", &rld_user_data13.T2, 50, apply_rld13, &rld_user_data13);

	cv::waitKey(0);

	return 0;
}