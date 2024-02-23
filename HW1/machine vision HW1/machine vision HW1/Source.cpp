#include "opencv2/opencv.hpp"

using namespace cv;

Mat ConvertToGray(Mat img) {
    Mat gray= Mat::zeros(img.size(), CV_8UC1);
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            Vec3b rgb = img.at<Vec3b>(i, j);
            gray.at<uchar>(i, j) = 0.3 * rgb[2] + 0.59 * rgb[1] + 0.11 * rgb[0];
        }
    }
    return gray;
}

Mat ConvertToBinary(Mat grayimg) {
    int threshold = 128;
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

Mat ResizeTimesTwo(Mat img) {
    Mat newImg(img.cols*2,img.rows*2, CV_8UC3);
    for (int i = 0; i < newImg.cols; i+=2) {
        for (int j = 0; j < newImg.rows; j += 2) {
            newImg.at<Vec3b>(i, j) = img.at<Vec3b>(i / 2, j / 2);
            newImg.at<Vec3b>(i+1, j) = img.at<Vec3b>(i / 2, j / 2);
            newImg.at<Vec3b>(i, j+1) = img.at<Vec3b>(i / 2, j / 2);
            newImg.at<Vec3b>(i+1, j + 1) = img.at<Vec3b>(i / 2, j / 2);
        }
    }
    return newImg;
}

Mat ResizeTimesTwo2(Mat img) {
    Mat newImg(img.cols * 2, img.rows * 2, CV_8UC3);
    for (int i = 0; i < newImg.cols; i++) {
        for (int j = 0; j < newImg.rows; j++) {
            float x = ((float)j) / 2.0;
            float y = ((float)i) / 2.0;
            int x1 = (int)x;
            int y1 = (int)y;
            int x2 = x1 + 1;
            int y2 = y1 + 1;
            if (x2 >= img.cols) {
                x2 = img.cols - 1;
            }
            if (y2 >= img.rows) {
                y2 = img.rows - 1;
            }
            float a = x - x1;
            float b = y - y1;
            //4 point
            Vec3b pixel1 = img.at<Vec3b>(y1, x1);
            Vec3b pixel2 = img.at<Vec3b>(y1, x2);
            Vec3b pixel3 = img.at<Vec3b>(y2, x1);
            Vec3b pixel4 = img.at<Vec3b>(y2, x2);

            Vec3b pixel;
            pixel[0] = (1 - a) * (1 - b) * pixel1[0] + a * (1 - b) * pixel2[0] + (1 - a) * b * pixel3[0] + a * b * pixel4[0];
            pixel[1] = (1 - a) * (1 - b) * pixel1[1] + a * (1 - b) * pixel2[1] + (1 - a) * b * pixel3[1] + a * b * pixel4[1];
            pixel[2] = (1 - a) * (1 - b) * pixel1[2] + a * (1 - b) * pixel2[2] + (1 - a) * b * pixel3[2] + a * b * pixel4[2];
            newImg.at<Vec3b>(i, j) = pixel;
        }
    }
    return newImg;
}

Mat ResizeDevideTwo(Mat img) {
    Mat newImg(img.cols / 2, img.rows / 2, CV_8UC3);
    for (int i = 0; i < img.cols; i += 2) {
        for (int j = 0; j < img.rows; j += 2) {
            newImg.at<Vec3b>(i/2, j/2) = img.at<Vec3b>(i, j);
        }
    }
    return newImg;
}

Mat ResizeDevideTwo2(Mat img) {
    Mat newImg(img.cols / 2, img.rows / 2, CV_8UC3);
    for (int i = 0; i < img.cols; i += 2) {
        for (int j = 0; j < img.rows; j += 2) {
            newImg.at<Vec3b>(i / 2, j / 2)[0] = (img.at<Vec3b>(i, j)[0] + img.at<Vec3b>(i + 1, j)[0] + img.at<Vec3b>(i, j + 1)[0] + img.at<Vec3b>(i + 1, j + 1)[0]) / 4;
            newImg.at<Vec3b>(i / 2, j / 2)[1] = (img.at<Vec3b>(i, j)[1] + img.at<Vec3b>(i + 1, j)[1] + img.at<Vec3b>(i, j + 1)[1] + img.at<Vec3b>(i + 1, j + 1)[1]) / 4;
            newImg.at<Vec3b>(i / 2, j / 2)[2] = (img.at<Vec3b>(i, j)[2] + img.at<Vec3b>(i + 1, j)[2] + img.at<Vec3b>(i, j + 1)[2] + img.at<Vec3b>(i + 1, j + 1)[2]) / 4;
        }
    }
    return newImg;
}

Mat ConvertToIndexColorImage(Mat img){
    Mat ColorMap(1, 256, CV_8UC3);
    for (int i = 0; i < 256; i++) {
        uchar* pixel = ColorMap.ptr<uchar>(0,i);
        pixel[0] = i/3; //b
        pixel[1] = i; //g
        pixel[2] = 255-i;   //r
    }
    Mat gray = ConvertToGray(img);
    Mat indexImg = Mat::zeros(img.size(), CV_8UC3);;
    for (int i = 0; i < gray.rows; i++) {
        for (int j = 0; j < gray.cols; j++) {
            uchar index = gray.at<uchar>(i, j);
            uchar* c = ColorMap.ptr<uchar>(0, index);
            indexImg.at<Vec3b>(i, j)[0] = c[0];
            indexImg.at<Vec3b>(i, j)[1] = c[1];
            indexImg.at<Vec3b>(i, j)[2] = c[2];
        }
    }
    //make the color map more visible
    Mat ViualColorMap(16,16, CV_8UC3);
    int counti = 0;
    int countj = 0;
    for (int i = 0; i < 256; i++) {
        if (i % 16 == 0 && i!=0) {
            counti++;
        }
        ViualColorMap.at<Vec3b>(countj, counti) = ColorMap.at<Vec3b>(0, i);
        countj++;
        if (countj >= 16) {
            countj = 0;
        }
    }
    imwrite("output/Q1-3/ColorMap.png", ViualColorMap);
    return indexImg;
}

void Question1_1() {
    Mat img = imread("images/House256.png");
    Mat img1 = imread("images/House512.png");
    Mat img2 = imread("images/JellyBeans.png");
    Mat img3 = imread("images/Lena.png");
    Mat img4 = imread("images/Mandrill.png");
    Mat img5 = imread("images/Peppers.png");

    Mat gray = ConvertToGray(img);
    imwrite("output/Q1-1/House256_gray.png", gray);
    Mat gray1 = ConvertToGray(img1);
    imwrite("output/Q1-1/House512_gray.png", gray1);
    Mat gray2 = ConvertToGray(img2);
    imwrite("output/Q1-1/JellyBeans_gray.png", gray2);
    Mat gray3 = ConvertToGray(img3);
    imwrite("output/Q1-1/Lena_gray.png", gray3);
    Mat gray4 = ConvertToGray(img4);
    imwrite("output/Q1-1/Mandrill_gray.png", gray4);
    Mat gray5 = ConvertToGray(img5);
    imwrite("output/Q1-1/Peppers_gray.png", gray5);
}

void Question1_2() {
    Mat img = imread("images/House256.png");
    Mat img1 = imread("images/House512.png");
    Mat img2 = imread("images/JellyBeans.png");
    Mat img3 = imread("images/Lena.png");
    Mat img4 = imread("images/Mandrill.png");
    Mat img5 = imread("images/Peppers.png");

    Mat gray = ConvertToGray(img);
    Mat binary = ConvertToBinary(gray);
    imwrite("output/Q1-2/House256_Binary.png", binary);

    Mat gray1 = ConvertToGray(img1);
    Mat binary1 = ConvertToBinary(gray1);
    imwrite("output/Q1-2/House512_Binary.png", binary1);
    
    Mat gray2 = ConvertToGray(img2);
    Mat binary2 = ConvertToBinary(gray2);
    imwrite("output/Q1-2/JellyBeans_Binary.png", binary2);
    
    Mat gray3 = ConvertToGray(img3);
    Mat binary3 = ConvertToBinary(gray3);
    imwrite("output/Q1-2/Lena_Binary.png", binary3);
    
    Mat gray4 = ConvertToGray(img4);
    Mat binary4 = ConvertToBinary(gray4);
    imwrite("output/Q1-2/Mandrill_Binary.png", binary4);
    
    Mat gray5 = ConvertToGray(img5);
    Mat binary5 = ConvertToBinary(gray5);
    imwrite("output/Q1-2/Peppers_Binary.png", binary5);
}

void Question1_3() {
    Mat img = imread("images/House256.png");
    Mat img1 = imread("images/House512.png");
    Mat img2 = imread("images/JellyBeans.png");
    Mat img3 = imread("images/Lena.png");
    Mat img4 = imread("images/Mandrill.png");
    Mat img5 = imread("images/Peppers.png");

    Mat indexImg = ConvertToIndexColorImage(img);
    imwrite("output/Q1-3/House256_indexImg.png", indexImg);
    Mat indexImg1 = ConvertToIndexColorImage(img1);
    imwrite("output/Q1-3/House512_indexImg.png", indexImg1);
    Mat indexImg2 = ConvertToIndexColorImage(img2);
    imwrite("output/Q1-3/JellyBeans_indexImg.png", indexImg2);
    Mat indexImg3 = ConvertToIndexColorImage(img3);
    imwrite("output/Q1-3/Lena_indexImg.png", indexImg3);
    Mat indexImg4 = ConvertToIndexColorImage(img4);
    imwrite("output/Q1-3/Mandrill_indexImg.png", indexImg4);
    Mat indexImg5 = ConvertToIndexColorImage(img5);
    imwrite("output/Q1-3/Peppers_indexImg.png", indexImg5);
}

void Question2_1() {
    Mat img = imread("images/House256.png");
    Mat img1 = imread("images/House512.png");
    Mat img2 = imread("images/JellyBeans.png");
    Mat img3 = imread("images/Lena.png");
    Mat img4 = imread("images/Mandrill.png");
    Mat img5 = imread("images/Peppers.png");

    Mat img_big = ResizeTimesTwo(img);
    Mat img_small = ResizeDevideTwo(img);
    imwrite("output/Q2-1/House256_big.png", img_big);
    imwrite("output/Q2-1/House256_small.png", img_small);

    Mat img1_big = ResizeTimesTwo(img1);
    Mat img1_small = ResizeDevideTwo(img1);
    imwrite("output/Q2-1/House512_big.png", img1_big);
    imwrite("output/Q2-1/House512_small.png", img1_small);

    Mat img2_big = ResizeTimesTwo(img2);
    Mat img2_small = ResizeDevideTwo(img2);
    imwrite("output/Q2-1/JellyBeans_big.png", img2_big);
    imwrite("output/Q2-1/JellyBeans_small.png", img2_small);

    Mat img3_big = ResizeTimesTwo(img3);
    Mat img3_small = ResizeDevideTwo(img3);
    imwrite("output/Q2-1/Lena_big.png", img3_big);
    imwrite("output/Q2-1/Lena_small.png", img3_small);

    Mat img4_big = ResizeTimesTwo(img4);
    Mat img4_small = ResizeDevideTwo(img4);
    imwrite("output/Q2-1/Mandrill_big.png", img4_big);
    imwrite("output/Q2-1/Mandrill_small.png", img4_small);

    Mat img5_big = ResizeTimesTwo(img5);
    Mat img5_small = ResizeDevideTwo(img5);
    imwrite("output/Q2-1/Peppers_big.png", img5_big);
    imwrite("output/Q2-1/Peppers_small.png", img5_small);
}

void Question2_2() {
    Mat img = imread("images/House256.png");
    Mat img1 = imread("images/House512.png");
    Mat img2 = imread("images/JellyBeans.png");
    Mat img3 = imread("images/Lena.png");
    Mat img4 = imread("images/Mandrill.png");
    Mat img5 = imread("images/Peppers.png");

    Mat img_big = ResizeTimesTwo2(img);
    Mat img_small = ResizeDevideTwo2(img);
    imwrite("output/Q2-2/House256_big.png", img_big);
    imwrite("output/Q2-2/House256_small.png", img_small);

    Mat img1_big = ResizeTimesTwo2(img1);
    Mat img1_small = ResizeDevideTwo2(img1);
    imwrite("output/Q2-2/House512_big.png", img1_big);
    imwrite("output/Q2-2/House512_small.png", img1_small);

    Mat img2_big = ResizeTimesTwo2(img2);
    Mat img2_small = ResizeDevideTwo2(img2);
    imwrite("output/Q2-2/JellyBeans_big.png", img2_big);
    imwrite("output/Q2-2/JellyBeans_small.png", img2_small);

    Mat img3_big = ResizeTimesTwo2(img3);
    Mat img3_small = ResizeDevideTwo2(img3);
    imwrite("output/Q2-2/Lena_big.png", img3_big);
    imwrite("output/Q2-2/Lena_small.png", img3_small);

    Mat img4_big = ResizeTimesTwo2(img4);
    Mat img4_small = ResizeDevideTwo2(img4);
    imwrite("output/Q2-2/Mandrill_big.png", img4_big);
    imwrite("output/Q2-2/Mandrill_small.png", img4_small);

    Mat img5_big = ResizeTimesTwo2(img5);
    Mat img5_small = ResizeDevideTwo2(img5);
    imwrite("output/Q2-2/Peppers_big.png", img5_big);
    imwrite("output/Q2-2/Peppers_small.png", img5_small);
}

int main() {
    Question1_1();
    Question1_2();
    Question1_3();
    Question2_1();
    Question2_2();
    return 0;
}
