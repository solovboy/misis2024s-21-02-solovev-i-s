#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <vector>

cv::Mat generateImage(const std::vector<std::vector<int>> levels) {
	std::vector<cv::Mat> images;


	for (int i = 0; i < levels.size(); i++) {
		int side_length = 99;
		cv::Mat image(side_length, side_length, CV_8UC1, cv::Scalar(levels[i][0]));

		int radius = 25;
		cv::Point center((side_length + 1) / 2, (side_length + 1) / 2);
		cv::circle(image, center, radius, cv::Scalar(levels[i][1]), -1);

		images.push_back(image);
	
	}
	cv::Mat result;
	cv::Mat top = images[0].clone();
	cv::Mat bottom = images[3].clone();

	for (int i = 1; i < 3; i++) {
		cv::hconcat(top, images[i], top);
	}
	for (int i = 4; i < 6; i++) {
		cv::hconcat(bottom, images[i], bottom);
	}

	cv::vconcat(top, bottom, result);

	return result;


}
int main() {
	std::vector<std::vector<int>> levels = { { 0, 127 }, {0, 255}, {127, 0}, {127, 255}, {255, 0}, {255, 127} };
	cv::Mat result = generateImage(levels);
	cv::imshow("image", result);
	cv::waitKey(0);

}