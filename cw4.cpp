#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/highgui/highgui.hpp>
#include <cstdio>
#include <cmath>
#include <iostream>

#define PI (3.141592653589793)
#define WINDOW_NAME "JPEG"
#define BLOCK_SIZE 8
#define foryx(_y,_x, _yto, _xto) \
	for(int (_y)=0; (_y) < (_yto); (_y)++) \
	for(int (_x)=0; (_x) < (_xto); (_x)++)

//quality level: 1-100
#define QUALITY 1

using namespace std;
using namespace cv;

float qMatrix[8][8] = {
	{16, 11, 10, 16, 24, 40, 51, 61},
	{12, 12, 14, 19, 26, 58, 60, 55},
	{14, 13, 16, 24, 40, 57, 69, 56},
	{14, 17, 22, 29, 51, 87, 80, 62},
	{18, 22, 37, 56, 68, 109, 103, 77},
	{24, 35, 55, 64, 81, 104, 113, 92},
	{49, 64, 78, 87, 103, 121, 120, 101},
	{72, 92, 95, 98, 112, 100, 103, 99},
};

float dct(float block[BLOCK_SIZE][BLOCK_SIZE], int v, int u)
{
	auto val = .0f;
	foryx(n, m, BLOCK_SIZE, BLOCK_SIZE)
		val += block[n][m]
			* cos((2 * n + 1) * v * PI / (2 * BLOCK_SIZE))
			* cos((2 * m + 1) * u * PI / (2 * BLOCK_SIZE));
	return val / powf(BLOCK_SIZE, 2);
};

float idct(float block[BLOCK_SIZE][BLOCK_SIZE], int n, int m)
{
	auto val = .0f;
	foryx(v, u, BLOCK_SIZE, BLOCK_SIZE)
		val += (u == 0 ? 1 : 2) * (v == 0 ? 1 : 2) * block[v][u]
			* cos((2 * n + 1) * v * PI / (2 * BLOCK_SIZE))
			* cos((2 * m + 1) * u * PI / (2 * BLOCK_SIZE));
	return val;
};

int main()
{
	cvNamedWindow(WINDOW_NAME, CV_WINDOW_AUTOSIZE);

	Mat image = imread("fruits2.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat_<uchar> imagePixels = image;

	const int blockCols = 320 / BLOCK_SIZE;
	const int blockRows = 240 / BLOCK_SIZE;

#define forBlocks(_yblock,_xblock,_yBlockPixel,_xBlockPixel) \
	foryx(_yblock, _xblock, blockRows, blockCols) \
	foryx(_yBlockPixel, _xBlockPixel, BLOCK_SIZE, BLOCK_SIZE)

	float block[blockRows][blockCols][BLOCK_SIZE][BLOCK_SIZE];
	float blockDCT[blockRows][blockCols][BLOCK_SIZE][BLOCK_SIZE];
	float blockIDCT[blockRows][blockCols][BLOCK_SIZE][BLOCK_SIZE];

	// -- quality level ----------------------------
	foryx(y, x, 8, 8)
		qMatrix[y][x] = qMatrix[y][x] * (100 - QUALITY) / 99 * 0.18 + 0.02;

	// -- podział obrazu na bloki ------------------
	foryx(y, x, image.rows, image.cols)
		block[y / BLOCK_SIZE][x / BLOCK_SIZE][y % BLOCK_SIZE][x % BLOCK_SIZE] = imagePixels[y][x];
	//foryx(y, x, BLOCK_SIZE, BLOCK_SIZE) block[3][3][y][x] = 255; //test czy dziala

	// -- zerowanie blockDCT i blockIDCT -----------
	forBlocks(y, x, yy, xx)
		blockDCT[y][x][yy][xx] = blockIDCT[y][x][yy][xx] = 0;

	// -- dct --------------------------------------
	forBlocks(y, x, yy, xx)
		blockDCT[y][x][yy][xx] = dct(block[y][x], yy, xx);

	// -- kwantyzacja ------------------------------
	forBlocks(y, x, yy, xx)
		blockDCT[y][x][yy][xx] = int(blockDCT[y][x][yy][xx] / qMatrix[yy % 8][xx % 8]);

	// -- dekwantyzacja ----------------------------
	forBlocks(y, x, yy, xx)
		blockDCT[y][x][yy][xx] = blockDCT[y][x][yy][xx] * qMatrix[yy % 8][xx % 8];

	// -- IDCT -------------------------------------
	forBlocks(y, x, yy, xx)
		blockIDCT[y][x][yy][xx] = idct(blockDCT[y][x], yy, xx);

	// -- skladanie obrazu z blokow -----------------
	auto between = [](auto min, auto val, auto max) { return val < min ? min : val > max ? max : val; };
	forBlocks(y, x, yy, xx)
		imagePixels[y * BLOCK_SIZE + yy][x * BLOCK_SIZE + xx] = between(0, blockIDCT[y][x][yy][xx], 255);
	normalize(imagePixels, imagePixels, 0, 255, NORM_MINMAX);
	imshow(WINDOW_NAME, image);

	char key;
	do key = cvWaitKey(1);
	while (key != 27 && key != 13); //esc or enter

	cvDestroyWindow(WINDOW_NAME);
	return 0;
}
