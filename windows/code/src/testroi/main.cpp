#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
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
	Mat src = imread("2.jpg");
	Rect rect(256, 256, 200, 200);
	Mat roi = src(rect);
	MatSize s = roi.size;

	namedWindow("roi", WINDOW_AUTOSIZE);
	namedWindow("src");
	imshow("src", src);
	imshow("roi", roi);
	waitKey();
	return 0;
}
