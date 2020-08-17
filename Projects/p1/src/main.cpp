#include "mainwindow.h"
#include "imageeditor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Mat image;
    ImageEditor mainImage(image);
    MainWindow w;
    w.show();

    return a.exec();
}
