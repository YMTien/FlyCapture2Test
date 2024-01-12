/*
This is a API for PointGrey camera Flea3.
This API is buit by libraries of PointGrey.
The destination is to help user to use camera more conveniently and conbine the API of PointGrey with OpenCV.

Editor: BB Tsai, Oyster Li
Version: 1.0.1
Build Date: 2015.10.01
Change Log:
			2015.10.01		Show the number of cameras detected in the method 'CheckCameras'.
*/

#ifndef PTGREYAPI_H
#define PTGREYAPI_H

#ifndef OPENCV_HPP
#define OPENCV_HPP <opencv2/opencv.hpp>
#include OPENCV_HPP
#endif

#ifndef FLYCAPTURE2_H
#define FLYCAPTURE2_H "FlyCapture2.h"
#include FLYCAPTURE2_H
#endif

#include <iomanip>

//using namespace FlyCapture2;
using namespace std;
using namespace cv;

class PtGreyCamera
{
public:
	PtGreyCamera(){};
	static bool CheckCameras(); // check if the cameras can be detected or not

	bool ConnectCameraFromIndex(int idx); // use index to get camera

	bool GetCameraInfo();

	bool SetVideoModeAndFrameRate(FlyCapture2::VideoMode VideoMode, FlyCapture2::FrameRate FrameRate);
		
	bool StartCapture(); // before reading image, must start camera
	
	bool StopCapture(); // after reading image, must stop camera
	
	bool DisconnectCamera(); // before closing program, must disconnect cameras first
	
	bool ReadImage(Mat &capture); // read image to Mat in color if flag==1 or gray level if flag==0

	bool SetCameraConfiguration_1280X1024();

	bool SetCameraConfiguration_640X512();

	bool SetExposure();

	bool SetWhiteBalance();

	bool GetFrameRate();

	void PrintError(FlyCapture2::Error error );

	void PrintCameraInfo(FlyCapture2::CameraInfo* pCamInfo );

	void PrintFormat7Capabilities(FlyCapture2::Format7Info fmt7Info );

private:
	FlyCapture2::Error error;
	FlyCapture2::Camera cam;

	//Method 'ReadImage' Variables 
	FlyCapture2::Image rawImage;
	FlyCapture2::Image convertedImage;
	unsigned int matRows ; 
	unsigned int matCols ;
	unsigned int rowBytes ;
	unsigned char* imgPtr ;

	
    
	

    
};

#endif