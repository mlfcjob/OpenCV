#pragma once

#include <QtWidgets/QWidget>
#include "ui_XVideoUI.h"

class XVideoUI : public QWidget
{
	Q_OBJECT

public:
	XVideoUI(QWidget *parent = Q_NULLPTR);
	void timerEvent(QTimerEvent *e);

public slots:
    void open();
	//����
	void Play();
	//��ͣ
	void Pause();

	void SliderPress();
	void SliderRelease();
	// �������϶�
	void SetPos(int);
	//���ù�����
	void Set();

	//������Ƶ
	void Export();
	//ֹͣ������Ƶ
	void ExportEnd();
	
	//ˮӡ
	void Mark();

	//�ں�
	void Blend();

	//�ϲ�
	void Merge();

private:
	Ui::XVideoUIClass ui;
};
