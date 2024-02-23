#include "iostream"
#include "vector"
#include "cmath"
#include "algorithm"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

#define PI acos(-1)

Mat ConvertToGray(Mat img) {
	Mat gray = Mat::zeros(img.size(), CV_8UC1);
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			Vec3b rgb = img.at<Vec3b>(i, j);
			gray.at<uchar>(i, j) = 0.3 * rgb[2] + 0.59 * rgb[1] + 0.11 * rgb[0];
		}
	}
	return gray;
}

Mat MeanFilter3X3(Mat img) {
	int row = img.rows;
	int col = img.cols;

	int o_row = row + 2;
	int o_col = col + 2;

	Mat calculateMatrix(o_row, o_col, CV_8UC3, Scalar(0));

	//re-construct matrix 
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			calculateMatrix.at<Vec3b>(i+1, j+1) = img.at<Vec3b>(i, j);
		}
	}

	//calculate median filter
	Mat output = Mat::zeros(row, col, CV_8UC3);
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			//run all neighbor pixel
			int avg1 = 0;
			int avg2 = 0;
			int avg3 = 0;
			for (int k = -1; k <= 1 ; k++) {
				for (int l = -1; l <= 1; l++) {
					Vec3b index = calculateMatrix.at<Vec3b>(i + 1 + k, j + 1 + l);
					avg1 += index[0];
					avg2 += index[1];
					avg3 += index[2];
				}
			}
			output.at<Vec3b>(i, j)[0] = avg1 / 9;
			output.at<Vec3b>(i, j)[1] = avg2 / 9;
			output.at<Vec3b>(i, j)[2] = avg3 / 9;
		}
	}
	return output;
}

Mat MeanFilter7X7(Mat img) {
	int row = img.rows;
	int col = img.cols;

	int o_row = row + 6;
	int o_col = col + 6;

	Mat calculateMatrix(o_row, o_col, CV_8UC3, Scalar(0));
	Mat x(row, col, CV_8UC1, Scalar(0));

	//re-construct matrix 
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			calculateMatrix.at<Vec3b>(i + 3, j + 3) = img.at<Vec3b>(i, j);
		}
	}

	//calculate median filter
	Mat output = Mat::zeros(row, col, CV_8UC3);
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			//run all neighbor pixel
			int avg1 = 0;
			int avg2 = 0;
			int avg3 = 0;
			for (int k = -3; k <= 3; k++) {
				for (int l = -3; l <= 3; l++) {
					Vec3b index = calculateMatrix.at<Vec3b>(i + 3 + k, j + 3 + l);
					avg1 += index[0];
					avg2 += index[1];
					avg3 += index[2];
				}
			}
			output.at<Vec3b>(i, j)[0] = avg1 / 49;
			output.at<Vec3b>(i, j)[1] = avg2 / 49;
			output.at<Vec3b>(i, j)[2] = avg3 / 49;
		}
	}
	return output;
}

Mat MedianFilter3X3(Mat img) {
	int row = img.rows;
	int col = img.cols;

	int o_row = row + 2;
	int o_col = col + 2;
	Mat calculateMatrix(o_row, o_col, CV_8UC1, Scalar(0));
	//re-construct matrix 
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			calculateMatrix.at<uchar>(i + 1, j + 1) = img.at<uchar>(i, j);
		}
	}
	printf("1\n");
	//calculate median filter
	Mat output = Mat::zeros(row, col, CV_8UC1);
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			//run all neighbor pixel
			int counter = 0;
			std::vector<uchar> list(9);
			for (int k = -1; k <= 1; k++) {
				for (int l = -1; l <= 1; l++) {
					list.at(counter) = calculateMatrix.at<uchar>(i + 1 + k, j + 1 + l);
					counter++;
				}
			}
			std::sort(list.begin(), list.end());
			output.at<uchar>(i, j) = list.at(5);
		}
	}
	printf("2\n");
	return output;
}

Mat MedianFilter7X7(Mat img) {
	int row = img.rows;
	int col = img.cols;

	int o_row = row + 6;
	int o_col = col + 6;

	Mat calculateMatrix(o_row, o_col, CV_8UC1, Scalar(0));

	//re-construct matrix 
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			calculateMatrix.at<uchar>(i + 3, j + 3) = img.at<uchar>(i, j);
		}
	}

	//calculate median filter
	Mat output = Mat::zeros(row, col, CV_8UC1);
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			//run all neighbor pixel
			int counter = 0;
			std::vector<uchar> list(49);
			for (int k = -3; k <= 3; k++) {
				for (int l = -3; l <= 3; l++) {
					list.at(counter) = calculateMatrix.at<uchar>(i + 3 + k, j + 3 + l);
					counter++;
				}
			}
			std::sort(list.begin(), list.end());
			output.at<uchar>(i, j) = list.at(25);
		}
	}
	return output;
}

Mat GaussianFilter5X5(Mat img) {
	int row = img.rows;
	int col = img.cols;

	//create 5X5 gaussian filter
	double GaussianFilter[5][5];
	double sigma = 0.707;
	for (int i = -2; i <= 2; i++) {
		for (int j = -2; j <= 2; j++) {
			GaussianFilter[i+2][j+2] = exp(((-1.0) * (i * i + j * j)) / (2 * sigma * sigma)) / (2 * PI * sigma * sigma);
		}
	}
	int o_row = img.rows + 4;
	int o_col = img.cols + 4;
	Mat calculateMatrix(o_row, o_col, CV_8UC3, Scalar(0));
	//re-construct matrix 
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			calculateMatrix.at<Vec3b>(i + 2, j + 2) = img.at<Vec3b>(i, j);
		}
	}
	//Calculate Gaussian Filter
	Mat output(row, col, CV_8UC3, Scalar(0));
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			//run all neighbor pixel
			double avg1 = 0;
			double avg2 = 0;
			double avg3 = 0;
			double sum = 0;
			for (int k = -2; k <= 2; k++) {
				for (int l = -2; l <= 2; l++) {
					Vec3b index = calculateMatrix.at<Vec3b>(i + 2 + k, j + 2 + l);
					avg1 += index[0] * GaussianFilter[k + 2][l + 2];
					avg2 += index[1] * GaussianFilter[k + 2][l + 2];
					avg3 += index[2] * GaussianFilter[k + 2][l + 2];
					sum += GaussianFilter[k + 2][l + 2];
				}
			}
			output.at<Vec3b>(i, j)[0] = avg1 / sum;
			output.at<Vec3b>(i, j)[1] = avg2 / sum;
			output.at<Vec3b>(i, j)[2] = avg3 / sum;
		}
	}
	return output;
}

void Pic1() {
	Mat img = imread("source_image/House256_noise.png");
	Mat mean33[7];
	mean33[0] = MeanFilter3X3(img);
	imwrite("output/House256_noise/Mean3X3/" + to_string(1) + ".png", mean33[0]);
	Mat mean77[7];
	mean77[0] = MeanFilter7X7(img);
	imwrite("output/House256_noise/Mean7X7/" + to_string(1) + ".png", mean77[0]);

	Mat median33[7];
	Mat g3 = ConvertToGray(img);
	median33[0] = MedianFilter3X3(g3);
	imwrite("output/House256_noise/Median3X3/" + to_string(1) + ".png", median33[0]);
	Mat median77[7];
	Mat g7 = ConvertToGray(img);
	median77[0] = MedianFilter7X7(g7);
	imwrite("output/House256_noise/Median7X7/" + to_string(1) + ".png", median77[0]);

	Mat gaussian55[7];
	gaussian55[0] = GaussianFilter5X5(img);
	imwrite("output/House256_noise/Gaussian5X5/" + to_string(1) + ".png", gaussian55[0]);

	for (int i = 1; i < 7; i++) {
		mean33[i] = MeanFilter3X3(mean33[i-1]);
		imwrite("output/House256_noise/Mean3X3/" + to_string(i+1) + ".png", mean33[i]);
		mean77[i] = MeanFilter7X7(mean77[i-1]);
		imwrite("output/House256_noise/Mean7X7/" + to_string(i+1) + ".png", mean77[i]);
		median33[i] = MedianFilter3X3(median33[i-1]);
		imwrite("output/House256_noise/Median3X3/" + to_string(i+1) + ".png", median33[i]);
		median77[i] = MedianFilter7X7(median77[i-1]);
		imwrite("output/House256_noise/Median7X7/" + to_string(i+1) + ".png", median77[i]);
		gaussian55[i] = GaussianFilter5X5(gaussian55[i-1]);
		imwrite("output/House256_noise/Gaussian5X5/" + to_string(i+1) + ".png", gaussian55[i]);
	}
}

void Pic2() {
	Mat img = imread("source_image/Lena_gray.png");
	Mat mean33[7];
	mean33[0] = MeanFilter3X3(img);
	imwrite("output/Lena_gray/Mean3X3/" + to_string(1) + ".png", mean33[0]);
	Mat mean77[7];
	mean77[0] = MeanFilter7X7(img);
	imwrite("output/Lena_gray/Mean7X7/" + to_string(1) + ".png", mean77[0]);

	Mat median33[7];
	Mat g3 = ConvertToGray(img);
	median33[0] = MedianFilter3X3(g3);
	imwrite("output/Lena_gray/Median3X3/" + to_string(1) + ".png", median33[0]);
	Mat median77[7];
	Mat g7 = ConvertToGray(img);
	median77[0] = MedianFilter7X7(g7);
	imwrite("output/Lena_gray/Median7X7/" + to_string(1) + ".png", median77[0]);

	Mat gaussian55[7];
	gaussian55[0] = GaussianFilter5X5(img);
	imwrite("output/Lena_gray/Gaussian5X5/" + to_string(1) + ".png", gaussian55[0]);

	for (int i = 1; i < 7; i++) {
		mean33[i] = MeanFilter3X3(mean33[i - 1]);
		imwrite("output/Lena_gray/Mean3X3/" + to_string(i + 1) + ".png", mean33[i]);
		mean77[i] = MeanFilter7X7(mean77[i - 1]);
		imwrite("output/Lena_gray/Mean7X7/" + to_string(i + 1) + ".png", mean77[i]);
		median33[i] = MedianFilter3X3(median33[i - 1]);
		imwrite("output/Lena_gray/Median3X3/" + to_string(i + 1) + ".png", median33[i]);
		median77[i] = MedianFilter7X7(median77[i - 1]);
		imwrite("output/Lena_gray/Median7X7/" + to_string(i + 1) + ".png", median77[i]);
		gaussian55[i] = GaussianFilter5X5(gaussian55[i - 1]);
		imwrite("output/Lena_gray/Gaussian5X5/" + to_string(i + 1) + ".png", gaussian55[i]);
	}
}

void Pic3() {
	Mat img = imread("source_image/Mandrill_gray.png");
	Mat mean33[7];
	mean33[0] = MeanFilter3X3(img);
	imwrite("output/Mandrill_gray/Mean3X3/" + to_string(1) + ".png", mean33[0]);
	Mat mean77[7];
	mean77[0] = MeanFilter7X7(img);
	imwrite("output/Mandrill_gray/Mean7X7/" + to_string(1) + ".png", mean77[0]);

	Mat median33[7];
	Mat g3 = ConvertToGray(img);
	median33[0] = MedianFilter3X3(g3);
	imwrite("output/Mandrill_gray/Median3X3/" + to_string(1) + ".png", median33[0]);
	Mat median77[7];
	Mat g7 = ConvertToGray(img);
	median77[0] = MedianFilter7X7(g7);
	imwrite("output/Mandrill_gray/Median7X7/" + to_string(1) + ".png", median77[0]);

	Mat gaussian55[7];
	gaussian55[0] = GaussianFilter5X5(img);
	imwrite("output/Mandrill_gray/Gaussian5X5/" + to_string(1) + ".png", gaussian55[0]);

	for (int i = 1; i < 7; i++) {
		mean33[i] = MeanFilter3X3(mean33[i - 1]);
		imwrite("output/Mandrill_gray/Mean3X3/" + to_string(i + 1) + ".png", mean33[i]);
		mean77[i] = MeanFilter7X7(mean77[i - 1]);
		imwrite("output/Mandrill_gray/Mean7X7/" + to_string(i + 1) + ".png", mean77[i]);
		median33[i] = MedianFilter3X3(median33[i - 1]);
		imwrite("output/Mandrill_gray/Median3X3/" + to_string(i + 1) + ".png", median33[i]);
		median77[i] = MedianFilter7X7(median77[i - 1]);
		imwrite("output/Mandrill_gray/Median7X7/" + to_string(i + 1) + ".png", median77[i]);
		gaussian55[i] = GaussianFilter5X5(gaussian55[i - 1]);
		imwrite("output/Mandrill_gray/Gaussian5X5/" + to_string(i + 1) + ".png", gaussian55[i]);
	}
}

void Pic4() {
	Mat img = imread("source_image/Peppers_noise.png");
	Mat mean33[7];
	mean33[0] = MeanFilter3X3(img);
	imwrite("output/Peppers_noise/Mean3X3/" + to_string(1) + ".png", mean33[0]);
	Mat mean77[7];
	mean77[0] = MeanFilter7X7(img);
	imwrite("output/Peppers_noise/Mean7X7/" + to_string(1) + ".png", mean77[0]);

	Mat median33[7];
	Mat g3 = ConvertToGray(img);
	median33[0] = MedianFilter3X3(g3);
	imwrite("output/Peppers_noise/Median3X3/" + to_string(1) + ".png", median33[0]);
	Mat median77[7];
	Mat g7 = ConvertToGray(img);
	median77[0] = MedianFilter7X7(g7);
	imwrite("output/Peppers_noise/Median7X7/" + to_string(1) + ".png", median77[0]);

	Mat gaussian55[7];
	gaussian55[0] = GaussianFilter5X5(img);
	imwrite("output/Peppers_noise/Gaussian5X5/" + to_string(1) + ".png", gaussian55[0]);

	for (int i = 1; i < 7; i++) {
		mean33[i] = MeanFilter3X3(mean33[i - 1]);
		imwrite("output/Peppers_noise/Mean3X3/" + to_string(i + 1) + ".png", mean33[i]);
		mean77[i] = MeanFilter7X7(mean77[i - 1]);
		imwrite("output/Peppers_noise/Mean7X7/" + to_string(i + 1) + ".png", mean77[i]);
		median33[i] = MedianFilter3X3(median33[i - 1]);
		imwrite("output/Peppers_noise/Median3X3/" + to_string(i + 1) + ".png", median33[i]);
		median77[i] = MedianFilter7X7(median77[i - 1]);
		imwrite("output/Peppers_noise/Median7X7/" + to_string(i + 1) + ".png", median77[i]);
		gaussian55[i] = GaussianFilter5X5(gaussian55[i - 1]);
		imwrite("output/Peppers_noise/Gaussian5X5/" + to_string(i + 1) + ".png", gaussian55[i]);
	}
}

int main() {
	Pic1();
	Pic2();
	Pic3();
	Pic4();
}