#ifndef IMAGEENTRY_H
#define IMAGEENTRY_H

#include <opencv2/core/core.hpp>

using namespace cv;

class ImageEntry
{
public:
    int id;
    Mat *h_hue;
    Mat *h_saturation;
    Mat *h_value;

    ImageEntry();
    ~ImageEntry();
};

#endif // IMAGEENTRY_H
