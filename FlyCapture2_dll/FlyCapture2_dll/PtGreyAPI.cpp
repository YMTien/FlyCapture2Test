#include "stdafx.h"
#include "PtGreyAPI.h"


bool PtGreyCamera::CheckCameras() // check if the cameras can be detected or not
{
	FlyCapture2::BusManager busMgr;
	unsigned int numCamDetected;
	FlyCapture2::Error err = busMgr.GetNumOfCameras(&numCamDetected);

	printf("\n");
	printf("Number of cameras detected: %d \n\n", numCamDetected);

	if (err == FlyCapture2::PGRERROR_OK)
		return 1;
	else 
		return 0;

}

bool PtGreyCamera::ConnectCameraFromIndex(int idx) // use index to get camera
{
	FlyCapture2::BusManager busMgr;
	FlyCapture2::PGRGuid guid;
	error = busMgr.GetCameraFromIndex(idx, &guid);
	if (error != FlyCapture2::PGRERROR_OK)
	{
		printf("Fail to connect to camera.\n");
		return 0;
	}

	error = cam.Connect(&guid);
	if (error == FlyCapture2::PGRERROR_OK)
		return 1;
	else
	{
		printf("Fail to connect to camera %d.\n", idx);
		return 0;
	}
}

bool PtGreyCamera::GetCameraInfo()
{
	// Get the camera information
	FlyCapture2::CameraInfo camInfo;
    error = cam.GetCameraInfo(&camInfo);
    if (error != FlyCapture2::PGRERROR_OK)
    {
        PrintError( error );
        return 0;
    }

    PrintCameraInfo(&camInfo); 
	return 1;
}

bool PtGreyCamera::SetVideoModeAndFrameRate(FlyCapture2::VideoMode VideoMode, FlyCapture2::FrameRate FrameRate)
{
	error=cam.SetVideoModeAndFrameRate(VideoMode, FrameRate);


	
	if ( error != FlyCapture2::PGRERROR_OK )
	{
		printf("Failed to set video mode or frame rate.\n\n"); 
		return 0;
	} 
	else
	{
		return 1;
	}
}

bool PtGreyCamera::StartCapture() // before reading image, must start camera
{
	// Start capturing images
	error = cam.StartCapture();

	if ( error == FlyCapture2::PGRERROR_ISOCH_BANDWIDTH_EXCEEDED )
	{
		printf("Bandwidth exceeded.\n\n"); 
		return 0;
	}
	else if ( error != FlyCapture2::PGRERROR_OK )
	{
		printf("Failed to start image capture.\n\n"); 
		return 0;
	} 
	else
	{
		return 1;
	}
}

bool PtGreyCamera::StopCapture() // after reading image, must stop camera
{
	// Start capturing images
	error = cam.StartCapture();
	if (error == FlyCapture2::PGRERROR_OK)
		return 1;
	else
		return 0;
}

bool PtGreyCamera::DisconnectCamera() // before closing program, must disconnect cameras first
{
	// Disconnect the camera
	error = cam.Disconnect();
	if (error == FlyCapture2::PGRERROR_OK)
		return 1;
	else
		return 0;
}


bool PtGreyCamera::ReadImage(Mat &capture)
{
	// Retrieve an image
	error = cam.RetrieveBuffer( &rawImage );
    if (error != FlyCapture2::PGRERROR_OK)
	{
		PrintError( error );
		return 0;
	}

	// Convert the raw image
    error = rawImage.Convert(FlyCapture2::PIXEL_FORMAT_BGR, &convertedImage );
    if (error != FlyCapture2::PGRERROR_OK)
    {
		PrintError( error );
		return 0;
	}


	// Convert to OpenCV Mat
	matRows = convertedImage.GetRows(); 
	matCols = convertedImage.GetCols();
	rowBytes = (double)convertedImage.GetReceivedDataSize()/(double)convertedImage.GetRows();
	imgPtr = convertedImage.GetData();
	capture=Mat( matRows, matCols, CV_8UC3,imgPtr,rowBytes).clone();

	// Check if the data is null or not
	if(capture.data)
		return 1;
	else
	{
		printf("The retrieved image is null in the method 'ReadImage'.");
		return 0;
	}
}

bool PtGreyCamera::SetCameraConfiguration_1280X1024()
{
	FlyCapture2::Mode k_fmt7Mode = FlyCapture2::MODE_0;
	FlyCapture2::PixelFormat k_fmt7PixFmt = FlyCapture2::PIXEL_FORMAT_RAW8;
	
	// Query for available Format 7 modes
	FlyCapture2::Format7Info fmt7Info;
    bool supported;
    fmt7Info.mode = k_fmt7Mode;
    error = cam.GetFormat7Info( &fmt7Info, &supported );
    if (error != FlyCapture2::PGRERROR_OK)
    {
        PrintError( error );
        return -1;
    }

    PrintFormat7Capabilities( fmt7Info );

    if ( (k_fmt7PixFmt & fmt7Info.pixelFormatBitField) == 0 )
    {
        // Pixel format not supported!
		printf("Pixel format is not supported.\n\n");
        return -1;
    }
	
	FlyCapture2::Format7ImageSettings fmt7ImageSettings;
	fmt7ImageSettings.mode = k_fmt7Mode;
    fmt7ImageSettings.offsetX = 0;
    fmt7ImageSettings.offsetY = 0;
    fmt7ImageSettings.width = fmt7Info.maxWidth;
    fmt7ImageSettings.height = fmt7Info.maxHeight;
    fmt7ImageSettings.pixelFormat = k_fmt7PixFmt;

    bool valid;
	FlyCapture2::Format7PacketInfo fmt7PacketInfo;

    // Validate the settings to make sure that they are valid
    error = cam.ValidateFormat7Settings(
        &fmt7ImageSettings,
        &valid,
        &fmt7PacketInfo );
    if (error != FlyCapture2::PGRERROR_OK)
    {
        PrintError( error );
        return -1;
    }

    if ( !valid )
    {
        // Settings are not valid
		printf("Format7 settings are not valid.\n\n");
        return -1;
    }

    // Set the settings to the camera
    error = cam.SetFormat7Configuration(
        &fmt7ImageSettings,
        fmt7PacketInfo.recommendedBytesPerPacket );
    if (error != FlyCapture2::PGRERROR_OK)
    {
        PrintError( error );
        return -1;
    }

	return 1;
}

bool PtGreyCamera::SetCameraConfiguration_640X512()
{
	FlyCapture2::Mode k_fmt7Mode = FlyCapture2::MODE_2;
	FlyCapture2::PixelFormat k_fmt7PixFmt = FlyCapture2::PIXEL_FORMAT_RAW8;
	
	// Query for available Format 7 modes
	FlyCapture2::Format7Info fmt7Info;
    bool supported;
    fmt7Info.mode = k_fmt7Mode;
    error = cam.GetFormat7Info( &fmt7Info, &supported );
    if (error != FlyCapture2::PGRERROR_OK)
    {
        PrintError( error );
        return -1;
    }

    PrintFormat7Capabilities( fmt7Info );

    if ( (k_fmt7PixFmt & fmt7Info.pixelFormatBitField) == 0 )
    {
        // Pixel format not supported!
		printf("Pixel format is not supported.\n\n");
        return -1;
    }
	
	FlyCapture2::Format7ImageSettings fmt7ImageSettings;
	fmt7ImageSettings.mode = k_fmt7Mode;
    fmt7ImageSettings.offsetX = 0;
    fmt7ImageSettings.offsetY = 0;
    fmt7ImageSettings.width = fmt7Info.maxWidth;
    fmt7ImageSettings.height = fmt7Info.maxHeight;
    fmt7ImageSettings.pixelFormat = k_fmt7PixFmt;

    bool valid;
	FlyCapture2::Format7PacketInfo fmt7PacketInfo;

    // Validate the settings to make sure that they are valid
    error = cam.ValidateFormat7Settings(
        &fmt7ImageSettings,
        &valid,
        &fmt7PacketInfo );
    if (error != FlyCapture2::PGRERROR_OK)
    {
        PrintError( error );
        return -1;
    }

    if ( !valid )
    {
        // Settings are not valid
		printf("Format7 settings are not valid.\n\n");
        return -1;
    }

    // Set the settings to the camera
    error = cam.SetFormat7Configuration(
        &fmt7ImageSettings,
        fmt7PacketInfo.recommendedBytesPerPacket );
    if (error != FlyCapture2::PGRERROR_OK)
    {
        PrintError( error );
        return -1;
    }

	return 1;
}

bool PtGreyCamera::GetFrameRate()
{
	// Retrieve frame rate property
	FlyCapture2::Property frmRate;
    frmRate.type = FlyCapture2::FRAME_RATE;
    error = cam.GetProperty( &frmRate );
    if (error != FlyCapture2::PGRERROR_OK)
    {
        PrintError( error );
        return -1;
    }

    cout << "Frame rate is " << fixed << setprecision(2) << frmRate.absValue << " fps" << endl; 
	return 1;
}

bool PtGreyCamera::SetExposure()
{
	// Retrieve frame rate property
	FlyCapture2::Property exposure;
    exposure.type = FlyCapture2::AUTO_EXPOSURE;
    error = cam.GetProperty( &exposure );
    if (error != FlyCapture2::PGRERROR_OK)
    {
        PrintError( error );
        return -1;
    }

	exposure.absValue=-0.3;
	exposure.autoManualMode=false;

	cam.SetProperty(&exposure);

    cout << "Exposure is " << fixed << setprecision(2) << exposure.absValue << endl; 
	return 1;
}

bool PtGreyCamera::SetWhiteBalance()
{
	// Retrieve frame rate property
	FlyCapture2::Property whiteBalance;
    whiteBalance.type = FlyCapture2::WHITE_BALANCE;
    error = cam.GetProperty( &whiteBalance );
    if (error != FlyCapture2::PGRERROR_OK)
    {
        PrintError( error );
        return -1;
    }

	whiteBalance.valueA=666;
	whiteBalance.valueB=666;
	whiteBalance.autoManualMode=false;

	cam.SetProperty(&whiteBalance);

    cout << "White Balance (Red) is " << fixed << setprecision(2) << whiteBalance.valueA << endl; 
	cout << "White Balance (Blue) is " << fixed << setprecision(2) << whiteBalance.valueB << endl; 
	return 1;
}

void PtGreyCamera::PrintError(FlyCapture2::Error error )
{
    error.PrintErrorTrace();
}

void PtGreyCamera::PrintCameraInfo(FlyCapture2::CameraInfo* pCamInfo )
{
    cout << endl;
	cout << "*** CAMERA INFORMATION ***" << endl;
	cout << "Serial number -" << pCamInfo->serialNumber << endl;
    cout << "Camera model - " << pCamInfo->modelName << endl;
    cout << "Camera vendor - " << pCamInfo->vendorName << endl;
    cout << "Sensor - " << pCamInfo->sensorInfo << endl;
    cout << "Resolution - " << pCamInfo->sensorResolution << endl;
    cout << "Firmware version - " << pCamInfo->firmwareVersion << endl;
    cout << "Firmware build time - " << pCamInfo->firmwareBuildTime << endl << endl;

	
}

void PtGreyCamera::PrintFormat7Capabilities(FlyCapture2::Format7Info fmt7Info )
{
    cout << "Max image pixels: (" << fmt7Info.maxWidth << ", " << fmt7Info.maxHeight << ")" << endl;
	cout << "Image Unit size: (" << fmt7Info.imageHStepSize << ", " << fmt7Info.imageVStepSize << ")" << endl;
    cout << "Offset Unit size: (" << fmt7Info.offsetHStepSize << ", " << fmt7Info.offsetVStepSize << ")" << endl;
	cout << "Pixel format bitfield: 0x" << fmt7Info.pixelFormatBitField << endl;
	
}