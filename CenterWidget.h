#pragma once

#include <QtWidgets/QDialog>
#include <qmainwindow.h>
#include <qframe.h>
#include <qstackedwidget.h>
#include <qpushbutton.h>
#include <OneWidget.h>
#include <TwoWidget.h>
#include <FourWidget.h>

//class VTKPractice : public QFrame
class CenterWidget : public QFrame
{
	Q_OBJECT

public:
	CenterWidget(QWidget *parent = 0);

	QStackedWidget *stack;
	QPushButton *ToolBtn;
	QPushButton *CloseBtn;
	OneWidget *onewidget;
	TwoWidget *twowidget;
	FourWidget *fourwidget; 

private:
	
};
