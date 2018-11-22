#include "stdafx.h"
#include "utils.h"

struct RLDUserData 
{
	cv::Mat & src_8uc3_img;
	cv::Mat & undistorted_8uc3_img;
	int K1;
	int K2;

	RLDUserData(const int K1, const int K2, cv::Mat & src_8uc3_img, cv::Mat & undistorted_8uc3_img) : K1(K1), K2(K2), src_8uc3_img(src_8uc3_img), undistorted_8uc3_img(undistorted_8uc3_img) 
	{

	}
};


void geom_dist(cv::Mat & src_8uc3_img, cv::Mat & dst_8uc3_img, bool bili, double K1 = 10.0, double K2 = 10.0)
{
	double c_u = src_8uc3_img.cols / 2;
	double c_v = src_8uc3_img.rows / 2;

	double R = sqrt(pow(c_u,2) + pow(c_v,2));
	
	for (int y_n = 0; y_n < src_8uc3_img.rows; y_n++) 
	{
		for (int x_n = 0; x_n < src_8uc3_img.cols; x_n++) 
		{
			double x_ = (x_n - c_u) / R;
			double y_ = (y_n - c_v) / R;
			double r_2 = pow(x_,2) + pow(y_,2);
			double theta = 1 + K1 * r_2 + K2 * pow(r_2,2);

			double x_d = (x_n - c_u) * (1 / theta) + c_u;
			double y_d = (y_n - c_v) * (1 / theta) + c_v;

			if (bili) 
			{
				dst_8uc3_img.at<cv::Vec3b>(y_n, x_n) = bilinearInterpolation(src_8uc3_img, y_d, x_d);
			}
			else
			{
				dst_8uc3_img.at<cv::Vec3b>(y_n, x_n) = src_8uc3_img.at<cv::Vec3b>(y_d, x_d);
			}
		}
	}
}



void apply_rld(int id, void * user_data)
{
	RLDUserData *rld_user_data = (RLDUserData*)user_data;

	geom_dist(rld_user_data->src_8uc3_img, rld_user_data->undistorted_8uc3_img, !false, rld_user_data->K1 / 100.0, rld_user_data->K2 / 100.0);
	cv::imshow("Geom Dist", rld_user_data->undistorted_8uc3_img);
}

int exercise8()
{
	cv::Mat src_8uc3_img, geom_8uc3_img;
	RLDUserData rld_user_data(3.0, 1.0, src_8uc3_img, geom_8uc3_img);

	src_8uc3_img = cv::imread("images/distorted_window.jpg", CV_LOAD_IMAGE_COLOR);
	if (src_8uc3_img.empty())
	{
		printf("Unable to load image!\n");
		exit(-1);
	}

	cv::namedWindow("Original Image");
	cv::imshow("Original Image", src_8uc3_img);

	geom_8uc3_img = src_8uc3_img.clone();
	apply_rld(0, (void*)&rld_user_data);

	cv::namedWindow("Geom Dist");
	cv::imshow("Geom Dist", geom_8uc3_img);

	cv::createTrackbar("K1", "Geom Dist", &rld_user_data.K1, 100, apply_rld, &rld_user_data);
	cv::createTrackbar("K2", "Geom Dist", &rld_user_data.K2, 100, apply_rld, &rld_user_data);

	cv::waitKey(0);

	return 0;
}
	