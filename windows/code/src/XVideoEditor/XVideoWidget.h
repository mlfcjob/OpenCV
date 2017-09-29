#pragma once

#include <QOpenGLWidget>
class XVideoWidget : public QOpenGLWidget
{
public:
	XVideoWidget(QWidget  *p);

	void paintEvent(QPaintEvent *e);
	virtual ~XVideoWidget();
};

