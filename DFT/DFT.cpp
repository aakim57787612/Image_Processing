#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/photo.hpp"
#include "opencv2/opencv.hpp"
#include <stdio.h>
using namespace cv;
using namespace std;

int main()
{
    Mat src = imread("image3.png", IMREAD_GRAYSCALE); //灰階輸入原始圖片 邊界用0填充
    int m = getOptimalDFTSize(src.rows); //計算傅立葉變換並擴展至最佳尺寸m*n
    int n = getOptimalDFTSize(src.cols);
    Mat pad;
    copyMakeBorder(src, pad, 0, m - src.rows, 0, n - src.cols, BORDER_CONSTANT, Scalar::all(0));

    Mat planes[] = { Mat_<float>(pad),Mat::zeros(pad.size(),CV_32F) };
    Mat complex;
    merge(planes, 2, complex); //將planes[0] planes[1]合併成複數矩陣complex
    dft(complex, complex); //進行傅立葉變換

    split(complex, planes); //將complex分成實數部分和虛數部分

    Mat mag;
    magnitude(planes[0], planes[1], mag); //計算幅值
    mag += Scalar::all(1); //防止log0 所以pixel++ 
    log(mag, mag); //對幅值取對數

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

    normalize(mag, mag, 0, 1, NORM_MINMAX); //正規化頻譜 將其數值範圍調整在0~1之間
    namedWindow("Magnitude Spectrum", 0);
    resizeWindow("Magnitude Spectrum", src.size() / 2);
    imshow("Magnitude Spectrum", mag);

    Mat phases;
    phase(planes[0], planes[1], phases); //取得相位圖
    normalize(phases, phases, 0, 255, NORM_MINMAX); //正規化頻譜 將其數值範圍調整在0~255之間
    phases.convertTo(phases, CV_8U);
    namedWindow("Phase Spectrum", 0);
    resizeWindow("Phase Spectrum", src.size() / 2);
    imshow("Phase Spectrum", phases);

    Mat recover;
    idft(complex, recover); //進行逆傅立葉轉換
    split(recover, planes); //分離通道
    magnitude(planes[0], planes[1], recover); //計算幅值
    normalize(recover, recover, 0, 1, NORM_MINMAX); //正規化頻譜 將其數值範圍調整在0~1之間
    namedWindow("Recover Image", 0);
    resizeWindow("Recover Image", src.size() / 2);
    imshow("Recover Image", recover);

    waitKey(0);
}