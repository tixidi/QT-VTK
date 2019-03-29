#include "CenterWidget.h"

CenterWidget::CenterWidget(QWidget *parent)
	: QFrame(parent)
{
	stack = new QStackedWidget(this);
	//对堆栈窗口的显示风格进行设置
	stack->setFrameStyle(QFrame::Panel | QFrame::Raised);
	/* 插入三个页面 */
	onewidget = new OneWidget();
	twowidget = new TwoWidget();
	fourwidget = new FourWidget();
	stack->addWidget(onewidget);
	stack->addWidget(twowidget);
	stack->addWidget(fourwidget);
	/* 创建两个按钮 */
	ToolBtn = new QPushButton(QString::fromLocal8Bit("工具箱"));
	CloseBtn = new QPushButton(QString::fromLocal8Bit("关闭"));
	QHBoxLayout *BtnLayout = new QHBoxLayout;
	BtnLayout->addStretch(1);
	BtnLayout->addWidget(ToolBtn);
	BtnLayout->addWidget(CloseBtn);
	/* 进行整体布局 */
	QVBoxLayout *RightLayout = new QVBoxLayout(this);
	RightLayout->setMargin(10);
	RightLayout->setSpacing(6);
	RightLayout->addWidget(stack);
	RightLayout->addLayout(BtnLayout);
}
