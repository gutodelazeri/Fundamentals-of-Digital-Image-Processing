import numpy as np
from numpy.fft import fftn, ifftn, fftshift
import cv2 as cv
import math
import argparse
from datetime import datetime


# Here we calculate a matrix with samples of the gaussian filter.
# Note that we need to shift the center of the function in order to match
# the center of the shifted fourier transform.
def gaussianFilter(numRows, numCols, sigma):
    centerI = int(numRows / 2) + 1 if numRows % 2 == 1 else int(numRows / 2)
    centerJ = int(numCols / 2) + 1 if numCols % 2 == 1 else int(numCols / 2)
    arr = np.zeros((numRows, numCols, 3))
    for i in range(numRows):
        for j in range(numCols):
            for k in range(3):
                arr[i][j][k] = math.exp(-1.0 * ((i - centerI) ** 2 + (j - centerJ) ** 2) / (2 * sigma ** 2))
    return arr


def readImage(img):
    i = cv.imread(img)
    return fftshift(fftn(i))


def writeImage(img):
    s = ifftn(fftshift(img))

    dateTimeObj = datetime.now()
    path = "../Results/"
    date = str(dateTimeObj.year) + '_' + str(dateTimeObj.month) + '_' + str(dateTimeObj.day)
    time = str(dateTimeObj.hour) + 'h' + str(dateTimeObj.minute) + 'm' + str(dateTimeObj.second) + 's'
    extension = ".jpeg"
    title = path + date + "-" + time + extension
    cv.imwrite(title, np.abs(s))


def getUserInput():
    parser = argparse.ArgumentParser(description='INF01046 - Final Project')
    parser.add_argument('main', help="Name of the main image (don't forget the extension)", type=str)
    # main = input("Name of the main image (don't forget the extension): ")
    parser.add_argument('alternative', help="Name of the main image (don't forget the extension):" , type=str)
    # alternative = input("Name of the alternative image (don't forget the extension): ")
    parser.add_argument('highpass', help="Highpass cut-off frequency", type=float)
    # f1 = int(input("Highpass cut-off frequency: "))
    parser.add_argument('lowpass', help="Lowpass cut-off frequency", type=float)
    # f2 = int(input("lowpass cut-off frequency: "))

    args = parser.parse_args()

    if args.highpass < 0 or args.lowpass < 0:
        print("Cut-off frequency value must be a non-negative real number!")
        exit(1)

    return [args.main, args.alternative, args.highpass, args.lowpass]


def main():
    [mainImage, alternativeImage, highpassCutoffFrequency, lowpassCutoffFrequency] = getUserInput()
    a = "../BaseImages/" + alternativeImage
    b = "../BaseImages/" + mainImage

    # Here we read an image and return its shifted fourier transform
    I1 = readImage(a)
    I2 = readImage(b)

    # Here we calculate the gaussian filter with the desired cut-off frequency
    G1 = gaussianFilter(I1.shape[0], I1.shape[1], lowpassCutoffFrequency)
    G2 = gaussianFilter(I2.shape[0], I2.shape[1], highpassCutoffFrequency)

    # Here we create the hibrid image  accordingly with the definition
    # given in ../docs/report.pdf
    H = I1 * G1 + I2 * (1 - G2)

    # Here we convert the result to the spatial domain and save it
    writeImage(H)

    print("Hybrid image created with success!")


if __name__ == "__main__":
    main()
