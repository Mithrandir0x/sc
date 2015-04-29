#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <imagedb.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    ImageDatabase database;

public slots:
    void importImage();
    void importFromTextFile();
    void quit();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
