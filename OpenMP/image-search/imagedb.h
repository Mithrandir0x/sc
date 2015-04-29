#ifndef IMAGEDATABASE_H
#define IMAGEDATABASE_H

#include <string>

#define IMG_DB_ERROR_FILE_NOT_FOUND -1
#define IMG_DB_ERROR_FILE_ERROR     -2

class ImageDatabase
{
public:
    ImageDatabase();
    ~ImageDatabase();

    void initialize();
    int importFromTextFile(const char *filePath);
    int importImage(const char *imageFilePath);

private:
    int imageCount;
};

#endif // IMAGEDATABASE_H
