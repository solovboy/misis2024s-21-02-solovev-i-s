#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


int main() {
    cv::Mat image(400, 400, CV_8UC3, cv::Scalar(255, 255, 255));


    int square_size = 200; 

    // ќпределение вершин внешнего квадрата
    int x1 = (image.cols - square_size) / 2;
    int y1 = (image.rows - square_size) / 2; 

    int x2 = x1 + square_size; 
    int y2 = y1 + square_size; 

    // ќпределение параметров внутреннего квадрата
    int inner_square_size = square_size / 2; // –азмер внутреннего квадрата

    int x3 = x1 + (square_size - inner_square_size) / 2; 
    int y3 = y1 + (square_size - inner_square_size) / 2;

    int x4 = x3 + inner_square_size;
    int y4 = y3 + inner_square_size; 

    // –исование внешнего квадрата
    cv::rectangle(image, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 0, 0), 2);

    // –исование внутреннего квадрата
    cv::rectangle(image, cv::Point(x3, y3), cv::Point(x4, y4), cv::Scalar(0, 0, 0), 2);

    cv::circle(image, cv::Point(200, 200), 25, cv::Scalar(0, 0, 0), 2);

    cv::imshow("Image", image);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}