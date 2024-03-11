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

void onTrackbar(int, void*) {}

int main(int argc, char* argv[]) {
	//Default parametrs
	int countCircles = 6;
	int minRadius = 15;
	int maxRadius = 30;
	int minContrast = 50;
	int maxContrast = 255;
	int blur = 4;
	int method = 1;

	if (argc >= 7) {
		countCircles = std::stoi(argv[1]);
		minRadius = std::stoi(argv[2]);
		maxRadius = std::stoi(argv[3]);
		minContrast = std::stoi(argv[4]);
		maxContrast = std::stoi(argv[5]);
		blur = std::stoi(argv[6]);
	}

	cv::Mat image = generateImage(countCircles, minRadius, maxRadius, minContrast, maxContrast, blur);


	//cv::Mat binaryBernsen = bernsenBinary(image, 3, 3);

	//cv::Mat binaryNiblack = niblackBinary(image, 2, -1);

	cv::Mat binaryImage;

	if (method == 1) {
		int windowSize = 1;
		int contrastThreshold = 1;

		cv::namedWindow("Binary Image");
		cv::createTrackbar("Window Size", "Binary Image", &windowSize, 30, onTrackbar);
		cv::setTrackbarMin("Window Size", "Binary Image", windowSize);
		cv::createTrackbar("Contrast Threshold", "Binary Image", &contrastThreshold, 255, onTrackbar);
		cv::setTrackbarMin("Contrast Threshold", "Binary Image", contrastThreshold);
		onTrackbar(0, 0);

		while (true) {
			binaryImage = bernsenBinary(image, windowSize, contrastThreshold);
			imshow("Binary Image", binaryImage);

			char key = cv::waitKey(10);
			if (key == 27) // ESC key to exit
				break;
		}
	
	}
	else if (method == 2){
		int windowSize = 1;
		double k = -10;

		cv::namedWindow("Binary Image");
		cv::createTrackbar("Window Size", "Binary Image", &windowSize, 15, onTrackbar);
		cv::setTrackbarMin("Window Size", "Binary Image", windowSize);
		cv::createTrackbar("k Value", "Binary Image", nullptr, 0, onTrackbar);
		cv::setTrackbarMin("k Value", "Binary Image", k);
		onTrackbar(0, 0);

		while (true) {
			k = static_cast<double>(cv::getTrackbarPos("k Value", "Binary Image")) / 5.0;
			binaryImage = niblackBinary(image, windowSize, k);
			imshow("Binary Image", binaryImage);

			char key = cv::waitKey(10);
			if (key == 27) // ESC key to exit
				break;
		}
	}
}