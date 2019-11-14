#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>
#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <math.h>

using namespace std;
using namespace cv;

Mat src = imread("E:\\filip\\5 rok\\Projekt\\zrenica4.jpg", 1);
Mat src_gray;
Mat dst = src.clone();
Mat iris_norm;
vector<Vec3f>circles;

const int slider_max = 200;
int alpha_slider = 200;
double alpha = 200;

static void on_trackbar1(int, void*)
{
	imshow("oryginal", dst);
	alpha = (double)alpha_slider;
	if (alpha_slider == 0 || alpha_slider==1)
		alpha = 2;
	cvtColor(src, dst, CV_BGR2GRAY);
	cvtColor(src, src_gray, CV_BGR2GRAY);

	GaussianBlur(dst, dst, Size(3, 3), 2, 2);
	HoughCircles(dst, circles, CV_HOUGH_GRADIENT, 1, src.rows , alpha, alpha/2); // src.rows  30 /100,60
	/*for (size_t i = 0; i < circles.size(); i++)
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		// circle center
		circle(src, center, 3, Scalar(0, 255, 0), -1, 8, 0);
		// circle outline
		circle(src, center, radius, Scalar(250, 0, 200), 3, 8, 0);
	}
	*/
	if (circles.size() != 0)
	{
		double radius;
		if (dst.cols > dst.rows)
			radius = dst.cols / 2;
		if (dst.cols <= dst.rows)
			radius = dst.rows / 2;
		double M = dst.cols / log(radius);
		
		//double M = dst.cols / 4;
		
		logPolar(src_gray, iris_norm, Point(cvRound(circles[0][0]), cvRound(circles[0][1])), M, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS);
		//linearPolar(dst, iris_norm, Point(cvRound(circles[0][0]), cvRound(circles[0][1])), 500, INTER_LINEAR + WARP_FILL_OUTLIERS);
		imshow("polarne", iris_norm);
		vector<int>pointL;
		vector<int>pointP;

		for (int i = 0; i < iris_norm.rows; i++)
			for (int j = 0; j < iris_norm.cols; j++)
				if (iris_norm.at<uchar>(i, j) != 0) {
					pointL.push_back(j);
					break;
				}

		for (int i = iris_norm.rows - 1; i >= 0; i--)
			for (int j = iris_norm.cols - 1; j >= 0; j--)
				if (iris_norm.at<uchar>(i, j) != 0) {
					pointP.push_back(j);
					break;
				}
		
		int maxL = *max_element(pointL.begin(), pointL.end());
		int maxP = *max_element(pointP.begin(), pointP.end());
		Mat normalizacja(iris_norm, Rect(maxL, 0, maxP - maxL, iris_norm.rows));
		cout << maxP - maxL << endl;
		transpose(normalizacja, normalizacja);
		resize(normalizacja, normalizacja, Size(256, 72), 0, 0, INTER_NEAREST);
		
		imshow("normalizacja", normalizacja);
	}
}

int main(int, char**) 
{
	namedWindow("oryginal", CV_WINDOW_AUTOSIZE);
	char TrackbarName1[50];
	sprintf_s(TrackbarName1, "Alpha");
	createTrackbar(TrackbarName1, "oryginal", &alpha_slider, slider_max, on_trackbar1);
	on_trackbar1(alpha_slider, 0);

	waitKey(0);
	return 0;
}