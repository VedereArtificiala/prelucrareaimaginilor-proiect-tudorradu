#include "Header.h"

// Folosim mai multe functii pentru a imbunatati calitatea imaginii
Mat enhance(Mat input_image) {
	Mat sharpened_img = input_image.clone();
	int h = input_image.rows;
	int w = input_image.cols;

	//marim sharpnessul cu functii din openCV , definim o masca de sharpness si facem convolutie pe imagine
	// masca de sharpness
	float mask[9] =
	{ 0, -1, 0,
	 -1, 5, -1,
	  0, -1, 0 };
	Mat mat_mask = Mat(3, 3, CV_32F, mask);
	// facem convolutie
	filter2D(sharpened_img, sharpened_img, -1, mat_mask);

	//marim contrastul folosing limitele A si B
	double A = 125, B = 175, SA = 100, SB = 200;
	double M = (double)SA / A;
	double N = (double)(SB - SA) / (B - A);
	double P = (double)(255 - SB) / (255 - B);

	double aux;
	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++) {
			if (input_image.at<uchar>(y, x) <= A)
				aux = M * input_image.at<uchar>(y, x);
			else if (input_image.at<uchar>(y, x) > A && input_image.at<uchar>(y, x) <= B)
				aux = N * (input_image.at<uchar>(y, x) - A) + SA;
			else
				aux = P * (input_image.at<uchar>(y, x) - B) + SB;

			if (aux > 255)
				sharpened_img.at<uchar>(y, x) = 255;
			else if (aux < 0)
				sharpened_img.at<uchar>(y, x) = 0;
			else
				sharpened_img.at<uchar>(y, x) = aux;
		}
	return sharpened_img;
}

Mat iterative_threshold(Mat input_image) {
	float range[] = { 0, 256 };
	const float* histRange = { range };
	int histSize = 256;
	Mat hist, output_image;
	calcHist(&input_image, 1, 0, Mat(), hist, 1, &histSize, &histRange);

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

	threshold(input_image, output_image, T, 255, THRESH_BINARY);
	return output_image;

}

Mat edge_detection(Mat input_image) {
	Mat edges;
	Canny(input_image, edges, 50, 150);
	return edges;
}



void erode_and_dilate(Mat& input_image) {
	Mat structuring_elem = getStructuringElement(MORPH_ELLIPSE, Size(2, 2));
	erode(input_image, input_image, structuring_elem);
	dilate(input_image, input_image, structuring_elem);
}

Mat colour_segmentation(Mat input_image) {
	// Initializarea unei matrici negre cu aceasi dimensiune ca input_image
	Mat output_image = Mat::zeros(input_image.size(), CV_8UC3);
	Mat labels_mat;

	// components contine numarul de elemente distincte din imaginea de baza (binarizata)
	int components = connectedComponents(input_image, labels_mat, 8);
	
	// initiez un vector pentru culori pt fiecare componenta detectata, fundalul e predefinit negru
	std::vector<Vec3b> colours(components);
	colours[0] = Vec3b(0, 0, 0);

	// Asociez cate o culoare random fiecarui segment individual
	for (int i = 1; i < components; i++)
		colours[i] = Vec3b(rand() & 255, rand() & 255, rand() & 255);

	// iteram prin fiecare pixel al imaginii de output si il facem ori negru (fundal),
	// ori culoarea din vectorul colours
	for (int i = 0; i < input_image.cols; i++) {
		for (int j = 0; j < input_image.rows; j++) {
			output_image.at<Vec3b>(Point(i, j)) = colours[(int)labels_mat.at<int>(Point(i, j))];
		}
	}
	Mat detected;

	return output_image;
}

