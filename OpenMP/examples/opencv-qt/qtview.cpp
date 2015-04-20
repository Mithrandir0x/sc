#include <iostream>

// Qt
#include <QtGui>

// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
  if( argc != 2)
  {
    cout <<" Usage: qtview image" << endl;
    return -1;
  }

  // Llegim les dades via OpenCV
  Mat img;
  img = imread(argv[1], CV_LOAD_IMAGE_COLOR);

  if(!img.data)                        
  {
    cout <<  "Could not open or find the image" << endl;
    return -1;
  }

  // Transformem a QImage de Qt 
  QImage qimg = QImage((uchar*) img.data, img.cols, img.rows, 
                      img.step, QImage::Format_RGB888);

  // Aplicació Qt que visualitza la imatge
  QApplication app(argc, argv);

  QLabel label;
  label.setPixmap(QPixmap::fromImage(qimg));
  label.show();

  return app.exec();
}
