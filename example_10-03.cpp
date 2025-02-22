// Example 10-3. Threshold versus adaptive threshold
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <iostream>

using namespace std;

enum adaptiveMethod
{
    meanFilter,
    gaaussianFilter,
    medianFilter
};

void AdaptiveThreshold(cv::Mat &src, cv::Mat &dst, double Maxval, int Subsize, double c, adaptiveMethod method = meanFilter)
{

    if (src.channels() > 1)
        cv::cvtColor(src, src, cv::COLOR_RGB2GRAY);

    cv::Mat smooth;
    switch (method)
    {
    case meanFilter:
        cv::blur(src, smooth, cv::Size(Subsize, Subsize)); //均值滤波
        break;
    case gaaussianFilter:
        cv::GaussianBlur(src, smooth, cv::Size(Subsize, Subsize), 0, 0); //高斯滤波
        break;
    case medianFilter:
        cv::medianBlur(src, smooth, Subsize); //中值滤波
        break;
    default:
        break;
    }

    smooth = smooth - c;

    //阈值处理
    src.copyTo(dst);
    for (int r = 0; r < src.rows; ++r)
    {
        const uchar *srcptr = src.ptr<uchar>(r);
        const uchar *smoothptr = smooth.ptr<uchar>(r);
        uchar *dstptr = dst.ptr<uchar>(r);
        for (int c = 0; c < src.cols; ++c)
        {
            if (srcptr[c] > smoothptr[c])
            {
                dstptr[c] = Maxval;
            }
            else
                dstptr[c] = 0;
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 7)
    {
        cout << "\nExample 10-3. Threshold versus adaptive threshold\n"
                "Usage:\n"
             << argv[0] << " fixed_threshold invert(0=off|1=on) "
                           "adaptive_type(0=mean|1=gaussian) block_size offset image\n"
                           "Example:\n"
             << argv[0] << " 100 1 0 15 10 ../src.jpg\n";
        return -1;
    }

    // Process command line arguments
    //
    double fixed_threshold = (double)atof(argv[1]);
    int threshold_type = atoi(argv[2]) ? cv::THRESH_BINARY : cv::THRESH_BINARY_INV;
    int adaptive_method = atoi(argv[3]) ? cv::ADAPTIVE_THRESH_MEAN_C
                                        : cv::ADAPTIVE_THRESH_GAUSSIAN_C;
    int block_size = atoi(argv[4]);
    double offset = (double)atof(argv[5]);
    cv::Mat Igray = cv::imread(argv[6], cv::IMREAD_GRAYSCALE);

    // Read in gray image.
    //
    if (Igray.empty())
    {
        cout << "Can not load " << argv[6] << endl;
        return -1;
    }

    // Declare the output images.
    //
    cv::Mat It, Iat, Imat;

    // double t2 = (double)cv::getTickCount();
    AdaptiveThreshold(Igray, Imat, 255, block_size, offset, meanFilter); //
    // t2 = (double)cv::getTickCount() - t2;
    // double time2 = (t2 *1000.) / ((double)cv::getTickFrequency());
    // std::cout << "my_process=" << time2 << " ms. " << std::endl << std::endl;

    // Thresholds.
    //
    cv::threshold(
        Igray,
        It,
        fixed_threshold,
        255,
        threshold_type);

    cv::adaptiveThreshold(
        Igray,
        Iat,
        255,
        adaptive_method,
        threshold_type,
        block_size,
        offset);

    // Show the results.
    //
    cv::imshow("Raw", Igray);
    cv::imshow("Threshold", It);
    cv::imshow("Adaptive Threshold", Iat);
    cv::imshow("MeanFilter Adaptive Threshold", Imat);
    cv::waitKey(0);
    return 0;
}
