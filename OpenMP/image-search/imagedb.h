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
} ImageDatabaseIterator;

class ImageDatabase
{
public:
    ImageDatabase();
    ~ImageDatabase();

    void initialize();
    ImageEntry* importExistentHistogram(int id);

    int importFromTextFile(const char *filePath);
    int importImage(const char *imageFilePath);

    void addImageEntry(ImageEntry *imageEntry);

    ImageEntry* loadImage(const char *imageFilePath);
    void saveHistogram(ImageEntry* imageEntry);

    ImageDatabaseIterator* iterator();
    int next(ImageDatabaseIterator *iterator);

    int size();

private:
    int imageCount;
    vector<ImageEntry*> images;
};

#endif // IMAGEDATABASE_H
