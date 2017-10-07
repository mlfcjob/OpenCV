#include "XImagePro.h"



XImagePro::XImagePro()
{
}


XImagePro::~XImagePro()
{
}

//����ԭͼ��������֮ǰ�Ĵ�����
void XImagePro::Set(cv::Mat mat1, cv::Mat mat2) 
{
	if (mat1.empty())
		return;

	this->src1 = mat1;
	this->src2 = mat2;

	this->src1.copyTo(des);
}

//�������ȺͶԱȶ�
// @para  bright  double   ����  0 ~ 100
// @para  contrast  int   �Աȶ� 1.0 ~ 3.0
void  XImagePro::Gain(double bright, double contrast)
{
	if (des.empty())
		return;

	des.convertTo(des, -1, contrast, bright);

}

//����Ƶͼ�������ת

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



//��Ƶ����

//���Ҿ���
void XImagePro::FlipX()
{
	if (des.empty())
		return;
	cv::flip(des, des, 0);
}

//���¾���
void XImagePro::FlipY()
{
	if (des.empty())
		return;
	cv::flip(des, des, 1);
}

//�������Ҿ���
void XImagePro::FlipXY()
{
	if (des.empty())
		return;
	cv::flip(des, des, -1);
}

//������Ƶ��С
// para width int  ������Ŀ�
// para height int  ������ĸ�
void XImagePro::Resize(int width, int height)
{
	if (des.empty())
		return;

	cv::resize(des, des, cv::Size(width, height), 0, 0, cv::INTER_NEAREST);
}


//ͼ�������
void XImagePro::PyDown(int count)   //��˹������
{
	if (des.empty())
		return;

	for (int i = 0; i < count; i++) {
		cv::pyrDown(des, des);
	}
}

//������˹������
void XImagePro::PyUp(int count)  
{
	if (des.empty())
		return;

	for (int i = 0; i < count; i++) {
		cv::pyrUp(des, des);
	}
}

//ROI,����ü�
void XImagePro::Clip(int x, int y, int width, int height)
{
	if (des.empty())
		return;

	if (x < 0 || y < 0 || width <= 0 || height <= 0)
		return;

	if (x > des.cols || y > des.rows)
		return;

	des = des(cv::Rect(x, y, width, height));
}

//ת��Ϊ�Ҷ�ͼ
void XImagePro::Gray()
{
	if (des.empty())
		return;

	cv::cvtColor(des, des, cv::COLOR_BGR2GRAY);
}

//ת��Ϊ��ֵ��ͼ
void XImagePro::Binary()
{
	if (des.empty())
		return;

	cv::threshold(des, des, 100, 255, cv::THRESH_BINARY_INV/*cv::THRESH_BINARY_INV*/);

}

//��Ƶ���ˮӡ
void XImagePro::Mark(int x, int y, double alpha)
{
	if (des.empty())
		return;
	
	if (src2.empty())
		return;

	cv::Mat roi = des(cv::Rect(x,y, src2.cols, src2.rows));

	cv::addWeighted(src2, alpha, roi, 1 - alpha, 0, roi);
}