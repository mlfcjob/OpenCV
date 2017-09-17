#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
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
		printf("%s = %d ms\n", text, ms);
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

	/* 不连续空间访问*/
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

	/* 使用ptr遍历 */
	for (int row = 0; row < mat.rows; row++)
	{
		for (int col = 0; col < mat.cols; col++)
		{
			Vec3b *c = mat.ptr<Vec3b>(row, col);
			c->val[0] = 0;   //B
			c->val[1] = 255; //G
			c->val[2] = 0; //R
		}
	}
	PrintMs("mat.ptr ms");

	/*使用at函数遍历*/
	try {
		for (int row = 0; row < mat.rows  * 2; row++)
		{
			for (int col = 0; col < mat.cols; col++)
			{
				Vec3b &m = mat.at<Vec3b>(row, col);
				m[0] = 100;
				m[1] = 100;
				m[2] = 100;
				/*mat.at<Vec3b>(row, col)[0] = 255;
				mat.at<Vec3b>(row, col)[1] = 0;
				mat.at<Vec3b>(row, col)[2] = 255;*/
			}
		}
	}
	catch (Exception &ex) /*...*/
	{
		cout << ex.what() << endl;
	}
	PrintMs("mat.at ms");

	namedWindow("mat");
	imshow("mat", mat);
	waitKey(0);
	return 0;
}
