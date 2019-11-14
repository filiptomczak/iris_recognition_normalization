#include<stdio.h>
#include<iostream>
#include"opencv2\opencv.hpp"
#include<math.h>

using namespace std;
using namespace cv;

int theta = 360;
int nr = 80;
int marker = 0;
int px, py, pr, ir;
Mat dst, iris_norm, iris;
const double PI = 3.14;


Mat normalizacja(Mat &src, Mat &dst, int px, int py, int pr, int ir);
Mat normalizacja2(Mat &src, Mat &dst, int px, int py, int pr, int ir);
Mat normalizacja3(Mat &src, Mat &dst, int px, int py, int pr, int ir);