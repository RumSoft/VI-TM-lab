#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace std;
using namespace cv;

void detectEdges(Mat_<uchar>& peppersPixels, Mat_<uchar>& handEdgesMergedPixels);

int main()
{
	// Stworzenie okna w którym przechwycone obrazy będą wyświetlane
	cvNamedWindow("Wykrywanie krawędzi", CV_WINDOW_AUTOSIZE);

	// Pobranie obrazu
	Mat imagePeppers = imread("hand.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	// Utworzenie obiektu przechowującego obraz z wyznaczonymi krawędziami
	Mat handEdgesMerged = imagePeppers.clone();

	// Uzyskanie macierzy pikseli na podstawie obiektów Mat
	Mat_<uchar> handPixels = imagePeppers;
	Mat_<uchar> handEdgesMergedPixels = handEdgesMerged;

	// Wykrywanie krawędzi
	detectEdges(handPixels, handEdgesMergedPixels);

	// Wyświetlanie obrazu
	imshow("Wykrywanie krawędzi", handEdgesMerged);

	// Oczekiwanie na wciśnięcie klawisza Esc lub Enter
	char key;
	do	key = cvWaitKey(1);
	while (key != 27 && key != 13);

	// Niszczenie okna
	cvDestroyWindow("Wykrywanie krawędzi");
	return 0;
}

void detectEdges(Mat_<uchar>& handPixels, Mat_<uchar>& handEdgesMergedPixels)
{
	/* Funkcja wyznaczająca krawędzie za pomocą maski morfologicznej podanej przez prowadzącego.
	Wyznaczane są najpierw krawędzie pionowe, później poziome. Następnie wykonywane jest scalenie
	krawędzi pionowych i poziomych, przeskalowanie obrazu scalonego do zakresu 0-255 oraz binaryzacja
	z eksperymentalnie dobranym progiem.
	Wynikowy obraz zostaje zapisany w handEdgesMergedPixels.*/

	// Utworzenie obiektów przechowujących obrazy z wyznaczonymi krawędziami: pionowymi i poziomymi
	Mat handEdgesVertical = handPixels.clone();
	Mat handEdgesHorizontal = handPixels.clone();

	// Uzyskanie macierzy pikseli na podstawie obiektów Mat
	Mat_<int> handEdgesVerticalPixels = handEdgesVertical;
	Mat_<int> handEdgesHorizontalPixels = handEdgesHorizontal;

	// TODO
}

