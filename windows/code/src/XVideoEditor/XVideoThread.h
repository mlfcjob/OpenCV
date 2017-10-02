#pragma once

#include <QThread>
#include <qmutex.h>
#include <opencv2/core.hpp>

class XVideoThread : public QThread
{
	Q_OBJECT

public:
	int fps = 0;
	static XVideoThread  *Get() {
		static XVideoThread vt;
		return &vt;
	}
	//��һ����ƵԴ�ļ�
	bool Open(const std::string file);

	//���ص�ǰ���ŵ�λ��
	double GetPos();

	//ʵ����Ƶ��ת
	//@ para frame int ֡λ��
	bool Seek(int frame);
	bool Seek(double pos);
	~XVideoThread();

	//�߳���ں���
	void run();

signals:
	void ViewImage1(cv::Mat mat);

protected:
	XVideoThread();
	QMutex  mutex;
};

