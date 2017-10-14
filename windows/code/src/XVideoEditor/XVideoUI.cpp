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

	//ԭʼ��Ƶ��ʾ�ź�
	QObject::connect(XVideoThread::Get(),
		SIGNAL(ViewImage1(cv::Mat)),
		ui.src1video,
		SLOT(SetImage(cv::Mat)));

	QObject::connect(XVideoThread::Get(),
		SIGNAL(ViewImage2(cv::Mat)),
		ui.src2video,
		SLOT(SetImage(cv::Mat)));

	//�����Ƶ��ʾ�ź�
	QObject::connect(XVideoThread::Get(),
		SIGNAL(ViewDes(cv::Mat)),
		ui.des,
		SLOT(SetImage(cv::Mat)));

	//������Ƶ����
	QObject::connect(XVideoThread::Get(),
		SIGNAL(SaveEnd()),
		this,
		SLOT(ExportEnd()));

	Pause();

	startTimer(40);
}


//����
void XVideoUI::Play()
{
	ui.pauseButton->show();
	XVideoThread::Get()->Play();
	ui.pauseButton->setGeometry(ui.playButton->geometry());
	ui.playButton->hide();
}

//��ͣ
void XVideoUI::Pause()
{
	ui.playButton->show();
	XVideoThread::Get()->Pause();
	//ui.pauseButton->setGeometry(ui.playButton->geometry());
	ui.pauseButton->hide();

}

void XVideoUI::open()
{
	QString name = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("��ѡ����Ƶ�ļ�"));

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

// �������϶�
void XVideoUI::SetPos(int pos) 
{
	XVideoThread::Get()->Seek((float)pos / 1000.0);
}

//ֹͣ������Ƶ
void  XVideoUI::ExportEnd()
{
	isExport = false;
	ui.exportButton->setText("Start Export");

	//������Ƶ
	string src = XVideoThread::Get()->src1File;
	string des = XVideoThread::Get()->desFile;

	XAudio::Get()->ExportA(src , src + ".mp3");
	string temp = des + ".avi";
	QFile::remove(temp.c_str());
	QFile::rename(des.c_str(), temp.c_str());
	XAudio::Get()->Merge(temp, src + ".mp3", des);
}

//ˮӡ
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

//�ں�
//�ݶ�ˮӡ���ں��ǳ�ͻ��
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

//�ϲ�
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

//������Ƶ
void XVideoUI::Export()
{
	if (isExport) {
		//Ҫֹͣ����
		XVideoThread::Get()->StopSave();
		isExport = false;
		ui.exportButton->setText("Start Export");
		return;
	}

	//��ʼ����
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

//���ù�����
void XVideoUI::Set() 
{
	//��ʼ������
	XFilter::Get()->Clear();
	isColor = true;


	//��Ƶͼ��ü�
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

	//ͼ�������
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


	//������Ƶ�ߴ�
	double w = ui.width->value();
	double h = ui.width->value();
	if (!isMerge && isClip && !isPy && ui.width->value() >0  && ui.width->value() > 0 )
	{
		XFilter::Get()->Add(XTask{ XTASK_RESIZE, {w, h}});
	}

	//�ԱȶȺ�����
	if (ui.bright->value() > 0 || ui.contrast->value() > 1.0) {
		XFilter::Get()->Add(XTask{ XTASK_GAIN, 
		            {(double)ui.bright->value(), ui.contrast->value()}});

	}

	//ͼ����ת 0:NONE, 1:90, 2:180, 3:270
	if(ui.rotate->currentIndex() == 1) {
		XFilter::Get()->Add(XTask{ XTASK_ROTATE90 });
	} 
	else if (ui.rotate->currentIndex() == 2) {
		XFilter::Get()->Add(XTask{ XTASK_ROTATE180 });
	}
	else if (ui.rotate->currentIndex() == 3) {
		XFilter::Get()->Add(XTask{ XTASK_ROTATE270 });
    }

	//ͼ����, 0: NONE 1: x, 2: y, 3:xy
	if(ui.flip->currentIndex() == 1) {
		XFilter::Get()->Add(XTask{ XTASK_FLIPX });
	}
	else if (ui.flip->currentIndex() == 2) {
		XFilter::Get()->Add(XTask{ XTASK_FLIPY });
	}
	else if (ui.flip->currentIndex() == 3) {
		XFilter::Get()->Add(XTask{ XTASK_FLIPXY });
	}


	//����Ƶת��Ϊ�Ҷ�ͼ
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

	//ˮӡ
	if (isMark)
	{
		double x = ui.mx->value();
		double y = ui.my->value();
		double a = ui.ma->value();
		XFilter::Get()->Add(XTask{ XTASK_MARK, {x, y, a} });
	}

	//�ں�
	if (isBlend) {
		double a = ui.ba->value();
		XFilter::Get()->Add(XTask{ XTASK_BLEND, {a} });
	}

	//�ϲ�
	if (isMerge) {
		double h1 = XVideoThread::Get()->height;
		double h2 = XVideoThread::Get()->height2;
		int w = XVideoThread::Get()->width2 * ( h2 / h1 );
		ui.width->setValue(XVideoThread::Get()->width + w);
		ui.height->setValue(h1);
		XFilter::Get()->Add(XTask{XTASK_MERGE});
	}
}

