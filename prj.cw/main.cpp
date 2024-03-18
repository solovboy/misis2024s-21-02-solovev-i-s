#include <filter/filter.hpp>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <exception>
#include <string>



int main(int argc, char* argv[]) {
    try {
        std::string file_path = "C:/Users/Иван/misis2024s-21-02-solovev-i-s/prj.cw/tomography.png";
        double a = 0.2;
        int T = 10;
        double k = 8;
        if (argc >= 5) {
            file_path = argv[1];
            a = std::stof(argv[2]);
            T = std::stoi(argv[3]);
            k = std::stof(argv[4]);
        }
        else if (argc != 1) {
            throw argc;
        }
        
        cv::Mat I = cv::imread(file_path, cv::IMREAD_GRAYSCALE);
        cv::imshow("Original image", I);

        PeronaMalik image(I, a, T, k);
        cv::Mat Processed_image = image.PeronaMalikGray();
        cv::imshow("Processed image", Processed_image);
        cv::waitKey(0);

    }
    catch (int argc) {
        std::cout << "Incorrect numbers of arguments" << std::endl;
        std::cerr << "Error: Not enough arguments. Usage: " << argv[0] << "<input_file_path> <a(update rate)> <T(number of iterations)> <k(sensitivity coefficient)>" << std::endl;
    }

}
