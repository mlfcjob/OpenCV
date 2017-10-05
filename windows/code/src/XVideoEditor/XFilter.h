#pragma once
#include <opencv2/core.hpp>
#include <vector>

enum XTaskType
{
	XTASK_NONE,
	XTASK_GAIN,     //亮度对比度调整
	XTASK_ROTATE90, //旋转图像
	XTASK_ROTATE180,
	XTASK_ROTATE270,
	XTASK_FLIPX,    //视频镜像
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

