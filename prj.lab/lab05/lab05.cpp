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
	std::vector<cv::Mat>r(3);
	cv::Mat image = generateImage(levels);
	cv::imshow("image", image);
	cv::waitKey(0);

	cv::Mat kernel1 = (cv::Mat_<double>(2, 2) << -1, 1, -1, 1);
	cv::Mat filteredI1;
	cv::filter2D(image, filteredI1, -1, kernel1);

	cv::Mat kernel2 = (cv::Mat_<double>(2, 2) << 1, 1, -1, -1);
	cv::Mat filteredI2;
	filter2D(image, filteredI2, -1, kernel2);

	
	filteredI1.convertTo(r[0], CV_32F);
	cv::pow(r[0], 2, r[0]);

	filteredI2.convertTo(r[1], CV_32F);
	cv::pow(r[1], 2, r[1]);

	r[2] = r[0] + r[1];
	cv::sqrt(r[2], r[2]);

	r[2].convertTo(r[2], CV_8U);
	cv::Mat filteredI3 = r[2];

	cv::imshow("Filtered Image I1", filteredI1);
	cv::imshow("Filtered Image I2", filteredI2);
	cv::imshow("Filtered Image I3", filteredI3);
	cv::waitKey(0);

	filteredI1.copyTo(r[0]);
	filteredI2.copyTo(r[1]);

	for (size_t i = 0; i < 2; i++)
	{
		r[i].convertTo(r[i], CV_8UC3, 0.5, 127);
	}
	r[2] = r[2] / sqrt(2);

	cv::imshow("r1", r[0]);
	cv::imshow("r2", r[1]);
	cv::imshow("r3", r[2]);
	cv::waitKey(0);

	cv::Mat result;
	cv::merge(r, result);
	cv::imshow("Result", result);
	cv::waitKey(0);

}