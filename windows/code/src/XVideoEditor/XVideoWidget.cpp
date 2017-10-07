#include "XVideoWidget.h"
#include <QPainter>
#include <opencv2/imgproc.hpp>

using namespace cv;
void XVideoWidget::SetImage(Mat mat)
{
	QImage::Format fmt = QImage::Format_RGB888;
	int pixSize = 3;

	//灰度图或者二值化图
	if (mat.type() == CV_8UC1)
	{
		fmt = QImage::Format_Grayscale8;
		pixSize = 1;
	}

	if (img.isNull() || img.format() != fmt) {
		delete img.bits();

		uchar *buf = new uchar[width() * height() * pixSize];
		img = QImage(buf, width(), height(), fmt);
	}

	Mat des;
	cv::resize(mat, des, Size(img.size().width(), img.size().height()));
	if(pixSize > 1){
		cv::cvtColor(des, des, COLOR_BGR2RGB);
	}

	memcpy(img.bits(), des.data, des.cols * des.rows * des.elemSize());
	update();
}

XVideoWidget::XVideoWidget(QWidget *p): QOpenGLWidget(p)
{
}

void XVideoWidget::paintEvent(QPaintEvent *e)
{
	QPainter p;
	p.begin(this);
	p.drawImage(QPoint(0,0), img);
	p.end();
}

XVideoWidget::~XVideoWidget()
{
}
