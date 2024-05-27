#include <opencv2/opencv.hpp>

#include <array>
#include <string>


cv::Mat contrast(const cv::Mat& img) {
    cv::Scalar sum{ cv::sum(img) };
    cv::Scalar norm{ sum / (img.rows * img.cols) };
    std::array<cv::Mat, 3> channels{};
    cv::split(img, channels);
    double scale = (norm[0] + norm[1] + norm[2]) / 3.0;
    channels[0] *= scale / norm[0];
    channels[1] *= scale / norm[1];
    channels[2] *= scale / norm[2];
    cv::Mat res{};
    cv::merge(channels, res);
    return res;
}

int main(int argc, char* argv[]) {
    std::string input = "C:/Users/Иван/misis2024s-21-02-solovev-i-s/prj.lab/lab09/assets/bgr.png";
    std::string output = "C:/Users/Иван/misis2024s-21-02-solovev-i-s/prj.lab/lab09/assets/result.png";
    if (argc >= 3) {
        input = argv[1];
        output = argv[2];
    }
    cv::Mat img{ cv::imread(input) };
    cv::Mat result{ contrast(img) };
    cv::imshow("original", img);
    cv::imshow("result", result);
    cv::waitKey(0);

}
