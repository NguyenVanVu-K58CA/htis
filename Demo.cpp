#include <iostream>
#include <opencv\cv.h>
#include <opencv\highgui.h>

#include "package_bgs/PBAS/PixelBasedAdaptiveSegmenter.h"
#include "package_tracking/BlobTracking.h"
#include "package_analysis/VehicleCouting.h"

int main(int argc, char **argv)
{
  std::cout << "Using OpenCV " << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << "." << CV_SUBMINOR_VERSION << std::endl;

  /* Open video file
  CvCapture *capture = 0;
  capture = cvCaptureFromAVI("dataset/video.avi");
  if(!capture){
    std::cerr << "Cannot open video!" << std::endl;
    return 1;
  } 

  */

  cv::Mat frameArray[3];

   frameArray[0] = cv::imread("dataset/image1.png");
   frameArray[1] = cv::imread("dataset/image2.png");
   frameArray[2] = cv::imread("dataset/image3.png");
 
  /* Background Subtraction Algorithm */
  IBGS *bgs;
  bgs = new PixelBasedAdaptiveSegmenter;
  
  /* Blob Tracking Algorithm */
  cv::Mat img_blob;
  BlobTracking* blobTracking;
  blobTracking = new BlobTracking;

  /* Vehicle Counting Algorithm */
  VehicleCouting* vehicleCouting;
  vehicleCouting = new VehicleCouting;

  std::cout << "Press 'q' to quit..." << std::endl;
  int key = 0;
  int dem = 0;
  cv::Mat frame;
  while( dem < 3)
  {
    cv::Mat img_input = frameArray[dem];
	//cvResize(frame, frame, (640, 480));
   // if(cvCountNonZero() < 1) break;

    //cv::Mat img_input(frame);
	cv::resize(img_input, img_input, cv::Size(320, 167));
 //   cv::imshow("Input", img_input);


	
	// bgs->process(...) internally process and show the foreground mask image
    cv::Mat img_mask;
    bgs->process(img_input, img_mask);
    
    if(!img_mask.empty())
    {
      // Perform blob tracking
      blobTracking->process(img_input, img_mask, img_blob);

      // Perform vehicle counting
      vehicleCouting->setInput(img_blob);
      vehicleCouting->setTracks(blobTracking->getTracks());
      vehicleCouting->process();
    }
	
	
	dem++;
  // key = cvWaitKey(0);
  }
  system("pause");

  delete vehicleCouting;
  delete blobTracking;
  delete bgs;

  cvDestroyAllWindows();
  //cvReleaseCapture(&capture);
  
  return 0;
}
