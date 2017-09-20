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
///////////////
/// @ b float: 1.0-3.0
/// @ a  in :  0 - 100
void ChangeGain(Mat &src, Mat &des, float a, int b)
{
	// g(r, c) = a * f(r, c)  + b
	des.create(src.rows, src.cols, src.type());
	for (int r = 0; r < src.rows; r++) {
		for (int c = 0; c < src.cols; c++) {
			for (int i = 0; i < 3; i++)
			{
				des.at<Vec3b>(r,c)[i] = saturate_cast<uchar>(a * src.at<Vec3b>(r, c)[i] + b);
			}
		}
	}
}

int main(int argc, char *argv[])
{
	//调整对比度和亮度
	Mat src = imread("5k.jpg");
	Mat des;
	Mat des2;

	PrintMs("");
	ChangeGain(src, des, 2.0, 50);
	PrintMs("ChangeGain");

	src.convertTo(des2, -1, 2.0, 50);
	PrintMs("convertTo");

	namedWindow("src");
	namedWindow("des");
	namedWindow("des2");

	imshow("src", src);
	imshow("des", des);
	imshow("des2", des2);

	waitKey(0);
	return 0;
}
