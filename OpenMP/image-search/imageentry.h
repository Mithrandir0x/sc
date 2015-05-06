#ifndef IMAGEENTRY_H
#define IMAGEENTRY_H

#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

class ImageEntry
{
public:
    int id;
    char imageFilePath[260];
    char name[260];
    Mat *h_hue;
    Mat *h_saturation;
    Mat *h_value;

    ImageEntry();
    ~ImageEntry();

    double compare(ImageEntry *imageEntry, int compareMethod);

    // This allows ImageEntry to be pretty printed like java toString
    // the friend particle is to allow access to private fields
    friend ostream& operator<<(ostream &str, const ImageEntry &ie) {
        return str << "[ImageEntry id [" <<ie.id << "] " <<
               "name [" << ie.name << "]" <<
               "]";
    }
};

#endif // IMAGEENTRY_H
