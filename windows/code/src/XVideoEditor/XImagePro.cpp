#include "XImagePro.h"



XImagePro::XImagePro()
{
}


XImagePro::~XImagePro()
{
}

//设置原图，会清理之前的处理结果
void XImagePro::Set(cv::Mat mat1, cv::Mat mat2) 
{
	if (mat1.empty())
		return;

	this->src1 = mat1;
	this->src2 = mat2;

	this->src1.copyTo(des);
}

//设置亮度和对比度
// @para  bright  double   亮度  0 ~ 100
// @para  contrast  int   对比度 1.0 ~ 3.0
void  XImagePro::Gain(double bright, double contrast)
{
	if (des.empty())
		return;

	des.convertTo(des, -1, contrast, bright);

}

//对视频图像进行旋转

void XImagePro::Rotate90()
{
	if (des.empty() || this->src1.empty())
		return;

	cv::rotate(this->src1, des, cv::ROTATE_90_CLOCKWISE);
}

void XImagePro::Rotate180()
{
	if (des.empty())
		return;

	cv::rotate(des, des, cv::ROTATE_180);
}

void XImagePro::Rotate270()
{
	if (des.empty())
		return;

	cv::rotate(des, des, cv::ROTATE_90_COUNTERCLOCKWISE);
}



//视频镜像

//左右镜像
void XImagePro::FlipX()
{
	if (des.empty())
		return;
	cv::flip(des, des, 0);
}

//上下镜像
void XImagePro::FlipY()
{
	if (des.empty())
		return;
	cv::flip(des, des, 1);
}

//上下左右镜像
void XImagePro::FlipXY()
{
	if (des.empty())
		return;
	cv::flip(des, des, -1);
}

//调整视频大小
// para width int  调整后的宽
// para height int  调整后的高
void XImagePro::Resize(int width, int height)
{
	if (des.empty())
		return;

	cv::resize(des, des, cv::Size(width, height), 0, 0, cv::INTER_NEAREST);
}


//图像金字塔
void XImagePro::PyDown(int count)   //高斯金字塔
{
	if (des.empty())
		return;

	for (int i = 0; i < count; i++) {
		cv::pyrDown(des, des);
	}
}

void XImagePro::PyUp(int count)   //拉普拉斯金字塔
{
	if (des.empty())
		return;

	for (int i = 0; i < count; i++) {
		cv::pyrUp(des, des);
	}
}