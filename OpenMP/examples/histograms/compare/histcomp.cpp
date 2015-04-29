#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/** @function main */
int main( int argc, char** argv )
{
  /// Load two images with different environment settings
  if( argc < 4 )
  {
    printf("** Error. Usage: ./comparehist <hist1.xml> <hist2.xml> <compare_method>\n");
    return -1;
  }

  int method, compare_method;

  method = atoi(argv[3]);
  if (method == 1)
    compare_method = CV_COMP_CORREL; // Correlation
  else if (method == 2)
    compare_method = CV_COMP_CHISQR; // Chi-Square
  else if (method == 3)
    compare_method = CV_COMP_INTERSECT; // Intersection
  else if (method == 4)
    compare_method = CV_COMP_BHATTACHARYYA; // Bhattacharyya distance
  else {
    printf("ERROR: no valid value for compare_method\n");
    exit(1);
  }

  Mat hist_h1, hist_s1, hist_v1;
  Mat hist_h2, hist_s2, hist_v2;

  // Read histogram1
  FileStorage fs1(argv[1], FileStorage::READ);

  string fname1;
  fs1["imageName"] >> fname1;
  fs1["hist_h"] >> hist_h1;
  fs1["hist_s"] >> hist_s1;
  fs1["hist_v"] >> hist_v1;

  fs1.release();

  // Read histogram2
  FileStorage fs2(argv[1], FileStorage::READ);
  string fname2;
  fs2["imageName"] >> fname2;
  fs2["hist_h"] >> hist_h2;
  fs2["hist_s"] >> hist_s2;
  fs2["hist_v"] >> hist_v2;

  fs2.release();

  /// Histogram comparison
  double comphist_h = compareHist( hist_h1, hist_h2, compare_method);
  double comphist_s = compareHist( hist_s1, hist_s2, compare_method);
  double comphist_v = compareHist( hist_v1, hist_v2, compare_method);

  // Compare
  cout << "Comparing " << fname1 << " and " << fname2 << endl;
  cout << "Method " << method << ", Result " << comphist_h + comphist_s + comphist_v << endl;

  return 0;
}
