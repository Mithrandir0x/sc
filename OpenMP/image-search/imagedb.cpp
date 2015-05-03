#include <imagedb.h>

#include <iostream>
#include <iomanip>

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define BUFFER_TXTFILE_LINE 200
#define BUFFER_COPYFILE     1024 * 8

using namespace std;

ImageDatabase::ImageDatabase()
{
    imageCount = 1;
}

ImageDatabase::~ImageDatabase()
{
}

void ImageDatabase::initialize()
{
    DIR *folder;
    struct dirent *file;
    char imageId[7];
    int id;

    mkdir("./images");
    mkdir("./histogrames");

    if ( ( folder = opendir("./histogrames") ) != NULL ) {
        cout << " INFO: Loading current database..." << endl;

        while ( ( file = readdir(folder) ) != NULL ) {
            if ( strcmp(file->d_name, ".") != 0 && strcmp(file->d_name, "..") != 0 ) {
                memcpy(imageId, file->d_name + 4, 6);
                imageId[6] = '\0';
                id = atoi(imageId);
                //cout << " INFO: substr [" << imageId << "]" << endl;
                //cout << " INFO: id [" << id << "]" << endl;

                ImageEntry *imageEntry = importExistentHistogram(id);

                imageCount++;
                images.push_back(imageEntry);

                cout << " INFO: Loaded histogram [" << file->d_name << "]" << endl;
            }
        }
        closedir(folder);
    }

    cout << " INFO: Image database size [" << images.size() << "]" << endl;
}

int ImageDatabase::importFromTextFile(const char *filePath)
{
    FILE *imageFileList;
    char imageFilePath[200];
    int l, importedImages = 0;

    imageFileList = fopen(filePath, "r");

    if ( imageFileList == 0 )
    {
        cout << "ERROR: File [" << filePath << "] does not exist or not accessible." << endl;
        return IMG_DB_ERROR_FILE_NOT_FOUND;
    }

    while ( fgets(imageFilePath, BUFFER_TXTFILE_LINE, imageFileList) )
    {
        l = strlen(imageFilePath);
        if ( imageFilePath[l - 1] == '\n' ) imageFilePath[l - 1] = '\0';

        importImage(imageFilePath);

        importedImages++;
    }

    cout << " INFO: Number of imported images [" << importedImages << "]" << endl;
    cout << " INFO: Image database size [" << images.size() << "]" << endl;
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
        cout << "ERROR: Image file [" << imageFilePath << "] does not exist" << endl;
        return IMG_DB_ERROR_FILE_NOT_FOUND;
    }

    sprintf(str, "./images/img_%06d.jpg", imageCount);
    destinationFile = fopen(str, "wb");
    if ( destinationFile == 0 )
    {
        cout << "ERROR: Could not create file [" << str << "]" << endl;
        return IMG_DB_ERROR_FILE_ERROR;
    }

    while ( size = fread(buf, 1, BUFFER_COPYFILE, originalFile) ) {
        fwrite(buf, 1, size, destinationFile);
    }

    fclose(originalFile);
    fclose(destinationFile);

    ImageEntry *imageEntry = generateHistogram(currentId);

    imageCount++;
    images.push_back(imageEntry);

    cout << " INFO: New image imported ["
         << imageFilePath
         << "] to [img_"
         << setfill('0') << setw(6)
         << currentId
         << "]" << endl;

    cout << " INFO: Image database current size [" << images.size() << "]" << endl;

    return 0;
}

ImageEntry* ImageDatabase::generateHistogram(int id)
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
    Mat *hist_h = new Mat();
    Mat *hist_s = new Mat();
    Mat *hist_v = new Mat();

    /// Calculate the histogram for the H image
    calcHist( &hsv_planes[0], 1, 0, Mat(), hist_h[0], 1, &h_bins, &h_ranges, true, false );
    normalize( hist_h[0], hist_h[0], 0, 1, NORM_MINMAX, -1, Mat() );

    calcHist( &hsv_planes[1], 1, 0, Mat(), hist_s[0], 1, &s_bins, &s_ranges, true, false );
    normalize( hist_s[0], hist_s[0], 0, 1, NORM_MINMAX, -1, Mat() );

    calcHist( &hsv_planes[2], 1, 0, Mat(), hist_v[0], 1, &v_bins, &v_ranges, true, false );
    normalize( hist_v[0], hist_v[0], 0, 1, NORM_MINMAX, -1, Mat() );

    // Store histograms on disc
    sprintf(str, "./histogrames/img_%06d.xml", id);
    FileStorage fs(str, FileStorage::WRITE);

    fs << "imageName" << str;
    fs << "hist_h" << hist_h[0];
    fs << "hist_s" << hist_s[0];
    fs << "hist_v" << hist_v[0];

    fs.release();

    // Store new ImageEntry
    ImageEntry *imageEntry = new ImageEntry();
    imageEntry->id = id;
    imageEntry->h_hue = hist_h;
    imageEntry->h_saturation = hist_s;
    imageEntry->h_value = hist_v;

    return imageEntry;
}

ImageEntry* ImageDatabase::importExistentHistogram(int id)
{
    // Histograms
    Mat *hist_h = new Mat();
    Mat *hist_s = new Mat();
    Mat *hist_v = new Mat();

    char str[120];
    sprintf(str, "./histogrames/img_%06d.xml", id);
    FileStorage fs(str, FileStorage::READ);

    // This is friggin' stoopid. I can send a char* but I cannot receive
    // a friggin' char*? Why oh why...
    string fname;
    fs["imageName"] >> fname;
    fs["hist_h"] >> hist_h[0];
    fs["hist_s"] >> hist_s[0];
    fs["hist_v"] >> hist_v[0];

    fs.release();

    ImageEntry *imageEntry = new ImageEntry();
    imageEntry->h_hue = hist_h;
    imageEntry->h_saturation = hist_s;
    imageEntry->h_value = hist_v;

    return imageEntry;
}
