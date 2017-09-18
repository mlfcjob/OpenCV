#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_testqtmat.h"

class testqtmat : public QMainWindow
{
	Q_OBJECT

public:
	testqtmat(QWidget *parent = Q_NULLPTR);

private:
	Ui::testqtmatClass ui;
};
