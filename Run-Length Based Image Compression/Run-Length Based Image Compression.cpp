#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <fstream>
using namespace cv;
using namespace std;

void traversal(Mat& img, Mat& compressImg) {
    for (int x = 0; x < img.rows; x++) {
        int count = 1;
        int index = 0;
        for (int y = 0; y < img.cols - 1; y++) {
            if (img.at<uchar>(x, y) == img.at<uchar>(x, y + 1)) {
                count++;
            }
            else {
                if (index < compressImg.cols - 2) {
                    compressImg.at<uchar>(x, index) = count;
                    compressImg.at<uchar>(x, index + 1) = img.at<uchar>(x, y);
                    index += 2;
                }
                count = 1;
            }
        }
    }
}

int main(int argc, char** argv) {
    Mat img = imread("img1.bmp");

    Mat compressImage(img.rows, img.cols, CV_8UC1);

    traversal(img, compressImage);

    imwrite("compressImage1.bmp", compressImage);

    ifstream infile("img1.bmp", ios::binary | ios::ate);
    ifstream outfile("compressImage1.bmp", ios::binary | ios::ate);

    double original = infile.tellg();
    double compress = outfile.tellg();
    double ratio = original / compress;

    cout << "original size: " << original << endl;
    cout << "compress size: " << compress << endl;
    cout << "compress ratio: " << ratio * 100 << "%" << endl;

    waitKey(0);
}