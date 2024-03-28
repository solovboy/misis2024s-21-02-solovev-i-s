#include <filter/filter.hpp>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <exception>
#include <string>


double getPSNR(const cv::Mat& I1, const cv::Mat& I2)
{
    cv::Mat s1;
    absdiff(I1, I2, s1);       
    s1.convertTo(s1, CV_32F);  
    s1 = s1.mul(s1);         

    cv::Scalar s = sum(s1);         
    double sse = s.val[0]; 

    if (sse <= 1e-10) 
        return 0;
    else
    {
        double  mse = sse / (double)(I1.channels() * I1.total());
        double psnr = 10.0 * log10((255 * 255) / mse);
        return psnr;
    }
}

int main(int argc, char* argv[]) {
    try {
        std::string file_path = "C:/Users/Иван/misis2024s-21-02-solovev-i-s/prj.cw/livingroom.png";
        double a = 0.2;
        int T = 10;
        double k = 12;
        if (argc >= 5) {
            file_path = argv[1];
            a = std::stof(argv[2]);
            T = std::stoi(argv[3]);
            k = std::stof(argv[4]);
        }
        else if (argc != 1) {
            throw argc;
        }
        
        const cv::Mat I = cv::imread(file_path, cv::IMREAD_GRAYSCALE);
        cv::Mat I1 = cv::imread(file_path, cv::IMREAD_GRAYSCALE);
        PeronaMalik image(I1, a, T, k);
        cv::Mat Processed_image = image.PeronaMalikGray();

        double getpsnr = getPSNR(I, Processed_image);

        cv::imshow("Original image", I);
        cv::imshow("Processed image", Processed_image);
        cv::waitKey(0);

    }
    catch (int argc) {
        std::cout << "Incorrect numbers of arguments" << std::endl;
        std::cerr << "Error: Not enough arguments. Usage: " << argv[0] << "<input_file_path> <a(update rate)> <T(number of iterations)> <k(sensitivity coefficient)>" << std::endl;
    }

}
