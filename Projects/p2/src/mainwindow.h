#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "imageeditor.h"
#include <QTableWidgetItem>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QHorizontalStackedBarSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>


namespace Ui{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateHistogram(std::vector<int> data);

    void on_flip_Horizontally_clicked();

    void on_flip_Vertically_clicked();

    void on_convertToGray_clicked();

    void on_actionOpen_Image_triggered();

    void on_actionSave_Image_triggered();

    void on_lineEdit_returnPressed();

    void on_lineEdit_brightness_returnPressed();

    void on_lineEdit_contrast_returnPressed();

    void on_negative_clicked();

    void on_equalization_clicked();

    void on_matching_clicked();

    void on_rot_Clockwise_clicked();

    void on_rot_Counterclockwise_clicked();

    void on_zoomIn_clicked();

    void on_convolutionFilters_currentIndexChanged(const QString &arg1);

    void on_applyFilter_clicked();

    void on_zoomOut_clicked();

    void on_pushButton_clicked();

private:
    ImageEditor OriginalImage;
    ImageEditor EditedImage;
    QtCharts::QChart *histogram;
    QtCharts::QBarSet *set0;
    QtCharts::QBarSeries *series;
    QtCharts::QValueAxis *axisX;
    QtCharts::QValueAxis *axisY;
    QTableWidgetItem *table_00;
    QTableWidgetItem *table_01;
    QTableWidgetItem *table_02;
    QTableWidgetItem *table_10;
    QTableWidgetItem *table_11;
    QTableWidgetItem *table_12;
    QTableWidgetItem *table_20;
    QTableWidgetItem *table_21;
    QTableWidgetItem *table_22;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
