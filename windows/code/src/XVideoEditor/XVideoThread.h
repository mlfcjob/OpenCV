#pragma once

#include <QThread>

class XVideoThread : public QThread
{
public:
	static XVideoThread  *Get() {
		static XVideoThread vt;
		return &vt;
	}
	//打开一号视频源文件
	bool Open(const std::string file);

	~XVideoThread();

	//线程入口函数
	void run();
protected:
	XVideoThread();
};

