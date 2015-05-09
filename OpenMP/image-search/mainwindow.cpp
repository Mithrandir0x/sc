#include <mainwindow.h>
#include <ui_mainwindow.h>

#include <iostream>
#include <vector>

#include <QFileDialog>
#include <QString>

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this->ui->actionImportNewImage, SIGNAL(triggered()), this, SLOT(importImage()));
    connect(this->ui->actionImportImages, SIGNAL(triggered()), this, SLOT(importFromTextFile()));
    connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(quit()));
    connect(this->ui->actionSearchMostCommonImages, SIGNAL(triggered()), this, SLOT(searchImage()));

    database.initialize();
    updateLoadedImageListView();
    showCurrentDatabaseSize();
}

void MainWindow::importImage()
{
    QStringList filePaths = QFileDialog::getOpenFileNames(this);

    database.importImages(&filePaths);

    updateLoadedImageListView();
    showCurrentDatabaseSize();
}

void MainWindow::importFromTextFile()
{
    QString filePath = QFileDialog::getOpenFileName(this);

    if ( !filePath.isNull() )
    {
        cout << " INFO: filePath [" << filePath.toStdString() << "]" << endl;
        database.importFromTextFile(filePath.toStdString().c_str());
    }

    updateLoadedImageListView();
    showCurrentDatabaseSize();
}

void MainWindow::updateLoadedImageListView()
{
    ImageEntry *imageEntry;
    ImageDatabaseIterator* iterator;
    QTabWidget *tabWidget = this->ui->tabWidget;
    QListWidget *listWidget = this->ui->loadedImagesListView;

    tabWidget->setCurrentIndex(0);
    listWidget->clear();

    iterator = database.iterator(50);
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

void MainWindow::searchImage()
{
    QTabWidget *tabWidget = this->ui->tabWidget;
    QListWidget *listWidget = this->ui->mostCommonImagesListView;
    QString filePath = QFileDialog::getOpenFileName(this);

    tabWidget->setCurrentIndex(1);

    if ( !filePath.isNull() )
    {
        cout << " INFO: filePath [" << filePath.toStdString() << "]" << endl;
        vector<ImageEntry*> candidates = database.searchCommonImages(filePath.toStdString().c_str(), 4);

        listWidget->clear();
        for ( int i = 0, n = candidates.size() ; i < n ; i++ )
        {
            listWidget->addItem(new QListWidgetItem(QIcon(candidates[i]->imageFilePath), candidates[i]->name));
        }
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
