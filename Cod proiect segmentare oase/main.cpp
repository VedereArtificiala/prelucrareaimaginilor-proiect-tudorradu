#include "Header.h"
#include <vector>
#include <string>

int main()
{
	// Vector de imagini
	std::vector<std::string> boneImages = { "bone1", "bone2", "bone3", "bone4", "bone5"};
	
	for (const auto& boneImage : boneImages)
	{
		//	IMREAD_GRAYSCALE converteste imaginea in grayscale
		Mat source_img = imread("img/" + boneImage + ".png", IMREAD_GRAYSCALE);
		Mat sharpened_img, iterative_threshold_img, colour_threshold_img;

		imshow("Original", source_img);


		// accentuam sharpness-ul si contrastul imaginii pentru a delimita mai clar zonele
		sharpened_img = enhance(source_img);
		imshow("Imagine enhanced", sharpened_img);


		// Segmentare prin aplicarea unui threshold iterativ
		iterative_threshold_img = iterative_threshold(sharpened_img);
		imshow("Threshold iterativ", iterative_threshold_img);


		// metoda de erodare + dilatare pentru a reduce zgomotul din segmentari
		erode_and_dilate(iterative_threshold_img);
		imshow("Dilate & erode", iterative_threshold_img);

		// Colorarea segmentelor
		colour_threshold_img = colour_segmentation(iterative_threshold_img);
		imshow("Coloured img", colour_threshold_img);



		// fara functia asta programul ar iesi imediat din executie
		waitKey(0);

		destroyAllWindows();
	}

	return 0;
}

