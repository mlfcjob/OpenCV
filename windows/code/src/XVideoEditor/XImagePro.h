#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

class XImagePro
{
public:
	//设置原图，会清理之前的处理结果
	void Set(cv::Mat mat1, cv::Mat mat2);

	//设置亮度和对比度
	// @para  bright  double   亮度  0 ~ 100
	// @para  contrast  int   对比度 1.0 ~ 3.0
	void Gain(double bright, double contrast);

    //图像旋转
	void Rotate90();
	void Rotate180();
	void Rotate270();

	//视频镜像
	void FlipX();
	void FlipY();
	void FlipXY();

	//调整视频大小
	// para width int  调整后的宽
	// para height int  调整后的高
	void Resize(int width, int height);


	//图像金字塔
	void PyDown(int count);    //高斯金字塔
	void PyUp(int count);      //拉普拉斯金字塔

	//获取处理后结果
	cv::Mat Get() { return des; }
	XImagePro();
	~XImagePro();
private:
	//原图
	cv::Mat  src1, src2;

	//目标图
	cv::Mat  des;
};

