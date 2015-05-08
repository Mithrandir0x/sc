#ifndef IMAGEDATABASE_H
#define IMAGEDATABASE_H

#include <imageentry.h>

#include <vector>

#include <opencv2/core/core.hpp>

#define IMG_DB_ERROR_FILE_NOT_FOUND -1
#define IMG_DB_ERROR_FILE_ERROR     -2

using namespace cv;
using namespace std;

typedef struct {
    unsigned int i;
    ImageEntry *current;
    unsigned int max;
} ImageDatabaseIterator;

class ImageDatabase
{
public:
    ImageDatabase();
    ~ImageDatabase();

    void initialize();

    int importFromTextFile(const char *filePath);
    int importImage(const char *imageFilePath);

    vector<ImageEntry*> searchCommonImages(const char *imageFilePath, int max);

    ImageDatabaseIterator* iterator();
    ImageDatabaseIterator* iterator(unsigned int max);
    int next(ImageDatabaseIterator *iterator);

    int size();

private:
    //int imageCount;
    vector<ImageEntry*> images;

    ImageEntry* importExistentHistogram(int id);

    void addImageEntry(ImageEntry *imageEntry);

    ImageEntry* loadImage(const char *imageFilePath);
    void saveHistogram(ImageEntry* imageEntry);
};

#endif // IMAGEDATABASE_H
