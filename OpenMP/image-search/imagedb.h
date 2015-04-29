#ifndef IMAGEDATABASE_H
#define IMAGEDATABASE_H

#include <string>

#include <opencv2/core/core.hpp>

#define IMG_DB_ERROR_FILE_NOT_FOUND -1
#define IMG_DB_ERROR_FILE_ERROR     -2

using namespace cv;

typedef struct {
    int id;
    Mat h_hue;
    Mat h_saturation;
    Mat h_value;
} ImageEntry;

class ImageDatabase
{
public:
    ImageDatabase();
    ~ImageDatabase();

    void initialize();
    int importFromTextFile(const char *filePath);
    int importImage(const char *imageFilePath);
    int generateHistogram(int id);

private:
    int imageCount;
};

#endif // IMAGEDATABASE_H
