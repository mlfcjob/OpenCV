#pragma once

#include <string>

class XAudio
{
public:
	static XAudio *Get();

	////////////////////////////////////////////////////////
	//������Ƶ�ļ�
	// @para src   string Դ�ļ�(��ý���ļ�)
	// @para out   string  �������Ƶ�ļ���mp3��
	virtual bool ExportA(std::string src, std::string out) = 0;

	///////////////////////////////////////////////////////
	// �ϲ���Ƶ��Ƶ�ļ�
	// @para video   string   ��Ƶ�ļ���avi��
	// @para auido   string   ��Ƶ�ļ���mp3��
	// @para  out    string   �����ý���ļ�(avi)
	virtual bool Merge(std::string video, std::string audio, std::string out) = 0;

	XAudio();
	virtual ~XAudio();
};

