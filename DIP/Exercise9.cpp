#include "stdafx.h"
#include "utils.h"

void drawHistogram(cv::Mat &hist, const char* name);
cv::Mat calcHistogram(cv::Mat &src);
cv::Mat calcCdf(cv::Mat &hist);	
int cdfMin(cv::Mat &cdf);

int exercise9()
{
	cv::Mat input = cv::imread("images/uneq.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	if (input.empty()) {
		printf("Unable to read input file (%s, %d).", __FILE__, __LINE__);
	}

	int rows = input.rows;
	int cols = input.cols;
	int size = rows * cols;

	cv::Mat normed = cv::Mat::zeros(rows, cols, CV_32FC1);

	cv::Mat inputHistogram = cv::Mat::zeros(256, 1, CV_32FC1);
	inputHistogram = calcHistogram(input);
	cv::Mat cdfInput = calcCdf(inputHistogram);

	float minInput = cdfMin(cdfInput);

	for (int y = 0; y < rows; y++) 
	{
		for (int x = 0; x < cols; x++)	
		{
			int srcPx = cv::saturate_cast<int>(input.at<uchar>(y, x));
			float cdfValue = cdfInput.at<float>(srcPx);

			normed.at<float>(y, x) = roundf(((cdfValue - minInput) / (size - minInput)) * (256 - 1));
		}
	}

	cv::Mat output = cv::Mat::zeros(rows, cols, CV_32FC1);
	cv::normalize(normed, output, 0.0f, 1.0f, CV_MINMAX);

	cv::Mat normed8;
	normed.convertTo(normed8, CV_8UC1);

	cv::Mat outputHistogram = cv::Mat::zeros(256, 1, CV_32FC1);
	outputHistogram = calcHistogram(normed8);
	cv::Mat cdfOutput = calcCdf(outputHistogram);

	viewImage(input, "Input image");

	drawHistogram(inputHistogram, "Input histogram");
	drawHistogram(cdfInput, "Input Comulative histogram");

	drawHistogram(outputHistogram, "Output histogram");
	drawHistogram(cdfOutput, "Output Comulative histogram");

	viewImage(output, "Output image");

	cvWaitKey();

	return 0;
}

void drawHistogram(cv::Mat &source, const char* name)
{
	cv::Mat histNorm;
	cv::normalize(source, histNorm, 0.0f, 255.0f, CV_MINMAX);
	cv::Mat histogram = cv::Mat::zeros(256, 256, CV_8UC1);

	for (int x = 0; x < histogram.cols - 1; x++) 
	{
		int vertical = cv::saturate_cast<int>(histNorm.at<float>(x));
		for (int y = 256 - 1; y > (256 - vertical - 1); y--) 
		{
			histogram.at<uchar>(y, x) = 255;
		}
	}

	cv::namedWindow(name, 0);
	cv::imshow(name, histogram);
	cv::resizeWindow(name, 256, 100);
}

cv::Mat calcHistogram(cv::Mat &src) 
{
	cv::Mat hist = cv::Mat::zeros(256, 1, CV_32FC1);

	for (int y = 0; y < src.rows; y++) 
	{
		for (int x = 0; x < src.cols; x++) 
		{
			uchar &srcPx = src.at<uchar>(y, x);
			hist.at<float>(srcPx) = hist.at<float>(srcPx)++;
		}
	}

	return hist;
}

cv::Mat calcCdf(cv::Mat &hist) 
{
	cv::Mat cdfInt;
	cv::integral(hist, cdfInt, CV_32F);
	return cdfInt(cv::Rect(1, 1, 1, hist.rows - 1));
}

int cdfMin(cv::Mat &cdf) 
{
	int min = 1;

	for (int i = 0; i < cdf.rows; i++) 
	{
		float newMin = cdf.at<float>(i);
		if (newMin < min) min = (int)newMin;
	}

	return min;
}