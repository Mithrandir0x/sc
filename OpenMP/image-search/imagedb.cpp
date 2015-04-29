#include <imagedb.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define BUFFER_TXTFILE_LINE 200
#define BUFFER_COPYFILE     1024 * 8

ImageDatabase::ImageDatabase()
{
    imageCount = 1;
}

ImageDatabase::~ImageDatabase()
{
}

void ImageDatabase::initialize()
{
    mkdir("./images");
    mkdir("./histogrames");

    // @TODO Import existent image count

    printf(" INFO: Image database size [%d]\n", imageCount);
}

int ImageDatabase::importFromTextFile(const char *filePath)
{
    FILE *imageFileList;
    char imageFilePath[200];
    int l, importedImages = 0;

    imageFileList = fopen(filePath, "r");

    if ( imageFileList == 0 )
    {
        printf("ERROR: File [%s] does not exist or not accessible.\n", filePath);
        return IMG_DB_ERROR_FILE_NOT_FOUND;
    }

    while ( fgets(imageFilePath, BUFFER_TXTFILE_LINE, imageFileList) )
    {
        l = strlen(imageFilePath);
        if ( imageFilePath[l - 1] == '\n' ) imageFilePath[l - 1] = '\0';

        importImage(imageFilePath);

        importedImages++;
    }

    printf(" INFO: Number of imported images [%d]\n", importedImages);
    printf(" INFO: Image database size [%d]\n", imageCount);
    return 0;
}

int ImageDatabase::importImage(const char *imageFilePath)
{
    FILE *originalFile;
    FILE *destinationFile;
    char str[120];
    char buf[BUFFER_COPYFILE];
    size_t size;

    originalFile = fopen(imageFilePath, "rb");
    if ( originalFile == 0 )
    {
        printf("ERROR: Image file [%s] does not exist\n", imageFilePath);
        return IMG_DB_ERROR_FILE_NOT_FOUND;
    }

    sprintf(str, "./images/img_%06d.jpg", imageCount);
    destinationFile = fopen(str, "wb");
    if ( destinationFile == 0 )
    {
        printf("ERROR: Could not create file [%s]\n", str);
        return IMG_DB_ERROR_FILE_ERROR;
    }

    while ( size = fread(buf, 1, BUFFER_COPYFILE, originalFile) ) {
        fwrite(buf, 1, size, destinationFile);
    }

    fclose(originalFile);
    fclose(destinationFile);

    // @TODO Generate histogram

    printf(" INFO: New image imported [%s] to [img_%06d]\n", imageFilePath, imageCount);

    imageCount++;

    return 0;
}
