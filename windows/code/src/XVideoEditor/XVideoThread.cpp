#include "XVideoThread.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//1 ∫≈ ”∆µ‘¥
static VideoCapture cap1;

XVideoThread::XVideoThread()
{
}


XVideoThread::~XVideoThread()
{
}

void XVideoThread::run()
{

}

bool XVideoThread::Open(const std::string file)
{
	cout << "open " << file << endl;
	bool re = cap1.open(file);
	cout << re << endl;

	return re;
}
