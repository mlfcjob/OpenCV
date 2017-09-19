#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void  PrintMs(const char *text = "")
{
	static long long last = 0;
	long long cur = getTickCount();  //Opencv

	if (last == 0)
	{
		last = cur;
		return;
	}

	long long ms;
	ms = (double)(cur - last) / getTickFrequency() * 1000;  //ms

	if (*text != 0)
	{
		printf("%s = %d ms\n", text, ms);
	}

	last = getTickCount();
}

void RGBToGray(Mat src, Mat &des)
{
	// GRay: £¨R * 30 + G * 59 +  B * 11 + 50) / 100
	des.create(src.rows, src.cols, CV_8UC1);

	for (int r = 0; r < src.rows; r++)
	{
		for (int c = 0; c < src.cols; c++)
		{
			Vec3b &m = src.at<Vec3b>(r, c);
			int gray = (m[2] * 30 + m[1] * 59 + m[0]*11 + 50) / 100;
			des.at<uchar>(r, c) = gray;
		}
	}
}

int main(int argc, char *argv[])
{
	Mat src = imread("2.jpg");
	//src.create(3000, 4000, CV_8UC3);
	Mat gray;

	PrintMs("");
	cvtColor(src, gray, COLOR_BGR2GRAY);
	PrintMs("cvtColor");

	cvtColor(src, gray, COLOR_BGR2GRAY);
	PrintMs("cvtColor2");

	Mat  mygray;
	RGBToGray(src, mygray);
	PrintMs("RGBToGray");

	namedWindow("src");
	namedWindow("gray");
	namedWindow("mygray");

	imshow("src", src);
	imshow("gray", gray);
	imshow("mygray", mygray);
	
	waitKey(0);

	return 0;
}
