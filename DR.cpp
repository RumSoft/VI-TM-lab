#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace std;
using namespace cv;

bool Diffuse(Mat_<uchar>& img1, Mat_<uchar>& img2, Mat_<uchar>& outputImg, float ratio);
float Clamp(float value, float min, float max);

#define WINDOW_NAME "przenikanie"
#define KEY_ESC 27
#define KEY_ENTER 13
#define KEY_UP 2490368
#define KEY_DOWN 2621440

float change = 0.02f;
int main()
{
	// Stworzenie okna w którym przechwycone obrazy będą wyświetlane
	cvNamedWindow(WINDOW_NAME, CV_WINDOW_AUTOSIZE);

	// Pobranie obrazu
	Mat im1 = imread("peppers.jpg", CV_LOAD_IMAGE_COLOR);
	Mat im2 = imread("fruits.jpg", CV_LOAD_IMAGE_COLOR);

	// Utworzenie obiektu przechowującego obraz, który będzie wynikiem przenikania
	Mat im3 = im1.clone();

	Mat_<uchar> im1pix = im1;
	Mat_<uchar> im2pix = im2;
	Mat_<uchar> im3pix = im3;

	auto forceStop = false;
	float ratio = .0f;
	int dir = 1;
	do
	{
		ratio = Clamp(ratio + change * dir, 0, 1);
		if (ratio >= 1 || ratio <= 0)
			dir = -dir;

		forceStop = Diffuse(im1pix, im2pix, im3pix, ratio);
		imshow(WINDOW_NAME, im3);
	} while (!forceStop);

	// Niszczenie okna
	cvDestroyWindow(WINDOW_NAME);
	return 0;
}

//hard-limit wartosci na wyjsciu z funkcji miedzy min i max
float Clamp(float value, float min, float max)
{
	if (value >= max) return max;
	if (value <= min) return min;
	return value;
}

bool Diffuse(Mat_<uchar>& img1, Mat_<uchar>& img2, Mat_<uchar>& outputImg, float ratio)
{
	for (auto y = 0; y < img1.rows; y++)
		for (auto x = 0; x < img1.cols; x++)
			outputImg[y][x] = img1[y][x] * ratio + img2[y][x] * (1 - ratio);

	const auto key = waitKeyEx(1);
	switch (key)
	{
	case KEY_ENTER:
	case KEY_ESC:
		return true;
	case KEY_UP:
		change = Clamp(change * 1.15f, 0.001f, 0.1);
		cout << change << endl;
		break;
	case KEY_DOWN:
		change = Clamp(change * 0.85f, 0.001f, 0.1);
		cout << change << endl;
		break;
	}

	return false;
}
