#include "imageeditor.h"

#define LUMINANCE 0
#define LAB       1
#define GRAY      2

using namespace cv;
using namespace std;

//It is used to calculate the histogram of Lab images wrt L channel
std::vector<int> calculateHistogramLab(Mat image)
{
    std::vector<int> hist(256, 0);

    for(int  i = 0 ; i<image.rows; i++)
        for(int j = 0; j<image.cols; j++)
            hist[image.at<Vec3b>(i,j)[0]]++;

    return hist;
}
//It is used to calculate the histogram of grayscale images
std::vector<int> calculateHistogramGrayScale(Mat image)
{
    std::vector<int> hist(256, 0);

    for(int  i = 0 ; i<image.rows; i++)
        for(int j = 0; j<image.cols; j++)
            hist[image.at<uchar>(i,j)]++;

    return hist;
}

ImageEditor::ImageEditor(Mat image)
{
    this->mainImage = image;
    this->thereIsAnImage = true;
    this->calculateAndSetHistogram();
    this->isItGray = false;
    this->grayTones = -1;
}

ImageEditor::ImageEditor()
{
    this->thereIsAnImage = false;
    this->histogram = std::vector<int>(256, 0);
    this->isItGray = false;
    this->grayTones = -1;
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

void ImageEditor::clockwiseRotation()
{
   Mat newImage = Mat(this->mainImage.cols, this->mainImage.rows, this->mainImage.type());

   for(int i = 0; i < this->mainImage.rows; i++)
   {
      for(int j = 0; j< this->mainImage.cols; j++)
      {
          newImage.at<Vec3b>(j, this->mainImage.rows - i - 1) = this->mainImage.at<Vec3b>(i, j);
      }
   }

   this->mainImage = newImage;
}

void ImageEditor::counterclockwiseRotation()
{
    Mat newImage = Mat(this->mainImage.cols, this->mainImage.rows, this->mainImage.type());

    for(int i = 0; i < this->mainImage.rows; i++)
    {
       for(int j = 0; j< this->mainImage.cols; j++)
       {
           newImage.at<Vec3b>(this->mainImage.cols - 1 -j, i) = this->mainImage.at<Vec3b>(i, j);
       }
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
    this->calculateAndSetHistogram();
}

Mat  ImageEditor::convertToGrayAndReturn()
{
    Mat newImage = Mat::zeros(this->mainImage.rows, this->mainImage.cols, CV_8UC1 );

    for (int y = 0; y < this->mainImage.rows; y++)
    {
        for (int x = 0; x < this->mainImage.cols; x++)
        {
           newImage.at<uchar>(y,x) = uchar(this->mainImage.at<Vec3b>(y,x)[0] * 0.114 + this->mainImage.at<Vec3b>(y,x)[1]*0.587 + this->mainImage.at<Vec3b>(y,x)[2]*0.299);
        }
    }

    return newImage;
}

Mat  ImageEditor::convertToGrayAndReturn(Mat image)
{
    Mat newImage = Mat::zeros(image.rows, image.cols, CV_8UC1 );

    for (int y = 0; y < image.rows; y++)
    {
        for (int x = 0; x < image.cols; x++)
        {
           newImage.at<uchar>(y,x) = uchar(image.at<Vec3b>(y,x)[0] * 0.114 + image.at<Vec3b>(y,x)[1]*0.587 + image.at<Vec3b>(y,x)[2]*0.299);
        }
    }

    return newImage;
}

void ImageEditor::brightness(int bias)
{

    for(int i = 0; i< this->mainImage.rows; i++)
        for(int j = 0; j<this->mainImage.cols; j++)
        {
            for(int channel = 0; channel<3; channel++)
            {
                int color = this->mainImage.at<Vec3b>(i, j)[channel] + bias;

                if(color > 255)
                    this->mainImage.at<Vec3b>(i, j)[channel] = 255;
                else if (color < 0)
                   this->mainImage.at<Vec3b>(i, j)[channel] = 0;
                else
                    this->mainImage.at<Vec3b>(i, j)[channel] = uchar(color);
            }
        }
    this->calculateAndSetHistogram();
}

void ImageEditor::constrast(float gain)
{
    for(int i = 0; i< this->mainImage.rows; i++)
        for(int j = 0; j<this->mainImage.cols; j++)
        {
            for(int channel = 0; channel<3; channel++)
            {
                int color = this->mainImage.at<Vec3b>(i, j)[channel]*gain;

                if(color > 255)
                    this->mainImage.at<Vec3b>(i, j)[channel] = 255;
                else if (color < 0)
                   this->mainImage.at<Vec3b>(i, j)[channel] = 0;
                else
                    this->mainImage.at<Vec3b>(i, j)[channel] = uchar(color);
            }
        }
    this->calculateAndSetHistogram();
}

void ImageEditor::negative()
{
    for(int i = 0; i< this->mainImage.rows; i++)
        for(int j = 0; j<this->mainImage.cols; j++)
        {
            for(int channel = 0; channel<3; channel++)
            {
                this->mainImage.at<Vec3b>(i, j)[channel] = 255 -  this->mainImage.at<Vec3b>(i, j)[channel];
            }
        }
    this->calculateAndSetHistogram();
}

void ImageEditor::calculateAndSetHistogram()
{
    Mat newImage = this->convertToGrayAndReturn();

    std::vector<int> hist(256, 0);


    for(int  i = 0 ; i< this->mainImage.rows; i++)
        for(int j = 0; j<this->mainImage.cols; j++)
            hist[newImage.at<uchar>(i,j)]++;


    this->histogram = hist;
}

void ImageEditor::equalization(int option)
{
    Mat newImage;
    long double cumHistogram[256] = {0}; //Cumulative histogram array
    long double alpha  = 255/static_cast<long double>(this->mainImage.rows*this->mainImage.cols); // Scaling factor

    if(option == GRAY || option == LUMINANCE)
    {

        //Create a new grayscale image with 1 channel
        newImage = this->convertToGrayAndReturn();

        //Calculate cumalative histogram
        cumHistogram[0] = alpha*static_cast<long double>(this->histogram[0]);
        for(size_t i = 1; i<= 255; i++)
            cumHistogram[i] = cumHistogram[i-1] + alpha*static_cast<long double>(this->histogram[i]);


        if(option == GRAY)
        {
            //Change pixels color
            for(int i = 0; i<this->mainImage.rows; i++)
                for(int j = 0; j<this->mainImage.cols; j++)
                    newImage.at<uchar>(i, j) = uchar(cumHistogram[newImage.at<uchar>(i,j)]);

            cvtColor(newImage,this->mainImage,COLOR_GRAY2BGR);
        }
        else
        {
            //Change pixels color
            for(int i = 0; i<this->mainImage.rows; i++)
                for(int j = 0; j<this->mainImage.cols; j++)
                    for(int channel = 0; channel<3; channel++ )
                        this->mainImage.at<Vec3b>(i,j)[channel] = uchar(cumHistogram[this->mainImage.at<Vec3b>(i,j)[channel]]);
        }

    }
    else //Equalization is made using L channel of L*a*b color space
    {

       cvtColor(this->mainImage,newImage,COLOR_BGR2Lab);

       std::vector<int> histogram_L = calculateHistogramLab(newImage);

       cumHistogram[0] = alpha*static_cast<long double>(histogram_L[0]);
       for(size_t i = 1; i<= 255; i++)
           cumHistogram[i] = cumHistogram[i-1] + alpha*static_cast<long double>(histogram_L[i]);


       for(int i = 0; i<newImage.rows; i++)
           for(int j = 0; j<newImage.cols; j++)
                   newImage.at<Vec3b>(i,j)[0] = uchar(cumHistogram[newImage.at<Vec3b>(i,j)[0]]); //We are just changing the value of the L channel


       cvtColor(newImage,this->mainImage,COLOR_Lab2BGR);
    }

    //Recalculate image histogram;
    this->calculateAndSetHistogram();
}

void ImageEditor::matching(QString filename)
{
    Mat targetImage = imread(filename.toStdString());
    Mat sourceImage = this->convertToGrayAndReturn();
    Mat targetImage_gray = this->convertToGrayAndReturn(targetImage);


    // Compute histogram for target
    std::vector<int> histogram_target = calculateHistogramGrayScale(targetImage_gray);

    // Compute normalized cumulative histogram for target
    long double alpha  = 255/static_cast<long double>(targetImage_gray.rows*targetImage_gray.cols);
    long double cumHistogram_target[256] = {0};
    cumHistogram_target[0] = alpha*static_cast<long double>(histogram_target[0]);
    for(size_t i = 1; i<= 255; i++)
        cumHistogram_target[i] = cumHistogram_target[i-1] + alpha*static_cast<long double>(histogram_target[i]);

    // Compute histogram for source
    std::vector<int> histogram_source = calculateHistogramGrayScale(sourceImage);

    // Compute normalized cumulative histogram for source
    alpha  = 255/static_cast<long double>(sourceImage.rows*sourceImage.cols);
    long double cumHistogram_source[256] = {0};
    cumHistogram_source[0] = alpha*static_cast<long double>(histogram_source[0]);
    for(size_t i = 1; i<= 255; i++)
        cumHistogram_source[i] = cumHistogram_source[i-1] + alpha*static_cast<long double>(histogram_source[i]);

    // Build Histogram Matched
    int HM[256];
    for(int i = 0; i<256; i++)
    {
        int closest = 0;
        long double difference = abs(cumHistogram_source[i] - cumHistogram_target[0]);

        for(int j = 0; j<256; j++)
        {
            if(abs(cumHistogram_source[i] - cumHistogram_target[j]) < difference)
            {
                closest = j;
                difference = abs(cumHistogram_source[i] - cumHistogram_target[j]);
            }
        }
        HM[i] = closest;
    }


    //Change pixels color
    for(int i = 0; i<this->mainImage.rows; i++)
        for(int j = 0; j<this->mainImage.cols; j++)
            sourceImage.at<uchar>(i, j) = uchar(HM[sourceImage.at<uchar>(i,j)]);

    cvtColor(sourceImage,this->mainImage,COLOR_GRAY2BGR);
    this->calculateAndSetHistogram();
}

void ImageEditor::convolution(double weights[3][3], bool plus127)
{
    Mat newImage = convertToGrayAndReturn();
    Mat auxImage = convertToGrayAndReturn();

    double rotated[3][3];

    rotated[0][0] = weights[2][2];
    rotated[0][1] = weights[2][1];
    rotated[0][2] = weights[2][0];
    rotated[1][0] = weights[1][2];
    rotated[1][1] = weights[1][1];
    rotated[1][2] = weights[1][0];
    rotated[2][0] = weights[0][2];
    rotated[2][1] = weights[0][1];
    rotated[2][2] = weights[0][0];

    for(int i = 0; i<newImage.rows; i++)
    {
        for(int j = 0; j<newImage.cols; j++)
        {
            if(i == 0 || i == newImage.rows - 1 || j == 0 || j == newImage.cols - 1)
            {
                newImage.at<uchar>(i, j) = auxImage.at<uchar>(i,j);
            }
            else
            {
                int intensity = double(auxImage.at<uchar>(i,j))*rotated[1][1]       +
                                double(auxImage.at<uchar>(i,j+1))*rotated[1][2]     +
                                double(auxImage.at<uchar>(i,j-1))*rotated[1][0]     +
                                double(auxImage.at<uchar>(i+1,j))*rotated[2][1]     +
                                double(auxImage.at<uchar>(i-1,j))*rotated[0][1]     +
                                double(auxImage.at<uchar>(i+1,j+1))*rotated[2][2]   +
                                double(auxImage.at<uchar>(i+1,j-1))*rotated[2][0]   +
                                double(auxImage.at<uchar>(i-1,j+1))*rotated[0][2]   +
                                double(auxImage.at<uchar>(i-1,j-1))*rotated[0][0];
                if(plus127)
                {
                    intensity += 127;
                }

                if(intensity < 0)
                {
                    intensity = 0;
                }
                else if(intensity > 255)
                {
                    intensity = 255;
                }

                newImage.at<uchar>(i,j) = intensity;
            }
        }
    }

  cvtColor(newImage,this->mainImage,COLOR_GRAY2BGR);
  this->calculateAndSetHistogram();
}

void ImageEditor::zoomIn()
{
    Mat newImage(2*this->mainImage.rows, 2*this->mainImage.cols, this->mainImage.type());

    for(int i = 0; i<newImage.rows; i += 2)
        for(int j = 0; j<newImage.cols; j += 2)
        {
            newImage.at<Vec3b>(i,j)[0] = this->mainImage.at<Vec3b>(i/2, j/2)[0];
            newImage.at<Vec3b>(i,j)[1] = this->mainImage.at<Vec3b>(i/2, j/2)[1];
            newImage.at<Vec3b>(i,j)[2] = this->mainImage.at<Vec3b>(i/2, j/2)[2];
        }

    for(int i = 0; i<newImage.rows; i += 2)
         for(int j = 1; j<newImage.cols; j += 2)
         {
             if(j == newImage.cols-1)
             {
                 newImage.at<Vec3b>(i,j)[0] = newImage.at<Vec3b>(i,j-1)[0];
                 newImage.at<Vec3b>(i,j)[1] = newImage.at<Vec3b>(i,j-1)[1];
                 newImage.at<Vec3b>(i,j)[2] = newImage.at<Vec3b>(i,j-1)[2];
             }
             else
             {
                  newImage.at<Vec3b>(i,j)[0] = (newImage.at<Vec3b>(i,j-1)[0] + newImage.at<Vec3b>(i,j+1)[0])/2;
                  newImage.at<Vec3b>(i,j)[1] = (newImage.at<Vec3b>(i,j-1)[1] + newImage.at<Vec3b>(i,j+1)[1])/2;
                  newImage.at<Vec3b>(i,j)[2] = (newImage.at<Vec3b>(i,j-1)[2] + newImage.at<Vec3b>(i,j+1)[2])/2;
             }
         }

    for(int i = 1; i<newImage.rows; i += 2)
        for(int j = 0; j<newImage.cols; j += 1)
         {
            if(i == newImage.rows -1)
            {
                newImage.at<Vec3b>(i,j)[0] = newImage.at<Vec3b>(i-1,j)[0];
                newImage.at<Vec3b>(i,j)[1] = newImage.at<Vec3b>(i-1,j)[1];
                newImage.at<Vec3b>(i,j)[2] = newImage.at<Vec3b>(i-1,j)[2];
            }
            else
            {
                newImage.at<Vec3b>(i,j)[0] = (newImage.at<Vec3b>(i-1,j)[0] + newImage.at<Vec3b>(i+1,j)[0])/2;
                newImage.at<Vec3b>(i,j)[1] = (newImage.at<Vec3b>(i-1,j)[1] + newImage.at<Vec3b>(i+1,j)[1])/2;
                newImage.at<Vec3b>(i,j)[2] = (newImage.at<Vec3b>(i-1,j)[2] + newImage.at<Vec3b>(i+1,j)[2])/2;
            }
         }

    this->mainImage = newImage;
    this->calculateAndSetHistogram();
}

void ImageEditor::zoomOut()
{
    Mat newImage(int(this->mainImage.rows/2),int(this->mainImage.cols/2), this->mainImage.type());
    int counter, i, j;
    bool map[2][2];


    for(i = 0; i<this->mainImage.rows; i+=2)
    {

        for(j=0; j<this->mainImage.cols; j+=2)
        {
            double blue = 0;
            double green = 0;
            double red = 0;
            counter = 0;
            for(int m = 0; m< 2; m++)
                for(int n = 0; n<2; n++)
                    map[m][n] = false;

            //Average of pixels inside square of side 2
            for(int k = 0; k<2; k++)
            {
                for(int l = 0; l<2; l++)
                 {
                    if(j+l < this->mainImage.cols && i + k < this->mainImage.rows)
                    {
                        blue = blue + this->mainImage.at<Vec3b>(i+k,j+l)[0];
                        green = green + this->mainImage.at<Vec3b>(i+k,j+l)[1];
                        red = red + this->mainImage.at<Vec3b>(i+k,j+l)[2];
                        counter++;
                        map[k][l] = true;
                    }
                }
            }

            if(i/2 < newImage.rows && j/2 < newImage.cols)
            {
                newImage.at<Vec3b>(i/2, j/2)[0] = blue/counter;
                newImage.at<Vec3b>(i/2, j/2)[1] = green/counter;
                newImage.at<Vec3b>(i/2, j/2)[2] = red/counter;
            }
        }

    }
    this->mainImage = newImage;
    this->calculateAndSetHistogram();
}

void ImageEditor::quantization(int numberOfTones)
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
        this->mainImage = newImage;
        this->calculateAndSetHistogram();
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

std::vector<int> ImageEditor::getHistogram()
{
    return this->histogram;
}

int ImageEditor::getNumberOfRows()
{
    return this->mainImage.rows;
}

int ImageEditor::getNumberOfCols()
{
    return this->mainImage.cols;
}

int ImageEditor::getNumberOfTones()
{
    return this->grayTones;
}

void ImageEditor::setNumberOfTones(int tones)
{
    this->grayTones = tones;
}

void ImageEditor::setMainImage(QString filename)
{
    this->mainImage = imread(filename.toStdString());
    this->calculateAndSetHistogram();
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

   img = this->mainImage;

   imwrite(filename.toStdString() , img);
}




