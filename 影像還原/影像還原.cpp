#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/photo.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui_c.h>
#include <stdio.h>
using namespace cv;
using namespace std;
typedef cv::Mat Mat;
using namespace cv;

void mouse_click(int Event, int x, int y, int flags, void* param) {
    if (Event == CV_EVENT_LBUTTONDOWN) {
        cout << x << "\t" << y << endl;
    }
}

void shiftDFT(Mat& mag)
{
    mag = mag(Rect(0, 0, mag.cols & -2, mag.rows & -2));
    int cx = mag.cols / 2;
    int cy = mag.rows / 2;
    Mat q0(mag, Rect(0, 0, cx, cy));
    Mat q1(mag, Rect(cx, 0, cx, cy));
    Mat q2(mag, Rect(0, cy, cx, cy));
    Mat q3(mag, Rect(cx, cy, cx, cy));
    Mat temp;

    q0.copyTo(temp);//q0 q3 swap 傅立葉變換後的圖像左上和右下互換
    q3.copyTo(q0);
    temp.copyTo(q3);

    q1.copyTo(temp);//q1 q2 swap 傅立葉變換後的圖像右上和左下互換
    q2.copyTo(q1);
    temp.copyTo(q2);
}

int main()
{
    Mat src = imread("image4.png", IMREAD_GRAYSCALE); //灰階輸入原始圖片

    Mat fI;
    src.convertTo(fI, CV_32F);

    //Perform DFT
    Mat fourierTransform1;
    Mat planes[2];

    dft(fI, fourierTransform1, DFT_SCALE | DFT_COMPLEX_OUTPUT);

    shiftDFT(fourierTransform1);

    split(fourierTransform1, planes);// planes[0] = Re(DFT(I)), planes[1] = Im(DFT(I))

    Mat ph, mag;
    mag.zeros(planes[0].rows, planes[0].cols, CV_32F);
    ph.zeros(planes[0].rows, planes[0].cols, CV_32F);
    cartToPolar(planes[0], planes[1], mag, ph);

    namedWindow("Magnitude Spectrum", 0);
    resizeWindow("Magnitude Spectrum", mag.size());
    imshow("Magnitude Spectrum", mag);
    setMouseCallback("Magnitude Spectrum", mouse_click, NULL);

    int cpoint[6][2] = { {794,385},{806,416},{793,450},{808,514},{795,545},{807,575} };
    int r = 15; // 半徑
    // 創建一個與原始圖像相同大小的mask圖像，初始化為1
    cv::Mat mask = cv::Mat::ones(mag.rows, mag.cols, CV_32F);

    // 遍歷峰值
    for (int i = 0; i < 6; i++) {
        // 獲取峰值座標
        int x = cpoint[i][0];
        int y = cpoint[i][1];
        cv::circle(mask, cv::Point(x, y), r, cv::Scalar(0, 0, 0), -1);
    }
    namedWindow("mask", 0);
    resizeWindow("mask", mask.size());
    imshow("mask", mask);

    multiply(mag, mask, mag);

    namedWindow("mag", 0);
    resizeWindow("mag", mag.size());
    imshow("mag", mag);

    polarToCart(mag, ph, planes[0], planes[1]);

    merge(planes, 2, fourierTransform1);

    shiftDFT(fourierTransform1);

    //Perform IDFT
    Mat inverseTransform1;

    dft(fourierTransform1, inverseTransform1, DFT_INVERSE | DFT_REAL_OUTPUT);

    cv::Mat out;
    inverseTransform1.convertTo(out, CV_8U);
    namedWindow("original image");
    imshow("original image", src);

    namedWindow("result image");
    imshow("result image", out);
    waitKey(0);
}
