#include "Header.h"

// Folosim mai multe functii pentru a imbunatati calitatea imaginii
Mat sharpen(Mat src) {
	Mat sharpened_img = src.clone();
	int h = src.rows;
	int w = src.cols;

	//marim sharpnessul cu fct predefinite din openCV , definim un kernel de sharpness si facem convolutie pe imagine
	// masca de sharpness
	float mask[9] =
	{ 0, -1, 0,
	 -1, 5, -1,
	  0, -1, 0 };
	Mat mat_mask = Mat(3, 3, CV_32F, mask);
	// facem convolutie
	filter2D(sharpened_img, sharpened_img, -1, mat_mask);

	//marim contrastul folosing limitele a si b (pentru care se cunosc valorile sa = Transformata(a) si sb)
	double a = 125, b = 175, sa = 100, sb = 200;
	double m = (double)sa / a;
	double n = (double)(sb - sa) / (b - a);
	double p = (double)(255 - sb) / (255 - b);

	double aux;
	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++) {
			if (src.at<uchar>(y, x) <= a)
				aux = m * src.at<uchar>(y, x);
			else if (src.at<uchar>(y, x) > a && src.at<uchar>(y, x) <= b)
				aux = n * (src.at<uchar>(y, x) - a) + sa;
			else
				aux = p * (src.at<uchar>(y, x) - b) + sb;

			if (aux > 255)
				sharpened_img.at<uchar>(y, x) = 255;
			else if (aux < 0)
				sharpened_img.at<uchar>(y, x) = 0;
			else
				sharpened_img.at<uchar>(y, x) = aux;
		}
	return sharpened_img;
}

Mat iterative_threshold(Mat src) {
	float range[] = { 0, 256 };
	const float* histRange = { range };
	int histSize = 256;
	Mat hist, binary;
	calcHist(&src, 1, 0, Mat(), hist, 1, &histSize, &histRange);

	int T = 127;
	int T_old = 0;
	do {
		int mu1, mu2;
		int sum = 0;
		for (int i = 0; i <= T; i++) {
			sum += hist.data[i];
		}

		int sum_temp = 0;
		for (mu1 = 0; mu1 <= T; mu1++) {
			sum_temp += hist.data[mu1];
			if (sum_temp >= sum / 2) {
				break;
			}
		}

		sum = 0;
		for (int i = T + 1; i < 256; i++) {
			sum += hist.data[i];
		}
		sum_temp = 0;
		for (mu2 = T + 1; mu2 < 256; mu2++) {
			sum_temp += hist.data[mu2];
			if (sum_temp >= sum / 2) {
				break;
			}
		}
		T_old = T;
		T = (mu1 + mu2) / 2;
	} while (T_old != T);

	threshold(src, binary, T, 255, THRESH_BINARY);
	return binary;

}

void erode_and_dilate(Mat& src) {
	Mat structuring_elem = getStructuringElement(MORPH_ELLIPSE, Size(2, 2));
	erode(src, src, structuring_elem);
	dilate(src, src, structuring_elem);
}

//void dilate_and_erode(Mat& src) {
//	Mat structuring_elem = getStructuringElement(MORPH_ELLIPSE, Size(2, 2));
//	dilate(src, src, structuring_elem);
//	erode(src, src, structuring_elem);
//}

Mat colour_segmentation(Mat src) {
	// Initializarea unei matrici negre cu aceasi dimensiune ca src
	Mat rez = Mat::zeros(src.size(), CV_8UC3);
	Mat labels;

	// nr_cmp stocheaza numarul de elemnte distincte din imaginea de baza (fiind deja o imagine binarizata)
	int nr_cmp = connectedComponents(src, labels, 8);


	return rez;
}
