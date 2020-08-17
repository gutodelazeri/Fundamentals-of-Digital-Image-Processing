#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QtCharts>

using namespace QtCharts;

#define LUMINANCE 0
#define LAB       1
#define GRAY      2

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->canvas_original->setScaledContents(true);
    ui->canvas_original->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );

    ui->canvas_edited->setScaledContents(true);
    ui->canvas_edited->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );

    ui->convolutionFilters->addItem("Default");
    ui->convolutionFilters->addItem("Gaussian");
    ui->convolutionFilters->addItem("Laplacian");
    ui->convolutionFilters->addItem("Generic Highpass");
    ui->convolutionFilters->addItem("Prewitt Hx");
    ui->convolutionFilters->addItem("Prewitt Hy Hx");
    ui->convolutionFilters->addItem("Sobel Hx");
    ui->convolutionFilters->addItem("Sobel Hy");

    table_00 = new QTableWidgetItem(tr("%1").arg(0));
    table_01 = new QTableWidgetItem(tr("%1").arg(0));
    table_02 = new QTableWidgetItem(tr("%1").arg(0));
    table_10 = new QTableWidgetItem(tr("%1").arg(0));
    table_11 = new QTableWidgetItem(tr("%1").arg(1));
    table_12 = new QTableWidgetItem(tr("%1").arg(0));
    table_20 = new QTableWidgetItem(tr("%1").arg(0));
    table_21 = new QTableWidgetItem(tr("%1").arg(0));
    table_22 = new QTableWidgetItem(tr("%1").arg(0));

    ui->filtersTable->setItem(0, 0, table_00);
    ui->filtersTable->setItem(0, 1, table_01);
    ui->filtersTable->setItem(0, 2, table_02);
    ui->filtersTable->setItem(1, 0, table_10);
    ui->filtersTable->setItem(1, 1, table_11);
    ui->filtersTable->setItem(1, 2, table_12);
    ui->filtersTable->setItem(2, 0, table_20);
    ui->filtersTable->setItem(2, 1, table_21);
    ui->filtersTable->setItem(2, 2, table_22);

    this->histogram = new QChart();
    this->set0 = new QBarSet("Black");
    this->series = new QBarSeries();
    this->axisX = new QValueAxis();
    this->axisY = new QValueAxis();
    QStringList categories;
    for(int i = 0; i<256; i++)
            categories << QString(i);

     histogram->legend()->setVisible(false);

    this->set0->setColor("black");
    for(int i = 0; i<256; i++)
            this->set0->append(0);

    this->series->append(this->set0);

    histogram->addSeries(this->series);
    histogram->setAnimationOptions(QChart::SeriesAnimations);


    this->axisX->setRange(0,255);
    histogram->addAxis( this->axisX, Qt::AlignBottom);
    this->series->attachAxis( this->axisX);


    this->axisY->setRange(0,10000);
    histogram->addAxis(this->axisY, Qt::AlignLeft);
    this->series->attachAxis(this->axisY);

    ui->histogram_box->setChart(histogram);
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateHistogram(std::vector<int> data)
{
    int max = data[0];
    for(size_t i = 0; i<256; i++)
    {
        if(data[i]> max)
        {
            max = data[i];
        }
    }

   this->axisY->setRange(0,max+100);

   for(size_t i = 0; i<256; i++)
   {
        this->set0->replace(i, data[i]);
   }

   ui->histogram_box->repaint();
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
        ui->Luminance->setEnabled(0);
        ui->Lab->setEnabled(0);
    }
     this->updateHistogram(this->EditedImage.getHistogram());
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

        if (tones < 1 || tones > 256 || tones > this->EditedImage.getNumberOfTones())
        {
            QMessageBox msg;
            if(tones > this->EditedImage.getNumberOfTones())
            {
                if(this->EditedImage.getNumberOfTones() == -1)
                {
                    msg.setText("Convert your image to gray to use quantization ;)");
                }
                else
                {
                      msg.setText("Your image already has less tones than that :(");
                }
            }
            else
            {
                 msg.setText("Your input must be an integer greater than zero and smallest than 257!");
            }
            msg.setIcon(QMessageBox::Information);
            msg.setWindowTitle("Something went wrong :(");
            msg.exec();
        }
        else
        {
            this->EditedImage.quantization(tones);
            Mat img = this->EditedImage.getRGBImage();
            ui->canvas_edited->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, int(img.step), QImage::Format_RGB888)));
            ui->Luminance->setEnabled(0);
            ui->Lab->setEnabled(0);

        }
    }
    ui->lineEdit->clear();
}

void MainWindow::on_lineEdit_brightness_returnPressed()
{
    QString userInput = ui->lineEdit_brightness->text();
    ui->lineEdit_brightness->clear();
    int bias = userInput.toInt();

    if(this->EditedImage.getThereIsAnImage())
    {
        if(bias < -255 || bias > 255)
        {
            QMessageBox msg;

            msg.setText("Your input must be an integer greater in the interval [-255,255]");
            msg.setIcon(QMessageBox::Information);
            msg.setWindowTitle("Something went wrong :(");
            msg.exec();
        }
        else
        {
            this->EditedImage.brightness(bias);
            Mat img = this->EditedImage.getRGBImage();
            ui->canvas_edited->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, int(img.step), QImage::Format_RGB888)));

            this->updateHistogram(this->EditedImage.getHistogram());
         }
    }


    ui->lineEdit_brightness->clear();
}

void MainWindow::on_lineEdit_contrast_returnPressed()
{
    QString userInput = ui->lineEdit_contrast->text();
    ui->lineEdit_contrast->clear();
    float gain = userInput.toFloat();

    if(this->EditedImage.getThereIsAnImage())
    {
        if(gain <= 0 || gain > 255)
        {
            QMessageBox msg;

            msg.setText("Your input must be an integer greater in the interval (0,255]");
            msg.setIcon(QMessageBox::Information);
            msg.setWindowTitle("Something went wrong :(");
            msg.exec();
        }
        else
        {
            this->EditedImage.constrast(gain);
            Mat img = this->EditedImage.getRGBImage();
            ui->canvas_edited->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, int(img.step), QImage::Format_RGB888)));

            this->updateHistogram(this->EditedImage.getHistogram());
        }
    }

    ui->lineEdit_contrast->clear();

}

void MainWindow::on_negative_clicked()
{
    if(this->EditedImage.getThereIsAnImage())
    {
        this->EditedImage.negative();
        Mat img = this->EditedImage.getRGBImage();
        ui->canvas_edited->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, int(img.step), QImage::Format_RGB888)));

        this->updateHistogram(this->EditedImage.getHistogram());
    }
}

void MainWindow::on_equalization_clicked()
{
    if(this->EditedImage.getThereIsAnImage())
    {
        if(ui->Luminance->isChecked())
            this->EditedImage.equalization(LUMINANCE);
        else if(ui->Lab->isChecked())
            this->EditedImage.equalization(LAB);
        else
            this->EditedImage.equalization(GRAY);

        Mat img = this->EditedImage.getRGBImage();
        ui->canvas_edited->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, int(img.step), QImage::Format_RGB888)));

        this->updateHistogram(this->EditedImage.getHistogram());
    }
}

void MainWindow::on_matching_clicked()
{
    if(this->EditedImage.getThereIsAnImage())
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), tr("Image Files (*.png *.jpg *.bmp)"));
        if(!fileName.isNull())
        {
            this->EditedImage.matching(fileName);
            Mat img = this->EditedImage.getRGBImage();
            ui->canvas_edited->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, int(img.step), QImage::Format_RGB888)));

            this->updateHistogram(this->EditedImage.getHistogram());
        }
    }
}

void MainWindow::on_rot_Clockwise_clicked()
{
    if(this->EditedImage.getThereIsAnImage())
    {
        this->EditedImage.clockwiseRotation();
        Mat img = this->EditedImage.getRGBImage();
        ui->canvas_edited->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, int(img.step), QImage::Format_RGB888)));
    }
}

void MainWindow::on_rot_Counterclockwise_clicked()
{
    if(this->EditedImage.getThereIsAnImage())
    {
        this->EditedImage.counterclockwiseRotation();
        Mat img = this->EditedImage.getRGBImage();
        ui->canvas_edited->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, int(img.step), QImage::Format_RGB888)));
    }
}

void MainWindow::on_zoomIn_clicked()
{
    if(this->EditedImage.getThereIsAnImage())
    {
        if(2*this->EditedImage.getNumberOfRows()/this->OriginalImage.getNumberOfRows() < 10 && 2*this->EditedImage.getNumberOfCols()/this->OriginalImage.getNumberOfCols() < 10)
        {
            this->EditedImage.zoomIn();
            ui->canvas_edited->setScaledContents(false);
            Mat img = this->EditedImage.getRGBImage();
            ui->canvas_edited->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, int(img.step), QImage::Format_RGB888)));
            this->updateHistogram(this->EditedImage.getHistogram());
        }
    }
}

void MainWindow::on_zoomOut_clicked()
{
    if(this->EditedImage.getThereIsAnImage())
    {
        if(this->EditedImage.getNumberOfRows()/2 >= 2 && this->EditedImage.getNumberOfCols()/2 >= 2)
        {
            this->EditedImage.zoomOut();
            Mat img = this->EditedImage.getRGBImage();
            ui->canvas_edited->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, int(img.step), QImage::Format_RGB888)));
            if(ui->canvas_edited->width()*ui->canvas_edited->height() >= this->EditedImage.getNumberOfRows()*this->EditedImage.getNumberOfCols())
                ui->canvas_edited->setScaledContents(false);
            else
                ui->canvas_edited->setScaledContents(true);
            this->updateHistogram(this->EditedImage.getHistogram());
        }
    }
}

void MainWindow::on_convolutionFilters_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "Gaussian")
    {
        ui->filtersTable->item(0,0)->setText("0.0625");
        ui->filtersTable->item(0,1)->setText("0.125");
        ui->filtersTable->item(0,2)->setText("0.0625");
        ui->filtersTable->item(1,0)->setText("0.125");
        ui->filtersTable->item(1,1)->setText("0.25");
        ui->filtersTable->item(1,2)->setText("0.125");
        ui->filtersTable->item(2,0)->setText("0.0625");
        ui->filtersTable->item(2,1)->setText("0.125");
        ui->filtersTable->item(2,2)->setText("0.0625");

    }
    else if(arg1 == "Laplacian")
    {
        ui->filtersTable->item(0,0)->setText("0");
        ui->filtersTable->item(0,1)->setText("-1");
        ui->filtersTable->item(0,2)->setText("0");
        ui->filtersTable->item(1,0)->setText("-1");
        ui->filtersTable->item(1,1)->setText("4");
        ui->filtersTable->item(1,2)->setText("-1");
        ui->filtersTable->item(2,0)->setText("0");
        ui->filtersTable->item(2,1)->setText("-1");
        ui->filtersTable->item(2,2)->setText("0");
    }
    else if(arg1 == "Generic Highpass")
    {
        ui->filtersTable->item(0,0)->setText("-1");
        ui->filtersTable->item(0,1)->setText("-1");
        ui->filtersTable->item(0,2)->setText("-1");
        ui->filtersTable->item(1,0)->setText("-1");
        ui->filtersTable->item(1,1)->setText("8");
        ui->filtersTable->item(1,2)->setText("-1");
        ui->filtersTable->item(2,0)->setText("-1");
        ui->filtersTable->item(2,1)->setText("-1");
        ui->filtersTable->item(2,2)->setText("-1");
    }
    else if(arg1 == "Prewitt Hx")
    {
        ui->filtersTable->item(0,0)->setText("-1");
        ui->filtersTable->item(0,1)->setText("0");
        ui->filtersTable->item(0,2)->setText("1");
        ui->filtersTable->item(1,0)->setText("-1");
        ui->filtersTable->item(1,1)->setText("0");
        ui->filtersTable->item(1,2)->setText("1");
        ui->filtersTable->item(2,0)->setText("-1");
        ui->filtersTable->item(2,1)->setText("0");
        ui->filtersTable->item(2,2)->setText("1");
    }
    else if(arg1 == "Prewitt Hy Hx")
    {
        ui->filtersTable->item(0,0)->setText("-1");
        ui->filtersTable->item(0,1)->setText("-1");
        ui->filtersTable->item(0,2)->setText("-1");
        ui->filtersTable->item(1,0)->setText("0");
        ui->filtersTable->item(1,1)->setText("0");
        ui->filtersTable->item(1,2)->setText("0");
        ui->filtersTable->item(2,0)->setText("1");
        ui->filtersTable->item(2,1)->setText("1");
        ui->filtersTable->item(2,2)->setText("1");
    }
    else if(arg1 == "Sobel Hx")
    {
        ui->filtersTable->item(0,0)->setText("-1");
        ui->filtersTable->item(0,1)->setText("0");
        ui->filtersTable->item(0,2)->setText("1");
        ui->filtersTable->item(1,0)->setText("-2");
        ui->filtersTable->item(1,1)->setText("0");
        ui->filtersTable->item(1,2)->setText("2");
        ui->filtersTable->item(2,0)->setText("-1");
        ui->filtersTable->item(2,1)->setText("0");
        ui->filtersTable->item(2,2)->setText("1");
    }
    else if(arg1 == "Sobel Hy")
    {
        ui->filtersTable->item(0,0)->setText("-1");
        ui->filtersTable->item(0,1)->setText("-2");
        ui->filtersTable->item(0,2)->setText("-1");
        ui->filtersTable->item(1,0)->setText("0");
        ui->filtersTable->item(1,1)->setText("0");
        ui->filtersTable->item(1,2)->setText("0");
        ui->filtersTable->item(2,0)->setText("1");
        ui->filtersTable->item(2,1)->setText("2");
        ui->filtersTable->item(2,2)->setText("1");
    }
}

void MainWindow::on_applyFilter_clicked()
{
    if(this->EditedImage.getThereIsAnImage())
    {
        double weights[3][3];

        for(int i = 0; i<3; i++)
            for(int j = 0; j<3; j++)
                weights[i][j] = ui->filtersTable->item(i,j)->data(0).toDouble();


        this->EditedImage.convolution(weights, ui->mais127->isChecked());
        Mat img = this->EditedImage.getRGBImage();
        ui->canvas_edited->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, int(img.step), QImage::Format_RGB888)));
        this->updateHistogram(this->EditedImage.getHistogram());
    }
}


void MainWindow::on_pushButton_clicked()
{
    if(this->OriginalImage.getThereIsAnImage())
    {
        this->EditedImage.setMainImage(this->OriginalImage.getMainImage());
        this->EditedImage.setThereIsAnImage(true);
        this->EditedImage.calculateAndSetHistogram();
        this->EditedImage.setNumberOfTones(-1);

        ui->canvas_edited->setScaledContents(true);
        Mat img = this->EditedImage.getRGBImage();
        ui->canvas_edited->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, int(img.step), QImage::Format_RGB888)));

        ui->Luminance->setEnabled(1);
        ui->Lab->setEnabled(1);

        this->updateHistogram(this->EditedImage.getHistogram());
    }
}
