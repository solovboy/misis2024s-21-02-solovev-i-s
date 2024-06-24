#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

struct Image
{
    cv::Mat src_image, result, result_for_estimation;
    cv::FileStorage json;
    int weight, height, color, blur;
    double noise;
};

void generateNoise(Image& img) {
    double mean = 0;
    cv::Mat noise(img.src_image.size(), CV_8SC1);
    cv::randn(noise, cv::Scalar{ mean }, cv::Scalar{ img.noise });
    img.src_image += noise;
}

void generateImage(Image& img)
{
    img.json.open("../prj.lab/lab07/input.json", cv::FileStorage::READ);
    if (!img.json.isOpened()) {
        std::cerr << "Unable to open file for writing." << std::endl;
    }

    cv::FileNode data = img.json["data"];
    cv::FileNode objects = data["objects"];
    cv::FileNode background = data["background"];
    cv::FileNode size_of_image = background["size"];

    img.weight = (int)size_of_image[0];
    img.height = (int)size_of_image[1];
    img.color = (int)background["color"];
    img.blur = (int)background["blur"];
    img.noise = (int)background["noise"];

    img.src_image = cv::Mat(img.weight, img.height, CV_8U, cv::Scalar(img.color));
    

    for (cv::FileNodeIterator it = objects.begin(); it != objects.end(); ++it) {
        cv::FileNode obj = *it;
        cv::FileNode p = obj["p"];

        int x = (int)p[0];
        int y = (int)p[1];
        int radius = (int)p[2];

        int contrast = (int)obj["c"];
        cv::Point center(x, y);
        cv::circle(img.src_image, center, radius, contrast, cv::FILLED);

    }

    cv::Mat img_for_bluring;
    img.src_image.copyTo(img_for_bluring);
    cv::GaussianBlur(img_for_bluring, img.src_image, cv::Size(5, 5), img.blur);

    generateNoise(img);
    
    img.json.release();
}

void results(Image& img)
{
    cv::Mat resultBW;
    cv::cvtColor(img.result, resultBW, cv::COLOR_BGR2GRAY);
    cv::Mat intersection = (img.src_image & resultBW);
    cv::Mat unionArea = (img.src_image | resultBW);

    double intersectionArea = cv::countNonZero(intersection);
    double unionAreaValue = cv::countNonZero(unionArea);

    double IoU = intersectionArea / unionAreaValue;
    double precision = intersectionArea / cv::countNonZero(resultBW);
    double recall = intersectionArea / cv::countNonZero(img.src_image);
    double f1Score = 2 * (precision * recall) / (precision + recall);

    std::cout << "Intersection over Union: " << IoU << std::endl;
    std::cout << "Precision: " << precision << std::endl;
    std::cout << "Recall: " << recall << std::endl;
    std::cout << "F1: " << f1Score << std::endl;

}

void segmentation(Image& img)
{
    cv::Mat edges, thresh;

    double otsu_threshold = cv::threshold(img.src_image, thresh, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    double low_threshold = otsu_threshold * 0.5;
    double high_threshold = otsu_threshold * 1.5;

    cv::Canny(img.src_image, edges, low_threshold, high_threshold);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    cv::cvtColor(img.src_image, img.result, cv::COLOR_GRAY2BGR);
    img.result_for_estimation = cv::Mat(img.weight, img.height, CV_8UC3, cv::Scalar(img.color));
    cv::Scalar color(0, 255, 0);

    for (size_t i = 0; i < contours.size(); ++i) {
        cv::drawContours(img.result, contours, static_cast<int>(i), color, cv::FILLED);
        cv::drawContours(img.result_for_estimation, contours, static_cast<int>(i), color, cv::FILLED);
    }

    results(img);

}

int main() {
    Image img;

    generateImage(img);
    cv::imwrite("example4.png", img.src_image);

    segmentation(img);
    cv::imwrite("result4.png", img.result);

    cv::Mat concated, bgr_source_image;
    cv::cvtColor(img.src_image, bgr_source_image, cv::COLOR_GRAY2BGR);
    cv::hconcat(bgr_source_image, img.result, concated);
    cv::imshow("Result", concated);
    cv::waitKey(0);

    return 0;
}