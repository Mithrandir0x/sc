#include <imagedb.h>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>

#include <limits.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define BUFFER_TXTFILE_LINE 200
#define BUFFER_COPYFILE     1024 * 8

using namespace std;

// On Windows with MinGW, realpath is not supported, so
// just fallback to the windows based implementation
#if defined(WIN32)
#define realpath(N,R) _fullpath((R),(N),_MAX_PATH)
#endif

// On Linux, mkdir requires 2 arguments, so we create with
// the most secure way humankind has ever designed... Nope.
#ifdef __linux__
#define mkdir(x) mkdir(x, 0777)
#endif

ImageDatabase::ImageDatabase()
{
    //imageCount = 1;
}

ImageDatabase::~ImageDatabase()
{
}

void ImageDatabase::initialize()
{
    DIR *folder;
    struct dirent *file;
    char imageId[7];
    unsigned int i;
    vector<int> ids;

    struct timespec start, end;
    double time;

    clock_gettime(CLOCK_MONOTONIC, &start);

    mkdir("./images");
    mkdir("./histogrames");

    cout << "images size [" << images.size() << "]" << endl;

    if ( ( folder = opendir("./histogrames") ) != NULL ) {
        cout << " INFO: Loading current database..." << endl;

        while ( ( file = readdir(folder) ) != NULL ) {
            if ( strcmp(file->d_name, ".") != 0 && strcmp(file->d_name, "..") != 0 ) {
                memcpy(imageId, file->d_name + 4, 6);
                imageId[6] = '\0';
                ids.push_back(atoi(imageId));
            }
        }
        closedir(folder);
        images.resize(ids.size());

        #pragma omp parallel for
        for ( i = 0 ; i < ids.size() ; i++ )
        {
            images[ids[i]] = importExistentHistogram(ids[i]);

            #ifdef ENABLE_DEBUG_LOG
            cout << "DEBUG: Loaded [" << ids[i] << "] histogram " << *images[ids[i]] << endl;
            #endif
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    time = end.tv_sec - start.tv_sec;
    time += ( end.tv_nsec - start.tv_nsec ) / 1000000000.0;

    cout << " INFO: 'initialize' time [" << time << "]" << endl;
    cout << " INFO: Image database size [" << images.size() << "]" << endl;
}

int ImageDatabase::importFromTextFile(const char *filePath)
{
    ifstream imageFileList(filePath);
    vector<string*> filePaths;
    string *line = new string();
    unsigned int i, k = images.size();

    struct timespec start, end;
    double time;

    clock_gettime(CLOCK_MONOTONIC, &start);

    if ( imageFileList.is_open() )
    {
        while ( getline(imageFileList, *line) )
        {
            filePaths.push_back(line);
            line = new string();
        }

        images.resize(images.size() + filePaths.size());

        #pragma omp parallel for
        for ( i = 0 ; i < filePaths.size() ; i++ )
        {
            images[k + i] = importImage(filePaths[i]->c_str(), k + i);
        }
    }
    else
    {
        cout << "ERROR: File [" << filePath << "] does not exist or not accessible." << endl;
        return IMG_DB_ERROR_FILE_NOT_FOUND;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    time = end.tv_sec - start.tv_sec;
    time += ( end.tv_nsec - start.tv_nsec ) / 1000000000.0;

    cout << " INFO: 'importFromTextFile' time [" << time << "]" << endl;
    cout << " INFO: Image database size [" << images.size() << "]" << endl;
    return 0;
}

int ImageDatabase::importImages(QStringList *filePaths)
{
    int i, k = images.size();

    struct timespec start, end;
    double time;

    clock_gettime(CLOCK_MONOTONIC, &start);

    images.resize(images.size() + filePaths->size());

    #pragma omp parallel for
    for ( i = 0 ; i < filePaths->size() ; i++ )
    {
        images[k + i] = importImage(filePaths->at(i).toStdString().c_str(), k + i);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    time = end.tv_sec - start.tv_sec;
    time += ( end.tv_nsec - start.tv_nsec ) / 1000000000.0;

    cout << " INFO: 'importFromTextFile' time [" << time << "]" << endl;
    cout << " INFO: Image database size [" << images.size() << "]" << endl;
    return 0;
}

ImageEntry* ImageDatabase::importImage(const char *imageFilePath, int currentId)
{
    FILE *originalFile;
    FILE *destinationFile;
    char str[120];
    char buf[BUFFER_COPYFILE];
    size_t size;

    originalFile = fopen(imageFilePath, "rb");
    if ( originalFile == 0 )
    {
        cout << "ERROR: Image file [" << imageFilePath << "] does not exist" << endl;
        return NULL;
    }

    sprintf(str, "./images/img_%06d.jpg", currentId);
    destinationFile = fopen(str, "wb");
    if ( destinationFile == 0 )
    {
        cout << "ERROR: Could not create file [" << str << "]" << endl;
        return NULL;
    }

    while ( size = fread(buf, 1, BUFFER_COPYFILE, originalFile) ) {
        fwrite(buf, 1, size, destinationFile);
    }

    fclose(originalFile);
    fclose(destinationFile);

    ImageEntry *imageEntry = loadImage(str);
    imageEntry->id = currentId;
    saveHistogram(imageEntry);

    #ifdef ENABLE_DEBUG_LOG
    cout << " DEBUG: New image imported ["
         << imageFilePath
         << "] to [img_"
         << setfill('0') << setw(6)
         << currentId
         << "]" << endl;
    #endif

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
    imageEntry->id = id;
    imageEntry->h_hue = hist_h;
    imageEntry->h_saturation = hist_s;
    imageEntry->h_value = hist_v;
    strcpy(imageEntry->name, fname.c_str());
    realpath(fname.c_str(), imageEntry->imageFilePath);

    return imageEntry;
}

ImageEntry* ImageDatabase::loadImage(const char *imageFilePath)
{
    Mat src_test, hsv_test;
    vector<Mat> hsv_planes;

    src_test = imread(imageFilePath, CV_LOAD_IMAGE_COLOR);

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

    /// Create new ImageEntry
    ImageEntry *imageEntry = new ImageEntry();
    imageEntry->id = -1;
    imageEntry->h_hue = hist_h;
    imageEntry->h_saturation = hist_s;
    imageEntry->h_value = hist_v;
    strcpy(imageEntry->name, imageFilePath);
    realpath(imageFilePath, imageEntry->imageFilePath);

    #ifdef ENABLE_DEBUG_LOG
    cout << "DEBUG: Created image entry " << imageEntry << endl;
    #endif

    return imageEntry;
}

void ImageDatabase::saveHistogram(ImageEntry* imageEntry)
{
    char str[120];

    sprintf(str, "./histogrames/img_%06d.xml", imageEntry->id);
    FileStorage fs(str, FileStorage::WRITE);

    fs << "imageName" << imageEntry->name;
    fs << "hist_h" << imageEntry->h_hue[0];
    fs << "hist_s" << imageEntry->h_saturation[0];
    fs << "hist_v" << imageEntry->h_value[0];

    fs.release();
}

ImageDatabaseIterator* ImageDatabase::iterator() {
    return iterator(images.size());
}

ImageDatabaseIterator* ImageDatabase::iterator(unsigned int max) {
    ImageDatabaseIterator *iter;
    iter = (ImageDatabaseIterator *) malloc(sizeof(ImageDatabaseIterator));
    iter->i = -1;
    iter->current = NULL;
    iter->max = max < images.size() ? max : images.size();
    return iter;
}

int ImageDatabase::next(ImageDatabaseIterator *iterator) {
    iterator->i++;
    if ( iterator->i < images.size() && iterator->i < iterator->max ) {
        iterator->current = images[iterator->i];
        return -1;
    } else {
        iterator->current = NULL;
        free(iterator);
        return 0;
    }
}

int ImageDatabase::size()
{
    return images.size();
}

typedef struct SearchValueType {
    double v;
    ImageEntry *e;
} SearchValue;

static int compareSearchValues(const void *p, const void *q)
{
    SearchValue x = *(SearchValue*) p;
    SearchValue y = *(SearchValue*) q;

    if ( x.v == y.v )
        return 0;
    else
        return ( x.v < y.v ) ? -1 : 1;
}

static void sort(SearchValue *array, size_t n)
{
    qsort(array, n, sizeof(SearchValue), compareSearchValues);
}

vector<ImageEntry*> ImageDatabase::searchCommonImages(const char *imageFilePath, int max)
{
    int i, n = images.size();
    ImageEntry *imageEntry = loadImage(imageFilePath);
    SearchValue compareValues[n];
    vector<ImageEntry*> candidates;

    struct timespec start, end;
    double time;

    clock_gettime(CLOCK_MONOTONIC, &start);

    #pragma omp parallel shared(n, imageEntry) private(i)
    {
        #pragma omp for
        for ( i = 0 ; i < n ; i++ )
        {
            compareValues[i].e = images[i];
            // http://docs.opencv.org/modules/imgproc/doc/histograms.html?highlight=comparehist#comparehist
            // 4. CV_COMP_BHATTACHARYYA
            // 3. CV_COMP_INTERSECT
            // 2. CV_COMP_CHISQR
            // 1. CV_COMP_CORREL
            compareValues[i].v = imageEntry->compare(images[i], CV_COMP_CORREL);

            #ifdef ENABLE_DEBUG_LOG
            cout << "DEBUG: Compare value [" << compareValues[i].v << "] with image " << *images[i] << endl;
            #endif
        }
    }

    cout << " INFO: Sorting values" << endl;
    sort(compareValues, n);

    for ( i = 1 ; i <= max ; i++ )
    {
        cout << " INFO: Compare value [" << compareValues[n - i].v << "] with image " << *compareValues[n - i].e << endl;
        candidates.push_back(compareValues[n - i].e);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    time = end.tv_sec - start.tv_sec;
    time += ( end.tv_nsec - start.tv_nsec ) / 1000000000.0;

    cout << " INFO: 'searchCommonImages' time [" << time << "]" << endl;

    return candidates;
}
