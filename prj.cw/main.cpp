#include <filter/filter.hpp>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <exception>
#include <string>



/*int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    try {
        if (argc < 5) {
            throw argc;
        }
        std::string executable_path = argv[0];
        std::string file_path = argv[1];
        std::string path_save = argv[2];
        cv::Mat I = cv::imread(file_path, cv::IMREAD_GRAYSCALE);
        cv::imshow("Original image", I);
        double a = atof(argv[3]);
        int T = atoi(argv[4]);
        std::string visualization = argv[5];

        PeronaMalik image(I, a, T);
        cv::Mat Processed_image = image.PeronaMalikGray();
        cv::imwrite(path_save, Processed_image);
        cv::imshow("Processed image", Processed_image);
        cv::waitKey(0);

    }
    catch (int argc) {
        std::cout << "Incorrect numbers of arguments" << std::endl;
        std::cerr << "Error: Not enough arguments. Usage: " << argv[0] << " <input_file_path> <save_file_path> <a(update rate)> <T(number of iterations)>" << std::endl;
    }

}*/


int main() {
    std::string file_path = "C:/Users/Иван/misis2024s-21-02-solovev-i-s/prj.cw/malik.png";
    cv::Mat I = cv::imread(file_path, cv::IMREAD_GRAYSCALE);
    cv::imshow("Original image", I);
    double a = 0.2;
    int T = 15;
    PeronaMalik image(I, a, T);
    cv::Mat Processed_image = image.PeronaMalikGray();
    cv::imshow("Processed image", Processed_image);
    cv::waitKey(0);

}