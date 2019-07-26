#pragma once
#include <qwidget.h>
#include <qsplitter.h>
#include <qlistwidget.h>
#include <qlabel.h>
#include <qtextedit.h>
#include <FileWidget.h>
#include <CenterWidget.h>

//CTK
#include <ctkTransferFunction.h>
#include <ctkVTKColorTransferFunction.h>
#include <ctkTransferFunctionView.h>
#include <ctkTransferFunctionGradientItem.h>
#include <ctkTransferFunctionControlPointsItem.h>
#include <ctkVTKVolumePropertyWidget.h>
#include <ctkVTKTextPropertyWidget.h>

class ShowWidget : public QWidget
{
	Q_OBJECT
public:
	explicit ShowWidget(QWidget *parent = 0);
	CenterWidget *practice;
	QSplitter *splitterMain;
	QListWidget *list;
	QHBoxLayout *horizontalLayout;
	QSplitter *splitterLeft;
	QLabel *hitLabel;
	QTextEdit *textEdit;
	FileWidget *fileview;
	//体绘数据的控制模板
	ctkVTKVolumePropertyWidget *volumePropertywidget;
};
