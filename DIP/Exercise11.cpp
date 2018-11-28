#include "stdafx.h"

const int rows = 240;
const int cols = 300;

int exercise11()
{
	cv::Mat original = cv::Mat::zeros(rows, cols, CV_64FC1);

	cv::circle(original, cv::Point(cols/2, rows / 2), 60, 255, 1);
	cv::rectangle(original, cv::Point(cols / 2, rows / 2), cv::Point(cols / 2 + 40, rows / 2 + 80), 255, -1);

	cv::Mat rotated, rotatedInverse, projected, projectedRotated;

	cv::Mat tmpResult = cv::Mat::zeros(rows, cols, CV_64FC1);
	cv::Mat result = cv::Mat::zeros(rows, cols, CV_64FC1);

	for (int i = 0; i < 360; i = i + 5)
	{
		cv::Mat tmpProjected = cv::Mat::zeros(rows, cols, CV_64FC1);
		cv::Mat tmpProjectedRotated;
		
		rotated = RotateImage(original, i);
		rotatedInverse = RotateImage(original, -i);
		cv::imshow("Image", rotated);
		printf("Angle: %d\n", i);

		for (int y = 0; y < rows; y++) 
		{
			for (int x = 0; x < cols; x++) 
			{
				tmpProjected.at<double>(y, 0)+= rotatedInverse.at<double>(y, x);
			}
		}

		for (int k = 1; k < cols; k++) 
		{
			tmpProjected.col(0).copyTo(tmpProjected.col(k));
		}

		tmpProjectedRotated = RotateImage(tmpProjected, i);

		for (int y = 0; y < rows; y++) 
		{
			for (int x = 0; x < cols; x++) 
			{
				tmpResult.at<double>(y, x) += tmpProjectedRotated.at<double>(y, x);
			}
		}

		cv::normalize(tmpProjected, projected, 0.0, 1.0, CV_MINMAX);
		cv::normalize(tmpProjectedRotated, projectedRotated, 0.0, 1.0, CV_MINMAX);

		cv::normalize(tmpResult, result, 0.0, 1.0, CV_MINMAX);

		for (int y = 0; y < rows; y++) 
		{
			for (int x = 0; x < cols; x++) 
			{
				result.at<double>(y, x) = result.at<double>(y, x) * (i/360.0);
			}
		}

		viewImage(projected,"Projected", rows, cols);
		viewImage(projectedRotated, "Projected Rotated", rows, cols);

		viewImage(result, "Result", rows, cols);

		cvWaitKey(1);
	}

	printf("\nFinished\n");
	
	cvWaitKey();

	return 0;
}