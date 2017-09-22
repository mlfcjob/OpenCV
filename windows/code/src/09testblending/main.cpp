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
	Mat img1 = imread("2.jpg");
	Mat img2 = imread("3.jpg");
	resize(img2, img2, img1.size());
	Mat dst;

	float a = 0.8;
	addWeighted(img1, a, img2, 1 - a, 80, dst);

	namedWindow("blending");
	imshow("blending", dst);
	waitKey(0);
	return 0;
}
