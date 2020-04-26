#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace std;
using namespace cv;

#define WINDOW_NAME "wykrywanie krawedzi"


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
	Mat handEdgesMerged = imagePeppers.clone();

	// Uzyskanie macierzy pikseli na podstawie obiektów Mat
	Mat_<uchar> handPixels = imagePeppers;
	Mat_<uchar> handEdgesMergedPixels = handEdgesMerged;

	// Wykrywanie krawędzi
	detectEdges(handPixels, handEdgesMergedPixels);

	// Wyświetlanie obrazu
	imshow(WINDOW_NAME, handEdgesMerged);

	createTrackbar("threshold", WINDOW_NAME, nullptr, 254, [](int v, void* d)
	{
		auto data = *static_cast<thresholdCallbackParams*>(d);
		detectEdges(data.in, data.out, v);
		imshow(WINDOW_NAME, data.out);
	}, new thresholdCallbackParams(handPixels, handEdgesMergedPixels));

	// Oczekiwanie na wciśnięcie klawisza Esc lub Enter
	char key;
	do key = cvWaitKey(1);
	while (key != 27 && key != 13);

	// Niszczenie okna
	cvDestroyWindow("Wykrywanie krawędzi");
	return 0;
}

//metoda uzywana do replikacji krawędzi aaaa|abcdefg|gggg
//ogranicza wybierany indeks elementu w tablicy do wartości  min <= val <= max
int between(int min, int v, int max) { return v <= min ? min : v >= max ? max : v; }

#define MASK_SIZE 3
Mat_<int> maskVertical = (Mat_<int>(MASK_SIZE, MASK_SIZE) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
Mat_<int> maskHorizontal = (Mat_<int>(MASK_SIZE, MASK_SIZE) << 1, 2, 1, 0, 0, 0, -1, -2, -1);

void detectEdges(Mat_<uchar>& handPixels, Mat_<uchar>& handEdgesMergedPixels, int binThreshold)
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

	//metoda do obliczenia wartości piksela przy użyciu maski
	auto edges = [](Mat_<uchar>& src, Mat_<int> mask, int y, int x) -> int
	{
		//s - suma pikseli dookoła,
		//r - radius - promień maski (dla 3x3 promieniem jest 1, 5x5 -> 2, itd..)
		int s = 0, r = MASK_SIZE / 2;
		for (int i = -r; i <= r; ++i)
			for (int j = -r; j <= r; ++j)
				s += src[between(0, y + i, src.rows - 1)][between(0, x + j, src.cols - 1)] * mask[i + r][j + r];
		return s;
	};

	//s(z)kalowanie tak jak podano w pdf-ie 
	auto scale = [](Mat img, auto from, auto to) -> Mat
	{
		double min, max;
		minMaxLoc(img, &min, &max);
		return (img - min) * (to - from) / (max - min);
	};

	//binaryzacja
	auto binarize = [](Mat_<uchar>& img, auto threshold)
	{
		for (int y = 0; y < img.rows; y++)
			for (int x = 0; x < img.cols; x++)
				img[y][x] = img[y][x] >= threshold ? 255 : 0;
	};

	for (int y = 0; y < handPixels.rows; y++)
		for (int x = 0; x < handPixels.cols; x++)
		{
			handEdgesVerticalPixels[y][x] = edges(handPixels, maskVertical, y, x);
			handEdgesHorizontalPixels[y][x] = edges(handPixels, maskHorizontal, y, x);
		}

	handEdgesMergedPixels = scale(abs(handEdgesHorizontalPixels) + abs(handEdgesVerticalPixels), 0, 255);

	if (binThreshold > 0)
		binarize(handEdgesMergedPixels, binThreshold);
}
