#pragma once
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;


Mat sharpen(Mat src);
Mat colour_segmentation(Mat src);
Mat iterative_threshold(Mat src);

void erode_and_dilate(Mat& src);
//void dilate_and_erode(Mat& src);
