#include "XVideoThread.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//1 号视频源
static VideoCapture cap1;
static bool isexit = false;

XVideoThread::XVideoThread()
{
	start();
}


XVideoThread::~XVideoThread()
{
	mutex.lock();
	isexit = true;
	mutex.unlock();
}

void XVideoThread::run()
{
	Mat  mat1;
	for (;;) {
		mutex.lock();
		if (isexit) {
			mutex.unlock();
			break;
		}
		//判断视频是否打开
		if (!cap1.isOpened())
		{
			mutex.unlock();
			msleep(5);
			continue;
		}
       
		//读取一帧视频，解码并颜色转换
		if (!cap1.read(mat1) || mat1.empty())
		{
			mutex.unlock();
			msleep(5);
			continue;
		}

		//显示图像
		ViewImage1(mat1);
		int s = 0;
		s = 1000 / fps;
		msleep(s);
		mutex.unlock();
	
	}
}

bool XVideoThread::Open(const std::string file)
{
	cout << "open " << file << endl;
	mutex.lock();
	bool re = cap1.open(file);
	mutex.unlock();
	cout << re << endl;

	if (!re)
	   return re;

	fps = cap1.get(CAP_PROP_FPS);
	if (fps <= 0)
		fps = 25;

	return true;
}

//返回当前播放位置
double XVideoThread::GetPos()
{
	double pos = 0.0;
	mutex.lock();
	if (!cap1.isOpened()) {
		mutex.unlock();
		return pos;
	}

	int count = cap1.get(CAP_PROP_FRAME_COUNT);
	int cur = cap1.get(CAP_PROP_POS_FRAMES);
	if (count > 0.001)
	   pos = (double)cur / count;
	/*cout << "count :" << count << "cur: " << cur << endl;
	cout << " pos: " << pos << endl;*/
 	mutex.unlock();
	return pos;
}

bool XVideoThread::Seek(double pos) 
{

	double count = cap1.get(CAP_PROP_FRAME_COUNT);
	int frame = count * pos;
	return Seek(frame);
}

//实现视频跳转
//@ para frame int 帧位置
bool XVideoThread::Seek(int frame)
{
	mutex.lock();

	if (!cap1.isOpened()) {
		mutex.unlock();
		return false;
	}

	int re = cap1.set(CAP_PROP_POS_FRAMES, frame);
	mutex.unlock();

	return re;
}

