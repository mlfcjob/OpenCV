#pragma once

#include <string>

class XAudio
{
public:
	static XAudio *Get();

	////////////////////////////////////////////////////////
	//导出音频文件
	// @para src   string 源文件(多媒体文件)
	// @para out   string  输出的音频文件（mp3）
	// @para beginMs  int   开始的音频位置（毫秒）
	// @para outMs    int   输出音频时常
	virtual bool ExportA(std::string src, std::string out, int beginMs = 0, int outMs = 0) = 0;

	///////////////////////////////////////////////////////
	// 合并视频音频文件
	// @para video   string   视频文件（avi）
	// @para auido   string   音频文件（mp3）
	// @para  out    string   输出的媒体文件(avi)
	virtual bool Merge(std::string video, std::string audio, std::string out) = 0;

	XAudio();
	virtual ~XAudio();
};

