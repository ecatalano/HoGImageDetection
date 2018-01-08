#include <iostream>
using namespace std;

#include <stdio.h>
#include <math.h>
#include <fstream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "opencv/cv.h"  



using namespace cv;

Mat ySobelOperator(Mat grayImg){

	int scale = 1;
  	int delta = 0;
  	int ddepth = CV_16S;
	
	Mat grad, grad_y;
  	Mat abs_grad_y;
	
	/// Gradient Y
  	Sobel(grayImg, grad_y, ddepth, 0, 1, 1, scale, delta, BORDER_DEFAULT );
  	convertScaleAbs( grad_y, abs_grad_y );
	return abs_grad_y;
}

Mat xSobelOperator(Mat grayImg){
	
	int scale = 1;
  	int delta = 0;
  	int ddepth = CV_16S;

	Mat grad, grad_x;
  	Mat abs_grad_x;

  	Sobel(grayImg, grad_x, ddepth, 1, 0, 1, scale, delta, BORDER_DEFAULT );
  	convertScaleAbs( grad_x, abs_grad_x );

	return abs_grad_x;
}

Mat computeMagnitude(Mat xSobel, Mat ySobel){
	//magnitude = sqrt(gx^2 + gy^2)
	
	Mat mag = xSobel;
	for(int i = 0; i < xSobel.rows; i++){
		for(int j = 0; j < xSobel.cols; j++){
			
			int xVal = xSobel.at<Vec3b>(i,j)[0];
			int yVal = ySobel.at<Vec3b>(i,j)[0];
			int magVal = sqrt(xVal * xVal + yVal * yVal);
			
			if(magVal > 255) magVal = 255;
	
			mag.at<Vec3b>(i,j)[0] = magVal;
			mag.at<Vec3b>(i,j)[1] = magVal;
			mag.at<Vec3b>(i,j)[2] = magVal;
		}
	}
	return mag;
}

Mat computeOrientation(Mat xSobel, Mat ySobel){
	//angle = arctan(gy / gx)

	Mat angle = xSobel;
	for(int i = 0; i < xSobel.rows; i++){
		for(int j = 0; j < xSobel.cols; j++){
			
			int xVal = xSobel.at<Vec3b>(i,j)[0];
			int yVal = ySobel.at<Vec3b>(i,j)[0];
			int magVal = atan2(yVal, xVal);
			
			if(magVal > 255) magVal = 255;
	
			angle.at<Vec3b>(i,j)[0] = magVal;
			angle.at<Vec3b>(i,j)[1] = magVal;
			angle.at<Vec3b>(i,j)[2] = magVal;
		}
	}
	return angle;
}

int main(int argc, char** argv){

	string inputImage = "fruits.png";

	Mat image = imread(inputImage);
	imshow("Input image", image);

	Mat grayImg;
	cvtColor(image, grayImg, CV_BGR2GRAY);

	Mat xSobel = xSobelOperator(grayImg);
	Mat ySobel = ySobelOperator(grayImg);

  	imshow("X Sobel", xSobel);
  	imshow("Y Sobel", ySobel);

	//Compute the magnitude and angle of 2D vectors.
	Mat magnitude = computeMagnitude(xSobel, ySobel);
	Mat angle = computeOrientation(xSobel, ySobel); 
	
	//imshow("Magnitude", magnitude);
	imshow("Angle", angle);

	waitKey(0);

}