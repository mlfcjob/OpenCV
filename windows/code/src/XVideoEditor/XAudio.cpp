#include "XAudio.h"

#include <iostream>

using namespace std;
#include <qmutex.h>

struct XTime
{
	XTime(int tms) {
		h = (tms / 1000) / 3600;
		m = ((tms / 1000) % 3600) / 60;
		s = (tms  / 1000) % (3600 * 60);
		ms = tms % 1000;
	}

	std::string ToString() {
		char buf[16] = { 0 };
		sprintf(buf,"%d:%d:%d.%d", h, m, s, ms);
		return buf;
	}

	int h = 0;
	int m = 0;
	int s = 0;
	int ms = 0;
};

class CXAuido :public XAudio {
public:
	QMutex  mutex;

	/////////////////////////////////////////
	//导出音频文件
	// @para src   string 源文件(多媒体文件)
	// @para out   string  输出的音频文件（mp3）
	bool ExportA(string src, string out, int beginMs, int outMs)
	{
		// ffmpeg -i -ss 0:1:10.112 -t 0:1:10.100 test.mp4 -vn -y out.mp3
		string cmd = "ffmpeg";
		cmd += " -i ";
		cmd += src;
		cmd += " -vn -y ";
		cmd += out;
		cmd += " ";

		if (beginMs > 0) {
			cmd += " -ss ";
			XTime xt(beginMs);
			cmd += xt.ToString();
		}

		if (outMs > 0) {
			cmd + " -t ";
			XTime xt(outMs);
			cmd += xt.ToString();
		}
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