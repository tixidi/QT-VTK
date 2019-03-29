#pragma once
#include <qwidget.h>
#include <QLineEdit>
#include <QListWidget>
#include <QVBoxLayout>
#include <QDir>
#include <qpushbutton.h>
#include <QListWidgetItem>
#include <QFileInfoList>
class FileWidget : public QWidget
{
	Q_OBJECT
public:
	explicit FileWidget(QWidget *parent = 0);
	void createBase();
	void showFileInfoList(QFileInfoList list);
public slots:
	void slotShow(QDir dir);
	void slotchanged();
	void slotDirShow(QListWidgetItem * item);
public:
	QLineEdit * fileLineEdit;
	QListWidget *fileListWidget;
	QPushButton *RenderButton;
	QVBoxLayout *mainLayout;
};
