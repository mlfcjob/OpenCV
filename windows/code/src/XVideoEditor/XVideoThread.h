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
	//��һ����ƵԴ�ļ�
	bool Open(const std::string file);

	//�򿪶�����ƵԴ�ļ�
	bool Open2(const std::string file);

	void Play();
	void Pause();

	//���ص�ǰ���ŵ�λ��
	double GetPos();

	//ʵ����Ƶ��ת
	//@ para frame int ֡λ��
	bool Seek(int frame);
	bool Seek(double pos);

	//��ʼ������Ƶ
	bool StartSave(const std::string filename ,int width = 0, int height = 0, bool isColor=true);

	//ֹͣ������Ƶ��д����Ƶ֡������
	void StopSave();

	//���ˮӡ
	void SetMark(cv::Mat mark);

	~XVideoThread();

	//�߳���ں���
	void run();

signals:
	//��ʾԴ��Ƶ1ͼ��
	void ViewImage1(cv::Mat mat);
	
	//��ʾԴ��Ƶ2ͼ��
	void ViewImage2(cv::Mat mat);

	//��ʾ����Ŀ��ͼ��
	void ViewDes(cv::Mat mat);

	//����ֹͣ
	void SaveEnd();

protected:
	XVideoThread();
	QMutex  mutex;
	cv::Mat  mark;

	//�Ƿ�ʼд��Ƶ
	bool isWrite = false;
	bool isPlay  = false;
};

