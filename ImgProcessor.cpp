#include <ImgProcessor.h>
#include <qfiledialog.h>
#include <qcolordialog.h>
#include <qcolor.h>
#include <qtextstream.h>
#include <QtPrintSupport\qprintdialog.h>
#include <QtPrintSupport\qprinter.h>
#include <qpainter.h>
ImgProcessor::ImgProcessor(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowTitle(tr("Easy Word"));         //设置窗体标题
	imagetext = new ImageAndText(this);
    setCentralWidget(imagetext);
	createFontBars();
	createlistBars();
	/* 创建动作、菜单、工具栏的函数 */
	createActions();
	createMenus();
	createToolBars();
	if (img.load("Icons/image.png"))
	{
		//在imageLabel对象中放置图像
		imagetext->imageLabel->setPixmap(QPixmap::fromImage(img));
	}
	connect(imagetext->text, SIGNAL(ShowCurrentFormatChanged(QTextCharFormat&)), this,
		SLOT(ShowCurrentFormatChanged(QTextCharFormat &)));
	connect(imagetext->text->document(), SIGNAL(undoAvailable(bool)), redoAction, SLOT(setEnabled(bool)));
	connect(imagetext->text->document(), SIGNAL(redoAvailable(bool)), redoAction, SLOT(setEnabled(bool)));
	connect(imagetext->text, SIGNAL(ShowCursorPositionChanged()), this, SLOT(ShowCursorPositionChanged()));
}

void ImgProcessor::createlistBars()
{
	//排序
	listLabel = new QLabel(QString::fromLocal8Bit("排序"));
	listComboBox = new QComboBox();
	listComboBox->addItem("Standard");
	listComboBox->addItem("QTextListFormat::ListDisc");
	listComboBox->addItem("QTextListFormat::ListCircle");
	listComboBox->addItem("QTextListFormat::ListSquare");
	listComboBox->addItem("QTextListFormat::ListDecimal");
	listComboBox->addItem("QTextListFormat::ListLowerAlpha");
	listComboBox->addItem("QTextListFormat::ListUpperAlpha");
	listComboBox->addItem("QTextListFormat::ListLowerRoman");
	listComboBox->addItem("QTextListFormat::ListUpperRoman");
	connect(listComboBox, SIGNAL(activated(int)), this, SLOT(ShowList(int)));
}

void ImgProcessor::createFontBars()
{
	//设置字体
	fontLabel1 = new QLabel(QString::fromLocal8Bit("字体:"));
	fontComboBox = new QFontComboBox();
	fontComboBox->setFontFilters(QFontComboBox::ScalableFonts);
	connect(fontComboBox, SIGNAL(activated(QString)), this, SLOT(ShowFontComboBox(QString)));
	fontLabel2 = new QLabel(QString::fromLocal8Bit("字号:"));
	sizeComboBox = new QComboBox();
	QFontDatabase db;
	foreach(int size, db.standardSizes())
		sizeComboBox->addItem(QString::number(size));
	connect(sizeComboBox, SIGNAL(activated(QString)), this, SLOT(ShowSizeSpinBox(QString)));
	boldBtn = new QToolButton();
	boldBtn->setIcon(QIcon("Icons/bold.png"));
	boldBtn->setCheckable(true);
	connect(boldBtn, SIGNAL(clicked()), this, SLOT(ShowBoldBtn()));
	italicBtn = new QToolButton();
	italicBtn->setIcon(QIcon("Icons/italic.png"));
	italicBtn->setCheckable(true);
	connect(italicBtn, SIGNAL(clicked()), this, SLOT(ShowItalicBtn()));
	underlineBtn = new QToolButton();
	underlineBtn->setIcon(QIcon("Icons/underline.png"));
	underlineBtn->setCheckable(true);
	connect(underlineBtn, SIGNAL(clicked()), this, SLOT(ShowUnderlineBtn()));
	colorBtn = new QToolButton();
	colorBtn->setIcon(QIcon("Icons/color.png"));
	colorBtn->setCheckable(true);
	connect(colorBtn, SIGNAL(clicked()), this, SLOT(ShowColorBtn()));
}


void ImgProcessor::createActions()
{
	//"打开"动作
	openFileAction = new QAction(QIcon("Icons/open.png"), QString::fromLocal8Bit("打开"), this);
	openFileAction->setShortcut(tr("Ctrl+O"));
	openFileAction->setStatusTip(QString::fromLocal8Bit("打开一个文件"));
	connect(openFileAction, SIGNAL(triggered()), this, SLOT(ShowOpenFile()));
	//"新建"动作
	NewFileAction = new QAction(QIcon("Icons/new.png"), QString::fromLocal8Bit("新建"), this);
	NewFileAction->setShortcut(tr("Ctrl+N"));
	NewFileAction->setStatusTip(QString::fromLocal8Bit("新建一个文件"));
	//"退出"动作
	exitAction = new QAction(QString::fromLocal8Bit("退出"), this);
	exitAction->setShortcut(tr("Ctrl+Q"));
	exitAction->setStatusTip(QString::fromLocal8Bit("退出程序"));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
	//"复制"动作
	copyAction = new QAction(QIcon("Icons/copy.png"), QString::fromLocal8Bit("复制"), this);
	copyAction->setShortcut(tr("Ctrl+C"));
	copyAction->setStatusTip(QString::fromLocal8Bit("复制文件"));
	connect(copyAction, SIGNAL(triggered()), imagetext->text, SLOT(copy()));
	//"剪切"动作
	cutAction = new QAction(QIcon("Icons/cut.png"), QString::fromLocal8Bit("剪切"), this);
	cutAction->setShortcut(tr("Ctrl+X"));
	cutAction->setStatusTip(QString::fromLocal8Bit("剪切文件"));
	connect(cutAction, SIGNAL(triggered()), imagetext->text, SLOT(cut()));
	//"粘贴"动作
	pasteAction = new QAction(QIcon("Icons/paste.png"), QString::fromLocal8Bit("粘贴"), this);
	pasteAction->setShortcut(tr("Ctrl+V"));
	pasteAction->setStatusTip(QString::fromLocal8Bit("粘贴文件"));
	connect(pasteAction, SIGNAL(triggered()), imagetext->text, SLOT(paste()));
	//"关于"动作
	aboutAction = new QAction(QString::fromLocal8Bit("关于"), this);
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(QApplication::aboutQt()));
	//"打印文本"动作
	PrintTextAction = new QAction(QIcon("Icons/printText.png"), QString::fromLocal8Bit("打印文本"), this);
	PrintTextAction->setStatusTip(QString::fromLocal8Bit("打印一个文本"));
	connect(PrintTextAction, SIGNAL(triggered()), this, SLOT(ShowPrintText()));
	//"打印图像"动作
	PrintImageAction = new QAction(QIcon("Icons/printImage.png"), QString::fromLocal8Bit("打印图像"), this);
	PrintImageAction->setStatusTip(QString::fromLocal8Bit("打印一副图像"));
	connect(PrintImageAction, SIGNAL(triggered()), this, SLOT(ShowPrintImage()));
	//"放大"动作
	zoomInAction = new QAction(QIcon("Icons/zoomin.png"), QString::fromLocal8Bit("放大"), this);
	zoomInAction->setStatusTip(QString::fromLocal8Bit("放大一张图片"));
	connect(zoomInAction, SIGNAL(triggered()), this, SLOT(ShowZoomIn()));
	//"缩小"动作
	zoomOutAction = new QAction(QIcon("Icons/zoomout.png"), QString::fromLocal8Bit("缩小"), this);
	zoomOutAction->setStatusTip(QString::fromLocal8Bit("缩小一张图片"));
	connect(zoomOutAction, SIGNAL(triggered()), this, SLOT(ShowZoomOut()));
	//实现图像旋转的动作(Action)
	//旋转90度
	rotate90Action = new QAction(QIcon("Icons/rotate90.png"), QString::fromLocal8Bit("旋转90度"), this);
	rotate90Action->setStatusTip(QString::fromLocal8Bit("将一副图旋转90度"));
	connect(rotate90Action, SIGNAL(triggered()), this, SLOT(ShowRotate90()));
	//旋转180度
	rotate180Action = new QAction(QIcon("Icons/rotate180.png"), QString::fromLocal8Bit("旋转180度"), this);
	rotate180Action->setStatusTip(QString::fromLocal8Bit("将一幅图旋转180度"));
	connect(rotate180Action, SIGNAL(triggered()), this, SLOT(ShowRotate180()));
	//旋转270度
	rotate270Action = new QAction(QIcon("Icons/rotate270.png"), QString::fromLocal8Bit("旋转270度"), this);
	rotate270Action->setStatusTip(QString::fromLocal8Bit("将一幅图旋转270度"));
	connect(rotate270Action, SIGNAL(triggered()), this, SLOT(ShowRotate270()));
	//实现图像镜像的动作(Action)
	//纵向镜像
	mirrorVerticalAction = new QAction(QIcon("Icons/mirrorVertical.png"), QString::fromLocal8Bit("纵向镜像"), this);
	mirrorVerticalAction->setStatusTip(QString::fromLocal8Bit("对一幅图做纵向镜像"));
	connect(mirrorVerticalAction, SIGNAL(triggered()), this, SLOT(ShowMirrorVertical()));
	//横向镜像
	mirrorHorizontalAction = new QAction(QIcon("Icons/mirrorHorizontal.png"), QString::fromLocal8Bit("横向镜像"), this);
	mirrorHorizontalAction->setStatusTip(QString::fromLocal8Bit("对一幅图做横向镜像"));
	connect(mirrorHorizontalAction, SIGNAL(triggered()), this, SLOT(ShowMirrorHorizontal()));
	//实现撤销和恢复的动作(Action)
	//撤销和恢复
	undoAction = new QAction(QIcon("Icons/undo.png"), "撤销", this);
	undoAction->setStatusTip(QString::fromLocal8Bit("撤销"));
	connect(undoAction, SIGNAL(triggered()), imagetext->text, SLOT(undo()));
	redoAction = new QAction(QIcon("Icons/redo.png"), "重做", this);
	redoAction->setStatusTip(QString::fromLocal8Bit("重做"));
	connect(redoAction, SIGNAL(triggered()), imagetext->text, SLOT(redo()));
	//排序：左对齐、右对齐、剧中和两端对齐
	actGrp = new QActionGroup(this);
	leftAction = new QAction(QIcon("Icons/left.png"), QString::fromLocal8Bit("左对齐"), actGrp);
	leftAction->setCheckable(true);
	rightAction = new QAction(QIcon("Icons/right.png"), QString::fromLocal8Bit("右对齐"), actGrp);
	rightAction->setCheckable(true);
	centerAction = new QAction(QIcon("Icons/center.png"), QString::fromLocal8Bit("居中"), actGrp);
	centerAction->setCheckable(true);
	justifyAction = new QAction(QIcon("Icons/justify.png"), QString::fromLocal8Bit("两端对齐"), actGrp);
	justifyAction->setCheckable(true);
	connect(actGrp, SIGNAL(triggered(QAction *)), this, SLOT(ShowAlignment(QAction *)));
}

void ImgProcessor::createMenus()
{
	//文件菜单
	fileMenu = menuBar()->addMenu(QString::fromLocal8Bit("文件"));
	fileMenu->addAction(openFileAction);
	fileMenu->addAction(NewFileAction);
	fileMenu->addAction(PrintTextAction);
	fileMenu->addAction(PrintImageAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);
	//缩放菜单
	zoomMenu = menuBar()->addMenu(QString::fromLocal8Bit("编辑"));
	zoomMenu->addAction(copyAction);
	zoomMenu->addAction(cutAction);
	zoomMenu->addAction(pasteAction);
	zoomMenu->addAction(aboutAction);
	zoomMenu->addSeparator();
	zoomMenu->addAction(zoomInAction);
	zoomMenu->addAction(zoomOutAction);
	//图像处理子菜单--旋转菜单
	rotateMenu = menuBar()->addMenu(QString::fromLocal8Bit("旋转"));
	rotateMenu->addAction(rotate90Action);
	rotateMenu->addAction(rotate180Action);
	rotateMenu->addAction(rotate270Action);
	//图像处理子菜单--镜像菜单
	mirrorMenu = menuBar()->addMenu(QString::fromLocal8Bit("镜像"));
	mirrorMenu->addAction(mirrorVerticalAction);
	mirrorMenu->addAction(mirrorHorizontalAction);
}

void ImgProcessor::createToolBars()
{
	//文件工具条
	fileTool = addToolBar("File");
	fileTool->addAction(openFileAction);
	fileTool->addAction(NewFileAction);
	fileTool->addAction(PrintTextAction);
	fileTool->addAction(PrintImageAction);
	//编辑工具条
	zoomTool = addToolBar("Edit");
	zoomTool->addAction(copyAction);
	zoomTool->addAction(cutAction);
	zoomTool->addAction(pasteAction);
	zoomTool->addSeparator();
	zoomTool->addAction(zoomInAction);
	zoomTool->addAction(zoomOutAction);
	//旋转工具条
	rotateTool = addToolBar("rotate");
	rotateTool->addAction(rotate90Action);
	rotateTool->addAction(rotate180Action);
	//撤销和重做工具条
	doToolBar = addToolBar("doEdit");
	doToolBar->addAction(undoAction);
	doToolBar->addAction(redoAction);
	//字体编辑工具条
	fontToolBar = addToolBar("Font");
	fontToolBar->addWidget(fontLabel1);
	fontToolBar->addWidget(fontComboBox);
	fontToolBar->addWidget(fontLabel2);
	fontToolBar->addWidget(sizeComboBox);
	fontToolBar->addSeparator();
	fontToolBar->addWidget(boldBtn);
	fontToolBar->addWidget(italicBtn);
	fontToolBar->addWidget(underlineBtn);
	fontToolBar->addSeparator();
	fontToolBar->addWidget(colorBtn);
	//排序工具条
	listToolBar = addToolBar("list");
	listToolBar->addWidget(listLabel);
	listToolBar->addWidget(listComboBox);
	listToolBar->addSeparator();
	listToolBar->addActions(actGrp->actions());
}

void ImgProcessor::ShowOpenFile()
{
	fileName = QFileDialog::getOpenFileName(this);
	if (!fileName.isEmpty())
	{
		if (imagetext->text->document()->isEmpty())
		{
			loadFile(fileName);
		}
		else
		{
			ImgProcessor *newImgProcessor = new ImgProcessor;
			newImgProcessor->show();
			newImgProcessor->loadFile(fileName);
		}
	}
}

void ImgProcessor::loadFile(QString filename)
{
	printf("file name:%s \n", filename.data());
	QFile file(filename);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream textStream(&file);
		while (!textStream.atEnd())
		{
			imagetext->text->append(textStream.readLine());
			printf("read line \n");
		}
		printf("end \n");
	}
}

void ImgProcessor::ShowPrintText()
{
	QPrinter printer(QPrinter::HighResolution);
	QPrintDialog printDialog(&printer, this);
	if (printDialog.exec())
	{
		//获得QTextEdit对象的文档
		QTextDocument *doc = imagetext->text->document();
		doc->print(&printer);
	}
}

void ImgProcessor::ShowPrintImage()
{
	QPrinter printer;                //新建一个QPrinter对象
	QPrintDialog printDialog(&printer, this);
	if (printDialog.exec())
	{
		QPainter painter(&printer);
		QRect rect = painter.viewport();
		QSize size = img.size();
		/* 按照图形的比例大小重新设置视图矩形区域 */
		size.scale(rect.size(), Qt::KeepAspectRatio);
		painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
		painter.setWindow(img.rect());
		painter.drawImage(0, 0, img);
	}
}

void ImgProcessor::ShowZoomIn()
{
	if (img.isNull())
		return;
	QMatrix matrix;
	matrix.scale(2, 2);
	img = img.transformed(matrix);
	//重新设置显示图形
	imagetext->imageLabel->setPixmap(QPixmap::fromImage(img));
}

void ImgProcessor::ShowZoomOut()
{
	if (img.isNull())
		return;
	QMatrix matrix;
	matrix.scale(0.5, 0.5);
	img = img.transformed(matrix);
	imagetext->imageLabel->setPixmap(QPixmap::fromImage(img));
}

void ImgProcessor::ShowRotate90()
{
	if (img.isNull())
		return;
	QMatrix matrix;
	matrix.rotate(90);
	img = img.transformed(matrix);
	imagetext->imageLabel->setPixmap(QPixmap::fromImage(img));
}

void ImgProcessor::ShowRotate180()
{
	if (img.isNull())
		return;
	QMatrix matrix;
	matrix.rotate(180);
	img = img.transformed(matrix);
	imagetext->imageLabel->setPixmap(QPixmap::fromImage(img));
}

void ImgProcessor::ShowRotate270()
{
	if (img.isNull())
		return;
	QMatrix matrix;
	matrix.rotate(270);
	img = img.transformed(matrix);
	imagetext->imageLabel->setPixmap(QPixmap::fromImage(img));
}

void ImgProcessor::ShowMirrorVertical()
{
	if (img.isNull())
		return;
	img = img.mirrored(false, true);
	imagetext->imageLabel->setPixmap(QPixmap::fromImage(img));
}

void ImgProcessor::ShowMirrorHorizontal()
{
	if (img.isNull())
		return;
	img = img.mirrored(true, false);
	imagetext->imageLabel->setPixmap(QPixmap::fromImage(img));
}

//设置字体
void ImgProcessor::ShowFontComboBox(QString comboStr)
{
	QTextCharFormat fmt;            //创建一个QTextCharFormat对象
	fmt.setFontFamily(comboStr);    //选择的字体名称设置给QTextCharFormat对象
	mergeFormat(fmt);               //将新的格式应用到光标选区内的字符
}

void ImgProcessor::mergeFormat(QTextCharFormat format)
{
	QTextCursor cursor = imagetext->text->textCursor();        //获得编辑框中的光标
	if (!cursor.hasSelection())
		cursor.select(QTextCursor::WordUnderCursor);
	cursor.mergeCharFormat(format);
	imagetext->text->mergeCurrentCharFormat(format);
}

//设置字号
void ImgProcessor::ShowSizeSpinBox(QString spinvalue)
{
	QTextCharFormat fmt;
	fmt.setFontPointSize(spinvalue.toFloat());
	imagetext->text->mergeCurrentCharFormat(fmt);
}

//设置文字显示加粗
void ImgProcessor::ShowBoldBtn()
{
	QTextCharFormat fmt;
	fmt.setFontWeight(boldBtn->isCheckable() ? QFont::Bold : QFont::Normal);
	imagetext->text->mergeCurrentCharFormat(fmt);
}

//设置文字显示斜体
void ImgProcessor::ShowItalicBtn()
{
	QTextCharFormat fmt;
	fmt.setFontItalic(italicBtn->isCheckable());
	imagetext->text->mergeCurrentCharFormat(fmt);
}

//设置文字加下划线
void ImgProcessor::ShowUnderlineBtn()
{
	QTextCharFormat fmt;
	fmt.setFontUnderline(underlineBtn->isCheckable());
	imagetext->text->mergeCurrentCharFormat(fmt);
}

void ImgProcessor::ShowColorBtn()
{
	QColor color = QColorDialog::getColor(Qt::red, this);
	if (color.isValid())
	{
		QTextCharFormat fmt;
		fmt.setForeground(color);
		imagetext->text->mergeCurrentCharFormat(fmt);
	}
}

void ImgProcessor::ShowCurrentFormatChanged(const QTextCharFormat &fmt)
{
	fontComboBox->setCurrentIndex(fontComboBox->findText(fmt.fontFamily()));
	sizeComboBox->setCurrentIndex(sizeComboBox->findText(QString::number(fmt.fontPointSize())));
	boldBtn->setChecked(fmt.font().bold());
	italicBtn->setChecked(fmt.fontItalic());
	underlineBtn->setChecked(fmt.fontUnderline());
}

void ImgProcessor::ShowAlignment(QAction *act)
{
	if (act == leftAction)
		imagetext->text->setAlignment(Qt::AlignLeft);
	if (act == rightAction)
		imagetext->text->setAlignment(Qt::AlignRight);
	if (act == centerAction)
		imagetext->text->setAlignment(Qt::AlignCenter);
	if (act == justifyAction)
		imagetext->text->setAlignment(Qt::AlignJustify);
}

void ImgProcessor::ShowCursorPositionChanged()
{
	if (imagetext->text->alignment() == Qt::AlignLeft)
		leftAction->setChecked(true);
	if (imagetext->text->alignment() == Qt::AlignRight)
		rightAction->setChecked(true);
	if (imagetext->text->alignment() == Qt::AlignCenter)
		centerAction->setChecked(true);
	if (imagetext->text->alignment() == Qt::AlignJustify)
		justifyAction->setChecked(true);
}

void ImgProcessor::ShowList(int index)
{
	//获得编辑框的QTextCursor对象指针
	QTextCursor cursor = imagetext->text->textCursor();
	if (index != 0)
	{
		QTextListFormat::Style style = QTextListFormat::ListDisc;         //设置style属性值
		switch (index)
		{
		default:
		case 1:
			style = QTextListFormat::ListDisc;
			break;
		case 2:
			style = QTextListFormat::ListCircle;
			break;
		case 3:
			style = QTextListFormat::ListSquare;
			break;
		case 4:
			style = QTextListFormat::ListDecimal;
			break;
		case 5:
			style = QTextListFormat::ListLowerAlpha;
			break;
		case 6:
			style = QTextListFormat::ListUpperAlpha;
			break;
		case 7:
			style = QTextListFormat::ListLowerRoman;
			break;
		case 8:
			style = QTextListFormat::ListUpperRoman;
			break;
		}
		/* 设置缩进值 */
		cursor.beginEditBlock();
		QTextBlockFormat blockFmt = cursor.blockFormat();
		QTextListFormat listFmt;
		if (cursor.currentList())
		{
			listFmt = cursor.currentList()->format();
		}
		else
		{
			listFmt.setIndent(blockFmt.indent() + 1);
			blockFmt.setIndent(0);
			cursor.setBlockFormat(blockFmt);
		}
		listFmt.setStyle(style);
		cursor.createList(listFmt);
		cursor.endEditBlock();
	}
	else
	{
		QTextBlockFormat bfmt;
		bfmt.setObjectIndex(-1);
		cursor.mergeBlockFormat(bfmt);
	}
}