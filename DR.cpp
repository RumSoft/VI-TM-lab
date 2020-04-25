#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace std;
using namespace cv;

#define WINDOW_NAME "Wykrywanie krawędzi"

void detectEdges(Mat_<uchar>& peppersPixels, Mat_<uchar>& handEdgesMergedPixels, int binThreshold = -1);

struct thresholdCallbackParams
{
	thresholdCallbackParams(Mat_<uchar>& in, Mat_<uchar>& out)
	{
		this->in = in;
		this->out = out;
	}

	Mat_<uchar> in;
	Mat_<uchar> out;
};

int main()
{
	// Stworzenie okna w którym przechwycone obrazy będą wyświetlane
	cvNamedWindow(WINDOW_NAME, CV_WINDOW_AUTOSIZE);

	// Pobranie obrazu
	Mat imagePeppers = imread("hand.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	// Utworzenie obiektu przechowującego obraz z wyznaczonymi krawędziami
	Mat out = imagePeppers.clone();

	// Uzyskanie macierzy pikseli na podstawie obiektów Mat
	Mat_<uchar> inPix = imagePeppers;
	Mat_<uchar> outPix = out;

	// Oczekiwanie na wciśnięcie klawisza Esc lub Enter
	char key;

	// Wykrywanie krawędzi
	detectEdges(inPix, outPix);

	// Wyświetlanie obrazu
	imshow(WINDOW_NAME, out);

	//trackbar z binaryzacją
	createTrackbar("bin", WINDOW_NAME, nullptr, 254, [](int v, void* d)
	{
		auto data = *static_cast<thresholdCallbackParams*>(d);
		detectEdges(data.in, data.out, v);
		imshow(WINDOW_NAME, data.out);
	}, new thresholdCallbackParams(inPix, outPix));

	do
		key = cvWaitKey(1);
	while (key != 27 && key != 13);

	// Niszczenie okna
	cvDestroyWindow(WINDOW_NAME);
	return 0;
}

Mat sobelx = (Mat_<int>(3, 3) << 1, 2, 1, 0, 0, 0, -1, -2, -1);
Mat sobely = (Mat_<int>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);


/* Funkcja wyznaczająca krawędzie za pomocą maski morfologicznej podanej przez prowadzącego.
Wyznaczane są najpierw krawędzie pionowe, później poziome. Następnie wykonywane jest scalenie
krawędzi pionowych i poziomych, przeskalowanie obrazu scalonego do zakresu 0-255 oraz binaryzacja
z eksperymentalnie dobranym progiem.
Wynikowy obraz zostaje zapisany w handEdgesMergedPixels.*/
void detectEdges(Mat_<uchar>& handPixels, Mat_<uchar>& handEdgesMergedPixels, int binThreshold)
{
	// Utworzenie obiektów przechowujących obrazy z wyznaczonymi krawędziami: pionowymi i poziomymi
	Mat handEdgesVertical = handPixels.clone();
	Mat handEdgesHorizontal = handPixels.clone();

	//zastosowanie filtru z maską do detekcji krawędxi
	filter2D(handPixels, handEdgesHorizontal, 3, sobelx);
	filter2D(handPixels, handEdgesVertical, 3, sobely);

	//złączenie i przeskalowanie złączonych obrazów do 0-255
	//kr = scale[0-255](|kr_poz| + |kr_pion])  : 
	normalize(abs(handEdgesVertical) + abs(handEdgesHorizontal), handEdgesMergedPixels, 0, 255, NORM_MINMAX);

	//binaryzacja
	if (binThreshold > 0)
		threshold(handEdgesMergedPixels, handEdgesMergedPixels, binThreshold, 255, 0);
}
