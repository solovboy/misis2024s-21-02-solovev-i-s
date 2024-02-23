#include <iostream>
#include <vector>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>



// Функция для генерации тестового изображения с тремя уровнями яркости
cv::Mat generate_test_image(std::vector<int> level) {
    int side_length = 256;
    cv::Mat image(side_length, side_length, CV_8UC1, cv::Scalar(level[0], level[0], level[0]));

    int inner_square_side = 209;
    int radius = 83;
    cv::Rect inner_square_rect((side_length - inner_square_side) / 2, (side_length - inner_square_side) / 2, inner_square_side, inner_square_side);
    rectangle(image, inner_square_rect, cv::Scalar(level[1], level[1], level[1]), -1);

    cv::Point center(side_length / 2, side_length / 2);
    cv::circle(image, center, radius, cv::Scalar(level[2], level[2], level[2]), -1);

    return image;
}

// Функция для рисования гистограммы яркости
cv::Mat draw_brightness_histogram(const cv::Mat& image) {
    cv::Mat histogram_image(256, 256, CV_8UC1, cv::Scalar(230));

    std::vector<int> hist(256, 0);
    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            hist[image.at<uchar>(i, j)]++;
        }
    }

    int max_hist = *max_element(hist.begin(), hist.end());
    float scale_factor = 230.0 / max_hist;

    for (int i = 0; i < 256; ++i) {
        int hist_height = hist[i] * scale_factor;
        cv::rectangle(histogram_image, cv::Point(i, 255), cv::Point(i + 1, 255 - hist_height), cv::Scalar(0), -1);
    }

    return histogram_image;
}

// Функция для добавления аддитивного нормального шума
cv::Mat add_gaussian_noise(const cv::Mat& image, double std_dev) {
    cv::Mat noisy_image;
    cv::Mat noise(image.size(), CV_32FC1);
    cv::randn(noise, 0, std_dev);

    image.convertTo(noisy_image, CV_32FC1);
    noisy_image += noise;
    noisy_image.convertTo(noisy_image, CV_8UC1);

    return noisy_image;
}

int main() {
    std::vector<std::vector<int>> test_levels = { {0, 127, 255}, {20, 127, 235}, {55, 127, 200}, {90, 127, 165} };
    std::vector<double> std_devs = {0, 3, 7, 15 };

    std::vector<cv::Mat> test_images;
    std::vector<std::vector<cv::Mat>> noisy_images;
    std::vector<std::vector<cv::Mat>> histograms;

    // Генерация тестовых изображений
    for (const auto& levels : test_levels) {
        cv::Mat test_image = generate_test_image(levels);
        test_images.push_back(test_image);

        std::vector<cv::Mat> noise_images_for_levels;
        std::vector<cv::Mat> histograms_for_levels;

        // Генерация зашумленных изображений и гистограмм
        for (double std_dev : std_devs) {
            cv::Mat noisy_image = add_gaussian_noise(test_image, std_dev);
            noise_images_for_levels.push_back(noisy_image);

            cv::Mat histogram = draw_brightness_histogram(noisy_image);
            histograms_for_levels.push_back(histogram);
        }

        noisy_images.push_back(noise_images_for_levels);
        histograms.push_back(histograms_for_levels);
    }

    // Склеивание изображений
    cv::Mat combined_image;
    for (size_t i = 0; i < test_levels.size(); ++i) {
        cv::Mat combined_col;
        for (size_t j = 0; j < std_devs.size(); ++j) {
            cv::Mat col;
            cv::vconcat(noisy_images[i][j], histograms[i][j], col);
            if (j == 0) {
                combined_col = col.clone();
            }
            else {
                cv::vconcat(combined_col, col, combined_col);
            }
        }
        if (i == 0) {
            combined_image = combined_col.clone();
        }
        else {
            cv::hconcat(combined_image, combined_col, combined_image);
        }
        
    }

    cv::imshow("Final Image", combined_image);
    cv::imwrite("lab02.png", combined_image);
    cv::waitKey(0);
    return 0;
}
