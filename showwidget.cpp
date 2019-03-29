#include <showwidget.h>
ShowWidget::ShowWidget(QWidget *parent) : QWidget(parent)
{
	//新建一个水平分割窗口对象，作为主布局框
	splitterMain = new QSplitter(Qt::Horizontal, 0);
	splitterMain->setOpaqueResize(true);
	splitterLeft = new QSplitter(Qt::Vertical, splitterMain);
	list = new QListWidget(splitterLeft);
	list->insertItem(0, QString::fromLocal8Bit("单个面板"));
	list->insertItem(1, QString::fromLocal8Bit("两个面板"));
	list->insertItem(2, QString::fromLocal8Bit("四个面板"));
	list->setMaximumSize(QSize(150, 600));
	hitLabel = new QLabel(QString::fromLocal8Bit("提示信息"), splitterLeft);
	textEdit = new QTextEdit(QString::fromLocal8Bit("相关操作提示"), splitterLeft);
	practice = new CenterWidget(splitterMain);
	QObject::connect(list, SIGNAL(currentRowChanged(int)), practice->stack, SLOT(setCurrentIndex(int)));
	fileview = new FileWidget(splitterMain);
	//设置主布局框即水平分割窗口的标题
	//splitterMain->setWindowTitle(QString::fromLocal8Bit("3D图形软件"));
	//设置主布局框即水平分割窗口的最小尺寸
	//splitterMain->setMinimumSize(splitterMain->minimumSize());
	//设置主布局框即水平分割窗口的最大尺寸
	//splitterMain->setMaximumSize(splitterMain->maximumSize());
	//splitterMain->show();
	horizontalLayout = new QHBoxLayout(this);
	horizontalLayout->addWidget(splitterMain);
}