#pragma once
#include <qwidget.h>
#include <qsplitter.h>
#include <qlistwidget.h>
#include <qlabel.h>
#include <qtextedit.h>
#include <FileWidget.h>
#include <CenterWidget.h>
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
};
