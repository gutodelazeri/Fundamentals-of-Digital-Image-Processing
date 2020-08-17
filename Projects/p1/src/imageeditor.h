#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H
#include <QString>
#include <cmath>
#include <opencv2/opencv.hpp>

#define GRAYTONES 0

using namespace cv;
using namespace std;

class ImageEditor
{
private:
    Mat mainImage;
    bool thereIsAnImage;
    bool isItGray;
    int grayTones;
public:
    ImageEditor(Mat image);
    ImageEditor();

    void verticalMirroring();
    void horizontalMirroring();
    void convertToGray();
    Mat quantization(int numberOfTones);

    Mat getRGBImage();
    Mat getMainImage();

    bool getThereIsAnImage();
    void setThereIsAnImage(bool state);

    void setMainImage(QString filename);
    void setMainImage(Mat image);

    void saveMainImage(QString filename);
};


#endif // IMAGEEDITOR_H
