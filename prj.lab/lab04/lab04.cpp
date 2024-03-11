#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

cv::Mat bernsenBinary(const cv::Mat& image, int windowSize, int contrastThreshold) {
	cv::Mat binary(image.size(), CV_8U, cv::Scalar(0));

	int halfWindowSize = windowSize / 2;
	int contrastMax = 255;

	for (int y = halfWindowSize; y < image.rows - halfWindowSize; ++y) {
		for (int x = halfWindowSize; x < image.cols - halfWindowSize; ++x) {
			cv::Rect windowRect(x - halfWindowSize, y - halfWindowSize, windowSize, windowSize);
			cv::Mat window = image(windowRect);

			// Ќаходим минимальное и максимальное значение в окне
			double minVal, maxVal;
			cv::minMaxLoc(window, &minVal, &maxVal);

			// ¬ычисл€ем контраст окна
			double contrast = maxVal - minVal;

			// ≈сли контраст больше порога, то пиксель становитс€ белым, иначе черным
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


int main() {

	cv::Mat image = generateImage(6, 15, 30, 50, 255, 4);

	cv::Mat binary = bernsenBinary(image, 3, 3);

	cv::imshow("Image1", image);
	cv::imshow("Image2", binary);
	cv::waitKey(0);
}