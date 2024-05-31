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

int main(int argc, char* argv[]) {
    try {
        std::string file_path = "C:/Users/Иван/misis2024s-21-02-solovev-i-s/prj.cw/dataset/text.png";
        double a = 0.2;
        int T = 12;
        double k = 16;
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
        cv::Mat Gaus;
        cv::GaussianBlur(I1, Gaus, cv::Size(3, 3), 10);
        PeronaMalik image(I1, a, T, k);
        cv::Mat Processed_image = image.PeronaMalikGray();

        double psnrP = getPSNR(I, Processed_image);
        double psnrG = getPSNR(I, Gaus);

        double msSSIMP = 0;
        double msSSIMG = 0;
        int levels = 5;
        for (int i = 0; i < levels; i++) {
            cv::Mat resizedI1, resizedI2;
            cv::resize(I, resizedI1, cv::Size(), pow(2, i), pow(2, i));

            cv::resize(Processed_image, resizedI2, cv::Size(), pow(2, i), pow(2, i));
            msSSIMP += getSSIM(resizedI1, resizedI2);

           
            cv::resize(Gaus, resizedI2, cv::Size(), pow(2, i), pow(2, i));
            msSSIMG += getSSIM(resizedI1, resizedI2);

        }
        msSSIMP /= levels;
        msSSIMG /= levels;

        std::cout << " "<< std::endl;
        std::cout << "PSNR: " << psnrP << "  " << psnrG << std::endl;
        std::cout << " " << std::endl;
        std::cout << "MS SSIM: " << msSSIMP << "  " << msSSIMG << std::endl;
        std::cout << " " << std::endl;

        cv::imshow("Original image", I);
        cv::imshow("Processed image", Processed_image);
        cv::imshow("Gaus", Gaus);
        cv::waitKey(0);

    }
    catch (int argc) {
        std::cout << "Incorrect numbers of arguments" << std::endl;
        std::cerr << "Error: Not enough arguments. Usage: " << argv[0] << "<input_file_path> <a(update rate)> <T(number of iterations)> <k(sensitivity coefficient)>" << std::endl;
    }

}
