#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

void  PrintMs(const char *text = "")
{
	// 计数除以频率等于时间
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
		printf("%s = %d ms", text, ms);
	}

	last = getTickCount();
}

int main(int argc, char *argv[])
{
	Mat mat(3000, 4000, CV_8UC3);
	//mat.create(3000, 4000, CV_8UC3);
	int es = mat.elemSize();
	int size = mat.rows * mat.cols * es;

	PrintMs();
	for (int i = 0; i < size; i += es){
		mat.data[i] =  255;//B
		mat.data[i + 1] = 0; //G
		mat.data[i + 2] = 255;  //R
	}
	PrintMs("mat.data ms");


	for (int row = 0; row < mat.rows; row++)
	{
		for (int col = 0; col < mat.cols; col++)
		{
			(&mat.data[row * mat.step])[col * es] = 0;  //b
			(&mat.data[row * mat.step])[col * es + 1] = 0;  //g
			(&mat.data[row * mat.step])[col * es + 2] = 255;  //r
		}
	}
	PrintMs("mat.step ms");

	namedWindow("mat");
	imshow("mat", mat);
	waitKey(0);
	return 0;
}
