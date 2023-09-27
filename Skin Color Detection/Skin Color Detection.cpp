#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    Mat src = imread("img3.jpg");

    //彩色影像轉hsv
    Mat hsv;
    cvtColor(src, hsv, COLOR_BGR2HSV);

    //創mask，並設定取mask的範圍為HSV中 7<H<20 28<S<256 50<V<256
    Mat mask;
    //inRange(hsv, Scalar(7, 28, 50), Scalar(20, 255, 255), mask);
    inRange(hsv, Scalar(0, 58, 40), Scalar(35, 174, 255), mask);

    //皮膚部分標記為紅色
    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            if (mask.at<uchar>(i, j)) {
                src.at<Vec3b>(i, j)[0] = 0;
                src.at<Vec3b>(i, j)[1] = 0;
                src.at<Vec3b>(i, j)[2] = 255;
            }
        }
    }

    //展示結果
    namedWindow("Skin Color Detection", 0);
    resizeWindow("Skin Color Detection", src.size() / 8);
    imshow("Skin Color Detection", src);

    waitKey(0);
}