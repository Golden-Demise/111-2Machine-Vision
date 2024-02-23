#include "opencv2/opencv.hpp"
#include <queue>
#include <stack>

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

Mat ConvertToBinary(Mat img, int threshold) {
    Mat grayimg = ConvertToGray(img);
    Mat binary = Mat::zeros(grayimg.size(), CV_8UC1);
    for (int i = 0; i < grayimg.rows; i++) {
        for (int j = 0; j < grayimg.cols; j++) {
            uchar n = grayimg.at<uchar>(i, j);
            if (n > threshold) {
                binary.at<uchar>(i, j) = 0;
            }
            else {
                binary.at<uchar>(i, j) = 1;
            }
        }
    }
    return binary;
}

Mat BinaryReConstruct(Mat img) {
    Mat binary = Mat::zeros(img.size(), CV_8UC1);
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            uchar n = img.at<uchar>(i, j);
            if (n == 1) {
                binary.at<uchar>(i, j) = 255;
            }
            else {
                binary.at<uchar>(i, j) = 0;
            }
        }
    }
    return binary;
}

Mat FourlabelConnect(Mat img, int threshold) {
    Mat binary = ConvertToBinary(img, threshold);
    Mat label=Mat::zeros(binary.size(),CV_32SC1);
    binary.convertTo(label, CV_32SC1);
    int counter = 1;
    int propCount = 0;
    int temp = 1;
    for (int i = 1; i < binary.rows - 2; i++) {
        int* data = label.ptr<int>(i);
        for (int j = 1; j < binary.cols - 2; j++) {
            if (data[j] == 1) {
                std::stack<std::pair<int, int>> neighbor;
                neighbor.push(std::pair<int, int>(i, j));
                ++counter;
                while (!neighbor.empty()) {
                    std::pair<int, int> cur = neighbor.top();
                    int curX = cur.first;
                    int curY = cur.second;
                    label.at<int>(curX, curY) = counter;
                    
                    neighbor.pop();

                    if (curY != 0) {
                        if (label.at<int>(curX, curY - 1) == 1) {
                            neighbor.push(std::pair<int, int>(curX, curY - 1));
                            temp++;
                        }
                    }
                    if (curY != binary.cols - 1) {
                        if (label.at<int>(curX, curY + 1) == 1) {
                            neighbor.push(std::pair<int, int>(curX, curY + 1));
                            temp++;
                        }
                    }
                    if(curX!=0){
                        if (label.at<int>(curX-1, curY) == 1) {;
                            neighbor.push(std::pair<int, int>(curX - 1, curY));
                            temp++;
                        }
                    }
                    if (curX != binary.rows - 1) {
                        if (label.at<int>(curX+1, curY) == 1) {
                            neighbor.push(std::pair<int, int>(curX + 1, curY));
                            temp++;
                        }
                    }
                }
                if (temp >= 100) {
                    propCount++;
                }
                temp = 0;
            }
        }
    }

    Mat colorLabel;
    std::vector<Vec3b> colors;
    for (int i = 0; i < label.rows * label.cols; i++) {
        colors.push_back(Vec3b(rand() % 256, rand() % 256, rand() % 256));
    }
    colorLabel = Mat::zeros(label.size(), CV_8UC3);
    for (int i = 0; i < colorLabel.rows; i++) {
        for (int j = 0; j < colorLabel.cols; j++) {
            int labelValue = label.at<int>(i, j);
            if (labelValue > 0) {
                colorLabel.at<Vec3b>(i, j) = colors[labelValue - 1];
            }
        }
    }
    int num = counter - 1;
    printf("count：%d\n", propCount);
    return colorLabel;
}

Mat EightlabelConnect(Mat img, int threshold) {
    Mat binary = ConvertToBinary(img, threshold);
    Mat label = Mat::zeros(binary.size(), CV_32SC1);
    binary.convertTo(label, CV_32SC1);
    int counter = 1;
    int propCount = 0;
    int temp=1;
    for (int i = 1; i < binary.rows - 2; i++) {
        int* data = label.ptr<int>(i);
        for (int j = 1; j < binary.cols - 2; j++) {
            if (data[j] == 1) {
                std::stack<std::pair<int, int>> neighbor;
                neighbor.push(std::pair<int, int>(i, j));
                ++counter;
                while (!neighbor.empty()) {
                    std::pair<int, int> cur = neighbor.top();
                    int curX = cur.first;
                    int curY = cur.second;
                    label.at<int>(curX, curY) = counter;
                    neighbor.pop();
                    for (int x = -1; x <= 1; x++) {
                        for (int y = -1; y <= 1; y++) {
                            if (x == 0 && y == 0) {
                                continue;
                            }
                            int x2 = curX + x;
                            int y2 = curY + y;
                            if (x2 >= 0 && x2 < binary.rows && y2 >= 0 && y2 < binary.cols) {
                                if (label.at<int>(x2, y2) == 1) {
                                    neighbor.push(std::pair<int, int>(x2, y2));
                                    temp++;
                                }
                            }
                        }
                    }
                }
                if (temp >= 100) {
                    propCount++;
                }
                temp = 0;
            }
        }
    }

    Mat colorLabel;
    std::vector<Vec3b> colors;
    for (int i = 0; i < label.rows * label.cols; i++) {
        colors.push_back(Vec3b(rand() % 256, rand() % 256, rand() % 256));
    }
    colorLabel = Mat::zeros(label.size(), CV_8UC3);
    for (int i = 0; i < colorLabel.rows; i++) {
        for (int j = 0; j < colorLabel.cols; j++) {
            int labelValue = label.at<int>(i, j);
            if (labelValue > 0) {
                colorLabel.at<Vec3b>(i, j) = colors[labelValue - 1];
            }
        }
    }
    int num = counter - 1;
    printf("count：%d\n", propCount);
    return colorLabel;
}

void ImageOne() {
    printf("1.\n");
    Mat img = imread("source image/1.png");
    Mat Binary = ConvertToBinary(img,100);
    Binary = BinaryReConstruct(Binary);
    Mat Four = FourlabelConnect(img, 100);
    Mat Eight = EightlabelConnect(img, 100);

    imwrite("output/1/1_Binary.png", Binary);
    imwrite("output/1/1_4Connect.png", Four);
    imwrite("output/1/1_8Connect.png", Eight);
}

void ImageTwo() {
    printf("2.\n");
    Mat img = imread("source image/2.png");
    Mat Binary = ConvertToBinary(img, 232);
    Binary = BinaryReConstruct(Binary);
    Mat Four = FourlabelConnect(img, 232);
    Mat Eight = EightlabelConnect(img, 232);

    imwrite("output/2/2_Binary.png", Binary);
    imwrite("output/2/2_4Connect.png", Four);
    imwrite("output/2/2_8Connect.png", Eight);
}

void ImageThree() {
    printf("3.\n");
    Mat img = imread("source image/3.png");
    Mat Binary = ConvertToBinary(img, 90);
    Binary = BinaryReConstruct(Binary);
    Mat Four = FourlabelConnect(img, 90);
    Mat Eight = EightlabelConnect(img, 90);

    imwrite("output/3/3_Binary.png", Binary);
    imwrite("output/3/3_4Connect.png", Four);
    imwrite("output/3/3_8Connect.png", Eight);
}

void ImageFour() {
    printf("4.\n");
    Mat img = imread("source image/4.png");
    Mat Binary = ConvertToBinary(img, 230);
    Binary = BinaryReConstruct(Binary);
    Mat Four = FourlabelConnect(img, 230);
    Mat Eight = EightlabelConnect(img, 230);

    imwrite("output/4/4_Binary.png", Binary);
    imwrite("output/4/4_4Connect.png", Four);
    imwrite("output/4/4_8Connect.png", Eight);
}

int main() {
    ImageOne();
    ImageTwo();
    ImageThree();
    ImageFour();
    return 0;
}
