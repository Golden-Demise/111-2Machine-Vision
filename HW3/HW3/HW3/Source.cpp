#include "iostream"
#include "vector"
#include "cmath"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

struct QuadTreeNode {
    Rect rect;
    bool is_leaf;
    uchar value;
    int level=1;
    int maxLevel;
    QuadTreeNode* child[4];
};

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
                binary.at<uchar>(i, j) = 255;
            }
            else {
                binary.at<uchar>(i, j) = 0;
            }
        }
    }
    return binary;
}

int isAllPixelSame(const Mat& img, const Rect& rect) {
    int sum = 0;
    for (int i = rect.y; i < rect.y + rect.height; i++) {
        for (int j = rect.x; j < rect.x + rect.width; j++) {
            sum += img.at<uchar>(i, j);
        }
    }

    int avg = sum / (rect.width * rect.height);
    if (avg == 0) {
        return 0;
    }
    else if (avg == 255) {
        return 255;
    }
    else {
        return 128;
    }
}

void drawQuadTree(Mat& img, const QuadTreeNode* node) {
    if (node->value == 0) {
        rectangle(img, node->rect, Scalar(0, 0, 0), -1);
    }
    else if (node->value == 255) {
        rectangle(img, node->rect, Scalar(255, 255, 255), -1);
    }
    else {
        rectangle(img, node->rect, Scalar(128, 128, 128), -1);
    }
    if (!node->is_leaf) {
        for (int i = 0; i < 4; i++) {
            drawQuadTree(img, node->child[i]);
        }
    }  
}

QuadTreeNode* createQuadTree(const Mat& img, int level,int maxLevel, const Rect& rect) {
    QuadTreeNode* node = new QuadTreeNode();
    node->rect = rect;
    node->is_leaf = true;
    node->level = level;
    if (isAllPixelSame(img, rect) == 0) {
        node->value = 0;
    }
    else if (isAllPixelSame(img, rect) == 255) {
        node->value = 255;
    }
    else if(isAllPixelSame(img, rect) == 128) {
        node->value = 128;
    }
    int w = rect.width / 2;
    int h = rect.height / 2;
    if (level < maxLevel) {
        node->is_leaf = false;
        node->child[0] = createQuadTree(img, node->level + 1, maxLevel, Rect(rect.x, rect.y, w, h));
        node->child[1] = createQuadTree(img, node->level + 1, maxLevel, Rect(rect.x + w, rect.y, w, h));
        node->child[2] = createQuadTree(img, node->level + 1, maxLevel, Rect(rect.x, rect.y + h, w, h));
        node->child[3] = createQuadTree(img, node->level + 1, maxLevel, Rect(rect.x + w, rect.y + h, w, h));
    }
    return node;
}

void pic1() {
    Mat im = imread("source image/1.png");
    Mat img = ConvertToBinary(im, 135);
    for (int i = 2; i <= int(log2(img.cols)) + 1; i++) {
        QuadTreeNode* root = createQuadTree(img, 1, i, Rect(0, 0, img.cols, img.rows));
        Mat img_tree(img.size(), CV_8UC3);
        drawQuadTree(img_tree, root);
        imwrite("output/pic1/pic1_Layer_" + to_string(i - 1) + ".png", img_tree);
        delete root;
    }
}

void pic2() {
    Mat im = imread("source image/2.png");
    Mat img = ConvertToBinary(im, 245);
    for (int i = 2; i <= int(log2(img.cols)) + 1; i++) {
        QuadTreeNode* root = createQuadTree(img, 1, i, Rect(0, 0, img.cols, img.rows));
        Mat img_tree(img.size(), CV_8UC3);
        drawQuadTree(img_tree, root);
        imwrite("output/pic2/pic2_Layer_" + to_string(i - 1) + ".png", img_tree);
        delete root;
    }
}

void pic3() {
    Mat im = imread("source image/3.png");
    Mat img = ConvertToBinary(im, 155);
    for (int i = 2; i <= int(log2(img.cols)) + 1;i++) {
        QuadTreeNode* root = createQuadTree(img, 1, i, Rect(0, 0, img.cols, img.rows));
        Mat img_tree(img.size(), CV_8UC3);
        drawQuadTree(img_tree, root);
        imwrite("output/pic3/pic3_Layer_"+to_string(i-1)+".png", img_tree);
        delete root;
    }
}

void pic4() {
    Mat im = imread("source image/4.png");
    Mat img = ConvertToBinary(im, 254);
    for (int i = 2; i <= int(log2(img.cols)) + 1; i++) {
        QuadTreeNode* root = createQuadTree(img, 1, i, Rect(0, 0, img.cols, img.rows));
        Mat img_tree(img.size(), CV_8UC3);
        drawQuadTree(img_tree, root);
        imwrite("output/pic4/pic4_Layer_" + to_string(i - 1) + ".png", img_tree);
        delete root;
    }
}

int main() {
    pic1();
    pic2();
    pic3();
    pic4();
    return 0;
}