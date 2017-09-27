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
	VideoCapture video;
	video.open("test.mp4");

	if (!video.isOpened()) {
		cout << "open video failed!" << endl;
		getchar();
		return -1;
	}
	
	cout << "open video success" << endl;
	getchar();
	
	namedWindow("video");
	Mat frame;
	for (;;) {
		if (!video.read(frame)){
			break;
		}

		//读帧解码
		//if (!video.grab()) {
		//	break;
		//}

		////颜色空间转换
		//if (!video.retrieve(frame)) {
		//	break;
		//}

		if (frame.empty()) break;
		imshow("video", frame);
		waitKey(30);
	}
	return 0;
}
