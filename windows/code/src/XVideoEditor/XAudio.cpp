#include "XAudio.h"

#include <iostream>

using namespace std;
#include <qmutex.h>

class CXAuido :public XAudio {
public:
	QMutex  mutex;

	/////////////////////////////////////////
	//导出音频文件
	// @para src   string 源文件(多媒体文件)
	// @para out   string  输出的音频文件（mp3）
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
	// 合并视频音频文件
	// @para video   string   视频文件（avi）
	// @para auido   string   音频文件（mp3）
	// @para  out    string   输出的媒体文件(avi)
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