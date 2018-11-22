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

#endif