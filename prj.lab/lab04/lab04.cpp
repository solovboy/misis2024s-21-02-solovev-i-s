#include <iostream>
#include <omp.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


cv::Mat niblackBinary(const cv::Mat& image, int windowSize, double k) {
	cv::Mat binary(image.size(), CV_8U, cv::Scalar(0));

	int halfWindowSize = windowSize / 2;

	#pragma omp parallel for collapse(2)
	for (int y = halfWindowSize; y < image.rows - halfWindowSize; ++y) {
		for (int x = halfWindowSize; x < image.cols - halfWindowSize; ++x) {
			cv::Rect windowRect(x - halfWindowSize, y - halfWindowSize, windowSize, windowSize);
			cv::Mat window = image(windowRect);

			// Calculate the average value and standard deviation in the window
			cv::Scalar mean, stddev;
			cv::meanStdDev(window, mean, stddev);

			double threshold = mean.val[0] + k * stddev.val[0];

			// If the contrast is greater than the threshold, then the pixel turns white, otherwise black
			if (image.at<uchar>(y, x) > threshold)
				binary.at<uchar>(y, x) = 255;
			else
				binary.at<uchar>(y, x) = 0;
		}
	}

	return binary;
}

cv::Mat bernsenBinary(const cv::Mat& image, int windowSize, int contrastThreshold) {
	cv::Mat binary(image.size(), CV_8U, cv::Scalar(0));

	int halfWindowSize = windowSize / 2;
	int contrastMax = 255;

	#pragma omp parallel for collapse(2)
	for (int y = halfWindowSize; y < image.rows - halfWindowSize; ++y) {
		for (int x = halfWindowSize; x < image.cols - halfWindowSize; ++x) {
			cv::Rect windowRect(x - halfWindowSize, y - halfWindowSize, windowSize, windowSize);
			cv::Mat window = image(windowRect);

			// Find the minimum and maximum values in the window
			double minVal, maxVal;
			cv::minMaxLoc(window, &minVal, &maxVal);

			// Calculating the window contrast
			double contrast = maxVal - minVal;

			// If the contrast is greater than the threshold, then the pixel turns white, otherwise black
			if (contrast >= contrastThreshold)
				binary.at<uchar>(y, x) = contrastMax;
			else
				binary.at<uchar>(y, x) = 0;
		}
	}

	return binary;
}


cv::Mat generateImage(const int& countCircles, int minRadius, int maxRadius, int minContrast, int maxContrast, int blur) {
	int side_length = 4 * maxRadius * (countCircles - 1);
	cv::Mat image(side_length, side_length, CV_8UC1, cv::Scalar(0));

	int radius = minRadius;
	int contrast = minContrast;
	for (int i = side_length / 10; i <= (side_length - side_length / 10); i += (side_length - side_length / 5) / (countCircles - 1)) {
		for (int j = side_length / 10; j <= (side_length - side_length / 10); j += (side_length - side_length / 5) / (countCircles - 1)){
			cv::Point center(i, j);
			cv::circle(image, center, radius, cv::Scalar(contrast), -1);
			contrast += (maxContrast - minContrast) / countCircles;
		}
		contrast = minContrast;
		radius += (maxRadius - minRadius) / countCircles;
	}

	cv::GaussianBlur(image, image, cv::Size(blur * 2 + 1, blur * 2 + 1), 0);

	return image;
}


int main(int argc, char* argv[]) {
	//Default parametrs
	int countCircles = 6;
	int minRadius = 15;
	int maxRadius = 30;
	int minContrast = 50;
	int maxContrast = 255;
	int blur = 4;

	if (argc >= 7) {
		countCircles = std::stoi(argv[1]);
		minRadius = std::stoi(argv[2]);
		maxRadius = std::stoi(argv[3]);
		minContrast = std::stoi(argv[4]);
		maxContrast = std::stoi(argv[5]);
		blur = std::stoi(argv[6]);
	}

	cv::Mat image = generateImage(countCircles, minRadius, maxRadius, minContrast, maxContrast, blur);

	cv::Mat binaryBernsen = bernsenBinary(image, 3, 3);

	cv::Mat binaryNiblack = niblackBinary(image, 3, -0.2);

	cv::imshow("Original", image);
	cv::imshow("binaryBernsen", binaryBernsen);
	cv::imshow("binaryNiblack", binaryNiblack);
	cv::waitKey(0);
}