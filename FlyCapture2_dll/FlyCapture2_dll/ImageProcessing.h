#include "opencv2/opencv.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "PtGreyAPI.h"
#include <iostream>
#pragma once
using namespace cv;

__declspec(dllexport) class ImageProcessing
{
public:
	PtGreyCamera cam1;
	void initialCamera();
	void closeCamera();
	void takePic();
	Mat image;
};




