#include <iostream>
#include <vector>
#include <string>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

cv::Mat drawContrastHistogram(const cv::Mat& image, const double& left, const double& right) {
	cv::Mat histogramImage(256, 256, CV_8UC1, cv::Scalar(230));

	std::vector<int> hist(256, 0);
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			hist[image.at<uchar>(i, j)]++;
		}
	}

	std::vector<int> sumHist;
	sumHist.push_back(hist[0]);
	for (int i = 1; i < hist.size(); i++) {
		sumHist.push_back(sumHist[i - 1] + hist[i]);
	}

	int maxHist = *max_element(hist.begin(), hist.end());
	float scaleFactor1 = 4 * 230.0 / maxHist;

	int maxSumHist = *max_element(sumHist.begin(), sumHist.end());
	float scaleFactor2 = 230.0 / maxSumHist;

	int last_x = 0, last_y = 255;
	for (int i = 0; i < 256; ++i) {
		int histHeight = hist[i] * scaleFactor1;
		int sumHistHeight = sumHist[i] * scaleFactor2;
		cv::rectangle(histogramImage, cv::Point(i, 255), cv::Point(i + 1, 255 - histHeight), cv::Scalar(120), -1);
		cv::rectangle(histogramImage, cv::Point(last_x, last_y), cv::Point(i + 1, 255 - sumHistHeight), cv::Scalar(0), -1);
		last_x = i + 1;
		last_y = 255 - sumHistHeight;

	}

	return histogramImage;
}

cv::Mat drawHistogram(const cv::Mat& image, const double& left, const double& right) {
	cv::Mat histogramImage(256, 256, CV_8UC1, cv::Scalar(230));

	std::vector<int> hist(256, 0);
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			hist[image.at<uchar>(i, j)]++;
		}
	}

	std::vector<int> sumHist;
	sumHist.push_back(hist[0]);
	for (int i = 1; i < hist.size(); i++) {
		sumHist.push_back(sumHist[i - 1] + hist[i]);
	}

	int maxHist = *max_element(hist.begin(), hist.end());
	float scaleFactor1 = 230.0 / maxHist;

	int maxSumHist = *max_element(sumHist.begin(), sumHist.end());
	float scaleFactor2 = 230.0 / maxSumHist;

	int last_x = 0, last_y = 255;
	for (int i = 0; i < 256; ++i) {
		int histHeight = hist[i] * scaleFactor1;
		int sumHistHeight = sumHist[i] * scaleFactor2;
		cv::rectangle(histogramImage, cv::Point(i, 255), cv::Point(i + 1, 255 - histHeight), cv::Scalar(120), -1);
		cv::rectangle(histogramImage, cv::Point(last_x, last_y), cv::Point(i + 1, 255 - sumHistHeight), cv::Scalar(0), -1);
		last_x = i + 1;
		last_y = 255 - sumHistHeight;

	}

	int currentSum = 0;
	for (int i = 0; i < hist.size(); i++) {
		currentSum += hist[i];
		if (currentSum >= image.rows * image.cols * left) {
			cv::rectangle(histogramImage, cv::Point(i, 255), cv::Point(i, 0), cv::Scalar(255), -1);
			currentSum = 0;
			break;
		}
	}

	for (int i = 0; i < hist.size(); i++) {
		currentSum += hist[i];
		if (currentSum >= image.rows * image.cols * right) {
			cv::rectangle(histogramImage, cv::Point(i, 255), cv::Point(i, 0), cv::Scalar(255), -1);
			currentSum = 0;
			break;
		}
	}

	return histogramImage;
}

std::vector<int> searchClowCHigh(cv::Mat& image, const double left, const double right) {
	std::vector<int> result;

	std::vector<int> hist(256, 0);
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			hist[image.at<uchar>(i, j)]++;
		}
	}

	int currentSum = 0;
	for (int i = 0; i < hist.size(); i++) {
		currentSum += hist[i];
		if (currentSum >= image.rows * image.cols * left) {
			int Clow = i;
			result.push_back(Clow);
			currentSum = 0;
			break;
		}
	}

	for (int i = 0; i < hist.size(); i++) {
		currentSum += hist[i];
		if (currentSum >= image.rows * image.cols * right) {
			int Clow = i;
			result.push_back(Clow);
			currentSum = 0;
			break;
		}
	}

	return result;
}

cv::Mat autoContrast(cv::Mat& image, const int& Cmin, const int& Cmax, const int& Clow, const int& Chigh) {
	cv::Mat contrastImage(image.rows, image.cols, CV_8UC1);
	for (int i = 0; i < contrastImage.rows; i++) {
		for (int j = 0; j < contrastImage.cols; j++) {
			float pixel = Cmin + (image.at<uchar>(i, j) - Clow) * ((Cmax - Cmin) / (Chigh - Clow));
			contrastImage.at<uchar>(i, j) = cv::saturate_cast<uchar>(pixel);
		}
	}
	return contrastImage;
}

void autoContrastTogetherBGR(cv::Mat& image, const double left, const double right) {
	std::vector<cv::Mat> BGR(3);
	cv::Mat contrastImage;
	int Cmin = 0, Cmax = 255;

	cv::split(image, BGR);

	int finalClow = 255, finalChigh = 0;
	for (int i = 0; i < BGR.size(); i++) {
		std::vector<int> ClowChigh = searchClowCHigh(BGR[i], left, right);
		int Clow = ClowChigh[0], Chigh = ClowChigh[1];
		if (Clow < finalClow) {
			finalClow = Clow;
		}
		if (Chigh < finalChigh) {
			finalChigh = Chigh;
		}
	}
	std::vector<cv::Mat> contrastChannels;
	for (int i = 0; i < BGR.size(); i++) {
		cv::Mat contrastChannel = autoContrast(BGR[i], Cmin, Cmax, finalClow, finalChigh);
		contrastChannels.push_back(contrastChannel);
	}

	cv::merge(contrastChannels, contrastImage);

	cv::Mat histogram = drawHistogram(image, left, right);
	cv::Mat contrastHistogram = drawContrastHistogram(contrastImage, left, right);

	cv::Mat images, histograms;
	cv::hconcat(image, contrastImage, images);
	cv::hconcat(histogram, contrastHistogram, histograms);

	cv::imwrite("ImagesTogetherBGR.png", images);
	cv::imwrite("HistogramsTogetherBGR.png", histograms);

	cv::imshow("ImagesTogetherBGR", images);
	cv::imshow("HistogramsTogetherBGR", histograms);
	cv::waitKey(0);
}

void autoContrastBGR(cv::Mat& image, const double left, const double right) {
	std::vector<cv::Mat> BGR(3);
	cv::Mat contrastImage;
	int Cmin = 0, Cmax = 255;

	cv::split(image, BGR);

	std::vector<cv::Mat> contrastChannels;
	for (int i = 0; i < BGR.size(); i++) {
		std::vector<int> ClowChigh = searchClowCHigh(BGR[i], left, right);
		int Clow = ClowChigh[0], Chigh = ClowChigh[1];
		cv::Mat contrastChannel = autoContrast(BGR[i], Cmin, Cmax, Clow, Chigh);
		contrastChannels.push_back(contrastChannel);
	}

	cv::merge(contrastChannels, contrastImage);

	cv::Mat histogram = drawHistogram(image, left, right);
	cv::Mat contrastHistogram = drawContrastHistogram(contrastImage, left, right);

	cv::Mat images, histograms;
	cv::hconcat(image, contrastImage, images);
	cv::hconcat(histogram, contrastHistogram, histograms);

	cv::imshow("ImagesBGR", images);
	cv::imshow("HistogramsBGR", histograms);
	cv::waitKey(0);


}

void autoContrastBW(cv::Mat& image, const double left, const double right) {
	int Cmin = 0, Cmax = 255;
	std::vector<int> ClowChigh = searchClowCHigh(image, left, right);
	int Clow = ClowChigh[0], Chigh = ClowChigh[1];
	std::cout << Clow << " " << Chigh << std::endl;
	cv::Mat histogram = drawHistogram(image, left, right);
	cv::Mat contrastImage = autoContrast(image, Cmin, Cmax, Clow, Chigh);
	cv::Mat contrastHistogram = drawContrastHistogram(contrastImage, left, right);

	cv::Mat images, histograms;
	cv::hconcat(image, contrastImage, images);
	cv::hconcat(histogram, contrastHistogram, histograms);

	cv::imshow("ImagesBW", images);
	cv::imshow("HistogramsBW", histograms);
	cv::waitKey(0);

}

int main(int argc, char* argv[]) {
	std::string imagePath= "C:/Users/Иван/misis2024s-21-02-solovev-i-s/prj.lab/lab03/assets/bgr.png";
	cv::Mat image = cv::imread(imagePath, cv::IMREAD_UNCHANGED);

	double left = 0.4, right = 0.6;

	if (argc >= 4) {
		imagePath = argv[1];
		left = std::stof(argv[2]);
		right = std::stof(argv[3]);
	}

	if (left + right != 1) {
		std::cout << "The sum of the quantiles must be equal to 1!" << std::endl;
		return -1;
	}

	if ((left < 0) || (right < 0)) {
		std::cout << "The quantiles must be positive!" << std::endl;
		return -1;
	}

	if (image.channels() == 1) {
		autoContrastBW(image, left, right);
	}
	else if (image.channels() == 3) {
		autoContrastBGR(image, left, right);
		autoContrastTogetherBGR(image, left, right);
	}
	else {
		std::cout << "The program works with 1 and 3 channel images!" << std::endl;
		return -1;
	}
	return 0;
}