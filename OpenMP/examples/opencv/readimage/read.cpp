#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
  if( argc != 2)
  {
    cout <<" Usage: read image" << endl;
    return -1;
  }

  Mat image;
  image = imread(argv[1], CV_LOAD_IMAGE_COLOR);

  if (!image.data)
  {
    cout <<  "Could not open or find the image" << std::endl ;
    return -1;
  }

  namedWindow( "Read Demo", CV_WINDOW_AUTOSIZE );
  imshow( "Read Demo", image );

  waitKey(0);
  return 0;
}