#include "XVideoUI.h"
#include <QtWidgets/QApplication>

//#include "XAudio.h"

int main(int argc, char *argv[])
{
	//XAudio::Get()->ExportA("lx.mp4", "3.mp3");
	//XAudio::Get()->Merge("lx.avi", "3.mp3", "merge.avi");

	QApplication a(argc, argv);
	XVideoUI w;
	w.show();
	return a.exec();
}
