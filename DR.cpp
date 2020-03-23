#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace std;
using namespace cv;

bool Diffuse(Mat& img1, Mat& img2, Mat& outputImg, float ratio);

#define WINDOW_NAME "przenikanie"
#define KEY_ESC 27
#define KEY_ENTER 13

int main()
{
	// Stworzenie okna w którym przechwycone obrazy będą wyświetlane
	cvNamedWindow(WINDOW_NAME, CV_WINDOW_AUTOSIZE);

	// Pobranie obrazu
	Mat imagePeppers = imread("peppers.jpg", CV_LOAD_IMAGE_COLOR);
	Mat imageFruits = imread("fruits.jpg", CV_LOAD_IMAGE_COLOR);

	// Utworzenie obiektu przechowującego obraz, który będzie wynikiem przenikania
	Mat imagesDiffusion = imagePeppers.clone();

		
	auto forceStop = false;
	do
	{
		auto ratio = 1.0f;
		auto change = 0.02f;
		for (ratio = .0f; ratio <= 1; ratio += change)
		{
			if (Diffuse(imagePeppers, imageFruits, imagesDiffusion, ratio) == true)
				goto end;

			imshow(WINDOW_NAME, imagesDiffusion);
		}
		for (ratio = 1.0f; ratio >= 0; ratio -= change)
		{
			if (Diffuse(imagePeppers, imageFruits, imagesDiffusion, ratio) == true)
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

bool Diffuse(Mat& img1, Mat& img2, Mat& outputImg, float ratio)
{
	for (auto y = 0; y < img1.rows; y++)
		for (auto x = 0; x < img1.cols; x++) {
			auto im1pix = img1.at<Vec3b>(y,x);
			auto im2pix = img2.at<Vec3b>(y,x);
			auto outpix = outputImg.at<Vec3b>(y, x);

			outpix[0] = im1pix[0] * ratio + im2pix[0] * (1 - ratio);
			outpix[1] = im1pix[1] * ratio + im2pix[1] * (1 - ratio);
			outpix[2] = im1pix[2] * ratio + im2pix[2] * (1 - ratio);
			outputImg.at<Vec3b>(y, x) = outpix;
		}

	auto key = cvWaitKey(1);
	switch (key)
	{
	case KEY_ENTER:
	case KEY_ESC:
		return true;
	}

	return false;
}
