#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "imageeditor.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_flip_Horizontally_clicked();

    void on_flip_Vertically_clicked();

    void on_convertToGray_clicked();

    void on_pasteImage_clicked();

    void on_actionOpen_Image_triggered();

    void on_actionSave_Image_triggered();

    void on_lineEdit_returnPressed();

private:
    ImageEditor OriginalImage;
    ImageEditor EditedImage;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
