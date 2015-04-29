#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/** @function main */
int main( int argc, char** argv )
{
  Mat src_test, hsv_test;
  vector<Mat> hsv_planes;

  /// Load two images with different environment settings
  if( argc < 3 )
  {
    printf("** Error. Usage: ./histextract <image> <file.xml>\n");
    return -1;
  }

  // Load image
  src_test = imread(argv[1], CV_LOAD_IMAGE_COLOR);

  /// Convert to HSV
  cvtColor(src_test, hsv_test, CV_BGR2HSV );

  // Extract HSV planes
  split(hsv_test, hsv_planes);

  /// Bins to use 
  int h_bins = 50; int s_bins = 50; int v_bins = 100;

  // Ranges 
  float hrang[] = {0, 180};
  const float *h_ranges = { hrang };
  
  float srang[] = {0, 256};
  const float *s_ranges = { srang };

  float vrang[] = {0, 256};
  const float *v_ranges = { vrang };

  /// Histograms
  Mat hist_h, hist_s, hist_v;

  /// Calculate the histogram for the H image
  calcHist( &hsv_planes[0], 1, 0, Mat(), hist_h, 1, &h_bins, &h_ranges, true, false );
  normalize( hist_h, hist_h, 0, 1, NORM_MINMAX, -1, Mat() );

  calcHist( &hsv_planes[1], 1, 0, Mat(), hist_s, 1, &s_bins, &s_ranges, true, false );
  normalize( hist_s, hist_s, 0, 1, NORM_MINMAX, -1, Mat() );

  calcHist( &hsv_planes[2], 1, 0, Mat(), hist_v, 1, &v_bins, &v_ranges, true, false );
  normalize( hist_v, hist_v, 0, 1, NORM_MINMAX, -1, Mat() );

  // Store histograms on disc
  FileStorage fs(argv[2], FileStorage::WRITE);

  fs << "imageName" << argv[1];
  fs << "hist_h" << hist_h;
  fs << "hist_s" << hist_s;
  fs << "hist_v" << hist_v;

  fs.release();
  
  return 0;
}
