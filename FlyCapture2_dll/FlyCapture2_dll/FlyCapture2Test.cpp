//=============================================================================
// Copyright ?2017 FLIR Integrated Imaging Solutions, Inc. All Rights Reserved.
//
// This software is the confidential and proprietary information of FLIR
// Integrated Imaging Solutions, Inc. ("Confidential Information"). You
// shall not disclose such Confidential Information and shall use it only in
// accordance with the terms of the license agreement you entered into
// with FLIR Integrated Imaging Solutions, Inc. (FLIR).
//
// FLIR MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE
// SOFTWARE, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE, OR NON-INFRINGEMENT. FLIR SHALL NOT BE LIABLE FOR ANY DAMAGES
// SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR DISTRIBUTING
// THIS SOFTWARE OR ITS DERIVATIVES.
//=============================================================================
//=============================================================================
// $Id: FlyCapture2Test.cpp 316528 2017-02-22 00:03:53Z alin $
//=============================================================================

#include "stdafx.h"
#include "opencv2/opencv.hpp"
#include "FlyCapture2.h"
#include <iostream>
#include <sstream>
#include "imageProcessing.h"

using namespace FlyCapture2;
//using namespace std;

ImageProcessing ImageProcessor;


int main(int /*argc*/, char ** /*argv*/)
{

	printf("open camera\n");
	ImageProcessor.initialCamera();
	while (true) {
		ImageProcessor.takePic();
		int key = cv::waitKey(1000*1/30);
		if (key >= 0) {
			break;
		}
	}       
    return 0;
}
