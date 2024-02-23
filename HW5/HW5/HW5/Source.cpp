#include "iostream"
#include "vector"
#include "cmath"
#include "algorithm"
#include "string"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

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

void SobelEdgeDetection(Mat img, string str) {
	int row = img.rows;
	int col = img.cols;

	Mat gray = ConvertToGray(img);
	Mat Sobel_Vertical(row, col, CV_8UC1);
	Mat Sobel_Horizontal(row, col, CV_8UC1);
	Mat Sobel(row, col, CV_8UC1);

	int kernel_vertical[3][3] = {
		{-1, -2, -1},
		{ 0,  0,  0},
		{-1,  2,  1}
	};
	int kernel_Horizontal[3][3] = {
		{-1,  0,  1},
		{-2,  0,  2},
		{-1,  0,  1}
	};
	//calculate
	for (int i = 1; i < row - 1; i++) {
		for (int j = 1; j < col - 1; j++) {
			int sum = 0;
			int sum2 = 0;
			for (int k = -1; k <= 1; k++) {
				for (int l = -1; l <= 1; l++) {
					sum += kernel_vertical[k + 1][l + 1] * gray.at<uchar>(i + k, j + l);
					sum2 += kernel_Horizontal[k + 1][l + 1] * gray.at<uchar>(i + k, j + l);
				}
			}
			Sobel_Vertical.at<uchar>(i, j) = saturate_cast<uchar>(sum);
			Sobel_Horizontal.at<uchar>(i, j) = saturate_cast<uchar>(sum2);
		}
	}
	add(abs(Sobel_Vertical), abs(Sobel_Horizontal), Sobel);
	imwrite("output/" + str + "/SobelEdgeDetection/" + str + "_SobelVertical.png", Sobel_Vertical);
	imwrite("output/" + str + "/SobelEdgeDetection/" + str + "_SobelHorizontal.png", Sobel_Horizontal);
	imwrite("output/" + str + "/SobelEdgeDetection/" + str + "_Sobel.png", Sobel);
}

void PrewittEdgeDetection(Mat img, string str) {
	int row = img.rows;
	int col = img.cols;

	Mat gray = ConvertToGray(img);
	int kernel_vertical[3][3] = {
		{-1, -1, -1},
		{ 0,  0,  0},
		{ 1,  1,  1}
	};
	int kernel_Horizontal[3][3] = {
		{-1,  0,  1},
		{-1,  0,  1},
		{-1,  0,  1}
	};

	Mat Prewitt_Vertical(row, col, CV_8UC1);
	Mat Prewitt_Horizontal(row, col, CV_8UC1);
	Mat Prewitt(row, col, CV_8UC1);
	//calculate
	for (int i = 1; i < row - 1; i++) {
		for (int j = 1; j < col - 1; j++) {
			int sum = 0;
			int sum2 = 0;
			for (int k = -1; k <= 1; k++) {
				for (int l = -1; l <= 1; l++) {
					sum += kernel_vertical[k + 1][l + 1] * gray.at<uchar>(i + k, j + l);
					sum2 += kernel_Horizontal[k + 1][l + 1] * gray.at<uchar>(i + k, j + l);
				}
			}
			Prewitt_Vertical.at<uchar>(i, j) = saturate_cast<uchar>(sum);
			Prewitt_Horizontal.at<uchar>(i, j) = saturate_cast<uchar>(sum2);
		}
	}

	add(abs(Prewitt_Vertical), abs(Prewitt_Horizontal), Prewitt);
	imwrite("output/" + str + "/PrewittEdgeDetection/" + str + "_PrewittVertical.png", Prewitt_Vertical);
	imwrite("output/" + str + "/PrewittEdgeDetection/" + str + "_PrewittHorizontal.png", Prewitt_Horizontal);
	imwrite("output/" + str + "/PrewittEdgeDetection/" + str + "_Prewitt.png", Prewitt);
}

void LaplacianEdgeDetection(Mat img, string str) {
	int row = img.rows;
	int col = img.cols;

	Mat gray = ConvertToGray(img);

	int kernel[3][3] = {
		{0,  1,  0},
		{1, -4,  1},
		{0,  1,  0}
	};
	int kernel2[3][3] = {
		{1,  1,  1},
		{1, -8,  1},
		{1,  1,  1}
	};

	Mat laplacian4(row, col, CV_8UC1);
	Mat laplacian8(row, col, CV_8UC1);
	//calculate
	for (int i = 1; i < row - 1; i++) {
		for (int j = 1; j < col - 1; j++) {
			int sum = 0;
			int sum2 = 0;
			for (int k = -1; k <= 1; k++) {
				for (int l = -1; l <= 1; l++) {
					sum += kernel[k + 1][l + 1] * gray.at<uchar>(i + k, j + l);
					sum2 += kernel2[k + 1][l + 1] * gray.at<uchar>(i + k, j + l);
				}
			}
			laplacian4.at<uchar>(i, j) = saturate_cast<uchar>(sum);
			laplacian8.at<uchar>(i, j) = saturate_cast<uchar>(sum2);
		}
	}
	imwrite("output/" + str + "/LaplacianEdgeDetection/" + str + "_Laplacian4.png", laplacian4);
	imwrite("output/" + str + "/LaplacianEdgeDetection/" + str + "_Laplacian8.png", laplacian8);
}

void House512() {
	Mat img = imread("source_image/House512.png");
	string name = "House512";
	SobelEdgeDetection(img, name);
	PrewittEdgeDetection(img, name);
	LaplacianEdgeDetection(img, name);
}

void Lena() {
	Mat img = imread("source_image/Lena.png");
	string name = "Lena";
	SobelEdgeDetection(img, name);
	PrewittEdgeDetection(img, name);
	LaplacianEdgeDetection(img, name);
}

void Mandrill() {
	Mat img = imread("source_image/Mandrill.png");
	string name = "Mandrill";
	SobelEdgeDetection(img, name);
	PrewittEdgeDetection(img, name);
	LaplacianEdgeDetection(img, name);
}

int main() {
	House512();
	Lena();
	Mandrill();
}