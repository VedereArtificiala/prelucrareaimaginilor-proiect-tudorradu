#include "Header.h"

int main()
{
	//	IMREAD_GRAYSCALE converteste imaginea in grayscale
	Mat src = imread("img/bone1.png", IMREAD_GRAYSCALE);
	Mat sharpened_img, iterative_threshold_img, colour_threshold_img;

	imshow("Original", src);

	// accentual sharpness-ul si contrastul imaginii pentru a delimita mai clar zonele
	sharpened_img = sharpen(src);
	imshow("Imagine enhanced", sharpened_img);

	//Segmentarea prin delimitarea cu prag , metoda OTSU
	//threshold(sharpened_img, prag_img, 200, 255, THRESH_OTSU + THRESH_BINARY);
	//imshow("Imagine delimitata cu prag", prag_img);

	// Segmentare prin aplicarea unui threshold iterativ
	iterative_threshold_img = iterative_threshold(sharpened_img);
	imshow("Threshold iterativ", iterative_threshold_img);

	// metoda de erodare + dilatare pentru a reduce zgomotul din segmentari
	erode_and_dilate(iterative_threshold_img);
	imshow("Dilate & erode", iterative_threshold_img);

	// Colorarea segmentelor
	colour_threshold_img = colour_segmentation(iterative_threshold_img);


	// fara functia asta programul ar iesi imediat din executie
	waitKey(0);

	return 0;
}

