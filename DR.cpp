#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace std;
using namespace cv;

bool Diffuse(Mat_<uchar>& img1, Mat_<uchar>& img2, Mat_<uchar>& outputImg, float ratio);

#define WINDOW_NAME "przenikanie"
#define KEY_ESC 27
#define KEY_ENTER 13

int main()
{
	// Stworzenie okna w którym przechwycone obrazy będą wyświetlane
	cvNamedWindow(WINDOW_NAME, CV_WINDOW_AUTOSIZE);

	// Pobranie obrazu
	Mat imagePeppers = imread("peppers.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat imageFruits = imread("fruits.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	// Utworzenie obiektu przechowującego obraz, który będzie wynikiem przenikania
	Mat imagesDiffusion = imagePeppers.clone();

	// Uzyskanie macierzy pikseli na podstawie obiektów Mat
	Mat_<uchar> peppersPixels = imagePeppers;
	Mat_<uchar> fruitsPixels = imageFruits;
	Mat_<uchar> imagesDiffusionPixels = imagesDiffusion;

	auto forceStop = false;
	do
	{
		auto ratio = 1.0f;
		auto change = 0.02f;
		for (ratio = .0f; ratio <= 1; ratio += change)
		{
			if (Diffuse(peppersPixels, fruitsPixels, imagesDiffusionPixels, ratio) == true)
				goto end;

			imshow(WINDOW_NAME, imagesDiffusion);
		}
		for (ratio = 1.0f; ratio >= 0; ratio -= change)
		{
			if (Diffuse(peppersPixels, fruitsPixels, imagesDiffusionPixels, ratio) == true)
				goto end;

			imshow(WINDOW_NAME, imagesDiffusion);
		}
	}
	while (!forceStop);

end:

	// Niszczenie okna
	cvDestroyWindow(WINDOW_NAME);
	return 0;
}

bool Diffuse(Mat_<uchar>& img1, Mat_<uchar>& img2, Mat_<uchar>& outputImg, float ratio)
{
	for (auto x = 0; x < img1.rows; x++)
		for (auto y = 0; y < img1.cols; y++)
			outputImg[x][y] = img1[x][y] * ratio + img2[x][y] * (1 - ratio);

	auto key = cvWaitKey(1);
	switch (key)
	{
	case KEY_ENTER:
	case KEY_ESC:
		return true;
	}

	return false;
}
