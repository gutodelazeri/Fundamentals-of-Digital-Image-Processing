#include "imageeditor.h"

using namespace cv;
using namespace std;

ImageEditor::ImageEditor(Mat image)
{
    this->mainImage = image;
    this->thereIsAnImage = true;
    this->isItGray = false;
    this->grayTones = 0;
}

ImageEditor::ImageEditor()
{
    this->thereIsAnImage = false;
    this->isItGray = false;
    this->grayTones = 0;
}

void ImageEditor::verticalMirroring()
{
    Mat newImage;

    for (int i = 0; i < this->mainImage.rows; i++)
    {
        newImage.push_back(this->mainImage.row(this->mainImage.rows - 1 - i));
    }

    this->mainImage = newImage;

}

void ImageEditor::horizontalMirroring()
{
    Mat newImage = this->mainImage.clone();

    for (int i = 0; i < this->mainImage.cols; i++)
     {
           this->mainImage.col(this->mainImage.cols - 1 - i).copyTo(newImage.col(i));
     }

    this->mainImage = newImage;

}

void ImageEditor::convertToGray()
{
    Mat newImage = Mat::zeros(this->mainImage.rows, this->mainImage.cols, CV_8UC1 );

    for (int y = 0; y < this->mainImage.rows; y++)
    {
        for (int x = 0; x < this->mainImage.cols; x++)
        {
           newImage.at<uchar>(y,x) = uchar(this->mainImage.at<Vec3b>(y,x)[0] * 0.114 + this->mainImage.at<Vec3b>(y,x)[1]*0.587 + this->mainImage.at<Vec3b>(y,x)[2]*0.299);
        }
    }

    cvtColor(newImage,this->mainImage,COLOR_GRAY2BGR);

    this->isItGray = true;
    this->grayTones = 256;
}

Mat ImageEditor::quantization(int numberOfTones)
{
    Mat newImage = Mat::zeros(this->mainImage.rows, this->mainImage.cols, CV_8UC1);

    vector<uchar> tones(256);
    int gap = 256/numberOfTones;
    uchar counter = 0;
    uchar tonesUtilized = 1;
    uchar currentTone = 0;

    for(size_t i = 0; i < 256; i++)
    {
        if(tonesUtilized >= numberOfTones)
        {
            tones[i] = tones[i-1];
        }
        else
        {
            if(counter == gap)
            {
                currentTone = uchar(i);
                tones[i] = currentTone;
                counter = 1;
                tonesUtilized++;
            }
            else
            {
                tones[i] = currentTone;
                counter++;
            }
        }
    }

    for (int y = 0; y < this->mainImage.rows; y++)
    {
        for (int x = 0; x < this->mainImage.cols; x++)
        {
            newImage.at<uchar>(y,x) = tones[uchar(this->mainImage.at<Vec3b>(y,x)[0] * 0.114 + this->mainImage.at<Vec3b>(y,x)[1]*0.587 + this->mainImage.at<Vec3b>(y,x)[2]*0.299)];
        }
    }
    cvtColor(newImage,newImage,COLOR_GRAY2BGR);

    this->isItGray = true;
    this->grayTones = numberOfTones;

    return newImage;
}

bool ImageEditor::getThereIsAnImage()
{
    return this->thereIsAnImage;
}

void ImageEditor::setThereIsAnImage(bool state)
{
    this->thereIsAnImage = state;
}

Mat ImageEditor::getRGBImage()
{
    Mat img = this->mainImage.clone();
    cvtColor(img, img, COLOR_RGB2BGR);
    return img;
}

Mat ImageEditor::getMainImage()
{
    return this->mainImage;
}

void ImageEditor::setMainImage(QString filename)
{
    this->mainImage = imread(filename.toStdString());
    this->thereIsAnImage = true;
    this->isItGray = false;
    this->grayTones = -1;
}

void ImageEditor::setMainImage(Mat image)
{
    this->mainImage = image.clone();
}

void ImageEditor::saveMainImage(QString filename)
{
   Mat img;
   if(this->isItGray)
   {
       img = this->quantization(grayTones);
   }
   else
   {
       img = this->mainImage;
   }
   imwrite(filename.toStdString() , img);
}




