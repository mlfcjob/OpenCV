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
	VideoCapture cam(0);

	if (!cam.isOpened()) {
		cout << " cam open failed " << endl;
		getchar();
		return -1;
	}

	cout << " cam open success. " << endl;

	namedWindow("cam");

	Mat img;
	VideoWriter vw;
	int fps = cam.get(CAP_PROP_FPS);
	if (fps <= 0) {
		fps = 25;
	}

	vw.open("out.avi", VideoWriter::fourcc('X','2', '6', '4'), 
		fps,
		Size(cam.get(CAP_PROP_FRAME_WIDTH),
			cam.get(CAP_PROP_FRAME_HEIGHT)),
		true);

	if (!vw.isOpened())
	{
		cout << " video open failed " << endl;
		getchar();
		return -1;
	}

	cout << " video open success " << endl;

	for (;;) {
		cam.read(img);
		if (img.empty()) break;

		imshow("cam", img);
		vw.write(img);

		if (waitKey(5) == 'q')
			break;
	
	}
	waitKey();

	return 0;
}
