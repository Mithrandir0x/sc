#include <QApplication>
#include <QtGui>
#include <QLabel>

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  QImage image("image1.png");

  QLabel label;
  label.setPixmap(QPixmap::fromImage(image));
  label.show();

  return app.exec();
}
