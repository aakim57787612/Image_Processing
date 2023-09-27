#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/photo.hpp"
#include "opencv2/opencv.hpp"
#include <stdio.h>
using namespace cv;
using namespace std;

int main()
{
    Mat image0 = imread("fig.jpg", IMREAD_GRAYSCALE),
        image, X_Gradient, Y_Gradient, Output, Edge_Image, Binary_Image, Gaussian_Edge_Image;

    GaussianBlur(image0, image, Size(3, 3), 0);
    Sobel(image, X_Gradient, CV_16S, 1, 0, 3);//對image X方向計算梯度並輸出給X_Grad
    Sobel(image, Y_Gradient, CV_16S, 0, 1, 3);//對image Y方向計算梯度並輸出給Y_Grad
    convertScaleAbs(X_Gradient, X_Gradient);
    convertScaleAbs(Y_Gradient, Y_Gradient);
    addWeighted(X_Gradient, 0.5, Y_Gradient, 0.5, 0, Edge_Image);
    imshow("Edge Image", Edge_Image);

    threshold(Edge_Image, Binary_Image, 50, 150, THRESH_BINARY_INV);
    threshold(Binary_Image, Binary_Image, 0, 80, THRESH_BINARY_INV);
    bitwise_not(Binary_Image, Binary_Image);
    GaussianBlur(Binary_Image, Binary_Image, Size(3, 3), 0);
    
    imshow("Sketch Image", Binary_Image);
    waitKey(0);

    return 0;
}