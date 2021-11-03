// Example 14-1. Finding contours based on a trackbar’s location; the contours are
// updated whenever the trackbar is moved

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;

cv::Mat g_gray, g_binary, g_hsv;
int g_thresh = 100;

void on_trackbar(int, void *)
{
    // cv::Mat hsv;
    // cv::threshold(g_hsv, hsv, g_thresh, 255, cv::THRESH_BINARY);
    // cv::namedWindow("win1", cv::WINDOW_AUTOSIZE);
    // cv::imshow("win1", hsv);
    cv::Mat gray;
    cv::cvtColor(g_gray, gray, cv::COLOR_RGB2GRAY);

    cv::Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));

    cv::dilate(gray, gray, element);

    cv::Mat bg_img;
    cv::medianBlur(gray, bg_img, 21);
    imshow("medianBlur", bg_img);

	// cv::Mat calcMat = ~(closeMat - gray);
    // imshow("calc", calcMat);

    cv::absdiff(gray, bg_img, gray);

    cv::Mat calcMat = ~(gray);
    imshow("calc", calcMat);

    cv::Mat removeShadowMat;
    cv::normalize(calcMat, gray, 0, 255, cv::NORM_MINMAX);
    imshow("dst", gray);

    // cv::Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    // int iteration = 9;

    // cv::Mat closeMat;
    // morphologyEx(gray, closeMat, cv::MORPH_CLOSE, element, cv::Point(-1, -1), iteration);
    // imshow("闭运算", closeMat);

    // cv::Mat calcMat = ~(closeMat - gray);
    // imshow("calc", calcMat);

    // cv::Mat removeShadowMat;
    // cv::normalize(calcMat, gray, 0, 255, cv::NORM_MINMAX);
    // imshow("dst", gray);

    // cv::threshold( gray, g_binary, g_thresh, 255, cv::THRESH_BINARY | cv::THRESH_BINARY_INV);
    // cv::threshold( g_binary, g_binary, 0, 255, cv::THRESH_BINARY);

    cv::threshold(gray, g_binary, g_thresh, 255, cv::THRESH_BINARY);
    vector<vector<cv::Point>> contours;
    cv::findContours(
        g_binary,
        contours,
        cv::noArray(),
        cv::RETR_TREE,
        cv::CHAIN_APPROX_NONE);
    //   g_binary = cv::Scalar::all(0);

    cv::drawContours(g_binary, contours, -1, cv::Scalar::all(255));
    cv::imshow("Contours", g_binary);
}

int main(int argc, char **argv)
{
    if (argc != 2 || (g_gray = cv::imread(argv[1])).empty())
    {
        cout << "\nExample 14-1: Find threshold dependent contours\nUsage:\n"
             << argv[0]
             << " ../fruits.jpg\n"
             << endl;
        return -1;
    }

    cv::cvtColor(g_gray, g_hsv, cv::COLOR_RGB2HSV);

    cv::namedWindow("Contours", 1);

    cv::createTrackbar(
        "Threshold",
        "Contours",
        &g_thresh,
        255,
        on_trackbar);
    on_trackbar(0, 0);

    cv::waitKey();

    return 0;
}
