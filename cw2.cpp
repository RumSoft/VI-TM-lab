#define _USE_MATH_DEFINES

#include <opencv2/imgproc/imgproc.hpp> // Gaussian Blur
#include <opencv2/core/core.hpp>       // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace std;
using namespace cv;

bool Rotate(Mat_<uchar> &peppersPixels, Mat_<uchar> &rotatedPeppersPixels);

#define WINDOW_NAME "Obracanie obrazu"

float degrees = 0;
float change = 1;
int dir = 1;

int main()
{
    // Stworzenie okna w ktC3rym przechwycone obrazy bDdD wyEwietlane
    cvNamedWindow(WINDOW_NAME, CV_WINDOW_AUTOSIZE);

    // Pobranie obrazu
    Mat im1 = imread("peppers.jpg", CV_LOAD_IMAGE_GRAYSCALE);

    // Utworzenie obiektu przechowujDcego obraz, ktC3ry bDdzie obracany
    Mat im2 = im1.clone();

    // Uzyskanie macierzy pikseli na podstawie obiektC3w Mat
    Mat_<uchar> im1pix = im1;
    Mat_<uchar> im2pix = im2;

    while (true)
    {
        // Obracanie obrazu

        if (Rotate(im1pix, im2pix))
            break;

        // WyEwietlanie obrazu
        imshow(WINDOW_NAME, im2);
    }

    // Niszczenie okna
    cvDestroyWindow(WINDOW_NAME);
    return 0;
}

bool Rotate(Mat_<uchar> &peppersPixels, Mat_<uchar> &rotatedPeppersPixels)
{
    degrees += dir * change;
    float rot = remainder(degrees, 360) * 2 * M_PI / 180;

    /* Funkcja obracajDca obraz peppersPixels o liczbD stopni zwiDkszanD o 1 przy kaE<dym kolejnym wywoEaniu.
	Kiedy liczba stopni dochodzi do 360, nastDpuje powtC3rzenie cyklu - ustawienie liczbD stopni na 0.
	Wynikowy obraz (obrC3cony) zostaje zapisywany w rotatedPeppersPixels. */

    for (auto y = 0; y < peppersPixels.rows; y++)
        for (auto x = 0; x < peppersPixels.cols; x++)
        {
            int hy = peppersPixels.rows / 2;
            int hx = peppersPixels.cols / 2;

            int xx = (x - hx) * cos(rot) - (y - hy) * sin(rot) + hx;
            int yy = (x - hx) * sin(rot) + (y - hy) * cos(rot) + hy;
            if (yy >= peppersPixels.rows || xx >= peppersPixels.cols || yy < 0 || xx < 0)
            {
                rotatedPeppersPixels[y][x] = 0;
                continue;
            }
            rotatedPeppersPixels[y][x] = peppersPixels[yy][xx];
        }

    // nie potrzeba filtra, program z zad1 i zad2 nie miaE dziur
    // wyjasnienie:
    // zamiast obraz E:rC3dEowy przetwarzaD  i uzyskiwaD  obrC3cone piksele zastosowano odwrotnD operacjD:
    // dla kazdego pikselu obrazu wynikowego oblicza siD piksel z obrazu E:rC3dEowego
    // unikamy w ten sposC3b bEDdC3w precyzji liczb zmiennoprzecinkowych, gdyE< kaE<dy piksel obrazu wynikowego
    // bDdzie posiadaE na pewno jakDE wartoED
    // #nomakeup #nofilter
    medianBlur(rotatedPeppersPixels, rotatedPeppersPixels, 3);

    // Oczekiwanie na wciEniDcie klawisza Esc lub Enter
    char key = cvWaitKey(1);
    if (key == 27 || key == 13 /*Esc lub Enter*/)
        return true;
    if (key == ' ')
        dir = -dir;
    if (key == '+')
        change *= 1.2;
    if (key == '-')
        change *= 0.8;

    //nie szybciej niz 10, nie wolniej niz 0.2...
    change = change >= 10 ? 10 : change <= 0.2 ? 0.2 : change;

    return false;
}
