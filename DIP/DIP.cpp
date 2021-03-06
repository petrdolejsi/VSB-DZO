// DIP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int defaultTemplate();
int defaultTemplate2();
int exercise2();	//Gamma correction
int exercise3();	//Convolution
int exercise4();	//Anisotropic diffusion
int exercise5();	//Discrete Fourier Transform
int exercise6();	//Inverse Discrete Fourier Transform
int exercise7noise(const char* source, int radiusLow, int radiusHigh);	//Remove a noise
int exercise7bars(const char* source, int radiusLow, int radiusHigh);	//Remove lines
int exercise8();	//Geometric distortion
int exercise9();	//Histograms
int exercise10();	//Projective transform
int exercise11();	//Backprojection
int exercise12();	//Central difference, Sobel
int exercise13();	//Edge detector

int main()
{
	//defaultTemplate();
	//defaultTemplate2();
	//exercise2();
	//exercise3();
	//exercise4();
	//exercise5();
	//exercise6();
	//exercise7noise("images/lena64_noise.png", 20, 8);
	//exercise7noise("images/lena64_noise2.png", 27, 8);
	//exercise7bars("images/lena64_bars.png", 25, 25);
	//exercise8();
	//exercise9();
	//exercise10();
	//exercise11();
	//exercise12();
	exercise13();
}

