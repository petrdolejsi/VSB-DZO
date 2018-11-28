#include "stdafx.h"
#ifndef   UTILS_H
#define   UTILS_H
#define PI 3.1415926

template<typename T>
static void flipQuadrants(cv::Mat &source) 
{
	int hRows = source.rows / 2;
	int hCols = source.cols / 2;

	for (int y = 0; y < hRows; y++) 
	{
		for (int x = 0; x < hCols; x++) 
		{
			T tl = source.at<T>(y, x);
			T tr = source.at<T>(y, x + hCols);
			T bl = source.at<T>(y + hRows, x);
			T br = source.at<T>(y + hRows, x + hCols);

			source.at<T>(y, x) = br;
			source.at<T>(y, x + hCols) = bl;
			source.at<T>(y + hRows, x) = tr;
			source.at<T>(y + hRows, x + hCols) = tl;
		}
	}
}

static void viewImage(cv::Mat &source, const char* name) 
{
	int imageHeightUtils = 300;
	int imageWidthUtils = 300;

	cv::namedWindow(name, 0);
	cv::imshow(name, source);
	cv::resizeWindow(name, imageWidthUtils, imageHeightUtils);
}

static void viewImage(cv::Mat &source, const char* name, int height, int width)
{
	cv::namedWindow(name, 0);
	cv::imshow(name, source);
	cv::resizeWindow(name, width, height);
}

static void FurierFunction(cv::Mat &source, cv::Mat &output) 
{
	viewImage(source, "Original");

	int M = source.rows;
	int N = source.cols;

	cv::Mat normalized;

	double sqrtMN_d = 1.0 / sqrt(M * N);
	double sumR = 0, sumI = 0;
	double M_d = 1.0 / M, N_d = 1.0 / N;

	source.convertTo(normalized, CV_64FC1, 1.0 / 255.0 * sqrtMN_d);

	for (int k = 0; k < M; k++)
	{
		for (int l = 0; l < N; l++)
		{
			sumR = sumI = 0;

			for (int m = 0; m < M; m++)
			{
				for (int n = 0; n < N; n++)
				{
					double px = normalized.at<double>(m, n);
					double expX = -(2 * PI) * (m * k * M_d + n * l * N_d);

					sumR += px * cos(expX);
					sumI += px * sin(expX);
				}
			}
			output.at<cv::Vec2d>(k, l) = cv::Vec2d(sumR, sumI);
		}
	}

	flipQuadrants<cv::Vec2d>(output);
}

static void FurierInvertedFunction(cv::Mat &source, cv::Mat &output) 
{

	flipQuadrants<cv::Vec2d>(source);
	
	int M = source.rows;
	int N = source.cols;

	double sqrtMN_d = 1.0 / sqrt(M * N);
	double sumR = 0, sumI = 0;
	double M_d = 1.0 / M, N_d = 1.0 / N;

	double sum = 0;

	for (int m = 0; m < M; m++) 
	{
		for (int n = 0; n < N; n++) 
		{
			sum = 0;

			for (int k = 0; k < M; k++) 
			{
				for (int l = 0; l < N; l++) 
				{
					double expX = (2 * PI) * ((k * m * M_d) + (l * n * N_d));

					double realX = cos(expX) * sqrtMN_d;
					double imagX = sin(expX) * sqrtMN_d;

					cv::Vec2d F = source.at<cv::Vec2d>(k, l);

					sum += F[0] * realX - F[1] * imagX;
				}
			}

			output.at<double>(m, n) = sum;
		}
	}

	viewImage(output, "Inverted");
}

static cv::Vec3b bilinearInterpolation(cv::Mat &source, double imageY, double imageX)
{
	double x1 = floor(imageX);
	double y1 = floor(imageY);
	double x2 = ceil(imageX);
	double y2 = ceil(imageY);

	cv::Vec3b f_00 = source.at<cv::Vec3b>(y1, x1);
	cv::Vec3b f_01 = source.at<cv::Vec3b>(y1, x2);
	cv::Vec3b f_10 = source.at<cv::Vec3b>(y2, x1);
	cv::Vec3b f_11 = source.at<cv::Vec3b>(y2, x2);

	double x = imageX - x1;
	double y = imageY - y1;

	return (f_00 * (1 - x) * (1 - y)) + (f_01 * x * (1 - y)) + (f_10 * (1 - x) * y) + (f_11 * x * y);
}

static cv::Mat RotateImage(const cv::Mat &source, const double angle)
{
	cv::Point center = cv::Point(source.cols / 2, source.rows / 2);
	double scale = 1.0;

	cv::Mat rotated_matrix(2, 3, CV_32FC1);

	cv::Mat rotated_image;

	rotated_matrix = cv::getRotationMatrix2D(center, angle, scale);

	cv::warpAffine(source, rotated_image, rotated_matrix, source.size());

	return rotated_image;
}

static void CentralDifference(cv::Mat &source, cv::Mat &output)
{
	for (int y = 1; y < source.rows - 1; y++)
	{
		for (int x = 1; x < source.cols - 1; x++)
		{
			uchar fxPlus = source.at<uchar>(y, x + 1);
			uchar fxMinus = source.at<uchar>(y, x - 1);

			uchar fyPlus = source.at<uchar>(y + 1, x);
			uchar fyMinus = source.at<uchar>(y - 1, x);

			output.at<uchar>(y, x) = sqrt(pow((fxMinus - fxPlus) / 2, 2) + pow((fyMinus - fyPlus) / 2, 2));
		}
	}
}

static void Sobel(cv::Mat &source, cv::Mat &output)
{
	const int size1 = 3;
	int xMask[size1][size1] = { { 1, 0,-1 } , { 2, 0,-2 } , { 1, 0,-1 } };
	int yMask[size1][size1] = { { 1, 2, 1 } , { 0, 0, 0 } , {-1,-2,-1 } };

	int halfSize1 = size1 / 2;

	for (int y = halfSize1; y < source.rows - halfSize1; y++)
	{
		for (int x = halfSize1; x < source.cols - halfSize1; x++)
		{
			double xPoint = 0, yPoint = 0;
			for (int i = 0; i < size1; i++)
			{
				for (int j = 0; j < size1; j++)
				{
					uchar xValue = source.at<uchar>(y + i - halfSize1, x + j - halfSize1);
					xPoint = xPoint + (xValue * xMask[i][j]);
					uchar yValue = source.at<uchar>(y + i - halfSize1, x + j - halfSize1);
					yPoint = yPoint + (yValue * yMask[i][j]);
				}
			}
			xPoint = xPoint / 9.0;
			yPoint = yPoint / 9.0;

			uchar temp = sqrt(pow(xPoint, 2) + pow(yPoint, 2));

			output.at<uchar>(y, x) = temp;
		}
	}
}

#endif