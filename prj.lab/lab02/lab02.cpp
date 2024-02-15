#include <iostream>
#include <fstream>
#include <random>
#include <array>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


void addGaussianNoise(cv::Mat& image, double avgNoiseVal, double stdDeviation) {
    cv::Mat noise = cv::Mat(image.size(), CV_8UC1);
    cv::randn(noise, avgNoiseVal, stdDeviation);
    image += noise;
}

void saveHistToCsv(std::array<int, 256> hist) {
    std::ofstream out; 
    std::string filename = "output.csv";
    out.open(filename);
    for (int i = 0; i < hist.size(); ++i) {
        out << i << ';' << hist[i] << '\n';       
        if (!out.good()) {
            break;
        }
    }  
}

int main() {
    cv::Mat image(400, 400, CV_8UC1);

    int square_size = 400; 

    // Определение вершин внешнего квадрата
    int x1 = (image.cols - square_size) / 2;
    int y1 = (image.rows - square_size) / 2; 

    int x2 = x1 + square_size; 
    int y2 = y1 + square_size; 

    // Определение параметров внутреннего квадрата
    int inner_square_size = square_size / 2; // Размер внутреннего квадрата

    int x3 = x1 + (square_size - inner_square_size) / 2; 
    int y3 = y1 + (square_size - inner_square_size) / 2;

    int x4 = x3 + inner_square_size;
    int y4 = y3 + inner_square_size; 

    // Отрисовка внешнего квадрата
    cv::rectangle(image, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(100, 100, 100), -1);

    // Отрисовка внутреннего квадрата
    cv::rectangle(image, cv::Point(x3, y3), cv::Point(x4, y4), cv::Scalar(150, 150, 150), -1);

    cv::circle(image, cv::Point(200, 200), 50, cv::Scalar(200, 200, 200), -1);

    // Изображение с шумом
    cv::Mat noisyImage = image.clone();
    addGaussianNoise(noisyImage, 0, 20);
    
    // Данные для отрисовки гистограмм
    std::array<int, 256> origArr = {0};
    std::array<int, 256> noisyArr = { 0 };

    for (int i = 0; i < image.rows; i++)
    {
        for (int j = 0; j < image.cols; j++)
        {
            origArr[image.at<uchar>(j, i)] += 1;
            noisyArr[noisyImage.at<uchar>(j, i)] += 1;
        }
    }

    cv::imshow("Original", image);
    cv::imshow("Noisy Image", noisyImage);
    cv::waitKey(0);

    //saveHistToCsv(origArr);
    saveHistToCsv(noisyArr);
    
    return 0;
}