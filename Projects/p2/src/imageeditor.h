#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H
#include <QString>
#include <cmath>
#include <vector>
#include <opencv2/opencv.hpp>

#define GRAYTONES 0

using namespace cv;
using namespace std;

class ImageEditor
{
private:
    Mat mainImage;
    std::vector<int> histogram;
    bool thereIsAnImage;
    bool isItGray;
    int grayTones;

public:
    ImageEditor(Mat image);
    ImageEditor();

    void verticalMirroring();
    void horizontalMirroring();
    void clockwiseRotation();
    void counterclockwiseRotation();
    void convertToGray();
    Mat  convertToGrayAndReturn();
    Mat  convertToGrayAndReturn(Mat image);
    void brightness(int bias);
    void constrast(float gain);
    void negative();
    void calculateAndSetHistogram();
    void equalization(int option);
    void matching(QString filename);
    void convolution(double weights[3][3], bool plus127);
    void zoomIn();
    void zoomOut();
    void quantization(int numberOfTones);

    Mat getRGBImage();
    Mat getMainImage();

    std::vector<int> getHistogram();
    int getNumberOfRows();
    int getNumberOfCols();

    int getNumberOfTones();
    void setNumberOfTones(int tones);

    bool getThereIsAnImage();
    void setThereIsAnImage(bool state);

    void setMainImage(QString filename);
    void setMainImage(Mat image);

    void saveMainImage(QString filename);
};


#endif // IMAGEEDITOR_H
