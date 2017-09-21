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
	Mat src = imread("2.jpg");
	Mat gsrc;
	Mat lsrc;

	pyrDown(src, gsrc);
	pyrUp(src, lsrc);

	namedWindow("src");
	moveWindow("src", 0, 0);
	namedWindow("gsrc");
	moveWindow("gsrc", 512, 0);
	namedWindow("lsrc");
	moveWindow("lsrc", 0, 512);

	imshow("src", src);
	imshow("gsrc", gsrc);
	imshow("lsrc", lsrc);
	waitKey(0);
	return 0;
}
