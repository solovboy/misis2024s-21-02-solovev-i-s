#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>



cv::Mat gammaCorrection(cv::Mat& img, float gamma) {
    // ��������� �����-��������� �������� �����������
    for (int i = img.rows / 2; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            float pixelValue = img.at<uchar>(i, j) / 255.0;
            pixelValue = pow(pixelValue, gamma);
            img.at<uchar>(i, j) = static_cast<uchar>(pixelValue * 255);
        }
    }
    return img;
}

int main(int argc, char* argv[]) {
    int s = 3;
    int h = 30;
    float gamma = 2.4;

    // ��������� ������� ���������� ��������� ������
    if (argc >= 4) {
        s = std::stoi(argv[1]);
        h = std::stoi(argv[2]);
        gamma = std::stof(argv[3]);
    }

    // ������� ����������� � ������ ������� ����������� ��������
    cv::Mat gradientImg(2*h, s * 255, CV_8UC1);
    for (int i = 0; i < 2* h; i++) {
        for (int j = 0; j < s * 255; j++) {
            gradientImg.at<uchar>(i, j) = static_cast<uchar>(j / s);
        }
    }
    // ��������� �����-��������� �� ������ ������ �����������
    cv::Mat gammaCorrectedImg = gammaCorrection(gradientImg, gamma);


    // ��������� ������� ��������� ����� ��������� �����
    if (argc >= 5) {
        // ��������� ����������� � �������� ����
        std::string filename = argv[4];
        cv::imwrite(filename, gammaCorrectedImg);
    }
    else {
        // ���������� ����������� �� ������ � ���� ������� �������
        cv::imshow("Gradient Image and Gamma Corrected Image", gammaCorrectedImg);
        cv::waitKey(0);
    }

    return 0;
}