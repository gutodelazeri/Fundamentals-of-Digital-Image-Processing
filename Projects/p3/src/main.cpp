
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;


// Operations Flags
#define R_90 0
#define R_180 1
#define R_270 2
#define R_360 3
#define VERTICAL_MIRRORING 0
#define HORIZONTAL_MIRRORING 1
#define NO_MIRRORING 2


// Frame processing states
#define TO_RECORD 0
#define TO_SHOW 1

// User Parameters
#define DX 2
#define DY 2

#define THRESHOLD_1 1
#define THRESHOLD_2 50


typedef struct arguments sysArguments;
struct arguments{
  bool gblur;
  bool canny;
  bool sobel;
  bool negative;
  bool togray;
  bool  vertical_mirror;
  bool horizontal_mirror;
  int orientation;
  int resize;

  void initialize(){
    gblur = false;
    canny = false;
    sobel = false;
    negative = false;
    togray = false;
    orientation = R_360;
    vertical_mirror = false;
    horizontal_mirror = false;
    resize = 0;
  }

};

Mat applyChanges(sysArguments *Args, Mat frame, int brightness, int contrast, int kernelSize, int state)
{
    frame.convertTo(frame, -1, contrast, brightness);

    if(Args->gblur)
    {
        if(kernelSize % 2 == 0)
          kernelSize += 1;
        GaussianBlur(frame, frame, Size(kernelSize, kernelSize),  0);
    }
    if(Args->canny)
    {
        cvtColor(frame, frame, COLOR_BGR2GRAY);
        Canny(frame,frame,THRESHOLD_1, THRESHOLD_2);
        cvtColor(frame, frame, COLOR_GRAY2BGR);
    }
    if(Args->sobel)
    {
        //Sobel(InputArray src, OutputArray dst, int ddepth, int dx, int dy,
        Sobel(frame, frame, frame.depth(), DX, DY);
    }
    if(Args->negative)
    {
         frame.convertTo(frame, -1, -1, 255);
    }
    if(Args->togray)
    {
         cvtColor(frame, frame, COLOR_BGR2GRAY);
    }
    if(Args->vertical_mirror)
    {
        flip(frame,frame,0);
    }
    if(Args->horizontal_mirror)
    {
        flip(frame,frame,1);
    }
    if(Args->resize && state == TO_SHOW)
    {
        int newRows = frame.rows/pow(2, Args->resize);
        int newCols = frame.cols/pow(2, Args->resize);
        if(newRows > 1 && newCols > 1)
          resize(frame, frame, Size(newRows, newCols));
    }
    if(state == TO_SHOW)
    {
        if(Args->orientation == R_90)
        {
            transpose(frame, frame);
            flip(frame, frame, +1);
        }
        else if(Args->orientation == R_180)
        {
            transpose(frame, frame);
            flip(frame, frame, +1);
            transpose(frame, frame);
            flip(frame, frame, +1);
        }
        else if(Args->orientation == R_270)
        {
            transpose(frame, frame);
            flip(frame, frame, +1);
            transpose(frame, frame);
            flip(frame, frame, +1);
            transpose(frame, frame);
            flip(frame, frame, +1);
        }
        else
            frame = frame; //Sorry for make you see that :(
    }

    return frame;  
}

int main(int argc, char** argv)
{
    // OpenCV parameters
    int camera = 0;
    int codec = VideoWriter::fourcc('M', 'J', 'P', 'G'); 
    double fps = 10.0;  
          

    //OpenCV variables
    Mat frame;
    Mat frameToShow;
    Mat frameToRecord;
    VideoCapture cap;
    VideoWriter writer;
    namedWindow("This is you, smile! :)", 1);

    //Sys parameters
    int brightness = 0;
    int contrast = 0;
    float size = 1;
    int kernelSize = 0;
    bool recording = false;
    bool saveRecording = false;
    sysArguments Args;


    //Trackbar to change brightness
    int iSliderValue1 = 0;
    createTrackbar("Brightness", "This is you, smile! :)", &iSliderValue1, 100);

    //Trackbar to change contrast
    int iSliderValue2 = 1;
    createTrackbar("Contrast", "This is you, smile! :)", &iSliderValue2, 100);

    //Trackbar to change kernel size
    int iSliderValue3 = 15;
    createTrackbar("Kernel Size", "This is you, smile! :)", &iSliderValue3, 100);



    //Initializations
    Args.initialize();

    if(!cap.open(camera))
        return 0;

    cap >> frame;
    if (frame.empty()) {
        cerr << "ERROR! blank frame grabbed\n";
        return -1;
    }
    bool isColor = (frame.type() == CV_8UC3);
    
    writer.open("./live.avi", codec, fps, frame.size(), isColor);
    if (!writer.isOpened()) 
        return 0;
  
    for(;;)
    {
        cap >> frame;
        brightness = iSliderValue1;
        contrast = iSliderValue2;
        kernelSize = iSliderValue3;
       
        if( frame.empty() ) break; 
        
        char key = waitKey(1);
        if(key == 27)
          break;
        else if(key == '1')
          Args.gblur = true;
        else if(key == '2')
          Args.canny = !Args.canny;
        else if(key == '3')
          Args.sobel = !Args.sobel;
        else if(key == '4')
          Args.negative = !Args.negative;
        else if(key == '5')
        {
            if(Args.orientation == R_90)
                Args.orientation = R_180;
            else if(Args.orientation == R_180)
                Args.orientation = R_270;
            else if(Args.orientation == R_270)
                Args.orientation = R_360;
            else
                Args.orientation = R_90;
        }
        else if(key == '6')
          Args.horizontal_mirror = !Args.horizontal_mirror;
        else if(key == '7')
          Args.vertical_mirror = !Args.vertical_mirror;
        else if(key == '8')
          Args.togray = true;
        else if(key == '9')
          Args.resize += 1;
        else if(key == '0')
          Args.initialize();
        else if(key == 'a')
          recording = true;
        else if(key == 'b')
          saveRecording = true;


        frameToShow = applyChanges(&Args, frame.clone(), brightness, contrast, kernelSize, TO_SHOW);
        imshow("This is you, smile! :)", frameToShow);  
        if(recording == true)
        {
              if(saveRecording == true)
              {
                  recording = false;
                  saveRecording = false;
              }
              else
              {   
                frameToRecord = frameToShow = applyChanges(&Args, frame.clone(), brightness, contrast, kernelSize, TO_RECORD);
                if(frameToRecord.channels() < 3)
                  cvtColor(frameToRecord, frameToRecord, COLOR_GRAY2RGB);
                writer.write(frameToRecord);    
              }
        }

    }
    cap.release();
    return 0;
}


