#include <mainwindow.h>
#include <ui_mainwindow.h>

#include <iostream>
#include <QFileDialog>
#include <QString>

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this->ui->actionImportNewImage, SIGNAL(triggered()), this, SLOT(importImage()));
    connect(this->ui->actionImportImages, SIGNAL(triggered()), this, SLOT(importFromTextFile()));
    connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(quit()));

    database.initialize();
}

void MainWindow::importImage()
{
    QString filePath = QFileDialog::getOpenFileName(this);

    if ( !filePath.isNull() )
    {
        cout << "filePath: " << filePath.toStdString() << endl;
        database.importImage(filePath.toStdString().c_str());
    }
}

void MainWindow::importFromTextFile()
{
    QString filePath = QFileDialog::getOpenFileName(this);

    if ( !filePath.isNull() )
    {
        cout << " INFO: filePath [" << filePath.toStdString() << "]" << endl;
    }
}

void MainWindow::quit()
{
    qApp->exit();
}

MainWindow::~MainWindow()
{
    delete ui;
}
