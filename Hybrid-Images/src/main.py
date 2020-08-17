import numpy as np
from numpy.fft import fftn, ifftn, fftshift, ifftshift
import cv2 as cv
import math
from datetime import datetime

# Here we calculate a matrix with samples of the gaussian filter.
# Note that we need to shift the center of the function in order to match
# the center of the shifted fourier transform.
def gaussianFilter(numRows, numCols, sigma):
	centerI = int(numRows/2) + 1 if numRows % 2 == 1 else int(numRows/2)
	centerJ = int(numCols/2) + 1 if numCols % 2 == 1 else int(numCols/2)
	arr = np.zeros((numRows, numCols, 3))
	for i in range(numRows):
		for j in range(numCols):
			for k in range(3):
				arr[i][j][k] = math.exp(-1.0 * ((i - centerI)**2 + (j - centerJ)**2) / (2 * sigma**2))
	return arr

def readImage(img):
	i = cv.imread(img)
	return fftshift(fftn(i))	

def writeImage(img):
	s = ifftn(fftshift(img)) 

	dateTimeObj = datetime.now()
	path = "../Results/"
	date = str(dateTimeObj.year) + '_' + str(dateTimeObj.month)   + '_' + str(dateTimeObj.day)
	time = str(dateTimeObj.hour) + 'h'  + str(dateTimeObj.minute) + 'm' + str(dateTimeObj.second) + 's'
	extension = ".jpeg"
	title = path + date + "-" + time + extension
	cv.imwrite(title, np.abs(s))

def instructionsScreen():
	print("-------------------------------------------------------------------------------------------")
	print("|Steps for using this awesome program:                                                     |")
	print("|   1) Choose the base images and put them on the directory named BaseImages.              |")
	print("|      Remember that both images must have the same size and the same color space.         |")
	print("|   2) Type the name (with the extension) of the image that you want as main image.        |")
	print("|   3) Type the name (with the extension) of the image that you want as alternative.       |")
	print("|   4) Type the desired cut-off frequency for the highpass filter.                         |")
	print("|   5) Type the desired cut-off frequency for the lowpass filter.                          |")
	print("|That's it. Now go to the directory named Results and check the final image :)             |")
	print("-------------------------------------------------------------------------------------------")
	print(" ")

def getUserInput():
	main = input("Name of the main image (don't forget the extension): ")
	alternative  = input("Name of the alternative image (don't forget the extension): ") 
	f1 = int(input("Highpass cut-off frequency: "))
	f2 = int(input("lowpass cut-off frequency: "))
	print(" ")
	return [main, alternative, f1, f2]

def main():
	
	instructionsScreen()
	[main, alternative, f1, f2] = getUserInput()
	a = "../BaseImages/"+alternative
	b = "../BaseImages/"+main

	# Here we read an image and return its shifted fourier transform
	I1 = readImage(a)
	I2 = readImage(b)

	# Here we calculate the gaussian filter with the desired cut-off frequency
	G1 = gaussianFilter(I1.shape[0], I1.shape[1], f2)
	G2 = gaussianFilter(I2.shape[0], I2.shape[1], f1)

	# Here we create the hibrid image  accordingly with the definition
	# given in ../docs/report.pdf
	H = I1*G1 + I2*(1 - G2)

	# Here we convert the result to the spatial domain and save it
	writeImage(H)

	print("Hibrid image created with success!")


if __name__ == "__main__":
	main()
	
	

	
	

	
	

	

	


	

	
	
	