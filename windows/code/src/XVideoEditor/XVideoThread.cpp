#include "XVideoThread.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2\videoio.hpp>
#include <iostream>
#include "XFilter.h"

using namespace std;
using namespace cv;

//1 ����ƵԴ
static VideoCapture cap1;

//2 ����ƵԴ
static VideoCapture cap2;

// ������Ƶ
static VideoWriter vw;

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
	wait();
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
		//�ж���Ƶ�Ƿ��
		if (!cap1.isOpened())
		{
			mutex.unlock();
			msleep(5);
			continue;
		}
       
		if (!isPlay) 
		{
			mutex.unlock();
			msleep(5);
			continue;
		}

		//��ȡһ֡��Ƶ�����벢��ɫת��
		if (!cap1.read(mat1) || mat1.empty())
		{
			mutex.unlock();
			//��������βλ�ã�ֹͣ����
			if (isWrite){
				StopSave();
				SaveEnd();
			}
			   
			msleep(5);
			continue;
		}

		//ͨ��������������Ƶ
		Mat mat2 = mark;

		if (cap2.isOpened()) {
			cap2.read(mat2);
		}

		//��ʾͼ��1
		//��ʾͼ��2
		if (!isWrite) 
		{
			ViewImage1(mat1);
			if (!mat2.empty()) {
				ViewImage2(mat2);
			}
		}

		Mat des = XFilter::Get()->Filter(mat1, mat2);
		//��ʾ���ɺ�ͼ��
		if (!isWrite) {
			ViewDes(des);
		}
	
		int s = 0;
		s = 1000 / fps;
		if (isWrite)
		{
			s = 1;
			vw.write(des);
		}

		msleep(s);
		mutex.unlock();
	
	}
}

bool XVideoThread::Open(const std::string file)
{
	cout << "open " << file << endl;
	Seek(0);

	mutex.lock();
	bool re = cap1.open(file);
	mutex.unlock();
	cout << re << endl;

	if (!re)
	   return re;

	fps = cap1.get(CAP_PROP_FPS);
	if (fps <= 0)
		fps = 25;

	width = cap1.get(CAP_PROP_FRAME_WIDTH);
	height = cap1.get(CAP_PROP_FRAME_HEIGHT);

	return true;
}

//�򿪶�����ƵԴ�ļ�
bool XVideoThread::Open2(const std::string file)
{
	cout << "open2 " << file << endl;
	Seek(0);

	mutex.lock();
	bool re = cap2.open(file);
	mutex.unlock();
	cout << re << endl;

	if (!re)
		return re;

	//fps = cap2.get(CAP_PROP_FPS);
	//if (fps <= 0)
	//	fps = 25;

	//width = cap2.get(CAP_PROP_FRAME_WIDTH);
	//height = cap2.get(CAP_PROP_FRAME_HEIGHT);

	return true;
}



void XVideoThread::Play()
{
	mutex.lock();
	isPlay = true;
	mutex.unlock();
}

void XVideoThread::Pause()
{
	mutex.lock();
	isPlay = false;
	mutex.unlock();
}

//���ص�ǰ����λ��
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

//ʵ����Ƶ��ת
//@ para frame int ֡λ��
bool XVideoThread::Seek(int frame)
{
	mutex.lock();

	if (!cap1.isOpened()) {
		mutex.unlock();
		return false;
	}

	int re = cap1.set(CAP_PROP_POS_FRAMES, frame);

	if (cap2.isOpened()) {
		cap2.set(CAP_PROP_POS_FRAMES, frame);
	}

	mutex.unlock();

	return re;
}

//��ʼ������Ƶ
bool XVideoThread::StartSave(const std::string filename, int width, int height, bool isColor)
{
	cout << "��ʼ����" << endl;
	
	Seek(0);

	mutex.lock();
	if (!cap1.isOpened())
	{
		cout << "open video failed " << endl;
		mutex.unlock();
		return false;
	}

	if (width <= 0)
		width = cap1.get(CAP_PROP_FRAME_WIDTH);

	if (height <= 0)
		height = cap1.get(CAP_PROP_FRAME_HEIGHT);

	vw.open(filename, 
		VideoWriter::fourcc('X', '2', '6', '4'),
		this->fps, 
		Size(width, height),
		isColor);

	if (!vw.isOpened())
	{
		cout << "open writer failed" << endl;
		mutex.unlock();
		return false;
	}

	this->isWrite = true;
	mutex.unlock();
	return true;
}

//ֹͣ������Ƶ��д����Ƶ֡������
void XVideoThread::StopSave()
{
	cout << "ֹͣ����" << endl;
	mutex.lock();
	vw.release();
	isWrite = false;
	mutex.unlock();

}

//���ˮӡ
void XVideoThread::SetMark(Mat m)
{
	mutex.lock();
	this->mark = m;
	mutex.unlock();
}
