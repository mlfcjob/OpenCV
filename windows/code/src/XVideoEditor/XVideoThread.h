#pragma once

#include <QThread>
#include <qmutex.h>
#include <opencv2/core.hpp>

class XVideoThread : public QThread
{
	Q_OBJECT

public:
	int fps = 0;
	int width = 0;
	int height = 0;
	static XVideoThread  *Get() {
		static XVideoThread vt;
		return &vt;
	}
	//打开一号视频源文件
	bool Open(const std::string file);

	//打开二号视频源文件
	bool Open2(const std::string file);

	void Play();
	void Pause();

	//返回当前播放的位置
	double GetPos();

	//实现视频跳转
	//@ para frame int 帧位置
	bool Seek(int frame);
	bool Seek(double pos);

	//开始保存视频
	bool StartSave(const std::string filename ,int width = 0, int height = 0, bool isColor=true);

	//停止保存视频，写入视频帧的索引
	void StopSave();

	//添加水印
	void SetMark(cv::Mat mark);

	~XVideoThread();

	//线程入口函数
	void run();

signals:
	//显示源视频1图像
	void ViewImage1(cv::Mat mat);
	
	//显示源视频2图像
	void ViewImage2(cv::Mat mat);

	//显示生成目标图像
	void ViewDes(cv::Mat mat);

	//导出停止
	void SaveEnd();

protected:
	XVideoThread();
	QMutex  mutex;
	cv::Mat  mark;

	//是否开始写视频
	bool isWrite = false;
	bool isPlay  = false;
};

