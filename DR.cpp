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
int change = 1;
int dir = 1;

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

	while (true)
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
	degrees += dir * change;
	float rot = degrees % 360 * 2 * M_PI / 180;


	/* Funkcja obracająca obraz peppersPixels o liczbę stopni zwiększaną o 1 przy każdym kolejnym wywołaniu.
	Kiedy liczba stopni dochodzi do 360, następuje powtórzenie cyklu - ustawienie liczbę stopni na 0.
	Wynikowy obraz (obrócony) zostaje zapisywany w rotatedPeppersPixels. */

	for (auto y = 0; y < peppersPixels.rows; y++)
		for (auto x = 0; x < peppersPixels.cols; x++)
		{
			int hy = peppersPixels.rows / 2;
			int hx = peppersPixels.cols / 2;

			int xx = (x - hx) * cos(rot) - (y - hy) * sin(rot) + hx;
			int yy = (x - hx) * sin(rot) + (y - hy) * cos(rot) + hy;
			if (yy >= peppersPixels.rows
				|| xx >= peppersPixels.cols
				|| yy < 0
				|| xx < 0)
			{
				rotatedPeppersPixels[y][x] = 0;
				continue;
			}
			rotatedPeppersPixels[y][x] = peppersPixels[yy][xx];
		}

	// nie potrzeba filtra, program z zad1 i zad2 nie miał dziur
	// wyjasnienie:
	// zamiast obraz źródłowy przetwarzać i uzyskiwać obrócone piksele zastosowano odwrotną operację:
	// dla kazdego pikselu obrazu wynikowego oblicza się piksel z obrazu źródłowego
	// unikamy w ten sposób błędów precyzji liczb zmiennoprzecinkowych, gdyż każdy piksel obrazu wynikowego
	// będzie posiadał na pewno jakąś wartość
	// #nomakeup #nofilter
	medianBlur(rotatedPeppersPixels, rotatedPeppersPixels, 3);

	// Oczekiwanie na wciśnięcie klawisza Esc lub Enter
	char key = cvWaitKey(1);
	if (key == 27 || key == 13/*Esc lub Enter*/)
		return true;
	if (key == ' ') dir = -dir;
	if (key == '+') change += 1;
	if (key == '-') change -= 1;

	//nie szybciej niz 10, nie wolniej niz 1...
	change = change >= 10 ? 10 : change <= 1 ? 1 : change;

	return false;
}
