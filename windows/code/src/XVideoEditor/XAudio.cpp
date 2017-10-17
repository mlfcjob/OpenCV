#include "XAudio.h"

#include <iostream>

using namespace std;
#include <qmutex.h>

class CXAuido :public XAudio {
public:
	QMutex  mutex;

	/////////////////////////////////////////
	//������Ƶ�ļ�
	// @para src   string Դ�ļ�(��ý���ļ�)
	// @para out   string  �������Ƶ�ļ���mp3��
	bool ExportA(string src, string out)
	{
		// ffmpeg -i test.mp4 -vn -y out.mp3
		string cmd = "ffmpeg";
		cmd += " -i ";
		cmd += src;
		cmd += " -vn -y ";
		cmd += out;

		cout << cmd << endl;
		mutex.lock();
		system(cmd.c_str());
		mutex.unlock();

		return true;
	}


	///////////////////////////////////////////////////////
	// �ϲ���Ƶ��Ƶ�ļ�
	// @para video   string   ��Ƶ�ļ���avi��
	// @para auido   string   ��Ƶ�ļ���mp3��
	// @para  out    string   �����ý���ļ�(avi)
	bool Merge(std::string video, std::string audio, std::string out)
	{
		// ffmpeg -i lol.mp3 -i lol.avi  -c copy  out.avi
		string cmd = "ffmpeg";
		cmd += " -i ";
		cmd += audio;
		cmd += " -i ";
		cmd += video;
		cmd += " -c copy ";
		cmd += out;

		cout << "Merge: " << cmd << endl;
		mutex.lock();
		system(cmd.c_str());
		mutex.unlock();
		return true;
	}
};

XAudio::XAudio()
{
}


XAudio::~XAudio()
{
}


XAudio* XAudio::Get()
{
	static CXAuido  ca;
	return &ca;
}