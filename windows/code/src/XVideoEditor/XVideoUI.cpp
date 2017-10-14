#include "XVideoUI.h"

#include <QFileDialog.h>
#include <QMessageBox.h>
#include <string>
#include "XVideoThread.h"
#include "XFilter.h"
#include "XAudio.h"
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>


using namespace std;
static bool isColor = true;

static bool pressSlider = false;
static bool isExport = false;
static bool isMark = false;
static bool isBlend = false;
static bool isMerge = false;

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

	QObject::connect(XVideoThread::Get(),
		SIGNAL(ViewImage2(cv::Mat)),
		ui.src2video,
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

	//处理音频
	string src = XVideoThread::Get()->src1File;
	string des = XVideoThread::Get()->desFile;

	XAudio::Get()->ExportA(src , src + ".mp3");
	string temp = des + ".avi";
	QFile::remove(temp.c_str());
	QFile::rename(des.c_str(), temp.c_str());
	XAudio::Get()->Merge(temp, src + ".mp3", des);
}

//水印
void XVideoUI::Mark()
{
	isMark = false;
	isBlend = false;
	isMerge = false;

	QString name = QFileDialog::getOpenFileName(this, "select image:");

	if (name.isEmpty()) {
		return;
	}

	std::string file = name.toLocal8Bit().data();
	cv::Mat mark = cv::imread(file);

	if (mark.empty()) 
	{
		return;
	}

	XVideoThread::Get()->SetMark(mark);
	isMark = true;

}

//融合
//暂定水印和融合是冲突的
void XVideoUI::Blend()
{
	isMark = false;
	isBlend = false;
	isMerge = false;

	QString name = QFileDialog::getOpenFileName(this, "select video:");

	if (name.isEmpty()) {
		return;
	}

	std::string file = name.toLocal8Bit().data();

	isBlend = XVideoThread::Get()->Open2(file);
}

//合并
void XVideoUI::Merge()
{
	isMark = false;
	isBlend = false;
	isMerge = false;

	QString name = QFileDialog::getOpenFileName(this, "select video:");

	if (name.isEmpty()) {
		return;
	}

	std::string file = name.toLocal8Bit().data();

	isMerge = XVideoThread::Get()->Open2(file);
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
	int w = ui.width->value();
	int h = ui.height->value();

	if (XVideoThread::Get()->StartSave(filename, w, h, isColor)) 
	{
		isExport = true;
		ui.exportButton->setText("Stop Export");
	}

}

//设置过滤器
void XVideoUI::Set() 
{
	//初始化操作
	XFilter::Get()->Clear();
	isColor = true;


	//视频图像裁剪
	bool  isClip = false;
	double cx = ui.cx->value();
	double cy = ui.cy->value();
	double cw = ui.cw->value();
	double ch = ui.ch->value();

	if (cx + cy + cw + ch > 0.0001)
	{
		isClip = true;
		XFilter::Get()->Add(XTask{ XTASK_CLIP, {cx, cy, cw, ch}});

		double w = XVideoThread::Get()->width;
		double h = XVideoThread::Get()->height;
		XFilter::Get()->Add(XTask{ XTASK_RESIZE, {w, h}});
	}

	//图像金字塔
	bool isPy = false;
	double downCount = (double)ui.pyDown->value();
	double upCount = (double)ui.pyUp->value();

	if (ui.pyDown->value() > 0)
	{
		isPy = true;
		XFilter::Get()->Add(XTask{ XTASK_PYDOWN,{ downCount } });
		
		int w = XVideoThread::Get()->width;
		int h = XVideoThread::Get()->height;
		for (int i = 0; i < downCount; i++) {
			w /= 2;
			h /= 2;
		}

		ui.width->setValue(w);
		ui.height->setValue(h);
	}

	if (ui.pyUp->value() > 0)
	{
		isPy = true;
		XFilter::Get()->Add(XTask{ XTASK_PYUP,{ upCount } });

		int w = XVideoThread::Get()->width;
		int h = XVideoThread::Get()->height;
		for (int i = 0; i < upCount; i++) {
			w *= 2;
			h *= 2;
		}

		ui.width->setValue(w);
		ui.height->setValue(h);
	}


	//调整视频尺寸
	double w = ui.width->value();
	double h = ui.width->value();
	if (!isMerge && isClip && !isPy && ui.width->value() >0  && ui.width->value() > 0 )
	{
		XFilter::Get()->Add(XTask{ XTASK_RESIZE, {w, h}});
	}

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


	//将视频转换为灰度图
	if (ui.color->currentIndex() == 0)
	{
		cout << " RGB " << endl;
	} else if (ui.color->currentIndex() == 1) {
		XFilter::Get()->Add(XTask{ XTASK_GRAY });
		isColor = false;
	} else if(ui.color->currentIndex() == 2){
		XFilter::Get()->Add(XTask{XTASK_BINARY});
		//isColor = false;
	}

	//水印
	if (isMark)
	{
		double x = ui.mx->value();
		double y = ui.my->value();
		double a = ui.ma->value();
		XFilter::Get()->Add(XTask{ XTASK_MARK, {x, y, a} });
	}

	//融合
	if (isBlend) {
		double a = ui.ba->value();
		XFilter::Get()->Add(XTask{ XTASK_BLEND, {a} });
	}

	//合并
	if (isMerge) {
		double h1 = XVideoThread::Get()->height;
		double h2 = XVideoThread::Get()->height2;
		int w = XVideoThread::Get()->width2 * ( h2 / h1 );
		ui.width->setValue(XVideoThread::Get()->width + w);
		ui.height->setValue(h1);
		XFilter::Get()->Add(XTask{XTASK_MERGE});
	}
}

