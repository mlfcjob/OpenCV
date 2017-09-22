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


int main(int argc, char *argv[])
{
	Mat img = imread("2.jpg");
	Mat rot;
	Mat fl;

	cv::rotate(img, rot, ROTATE_90_CLOCKWISE);
	cv::flip(img, fl, 1);

	namedWindow("rotate");
	namedWindow("flip");
	namedWindow("src");

	imshow("rotate", rot);
	imshow("flip", fl);
	imshow("src", img);
	waitKey(0);
	return 0;
}
