#pragma once
#include <qwidget.h>
#include <qlabel.h>
#include <qtextedit.h>
#include <qimage.h>
class ImageAndText : public QWidget
{
	Q_OBJECT
public:
	explicit ImageAndText(QWidget *parent = 0);
	QImage img;
	QLabel *imageLabel;
	QTextEdit *text;
signals:
	public slots:
};
