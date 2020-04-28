#define _USE_MATH_DEFINES

#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace std;
using namespace cv;

bool Rotate(Mat_<uchar>& peppersPixels, Mat_<uchar>& rotatedPeppersPixels);

#define WINDOW_NAME "Obracanie obrazu"

int degrees = 0;
int main()
{
	// Stworzenie okna w którym przechwycone obrazy będą wyświetlane
	cvNamedWindow(WINDOW_NAME, CV_WINDOW_AUTOSIZE);

	// Pobranie obrazu
	Mat im1 = imread("peppers.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	// Utworzenie obiektu przechowującego obraz, który będzie obracany
	Mat im2 = im1.clone();

	// Uzyskanie macierzy pikseli na podstawie obiektów Mat
	Mat_<uchar> im1pix = im1;
	Mat_<uchar> im2pix = im2;

	while (1)
	{
		// Obracanie obrazu
		if (Rotate(im1pix, im2pix))
			break;

		// Wyświetlanie obrazu
		imshow(WINDOW_NAME, im2);
	}

	// Niszczenie okna
	cvDestroyWindow(WINDOW_NAME);
	return 0;
}

bool Rotate(Mat_<uchar>& peppersPixels, Mat_<uchar>& rotatedPeppersPixels)
{

	/* Funkcja obracająca obraz peppersPixels o liczbę stopni zwiększaną o 1 przy każdym kolejnym wywołaniu.
	Kiedy liczba stopni dochodzi do 360, następuje powtórzenie cyklu - ustawienie liczbę stopni na 0.
	Wynikowy obraz (obrócony) zostaje zapisywany w rotatedPeppersPixels. */
	degrees += 1;

	for (auto y = 0; y < peppersPixels.rows; y++)
		for (auto x = 0; x < peppersPixels.cols; x++)
		{
			float rot = degrees % 360 * 2 * M_PI / 180;
			int xx = x * cos(rot) - y * sin(rot);
			int yy = x * sin(rot) + y * cos(rot);
			if (yy >= peppersPixels.rows
				|| xx >= peppersPixels.cols
				|| yy < 0
				|| xx < 0) {
				rotatedPeppersPixels[y][x] = 0;
				continue;
			}
			rotatedPeppersPixels[y][x] = peppersPixels[yy][xx];
		}


	// Oczekiwanie na wciśnięcie klawisza Esc lub Enter
	char key = cvWaitKey(1);
	if (key == 27 || key == 13/*Esc lub Enter*/)
		return true;

	return false;
}

