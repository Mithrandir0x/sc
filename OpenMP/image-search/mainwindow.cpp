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
    updateLoadedImageListView();
    showCurrentDatabaseSize();
}

void MainWindow::importImage()
{
    QString filePath = QFileDialog::getOpenFileName(this);

    if ( !filePath.isNull() )
    {
        cout << " INFO: filePath [" << filePath.toStdString() << "]" << endl;
        database.importImage(filePath.toStdString().c_str());

        updateLoadedImageListView();
        showCurrentDatabaseSize();
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

void MainWindow::updateLoadedImageListView()
{
    ImageEntry* imageEntry;
    ImageDatabaseIterator* iterator;
    QListWidget* listWidget = this->ui->loadedImagesListView;

    listWidget->clear();

    iterator = database.iterator();
    while ( database.next(iterator) )
    {
        imageEntry = iterator->current;
        listWidget->addItem(new QListWidgetItem(QIcon(imageEntry->imageFilePath), imageEntry->name));
    }
}

void MainWindow::showCurrentDatabaseSize()
{
    char str[120];
    sprintf(str, "Current database size [%d]", database.size());
    this->ui->statusBar->showMessage(str);
}

void MainWindow::quit()
{
    qApp->exit();
}

MainWindow::~MainWindow()
{
    delete ui;
}
