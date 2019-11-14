#include"Nag³ówek.h"

Mat normalizacja(Mat &src, Mat &dst, int px, int py, int pr, int ir)
{
	int iw = ir - pr; //szerokosc teczowki=promien teczowki-promien zrenicy
	iris_norm = Mat(nr, theta, CV_8U, cv::Scalar(255));
	for (int i = 0; i < theta; i++)
	{
		double alpha = 2 * PI * i / theta; //aktualny kat obrotu
		for (int j = 0; j < nr; j++) //nr-ilosc probek pobrane z promienia
		{
			double r = pr + iw * (j + 1) / nr; // aktualna dlugosc promienia
			int x = (int)px + r * cos(alpha);//wsp x 
			int y = (int)py + r * sin(alpha);//wsp y
			iris_norm.at<uchar>(j, i) = src.at<uchar>(y, x);//macierz znormalizowana, zmieniona w prostokat
		}
	}
	imshow("norm", iris_norm);
	dst = iris_norm.clone();
	return dst;
}

Mat normalizacja2(Mat &src, Mat &dst, int px, int py, int pr, int ir)
{
	iris_norm = Mat(nr, theta, CV_8U, cv::Scalar(255));
	for (int i = 0; i < 360; i++)
	{
		double alpha = 2 * PI * i / theta;
		for (int j = 0; j < nr; j++)
		{
			double r = 1.0*j / nr;
			int x = (int)((1 - r)*(px + pr * cos(alpha)) + r * (px + ir * cos(alpha)));
			int y = (int)((1 - r)*(py + pr * sin(alpha)) + r * (py + ir * sin(alpha)));
			iris_norm.at<uchar>(j, i) = src.at<uchar>(y, x);
		}
	}
	imshow("norm2", iris_norm);
	dst = iris_norm.clone();
	return dst;
}

Mat normalizacja3(Mat &src, Mat &dst, int px, int py, int pr, int ir)
{
	iris_norm = Mat(nr, theta, CV_8U, cv::Scalar(255));
	double M = 80;

	vector<int>pointL;
	vector<int>pointP;
	logPolar(iris, iris_norm, Point(px, py), M, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS);
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
	transpose(normalizacja, normalizacja);
	resize(normalizacja, normalizacja, Size(theta, nr), 0, 0, INTER_NEAREST);

	imshow("norm3", normalizacja);
	dst = normalizacja.clone();
	return dst;

}

int main()
{
	resize(iris, iris, Size(512, 512));
	imshow("iris", iris);
	normalizacja(iris, dst, px, py, pr, ir);
	normalizacja2(iris, dst, px, py, pr, ir);
	normalizacja3(iris, dst, px, py, pr, ir);

	waitKey(0);
	return 0;
}

