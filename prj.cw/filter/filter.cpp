#include <filter/filter.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>

PeronaMalik::PeronaMalik() {
    cv::Mat Img_;
    double a_ = 0;
    int T_ = 0;
    double k_ = 1;
}
PeronaMalik::PeronaMalik(cv::Mat Img, double a, int T, double k) {
    Img_ = Img;
    a_ = a;
    T_ = T;
    k_ = k;
}

cv::Mat PeronaMalik::PeronaMalikGray() {
    cv::Mat I2 = Img_;
    int rows = Img_.size().height;
    int cols = Img_.size().width;
    for (int i = 1; i <= T_; i++) {
        std::vector<int> av(5);
        for (int col = 0; col < cols; col++) {
            for (int row = 0; row < rows; row++) {
                if (col == 0 || col == cols - 1 || row == 0 || row == rows - 1) {
                    continue;
                }
                double a_middle = Img_.at<uchar>(row, col);
                double a_right = Img_.at<uchar>(row, col + 1);
                double a_left = Img_.at<uchar>(row, col - 1);
                double a_up = Img_.at<uchar>(row + 1, col);
                double a_down = Img_.at<uchar>(row - 1, col);
                av[0] = a_middle;
                av[1] = a_right;
                av[2] = a_left;
                av[3] = a_up;
                av[4] = a_down;
                double grad = 0;
                for (int i = 1; i < 5; i = i + 1) {
                    double difference = av[i] - av[0];
                    grad = grad + conductivity_function(difference, k_) * difference;
                }
                I2.at<uchar>(row, col) = Img_.at<uchar>(row, col) + a_ * grad;
            }
        }
    }
    return I2;
};

double conductivity_function(double difference, double k) {
    double value = 0;
    value = 1 / (1 + ((difference / k) * (difference / k)));
    return value;
}