#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;
int alpha_slider;
const int alpha_slider_max = 360;
Mat src1 = imread(samples::findFile("yzu.bmp"));
int width = src1.cols / 2;
int height = src1.rows / 2;

static void Rotated_image(int, void*)
{
	Mat rotation = getRotationMatrix2D(Point(width, height), alpha_slider, 1);
	Mat rotated;
	warpAffine(src1, rotated,rotation, src1.size());
	imshow("Rotated image", rotated);
}
static void Rotated_RC_image(int, void*)
{
	Mat mask = Mat::zeros(src1.size(),CV_8UC1);//make a circle mask
	circle(mask, Point(width, height), 170, Scalar(static_cast<double>(255)), -1, LINE_AA);
	
	Mat rotated;
	Mat rotation = getRotationMatrix2D(Point(width, height), alpha_slider, 1);
	warpAffine(src1, rotated, rotation, src1.size());
	
	Mat result = src1.clone();
	rotated.copyTo(result, mask);
	
	imshow("Rotated RC image", result);
}

int main(void)
{
	int mode;
	cout << "Type your mode(0 for Rotated image;1 for Rotated CR image):";
	cin >> mode;
	if (mode == 0) {
		alpha_slider = 0;
		namedWindow("Rotated image", WINDOW_AUTOSIZE); // Create Window
		char TrackbarName[7] = { "Degree" };
		createTrackbar(TrackbarName, "Rotated image", &alpha_slider, alpha_slider_max, Rotated_image);
		Rotated_image(alpha_slider, 0);
	}
	else {
		alpha_slider = 0;
		namedWindow("Rotated RC image", WINDOW_AUTOSIZE); // Create Window
		char TrackbarName[7] = { "Degree" };
		createTrackbar(TrackbarName, "Rotated RC image", &alpha_slider, alpha_slider_max, Rotated_RC_image);
		Rotated_RC_image(alpha_slider, 0);
	}


	waitKey(0);
}