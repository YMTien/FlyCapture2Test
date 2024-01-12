#include "ImageProcessing.h"
#include "stdafx.h"
#include "opencv2/opencv.hpp"


void ImageProcessing::initialCamera()
{
	cam1.CheckCameras();
	if (!cam1.ConnectCameraFromIndex(0))
		return;
	if (!cam1.SetCameraConfiguration_640X512())
		return;
	if (!cam1.SetExposure())
		return;
	if (!cam1.SetWhiteBalance())
		return;
	// Switch on the capture ability
	if (!cam1.StartCapture())
		return;
}
void ImageProcessing::closeCamera()
{
	cam1.StopCapture();
	cam1.DisconnectCamera();

}
void ImageProcessing::takePic()
{
	if (cam1.ReadImage(image))
	{
		//Mat image_;
		//pyrDown(image1, image_);
		cv::imshow("Cam", image);
	}
	//cv::waitKey(0);

}