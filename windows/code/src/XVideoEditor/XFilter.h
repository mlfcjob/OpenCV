#pragma once
#include <opencv2/core.hpp>
#include <vector>

enum XTaskType
{
	XTASK_NONE,
	XTASK_GAIN,     //���ȶԱȶȵ���
	XTASK_ROTATE90, //��תͼ��
	XTASK_ROTATE180,
	XTASK_ROTATE270,
	XTASK_FLIPX,    //��Ƶ����
	XTASK_FLIPY,
	XTASK_FLIPXY,
};
struct XTask {
	XTaskType type;
	std::vector<double> para;
};

class XFilter
{
public:
	virtual  cv::Mat Filter(cv::Mat mat1, cv::Mat mat2) = 0;
	virtual  void Add(XTask task) = 0;
	virtual  void Clear() = 0;
	static XFilter  *Get();
	XFilter();
	virtual ~XFilter();
protected:

};

