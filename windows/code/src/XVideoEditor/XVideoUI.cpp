#include "XVideoUI.h"

#include <QFileDialog.h>
#include <QMessageBox.h>
#include <string>
#include "XVideoThread.h"

using namespace std;

XVideoUI::XVideoUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
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