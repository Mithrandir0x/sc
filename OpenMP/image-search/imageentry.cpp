#include <imageentry.h>

#include <stdlib.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;

ImageEntry::ImageEntry()
{
    h_hue = NULL;
    h_saturation = NULL;
    h_value = NULL;
}

double ImageEntry::compare(ImageEntry *imageEntry, int compareMethod)
{
    double c_hue, c_saturation, c_value;

    c_hue = compareHist(h_hue[0], imageEntry->h_hue[0], compareMethod);
    c_saturation = compareHist(h_saturation[0], imageEntry->h_saturation[0], compareMethod);
    c_value = compareHist(h_value[0], imageEntry->h_value[0], compareMethod);

    return c_hue + c_saturation + c_value;
}

ImageEntry::~ImageEntry()
{
    if ( h_hue != NULL ) delete h_hue;
    if ( h_saturation != NULL ) delete h_saturation;
    if ( h_value != NULL ) delete h_value;
}

