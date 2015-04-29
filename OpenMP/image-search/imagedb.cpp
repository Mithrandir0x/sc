#include <imagedb.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define BUFFER_TXTFILE_LINE 200
#define BUFFER_COPYFILE     1024 * 8

using namespace cv;

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

    int currentId = imageCount;

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

    generateHistogram(currentId);

    printf(" INFO: New image imported [%s] to [img_%06d]\n", imageFilePath, currentId);

    imageCount++;

    return 0;
}

int ImageDatabase::generateHistogram(int id)
{
    Mat src_test, hsv_test;
    vector<Mat> hsv_planes;
    char str[120];

    // Load image
    sprintf(str, "./images/img_%06d.jpg", id);
    src_test = imread(str, CV_LOAD_IMAGE_COLOR);

    /// Convert to HSV
    cvtColor(src_test, hsv_test, CV_BGR2HSV );

    // Extract HSV planes
    split(hsv_test, hsv_planes);

    /// Bins to use
    int h_bins = 50; int s_bins = 50; int v_bins = 100;

    // Ranges
    float hrang[] = {0, 180};
    const float *h_ranges = { hrang };

    float srang[] = {0, 256};
    const float *s_ranges = { srang };

    float vrang[] = {0, 256};
    const float *v_ranges = { vrang };

    /// Histograms
    Mat hist_h, hist_s, hist_v;

    /// Calculate the histogram for the H image
    calcHist( &hsv_planes[0], 1, 0, Mat(), hist_h, 1, &h_bins, &h_ranges, true, false );
    normalize( hist_h, hist_h, 0, 1, NORM_MINMAX, -1, Mat() );

    calcHist( &hsv_planes[1], 1, 0, Mat(), hist_s, 1, &s_bins, &s_ranges, true, false );
    normalize( hist_s, hist_s, 0, 1, NORM_MINMAX, -1, Mat() );

    calcHist( &hsv_planes[2], 1, 0, Mat(), hist_v, 1, &v_bins, &v_ranges, true, false );
    normalize( hist_v, hist_v, 0, 1, NORM_MINMAX, -1, Mat() );

    // Store histograms on disc
    sprintf(str, "./histogrames/img_%06d.xml", id);
    FileStorage fs(str, FileStorage::WRITE);

    fs << "imageName" << str;
    fs << "hist_h" << hist_h;
    fs << "hist_s" << hist_s;
    fs << "hist_v" << hist_v;

    fs.release();

    return 0;
}
