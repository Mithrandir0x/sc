#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

void detectAndDisplay(Mat frame)
{
  vector<Rect> faces;
  Mat frame_gray;
  
  CascadeClassifier face_cascade;
  
  // Load the cascades
  if (!face_cascade.load("haarcascade_frontalface_alt.xml")) { 
    cout << "-- Error loading cascade\n" << endl; exit(1); 
  }

  cvtColor(frame, frame_gray, CV_BGR2GRAY);
  equalizeHist(frame_gray, frame_gray);

  // Detect faces
  face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 
				 0|CV_HAAR_SCALE_IMAGE, Size(30, 30));
  
  // Draw ellipses in image
  for( int i = 0; i < faces.size(); i++ )
  {
    Point center(faces[i].x + faces[i].width*0.5, 
		 faces[i].y + faces[i].height*0.5 );
    ellipse(frame, center, Size(faces[i].width*0.5, 
            faces[i].height*0.5), 0, 0, 360, Scalar(255, 0, 255 ), 
	    4, 8, 0);
  }
  // Show result
  imshow("Face detection demo", frame );
}

int main( int argc, const char** argv )
{
  if( argc != 2)
  {
    cout <<" Usage: face image" << endl;
    return -1;
  }

  // Read the image
  Mat image;
  image = imread(argv[1], CV_LOAD_IMAGE_COLOR);  

  if(! image.data )                             
  {
    cout <<  "Could not open or find the image" << endl;
    return -1;
  }

  detectAndDisplay(image);

  int c = waitKey(0);

  return 0;
}

