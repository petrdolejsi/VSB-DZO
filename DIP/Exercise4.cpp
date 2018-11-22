#include "stdafx.h"

int exercise4() 
{
	cv::Mat original = cv::imread("images/lena.png", CV_LOAD_IMAGE_COLOR);

	if (original.empty()) 
	{
		printf("Unable to read input file (%s, %d).", __FILE__, __LINE__);
	}
	cv::cvtColor(original, original, CV_BGR2GRAY);

	cv::imshow("Original", original);

	double lambda = 0.1;
	double sigma = 0.015;

	int iterations = 30;
	
	cv::Mat edited1;
	original.convertTo(edited1, CV_64FC1, 1.0 / 255.0);

	cv::Mat edited2 (original.rows, original.cols, CV_64FC1);

	double cn = 0, cs = 0, cw = 0, ce = 0;
	double in, is, iw, ie;

	for (int i = 1; i <= iterations; i++)
	{
		for (int y = 1; y < original.rows - 1; y++)
		{
			for (int x = 1; x < original.cols - 1; x++)
			{
				in = edited1.at<double>(y - 1, x) - edited1.at<double>(y, x);
				is = edited1.at<double>(y + 1, x) - edited1.at<double>(y, x);
				iw = edited1.at<double>(y, x - 1) - edited1.at<double>(y, x);
				ie = edited1.at<double>(y, x + 1) - edited1.at<double>(y, x);

				cn = exp(-1 * ((in*in) / (sigma*sigma)));
				cs = exp(-1 * ((is*is) / (sigma*sigma)));
				cw = exp(-1 * ((iw*iw) / (sigma*sigma)));
				ce = exp(-1 * ((ie*ie) / (sigma*sigma)));

				edited2.at<double>(y, x) = edited1.at<double>(y, x) * (1 - lambda * (cn + cs + ce + cw)) +
					lambda * (cn*edited1.at<double>(y - 1, x) + cs * edited1.at<double>(y + 1, x)  +
						cw * edited1.at<double>(y, x - 1) + ce * edited1.at<double>(y, x + 1));
			}
		}
		for (int y = 1; y < original.rows - 1; y++)
		{
			for (int x = 1; x < original.cols - 1; x++)
			{
				edited1.at<double>(y, x) = edited2.at<double>(y, x);
			}
		}
	}


	cv::imshow("Anisotropic Filtration", edited1);
	cvWaitKey();

	return 0;
}