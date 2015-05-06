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
    ImageDatabase database;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void showCurrentDatabaseSize();

public slots:
    void importImage();
    void importFromTextFile();
    void updateLoadedImageListView();
    void searchImage();
    void quit();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
