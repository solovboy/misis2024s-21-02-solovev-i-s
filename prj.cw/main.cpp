#include <filter/filter.hpp>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <exception>
#include <string>

double getSSIM(const cv::Mat& image_ref, const cv::Mat& image_obj)
{
    double C1 = 6.5025, C2 = 58.5225;

    int width = image_ref.cols;
    int height = image_ref.rows;
    double mean_x = 0;
    double mean_y = 0;
    double sigma_x = 0;
    double sigma_y = 0;
    double sigma_xy = 0;
    for (int v = 0; v < height; v++)
    {
        for (int u = 0; u < width; u++)
        {
            mean_x += image_ref.at<uchar>(v, u);
            mean_y += image_obj.at<uchar>(v, u);

        }
    }
    mean_x = mean_x / width / height;
    mean_y = mean_y / width / height;
    for (int v = 0; v < height; v++)
    {
        for (int u = 0; u < width; u++)
        {
            sigma_x += (image_ref.at<uchar>(v, u) - mean_x) * (image_ref.at<uchar>(v, u) - mean_x);
            sigma_y += (image_obj.at<uchar>(v, u) - mean_y) * (image_obj.at<uchar>(v, u) - mean_y);
            sigma_xy += abs((image_ref.at<uchar>(v, u) - mean_x) * (image_obj.at<uchar>(v, u) - mean_y));
        }
    }
    sigma_x = sigma_x / (width * height - 1);
    sigma_y = sigma_y / (width * height - 1);
    sigma_xy = sigma_xy / (width * height - 1);
    double fenzi = (2 * mean_x * mean_y + C1) * (2 * sigma_xy + C2);
    double fenmu = (mean_x * mean_x + mean_y * mean_y + C1) * (sigma_x + sigma_y + C2);
    double ssim = fenzi / fenmu;
    return ssim;

}
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

cv::Mat addNoise(const cv::Mat& image, const double std_dev) {
    cv::Mat noisy_image;
    cv::Mat noise(image.size(), CV_32FC1);
    cv::randn(noise, 0, std_dev);

    image.convertTo(noisy_image, CV_32FC1);
    noisy_image += noise;
    noisy_image.convertTo(noisy_image, CV_8UC1);

    return noisy_image;
}

int main(int argc, char* argv[]) {
    try {
        std::string file_path = "../prj.cw/dataset/livingroom.png";
        double noise = 0;
        double a = 0.2;
        int T = 10;
        double k = 8;
        std::string method= "Gauss";
        int kernel = 3;
        if (argc >= 8) {
            file_path = argv[1];
            noise = std::stof(argv[2]);
            a = std::stof(argv[3]);
            T = std::stoi(argv[4]);
            k = std::stof(argv[5]);
            method = argv[6];
            kernel = std::stoi(argv[7]);
        }
        else if (argc != 1) {
            throw argc;
        }
        
        const cv::Mat I = addNoise(cv::imread(file_path, cv::IMREAD_GRAYSCALE), noise);
        cv::Mat I1 = addNoise(cv::imread(file_path, cv::IMREAD_GRAYSCALE), noise);

        cv::Mat methodImage;
        if (method == "Gauss") {
            cv::GaussianBlur(I1, methodImage, cv::Size(kernel, kernel), 0);
        }
        else if (method == "DefaultBlur") {
            cv::blur(I1, methodImage, cv::Size(kernel, kernel));
        }
        else if (method == "Median") {
            cv::medianBlur(I1, methodImage, kernel);
        }
        else {
            throw std::invalid_argument("Unsupported blur method. ");
        }

        PeronaMalik image(I1, a, T, k);
        cv::Mat peronaImage = image.PeronaMalikGray();

        double psnr1 = getPSNR(I, peronaImage);
        double psnr2 = getPSNR(I, methodImage);

        double msSSIM1 = 0;
        double msSSIM2 = 0;
        int levels = 3;
        for (int i = 0; i < levels; i++) {
            cv::Mat resizedI1, resizedI2;
            cv::resize(I, resizedI1, cv::Size(), pow(2, i), pow(2, i));

            cv::resize(peronaImage, resizedI2, cv::Size(), pow(2, i), pow(2, i));
            msSSIM1 += getSSIM(resizedI1, resizedI2);

           
            cv::resize(methodImage, resizedI2, cv::Size(), pow(2, i), pow(2, i));
            msSSIM2 += getSSIM(resizedI1, resizedI2);

        }
        msSSIM1 /= levels;
        msSSIM2 /= levels;

        std::cout << " "<< std::endl;
        std::cout << "PSNR: " << psnr1 << "  " << psnr2 << std::endl;
        std::cout << " " << std::endl;
        std::cout << "MS SSIM: " << msSSIM1 << "  " << msSSIM2 << std::endl;
        std::cout << " " << std::endl;

        cv::imshow("Original", I);
        cv::imshow("Perona&Malik", peronaImage);
        cv::imshow(method, methodImage);
        cv::waitKey(0);

    }
    catch (const int argc) {
        std::cout << "Incorrect numbers of arguments" << std::endl;
        std::cerr << "Error: Not enough arguments. Usage: " << argv[0] << "<input_file_path> <noise> <a(update rate)> <T(number of iterations)> <k(sensitivity coefficient)> <method(second filter)> <kernel>" << std::endl;
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Incorrect blur method" << std::endl;
        std::cerr << "Error: "<< e.what() << "You can use only : Gauss, DefaultBlur, Median"<< std::endl;
    }

}
