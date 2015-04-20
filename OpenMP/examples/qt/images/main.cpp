#include <QApplication>
#include "images.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv); 

  QGridImages window;

  window.move(300, 300);
  window.setWindowTitle("View Images");
  window.show();

  return app.exec();
}
