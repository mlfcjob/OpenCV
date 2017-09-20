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

void xresize(Mat &src, Mat &des, Size &size)
{
	des.create(size, src.type());
	//Ó³ÉäµÄÔ­Í¼×ø±ê
	int sx, sy = 0;

	float fx = (float)src.cols / des.cols;
	float fy = (float)src.rows / des.rows;

	for (int x = 0; x < des.cols; x++) {
		sx = fx * x;
		for (int y = 0; y < des.rows; y++) {
			sy = fy * y;
			des.at<Vec3b>(y, x) = src.at<Vec3b>(sy, sx);
		}
	}
}

int main(int argc, char *argv[])
{
	Mat src = imread("3.jpg");  // 512 * 512 ----> 256X256   ---->1024 * 1204
	Mat img256;
	Mat img768;
	Mat img1024;
	Mat cv256;

	PrintMs("");
	xresize(src, img256, Size(256, 256));
	PrintMs("256");
	xresize(src, img768, Size(768, 768));
	PrintMs("768");
	xresize(src, img1024, Size(1024, 1024));
	PrintMs("1024");

	resize(src, cv256, Size(256, 256), 0, 0, INTER_NEAREST);
	PrintMs("cv256");

	namedWindow("img256");
	namedWindow("img768");
	namedWindow("img1024");
	namedWindow("cv256");
	namedWindow("src");

	imshow("src", src);
	imshow("img256", img256);
	imshow("img768", img768);
	imshow("img1024", img1024);
	imshow("cv256", cv256);
	waitKey(0);
	return 0;
}
