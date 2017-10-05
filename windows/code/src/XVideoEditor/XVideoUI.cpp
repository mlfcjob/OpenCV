#include "XVideoUI.h"

#include <QFileDialog.h>
#include <QMessageBox.h>
#include <string>
#include "XVideoThread.h"
#include "XFilter.h"
#include <iostream>
using namespace std;

static bool pressSlider = false;
static bool isExport = false;

XVideoUI::XVideoUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);

	qRegisterMetaType<cv::Mat>("cv::Mat");

	//原始视频显示信号
	QObject::connect(XVideoThread::Get(),
		SIGNAL(ViewImage1(cv::Mat)),
		ui.src1video,
		SLOT(SetImage(cv::Mat)));

	//输出视频显示信号
	QObject::connect(XVideoThread::Get(),
		SIGNAL(ViewDes(cv::Mat)),
		ui.des,
		SLOT(SetImage(cv::Mat)));

	//导出视频结束
	QObject::connect(XVideoThread::Get(),
		SIGNAL(SaveEnd()),
		this,
		SLOT(ExportEnd()));

	Pause();

	startTimer(40);
}


//播放
void XVideoUI::Play()
{
	ui.pauseButton->show();
	XVideoThread::Get()->Play();
	ui.pauseButton->setGeometry(ui.playButton->geometry());
	ui.playButton->hide();
}

//暂停
void XVideoUI::Pause()
{
	ui.playButton->show();
	XVideoThread::Get()->Pause();
	//ui.pauseButton->setGeometry(ui.playButton->geometry());
	ui.pauseButton->hide();

}

void XVideoUI::open()
{
	QString name = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("请选择视频文件"));

	if (name.isEmpty())
		return;

	string  file = name.toLocal8Bit().data();

	//QMessageBox::information(this, "", name);

	if (!XVideoThread::Get()->Open(file))
	{
		QMessageBox::information(this, "error", name + "open failed!");
		return;
	}
}

void XVideoUI::timerEvent(QTimerEvent *e)
{
	if (pressSlider)
		return;

	double pos = XVideoThread::Get()->GetPos();
	ui.playSlider->setValue(pos * 1000);
}

void XVideoUI::SliderPress()
{
	pressSlider = true;
}

void XVideoUI::SliderRelease()
{
	pressSlider = false;
}

// 滑动条拖动
void XVideoUI::SetPos(int pos) 
{
	XVideoThread::Get()->Seek((float)pos / 1000.0);
}

//停止导出视频
void  XVideoUI::ExportEnd()
{
	isExport = false;

	ui.exportButton->setText("Start Export");
}

//导出视频
void XVideoUI::Export()
{


	if (isExport) {
		//要停止导出
		XVideoThread::Get()->StopSave();
		isExport = false;
		ui.exportButton->setText("Start Export");
		return;
	}

	//开始导出
	QString name = QFileDialog::getSaveFileName(this, "save", "out1.avi");
	if (name.isEmpty())
		return;

	std::string filename = name.toLocal8Bit().data();

	if (XVideoThread::Get()->StartSave(filename)) 
	{
		isExport = true;
		ui.exportButton->setText("Stop Export");
	}

}

//设置过滤器
void XVideoUI::Set() 
{
	XFilter::Get()->Clear();

	//对比度和亮度
	if (ui.bright->value() > 0 || ui.contrast->value() > 1.0) {
		XFilter::Get()->Add(XTask{ XTASK_GAIN, 
		            {(double)ui.bright->value(), ui.contrast->value()}});

	}

	//图像旋转 0:NONE, 1:90, 2:180, 3:270
	if(ui.rotate->currentIndex() == 1) {
		XFilter::Get()->Add(XTask{ XTASK_ROTATE90 });
	} 
	else if (ui.rotate->currentIndex() == 2) {
		XFilter::Get()->Add(XTask{ XTASK_ROTATE180 });
	}
	else if (ui.rotate->currentIndex() == 3) {
		XFilter::Get()->Add(XTask{ XTASK_ROTATE270 });
    }

	//图像镜像, 0: NONE 1: x, 2: y, 3:xy
	if(ui.flip->currentIndex() == 1) {
		XFilter::Get()->Add(XTask{ XTASK_FLIPX });
	}
	else if (ui.flip->currentIndex() == 2) {
		XFilter::Get()->Add(XTask{ XTASK_FLIPY });
	}
	else if (ui.flip->currentIndex() == 3) {
		XFilter::Get()->Add(XTask{ XTASK_FLIPXY });
	}
}

