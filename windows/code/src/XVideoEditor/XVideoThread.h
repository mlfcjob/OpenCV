#pragma once

#include <QThread>

class XVideoThread : public QThread
{
public:
	static XVideoThread  *Get() {
		static XVideoThread vt;
		return &vt;
	}
	//��һ����ƵԴ�ļ�
	bool Open(const std::string file);

	~XVideoThread();

	//�߳���ں���
	void run();
protected:
	XVideoThread();
};

