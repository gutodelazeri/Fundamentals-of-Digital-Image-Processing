#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->canvas_original->setScaledContents( true );
    ui->canvas_original->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );

    ui->canvas_edited->setScaledContents( true );
    ui->canvas_edited->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_flip_Horizontally_clicked()
{
    if(this->EditedImage.getThereIsAnImage())
    {
        this->EditedImage.horizontalMirroring();
        Mat img = this->EditedImage.getRGBImage();
        ui->canvas_edited->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, int(img.step), QImage::Format_RGB888)));
    }
}

void MainWindow::on_flip_Vertically_clicked()
{
    if(this->EditedImage.getThereIsAnImage())
    {
        this->EditedImage.verticalMirroring();
        Mat img = this->EditedImage.getRGBImage();
        ui->canvas_edited->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, int(img.step), QImage::Format_RGB888)));
    }
}

void MainWindow::on_convertToGray_clicked()
{
    if(this->EditedImage.getThereIsAnImage())
    {
        this->EditedImage.convertToGray();
        Mat img = this->EditedImage.getRGBImage();
        ui->canvas_edited->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, int(img.step), QImage::Format_RGB888)));
    }
}

void MainWindow::on_pasteImage_clicked()
{
    if(this->OriginalImage.getThereIsAnImage())
    {
        this->EditedImage.setMainImage(this->OriginalImage.getMainImage());
        this->EditedImage.setThereIsAnImage(true);

        Mat img = this->EditedImage.getRGBImage();
        ui->canvas_edited->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, int(img.step), QImage::Format_RGB888)));
    }
}

void MainWindow::on_actionOpen_Image_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), tr("Image Files (*.png *.jpg *.bmp)"));
    if(!fileName.isNull())
    {
        this->OriginalImage.setMainImage(fileName);
        Mat img = this->OriginalImage.getRGBImage();
        ui->canvas_original->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, int(img.step), QImage::Format_RGB888)));
    }
}

void MainWindow::on_actionSave_Image_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save your image"), tr("Image Files (*.png *.jpg *.bmp)"));
    if(!fileName.isNull() && this->EditedImage.getThereIsAnImage())
    {
       this->EditedImage.saveMainImage(fileName);
    }

}

void MainWindow::on_lineEdit_returnPressed()
{
    QString userInput = ui->lineEdit->text();
    ui->lineEdit->clear();
    int tones = userInput.toInt();

    if(this->EditedImage.getThereIsAnImage())
    {

        if (tones < 1 || tones > 256)
        {
            QMessageBox msg;
            msg.setText("Your input must be an integer greater than zero and smallest than 257!");
            msg.setIcon(QMessageBox::Information);
            msg.setWindowTitle("Something went wrong :(");
            msg.exec();
            ui->lineEdit->clear();
        }
        else
        {
            Mat img = this->EditedImage.quantization(tones);
            ui->canvas_edited->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, int(img.step), QImage::Format_RGB888)));
            ui->lineEdit->clear();
        }
    }
}

