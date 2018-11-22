#include "stdafx.h"
#include "utils.h"

void fillData(cv::Mat &leftSide, cv::Mat &rightSide, cv::Point2d backgroundPoint, cv::Point flagPoint, int i);

int exercise10() 
{
	cv::Mat background = cv::imread("images/vsb.jpg", CV_LOAD_IMAGE_COLOR);
	cv::Mat flag = cv::imread("images/flag.png", CV_LOAD_IMAGE_COLOR);

	if (background.empty()) {
		printf("Unable to read input file (%s, %d).", __FILE__, __LINE__);
	}
	if (flag.empty()) {
		printf("Unable to read input file (%s, %d).", __FILE__, __LINE__);
	}

	viewImage(background, "Background image", 220, 425);
	viewImage(flag, "Flag image", 108, 162);

	cv::Point2d backgroundPoints[4];
	backgroundPoints[0] = cv::Point2d(69, 107);
	backgroundPoints[1] = cv::Point2d(227, 76);
	backgroundPoints[2] = cv::Point2d(228, 122);
	backgroundPoints[3] = cv::Point2d(66, 134);

	cv::Point2d flagPoints[4];
	flagPoints[0] = cv::Point2d(0, 0);
	flagPoints[1] = cv::Point2d(323, 0);
	flagPoints[2] = cv::Point2d(323, 215);
	flagPoints[3] = cv::Point2d(0, 215);

	cv::Mat leftSide = cv::Mat(8, 8, CV_64F);
	cv::Mat rightSide = cv::Mat(8, 1, CV_64F);

	for (int i = 0; i < 4; i++)
	{
		fillData(leftSide, rightSide, backgroundPoints[i], flagPoints[i], i);
	}

	cv::Mat results;
	cv::solve(leftSide, rightSide, results);

	cv::Mat mat3x3 = (cv::Mat_<double>(3, 3) << 1, results.at<double>(0, 0), results.at<double>(1, 0),
		results.at<double>(2, 0), results.at<double>(3, 0), results.at<double>(4, 0),
		results.at<double>(5, 0), results.at<double>(6, 0), results.at<double>(7, 0));

	cv::Mat w, point;

	cv::Mat result = cv::Mat(background.rows, background.cols, CV_8UC3);

	for (int y = 0; y < background.rows; y++) {
		for (int x = 0; x < background.cols; x++) {
			point = (cv::Mat_<double>(3, 1) << x, y, 1);

			w = mat3x3 * point;
			double devided = w.at<double>(2, 0);

			int px = (int)(w.at<double>(0, 0) / devided);
			int py = (int)(w.at<double>(1, 0) / devided);

			if (px > 0 && py > 0 && px < flag.cols && py < flag.rows) {
				result.at<cv::Vec3b>(y, x) = flag.at<cv::Vec3b>(py, px);
			}
			else
			{
				result.at<cv::Vec3b>(y, x) = background.at<cv::Vec3b>(y, x);
			}
		}
	}

	viewImage(result, "Result image", 220, 425);

	cvWaitKey(0);

	return 0;
}

void fillData(cv::Mat &leftSide, cv::Mat &rightSide, cv::Point2d backgroundPoint, cv::Point flagPoint, int i)
{
	leftSide.at<double>(2 * i, 0) = backgroundPoint.y;
	leftSide.at<double>(2 * i, 1) = 1;
	leftSide.at<double>(2 * i, 2) = 0;
	leftSide.at<double>(2 * i, 3) = 0;
	leftSide.at<double>(2 * i, 4) = 0;
	leftSide.at<double>(2 * i, 5) = -flagPoint.x * backgroundPoint.x;
	leftSide.at<double>(2 * i, 6) = -flagPoint.x * backgroundPoint.y;
	leftSide.at<double>(2 * i, 7) = -flagPoint.x;

	rightSide.at<double>(2 * i, 0) = -backgroundPoint.x;

	leftSide.at<double>(2 * i + 1, 0) = 0;
	leftSide.at<double>(2 * i + 1, 1) = 0;
	leftSide.at<double>(2 * i + 1, 2) = backgroundPoint.x;
	leftSide.at<double>(2 * i + 1, 3) = backgroundPoint.y;
	leftSide.at<double>(2 * i + 1, 4) = 1;
	leftSide.at<double>(2 * i + 1, 5) = -flagPoint.y * backgroundPoint.x;
	leftSide.at<double>(2 * i + 1, 6) = -flagPoint.y * backgroundPoint.y;
	leftSide.at<double>(2 * i + 1, 7) = -flagPoint.y;

	rightSide.at<double>(2 * i + 1, 0) = 0;
}