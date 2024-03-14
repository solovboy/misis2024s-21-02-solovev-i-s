#pragma once

#include <opencv2/core/core.hpp>


class PeronaMalik :public cv::Algorithm {
public:
    PeronaMalik();
    PeronaMalik(cv::Mat Imog, double a, int T);
    cv::Mat PeronaMalikGray();
private:
    cv::Mat Imog_;
    double a_ = 0;
    int T_ = 0;
};


double conductivity_function(double difference, double k);

