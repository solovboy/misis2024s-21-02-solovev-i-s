#include <filter/filter.hpp>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <exception>
#include <string>



int main(int argc, char* argv[]) {
    try {
        std::string file_path = "C:/Users/Иван/misis2024s-21-02-solovev-i-s/prj.cw/malik.png";
        double a = 0.6;
        int T = 15;
        if (argc >= 4) {
            file_path = argv[1];
            a = atof(argv[2]);
            int T = atoi(argv[3]);
        }
        else if (argc != 1) {
            throw argc;
        }
        
        cv::Mat I = cv::imread(file_path, cv::IMREAD_GRAYSCALE);
        cv::imshow("Original image", I);

        PeronaMalik image(I, a, T);
        cv::Mat Processed_image = image.PeronaMalikGray();
        cv::imshow("Processed image", Processed_image);
        cv::waitKey(0);

    }
    catch (int argc) {
        std::cout << "Incorrect numbers of arguments" << std::endl;
        std::cerr << "Error: Not enough arguments. Usage: " << argv[0] << "<input_file_path> <a(update rate)> <T(number of iterations)>" << std::endl;
    }

}
