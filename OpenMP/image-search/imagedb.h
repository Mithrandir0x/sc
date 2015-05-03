#ifndef IMAGEDATABASE_H
#define IMAGEDATABASE_H

#include <imageentry.h>

#include <vector>

#include <opencv2/core/core.hpp>

#define IMG_DB_ERROR_FILE_NOT_FOUND -1
#define IMG_DB_ERROR_FILE_ERROR     -2

using namespace cv;
using namespace std;

class ImageDatabase
{
public:
    ImageDatabase();
    ~ImageDatabase();

    void initialize();
    int importFromTextFile(const char *filePath);
    int importImage(const char *imageFilePath);
    ImageEntry* importExistentHistogram(int id);
    ImageEntry* generateHistogram(int id);

private:
    int imageCount;
    vector<ImageEntry*> images;
};

#endif // IMAGEDATABASE_H
