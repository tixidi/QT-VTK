#include <InputDialog.h>
#include <qlineedit.h>
#include <qfiledialog.h>
#include "qtextstream.h"
#include <stdio.h>
#include <qpalette.h>
#include <stdlib.h>
#include <qmessagebox.h>
MarchingDialogSingle::MarchingDialogSingle(QWidget *parent, int picturetype, int maxvlaue)
	: QDialog(parent)
{
	startindex = NULL;
	endindex = NULL;
	skinSpinBox = NULL;
	boneSpinBox = NULL;
	if (picturetype == 0)
	{
		createDICOMDialog(maxvlaue);
	}
	else if (picturetype == 1)
	{
		createJPGDialog(maxvlaue);
	}
}

void MarchingDialogSingle::createDICOMDialog(int maxvalue)
{
	skinvalue = 300;
	bonevalue = 1150;
	startvalue = 0;
	endvalue = 0;
	setWindowTitle(QString::fromLocal8Bit("设置数值"));
	startLabel = new QLabel(QString::fromLocal8Bit("开始位置"));
	startindex = new QSpinBox();
	startindex->setRange(0, maxvalue);
	startindex->setSingleStep(1);
	startindex->setValue(0);
	endLabel = new QLabel(QString::fromLocal8Bit("结束的位置"));
	endindex = new QSpinBox();
	endindex->setRange(1, maxvalue);
	endindex->setSingleStep(1);
	endindex->setValue(maxvalue);
	skinLabel = new QLabel(QString::fromLocal8Bit("皮肤的阈值"));
	skinSpinBox = new QSpinBox();
	skinSpinBox->setRange(-1000, 1500);
	skinSpinBox->setSingleStep(1);
	skinSpinBox->setValue(300);
	boneLabel = new QLabel(QString::fromLocal8Bit("骨头的阈值"));
	boneSpinBox = new QSpinBox();
	boneSpinBox->setRange(-1000, 1500);
	boneSpinBox->setSingleStep(1);
	boneSpinBox->setValue(1150);
	OKBtn = new QPushButton(QString::fromLocal8Bit("确定"));
	connect(OKBtn, SIGNAL(clicked()), this, SLOT(setCurrentvalue()));
	CancelBtn = new QPushButton(QString::fromLocal8Bit("取消"));
	connect(CancelBtn, SIGNAL(clicked()), this, SLOT(reject()));
	verLayout = new QVBoxLayout(this);
	hLayout = new QHBoxLayout();
	hLayout->addWidget(OKBtn);
	hLayout->addWidget(CancelBtn);
	gridLayout = new QGridLayout();
	gridLayout->addWidget(startLabel, 0, 0);
	gridLayout->addWidget(startindex, 0, 1);
	gridLayout->addWidget(endLabel, 1, 0);
	gridLayout->addWidget(endindex, 1, 1);
	gridLayout->addWidget(skinLabel, 2, 0);
	gridLayout->addWidget(skinSpinBox, 2, 1);
	gridLayout->addWidget(boneLabel, 3, 0);
	gridLayout->addWidget(boneSpinBox, 3, 1);
	verLayout->addLayout(gridLayout);
	verLayout->addLayout(hLayout);
}

void MarchingDialogSingle::createJPGDialog(int maxvalue)
{
	skinvalue = 100;
	startvalue = 0;
	endvalue = 0;
	setWindowTitle(QString::fromLocal8Bit("设置数值"));
	startLabel = new QLabel(QString::fromLocal8Bit("开始位置"));
	startindex = new QSpinBox();
	startindex->setRange(0, maxvalue);
	startindex->setSingleStep(1);
	startindex->setValue(0);
	endLabel = new QLabel(QString::fromLocal8Bit("结束的位置"));
	endindex = new QSpinBox();
	endindex->setRange(1, maxvalue);
	endindex->setSingleStep(1);
	endindex->setValue(maxvalue);
	skinLabel = new QLabel(QString::fromLocal8Bit("皮肤的阈值"));
	skinSpinBox = new QSpinBox();
	skinSpinBox->setRange(1, 256);
	skinSpinBox->setSingleStep(1);
	skinSpinBox->setValue(100);
	OKBtn = new QPushButton(QString::fromLocal8Bit("确定"));
	connect(OKBtn, SIGNAL(clicked()), this, SLOT(setCurrentvalue()));
	CancelBtn = new QPushButton(QString::fromLocal8Bit("取消"));
	connect(CancelBtn, SIGNAL(clicked()), this, SLOT(reject()));
	verLayout = new QVBoxLayout(this);
	hLayout = new QHBoxLayout();
	hLayout->addWidget(OKBtn);
	hLayout->addWidget(CancelBtn);
	gridLayout = new QGridLayout();
	gridLayout->addWidget(startLabel, 0, 0);
	gridLayout->addWidget(startindex, 0, 1);
	gridLayout->addWidget(endLabel, 1, 0);
	gridLayout->addWidget(endindex, 1, 1);
	gridLayout->addWidget(skinLabel, 2, 0);
	gridLayout->addWidget(skinSpinBox, 2, 1);
	verLayout->addLayout(gridLayout);
	verLayout->addLayout(hLayout);
}

void MarchingDialogSingle::setCurrentvalue()
{
	if (skinSpinBox != NULL)
	{
		skinvalue = skinSpinBox->value();
	}
	if (boneSpinBox != NULL)
	{
		bonevalue = boneSpinBox->value();
	}
	if (startindex != NULL)
	{
		startvalue = startindex->value();
	}
	if (endindex != NULL)
	{
		endvalue = endindex->value();
	}
	QDialog::accept();
}

int MarchingDialogSingle::getskinvalue()
{
	return skinvalue;
}

int MarchingDialogSingle::getbonevalue()
{
	return bonevalue;
}

int MarchingDialogSingle::getstartvalue()
{
	return startvalue;
}

int MarchingDialogSingle::getendvalue()
{
	return endvalue;
}

VolumeInputDialog::VolumeInputDialog(QWidget* parent, int maxvalue)
	: QDialog(parent)
{
	value = maxvalue;
	createGridLayout();
	createPropertyModel();
	createScalarOpacityModel();
	createGradientOpacityModel();
	createColorModel();
	createHBoxLayout();
	setupView();
}

void VolumeInputDialog::createGridLayout()
{
	startLabel = new QLabel(QString::fromLocal8Bit("开始位置"));
	QValidator* validator = new QIntValidator(0, value , this);
	startInput = new QLineEdit();
	startInput->setText(QString::number(0));
	startInput->setValidator(validator);
	startInput->setToolTip(QString::fromLocal8Bit("超出范围的数据无法输入和显示"));
	endLabel = new QLabel(QString::fromLocal8Bit("结束的位置"));
	endInput = new QLineEdit();
	endInput->setText(QString::number(value));
	endInput->setValidator(validator);
	endInput->setToolTip(QString::fromLocal8Bit("超出范围的数据无法输入和显示"));
	volumeTextLabel = new QLabel(QString::fromLocal8Bit("选择默认的体绘数据"));
	volumeTextComboBox = new QComboBox;
	createVolumeTextCombox();
	connect(volumeTextComboBox, SIGNAL(currentIndexChanged(const QString &)), 
		this, SLOT(slotComboBoxChanged(const QString &)));
	OpenFileButton = new QPushButton(QString::fromLocal8Bit("从TXT文件中输入体绘数据"));
	connect(OpenFileButton, SIGNAL(clicked()), this, SLOT(slotOpen()));
	baseLayout = new QGridLayout();
	baseLayout->addWidget(startLabel, 0, 0);
	baseLayout->addWidget(startInput, 0, 1);
	baseLayout->addWidget(endLabel, 1, 0);
	baseLayout->addWidget(endInput, 1, 1);
	baseLayout->addWidget(volumeTextLabel, 2, 0);
	baseLayout->addWidget(volumeTextComboBox, 2, 1);
	baseLayout->addWidget(OpenFileButton, 3, 0, 1, 2);
}

void VolumeInputDialog::createVolumeTextCombox()
{
	QString volumeTextPath = "Data/VolumeText";
	QDir dir(volumeTextPath);
	QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);

	QString ext; //获取文件名的拓展
	volumeTextComboBox->addItem("None");

	for (auto & fileinfo : file_list)
	{
		ext = fileinfo.suffix();    //后缀名
		if (ext == "txt")
		{
			volumeTextComboBox->addItem(fileinfo.baseName());
		}
	}
}


void VolumeInputDialog::slotComboBoxChanged(const QString &text)
{
	if (text != "None")
	{
		QString filePath = "Data/VolumeText/" + text + ".txt";
		openFile(filePath);
		qDebug("Hello");
	}
}

void VolumeInputDialog::slotOpen()
{
	QString name;
	name = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("打开"), "." , "volumedata files(*.txt)");
	if (!name.isEmpty())
		openFile(name);
}

void VolumeInputDialog::openFile(QString path)
{
	if (!path.isEmpty())
	{
		QFile file(path);
		if (file.open(QFile::ReadOnly | QFile::Text))
		{
			QTextStream stream(&file);
			QString line;
			PropertyModel->removeRows(0, PropertyModel->rowCount(QModelIndex()), QModelIndex());
			ScalarOpacityModel->removeRows(0, ScalarOpacityModel->rowCount(QModelIndex()), QModelIndex());
			GradientOpacityModel->removeRows(0, GradientOpacityModel->rowCount(QModelIndex()), QModelIndex());
			ColorModel->removeRows(0, ColorModel->rowCount(QModelIndex()), QModelIndex());
			int Prow = 0, Srow = 0, Grow = 0, Crow = 0;
			do
			{
				line = stream.readLine();
				QStringList pieces = line.split(QRegExp("[,*/^:]"));
				if (!pieces.value(0).compare("volumeProperty"))
				{
					PropertyModel->insertRows(Prow, 1, QModelIndex());
					PropertyModel->setData(PropertyModel->index(Prow, 0, QModelIndex()), pieces.value(1).trimmed());
					PropertyModel->setData(PropertyModel->index(Prow, 1, QModelIndex()), pieces.value(2).trimmed());
					PropertyModel->setData(PropertyModel->index(Prow, 2, QModelIndex()), pieces.value(3).trimmed());
					PropertyModel->setData(PropertyModel->index(Prow, 3, QModelIndex()), pieces.value(4).trimmed());
					Prow++;
				}
				else if (!pieces.value(0).compare("ScalarOpacity"))
				{
					ScalarOpacityModel->insertRows(Srow, 1, QModelIndex());
					ScalarOpacityModel->setData(ScalarOpacityModel->index(Srow, 0, QModelIndex()), pieces.value(1).trimmed());
					ScalarOpacityModel->setData(ScalarOpacityModel->index(Srow, 1, QModelIndex()), pieces.value(2).trimmed());
					ScalarOpacityModel->setData(ScalarOpacityModel->index(Srow, 2, QModelIndex()), pieces.value(3).trimmed());
					ScalarOpacityModel->setData(ScalarOpacityModel->index(Srow, 3, QModelIndex()), pieces.value(4).trimmed());
					Srow++;
				}
				else if (!pieces.value(0).compare("GradientOpacity"))
				{
					GradientOpacityModel->insertRows(Grow, 1, QModelIndex());
					GradientOpacityModel->setData(GradientOpacityModel->index(Grow, 0, QModelIndex()), pieces.value(1).trimmed());
					GradientOpacityModel->setData(GradientOpacityModel->index(Grow, 1, QModelIndex()), pieces.value(2).trimmed());
					GradientOpacityModel->setData(GradientOpacityModel->index(Grow, 2, QModelIndex()), pieces.value(3).trimmed());
					GradientOpacityModel->setData(GradientOpacityModel->index(Grow, 3, QModelIndex()), pieces.value(4).trimmed());
					Grow++;
				}
				else if (!pieces.value(0).compare("Color"))
				{
					ColorModel->insertRows(Crow, 1, QModelIndex());
					ColorModel->setData(ColorModel->index(Crow, 0, QModelIndex()), pieces.value(1).trimmed());
					ColorModel->setData(ColorModel->index(Crow, 1, QModelIndex()), pieces.value(2).trimmed());
					ColorModel->setData(ColorModel->index(Crow, 2, QModelIndex()), pieces.value(3).trimmed());
					ColorModel->setData(ColorModel->index(Crow, 3, QModelIndex()), pieces.value(4).trimmed());
					ColorModel->setData(ColorModel->index(Crow, 4, QModelIndex()), pieces.value(5).trimmed());
					ColorModel->setData(ColorModel->index(Crow, 5, QModelIndex()), pieces.value(6).trimmed());
					Crow++;
				}
				else
				{
					continue;
				}
			} while (!line.isEmpty());
			file.close();
		}
	}	
}
void VolumeInputDialog::createPropertyModel()
{
	PropertyModel = new QStandardItemModel(1, 4, this);
	PropertyModel->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("环境光"));
	PropertyModel->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("漫反射"));
	PropertyModel->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("高光"));
	PropertyModel->setHeaderData(3, Qt::Horizontal, QString::fromLocal8Bit("高光强度"));
	PropertyTable = new QTableView;
	PropertyTable->setModel(PropertyModel);
	PropertyTable->setMinimumSize(QSize(600, 100));
	PropertyTable->setMaximumSize(QSize(600, 100));
	PropertyTable->setToolTip(QString::fromLocal8Bit("设置体绘属性参数"));
	//PropertyTable->setHorizontalHeader();
}

void VolumeInputDialog::createScalarOpacityModel()
{
	ScalarOpacityModel = new QStandardItemModel(1, 4, this);
	ScalarOpacityModel->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("灰度值"));
	ScalarOpacityModel->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("透明度"));
	ScalarOpacityModel->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("中间值"));
	ScalarOpacityModel->setHeaderData(3, Qt::Horizontal, QString::fromLocal8Bit("曲线类型"));
	ScalarOpacityTable = new QTableView;
	ScalarOpacityTable->setModel(ScalarOpacityModel);
	ScalarOpacityTable->setToolTip(QString::fromLocal8Bit("不透明度传输参数"));
}
void VolumeInputDialog::createGradientOpacityModel()
{
	GradientOpacityModel = new QStandardItemModel(1, 4, this);
	GradientOpacityModel->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("灰度值"));
	GradientOpacityModel->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("透明度"));
	GradientOpacityModel->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("中间值"));
	GradientOpacityModel->setHeaderData(3, Qt::Horizontal, QString::fromLocal8Bit("曲线类型"));
	GradientOpacityTable = new QTableView();
	GradientOpacityTable->setModel(GradientOpacityModel);
	GradientOpacityTable->setToolTip(QString::fromLocal8Bit("梯度不透明度参数"));
}
void VolumeInputDialog::createColorModel()
{
	ColorModel = new QStandardItemModel(1, 6, this);
	ColorModel->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("灰度值"));
	ColorModel->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("R分度"));
	ColorModel->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("G分度"));
	ColorModel->setHeaderData(3, Qt::Horizontal, QString::fromLocal8Bit("B分度"));
	ColorModel->setHeaderData(4, Qt::Horizontal, QString::fromLocal8Bit("中间值"));
	ColorModel->setHeaderData(5, Qt::Horizontal, QString::fromLocal8Bit("曲线类型"));
	ColorTable = new QTableView;
	ColorTable->setModel(ColorModel);
	ColorTable->setToolTip(QString::fromLocal8Bit("设置颜色属性"));
}
void VolumeInputDialog::createHBoxLayout()
{
	hLayout = new QHBoxLayout();
	OKBtn = new QPushButton(QString::fromLocal8Bit("确定"));
	connect(OKBtn, SIGNAL(clicked()), this, SLOT(setVolumevalue()));
	CancelBtn = new QPushButton(QString::fromLocal8Bit("取消"));
	connect(CancelBtn, SIGNAL(clicked()), this, SLOT(reject()));
	hLayout->addWidget(OKBtn);
	hLayout->addWidget(CancelBtn);
}

void VolumeInputDialog::setVolumevalue()
{
	int start = startInput->text().toInt();
	int end = endInput->text().toInt();
	int Pn = PropertyModel->rowCount();
	int Sn = ScalarOpacityModel->rowCount();
	int Gn = GradientOpacityModel->rowCount();
	int Cn = ColorModel->rowCount();
	inputdata = new VolumeData(start, end, Pn, Sn, Gn, Cn);
	for (int i = 0; i < Pn; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			inputdata->Property[i][j] = PropertyModel->data(PropertyModel->index(i, j)).toDouble();
		}
	}
	for (int i = 0; i < Sn; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			inputdata->Scalar[i][j] = ScalarOpacityModel->data(ScalarOpacityModel->index(i, j)).toDouble();
		}
	}
	for (int i = 0; i < Gn; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			inputdata->Gradient[i][j] = GradientOpacityModel->data(GradientOpacityModel->index(i, j)).toDouble();
		}
	}
	for (int i = 0; i < Cn; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			inputdata->Color[i][j] = ColorModel->data(ColorModel->index(i, j)).toDouble();
		}
	}
	QDialog::accept();
}

VolumeData *VolumeInputDialog::getdata( )
{
	return inputdata;
}
void VolumeInputDialog::setupView()
{
	modelLayout = new QVBoxLayout();
	modelLayout->addWidget(PropertyTable);
	modelLayout->addWidget(ScalarOpacityTable);
	modelLayout->addWidget(GradientOpacityTable);
	modelLayout->addWidget(ColorTable);
	mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(baseLayout);
	mainLayout->addLayout(modelLayout);
	mainLayout->addLayout(hLayout);
}

obj2pcdDialog::obj2pcdDialog(QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle(QString::fromLocal8Bit("OBJ转成PCD"));
	createbase();
}

void obj2pcdDialog::createbase()
{
	objLabel = new QLabel(QString::fromLocal8Bit("输入obj文件的地址"));
	objLineEdit = new QLineEdit();
	objButton = new QPushButton();
	objButton->setIcon(QPixmap("Icons/open.png"));
	//objButton->setIconSize(QPixmap("Icons/open.png").size());
	objButton->setFixedSize(QSize(28, 28));
	connect(objButton, SIGNAL(clicked()), this, SLOT(slotOpenOBJ()));
	pcdLabel = new QLabel(QString::fromLocal8Bit("输入pcd文件的保存目录"));
	pcdLineEdit = new QLineEdit();
	pcdButton = new QPushButton();
	pcdButton->setIcon(QPixmap("Icons/open.png"));
	pcdButton->setFixedSize(QSize(28, 28));
	//pcdButton->setIconSize(QPixmap("Icons/open.png").size());
	connect(pcdButton, SIGNAL(clicked()), this, SLOT(slotOpenPCD()));
	OKButton = new QPushButton(QString::fromLocal8Bit("确定"));
	connect(OKButton, SIGNAL(clicked()), this, SLOT(slotTransform()));
	CancelButton = new QPushButton(QString::fromLocal8Bit("取消"));
	connect(CancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	pcdNameLabel = new QLabel(QString::fromLocal8Bit("新文件名"));
	pcdNameEdit = new QLineEdit();
	baseLayout = new QGridLayout(this);
	baseLayout->addWidget(objLabel, 0, 0, 1, 2);
	baseLayout->addWidget(objLineEdit, 1, 0);
	baseLayout->addWidget(objButton, 1, 1);
	baseLayout->addWidget(pcdLabel, 2, 0, 1, 2);
	baseLayout->addWidget(pcdLineEdit, 3, 0);
	baseLayout->addWidget(pcdButton, 3, 1);
	baseLayout->addWidget(pcdNameEdit, 4, 0);
	baseLayout->addWidget(pcdNameLabel, 4, 1);
	baseLayout->addWidget(OKButton, 5, 0);
	baseLayout->addWidget(CancelButton, 5, 1);
}

void obj2pcdDialog::slotOpenOBJ()
{
	QString QSobjPath = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择需要转化的obj文件"), "/",
		"OBJ files (*.obj)");
	objLineEdit->setText(QSobjPath);
}

void obj2pcdDialog::slotOpenPCD()
{
	QString QSpcdPath = QFileDialog::getExistingDirectory(this);
	pcdLineEdit->setText(QSpcdPath);
}

void obj2pcdDialog::slotTransform()
{
	if (objLineEdit->text().length() == 0 ||
		pcdLineEdit->text().length() == 0 ||
		pcdNameEdit->text().length() == 0)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
			QString::fromLocal8Bit("请输入相关文件的信息"));
		return;
	}
	QString QSobjpath = objLineEdit->text();
	//QByteArray QBobjpath = QSobjpath.toLocal8Bit();
	//const char* objpath = QBobjpath.data();
	filespath.append(QSobjpath);
	QString QSpcdpath = pcdLineEdit->text() + "/" + pcdNameEdit->text();
	//QByteArray QBpcdpath = QSpcdpath.toLocal8Bit();
	//const char* pcdpath = QBpcdpath.data();
	filespath.append(QSpcdpath);
	//获取扩展名
	int exetension = pcdNameEdit->text().lastIndexOf(".");
	QString fileExt = pcdNameEdit->text().right(pcdNameEdit->text().length() - exetension - 1);
	QByteArray Extba = fileExt.toLocal8Bit();
	const char* fileExten = Extba.data();
	if (_stricmp(fileExten, "pcd"))
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
			QString::fromLocal8Bit("文件名应该带有.pcd后缀"));
		return;
	}
	QDialog::accept();
}

QStringList obj2pcdDialog::getfilespath()
{
	return filespath;
}

ColorDialog::ColorDialog(QWidget *parent)
	: QDialog(parent)
{
	createbase();
}

ColorDialog::ColorDialog()
{
	//给子类调用使用
}

void ColorDialog::createbase()
{
	ColorButton = new QPushButton(QString::fromLocal8Bit("标准颜色对话框"));
	connect(ColorButton, SIGNAL(clicked()), this, SLOT(showColor()));
	colorFrame = new QFrame;
	colorFrame->setFrameShape(QFrame::Box);
	colorFrame->setAutoFillBackground(true);
	OKButton = new QPushButton(QString::fromLocal8Bit("确定"));
	connect(OKButton, SIGNAL(clicked()), this, SLOT(slotAccept()));
	CancelButton = new QPushButton(QString::fromLocal8Bit("取消"));
	connect(CancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	verLayout = new QVBoxLayout(this);
	hLayout = new QHBoxLayout();
	hLayout->addWidget(OKButton);
	hLayout->addWidget(CancelButton);
	gridLayout = new QGridLayout();
	gridLayout->addWidget(ColorButton, 0, 0);
	gridLayout->addWidget(colorFrame, 0, 1);
	verLayout->addLayout(gridLayout);
	verLayout->addLayout(hLayout);
}

void ColorDialog::showColor()
{
	color = QColorDialog::getColor(Qt::red);
	if (color.isValid())
	{
		colorFrame->setPalette(QPalette(color));
	}
}

void ColorDialog::slotAccept()
{
	rgbdial = color.rgb();
	setRGB(rgbdial);
	QDialog::accept();
}


void ColorDialog::setRGB(QRgb rgb)
{
	this->rgbdial = rgb;
}

QRgb ColorDialog::getRGB()
{
	return rgbdial;
}

ColorPointPickDialog::ColorPointPickDialog(QWidget *parent)
{
	createbase();
}

void ColorPointPickDialog::createbase()
{
	
	radiusLabel = new QLabel(QString::fromLocal8Bit("输入点的半径"));
	radiusvalue = new QLineEdit();
	radiusvalue->setText("0.1");
	ColorButton = new QPushButton(QString::fromLocal8Bit("标准颜色对话框"));
	connect(ColorButton, SIGNAL(clicked()), this, SLOT(showColor()));
	colorFrame = new QFrame;
	colorFrame->setFrameShape(QFrame::Box);
	colorFrame->setAutoFillBackground(true);
	OKButton = new QPushButton(QString::fromLocal8Bit("确定"));
	connect(OKButton, SIGNAL(clicked()), this, SLOT(slotAccept()));
	CancelButton = new QPushButton(QString::fromLocal8Bit("取消"));
	connect(CancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	verLayout = new QVBoxLayout(this);
	hLayout = new QHBoxLayout();
	hLayout->addWidget(OKButton);
	hLayout->addWidget(CancelButton);
	gridLayout = new QGridLayout();
	gridLayout->addWidget(radiusLabel, 0, 0);
	gridLayout->addWidget(radiusvalue, 0, 1);
	gridLayout->addWidget(ColorButton, 1, 0);
	gridLayout->addWidget(colorFrame, 1, 1);
	verLayout->addLayout(gridLayout);
	verLayout->addLayout(hLayout);
}

void ColorPointPickDialog::slotAccept()
{
	radiusdial = radiusvalue->text().toDouble();
	setRadius(radiusdial);
	rgbdial = color.rgb();
	setRGB(rgbdial);
	QDialog::accept();
}

void ColorPointPickDialog::setRadius(double radius)
{
	this->radiusdial = radius;
}


double ColorPointPickDialog::getRadius()
{
	return radiusdial;
}

PyImageSegmentDialog::PyImageSegmentDialog(QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle(QString::fromLocal8Bit("Python图像分割"));
	createbase();
}

void PyImageSegmentDialog::createbase()
{
	SrcLabel = new QLabel(QString::fromLocal8Bit("输入图像源文件的地址"));
	SrcLineEdit = new QLineEdit();
	SrcButton = new QPushButton();
	SrcButton->setIcon(QPixmap("Icons/open.png"));
	//objButton->setIconSize(QPixmap("Icons/open.png").size());
	SrcButton->setFixedSize(QSize(28, 28));
	connect(SrcButton, SIGNAL(clicked()), this, SLOT(slotOpenSrc()));
	GrowingLabel = new QLabel(QString::fromLocal8Bit("输入分割后图像文件的保存目录"));
	GrowingLineEdit = new QLineEdit();
	GrowingButton = new QPushButton();
	GrowingButton->setIcon(QPixmap("Icons/open.png"));
	GrowingButton->setFixedSize(QSize(28, 28));
	//pcdButton->setIconSize(QPixmap("Icons/open.png").size());
	connect(GrowingButton, SIGNAL(clicked()), this, SLOT(slotOpenGrowing()));
	EdgeLabel = new QLabel(QString::fromLocal8Bit("输入边缘检测后图像文件的保存目录"));
	EdgeLineEdit = new QLineEdit();
	EdgeButton = new QPushButton();
	EdgeButton->setIcon(QPixmap("Icons/open.png"));
	EdgeButton->setFixedSize(QSize(28, 28));
	//pcdButton->setIconSize(QPixmap("Icons/open.png").size());
	connect(EdgeButton, SIGNAL(clicked()), this, SLOT(slotOpenEdge()));
	OKButton = new QPushButton(QString::fromLocal8Bit("确定"));
	connect(OKButton, SIGNAL(clicked()), this, SLOT(slotTransform()));
	CancelButton = new QPushButton(QString::fromLocal8Bit("取消"));
	connect(CancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	baseLayout = new QGridLayout(this);
	baseLayout->addWidget(SrcLabel, 0, 0, 1, 2);
	baseLayout->addWidget(SrcLineEdit, 1, 0);
	baseLayout->addWidget(SrcButton, 1, 1);
	baseLayout->addWidget(GrowingLabel, 2, 0, 1, 2);
	baseLayout->addWidget(GrowingLineEdit, 3, 0);
	baseLayout->addWidget(GrowingButton, 3, 1);
	baseLayout->addWidget(EdgeLabel, 4, 0, 1, 2);
	baseLayout->addWidget(EdgeLineEdit, 5, 0);
	baseLayout->addWidget(EdgeButton, 5, 1);
	baseLayout->addWidget(OKButton, 6, 0);
	baseLayout->addWidget(CancelButton, 6, 1);
}

void PyImageSegmentDialog::slotOpenSrc()
{
	QString QSobjPath = QFileDialog::getExistingDirectory(this);
	SrcLineEdit->setText(QSobjPath);
}

void PyImageSegmentDialog::slotOpenGrowing()
{
	QString QSobjPath = QFileDialog::getExistingDirectory(this);
	GrowingLineEdit->setText(QSobjPath);
}

void PyImageSegmentDialog::slotOpenEdge()
{
	QString QSobjPath = QFileDialog::getExistingDirectory(this);
	EdgeLineEdit->setText(QSobjPath);
}

void PyImageSegmentDialog::slotTransform()
{
	if (SrcLineEdit->text().length() == 0 ||
		GrowingLineEdit->text().length() == 0 ||
		EdgeLineEdit->text().length() == 0)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
			QString::fromLocal8Bit("请输入相关文件的信息"));
		return;
	}
	filespath.append(SrcLineEdit->text());
	filespath.append(GrowingLineEdit->text());
	filespath.append(EdgeLineEdit->text());
	
	QDialog::accept();
}

QStringList PyImageSegmentDialog::getfilespath()
{
	return filespath;
}



CTDirPLYDialog::CTDirPLYDialog(QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle(QString::fromLocal8Bit("DICOM图像面绘"));
	createbase();
}

void CTDirPLYDialog::createbase()
{
	CTDirLabel = new QLabel(QString::fromLocal8Bit("输入CT文件的地址"));
	CTDirLineEdit = new QLineEdit();
	CTDirButton = new QPushButton();
	CTDirButton->setIcon(QPixmap("Icons/open.png"));
	//objButton->setIconSize(QPixmap("Icons/open.png").size());
	CTDirButton->setFixedSize(QSize(28, 28));
	connect(CTDirButton, SIGNAL(clicked()), this, SLOT(slotOpenCTDir()));
	grayValueLabel = new QLabel(QString::fromLocal8Bit("提取的灰度值"));
	grayValueSpinBox = new QSpinBox();
	grayValueSpinBox->setRange(-1000, 1500);
	grayValueSpinBox->setSingleStep(1);
	grayValueSpinBox->setValue(500);
	PLYDirLabel = new QLabel(QString::fromLocal8Bit("输入PLY文件的保存目录"));
	PLYDirLineEdit = new QLineEdit();
	PLYDirButton = new QPushButton();
	PLYDirButton->setIcon(QPixmap("Icons/open.png"));
	PLYDirButton->setFixedSize(QSize(28, 28));
	//pcdButton->setIconSize(QPixmap("Icons/open.png").size());
	connect(PLYDirButton, SIGNAL(clicked()), this, SLOT(slotOpenPLY()));
	OKButton = new QPushButton(QString::fromLocal8Bit("确定"));
	connect(OKButton, SIGNAL(clicked()), this, SLOT(slotTransform()));
	CancelButton = new QPushButton(QString::fromLocal8Bit("取消"));
	connect(CancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	PLYNameLabel = new QLabel(QString::fromLocal8Bit("PLY文件名"));
	PLYNameEdit = new QLineEdit();
	baseLayout = new QGridLayout(this);
	baseLayout->addWidget(CTDirLabel, 0, 0, 1, 2);
	baseLayout->addWidget(CTDirLineEdit, 1, 0);
	baseLayout->addWidget(CTDirButton, 1, 1);
	baseLayout->addWidget(grayValueLabel, 2, 0);
	baseLayout->addWidget(grayValueSpinBox, 2, 1);
	baseLayout->addWidget(PLYDirLabel, 3, 0, 1, 2);
	baseLayout->addWidget(PLYDirLineEdit, 4, 0);
	baseLayout->addWidget(PLYDirButton, 4, 1);
	baseLayout->addWidget(PLYNameEdit, 5, 0);
	baseLayout->addWidget(PLYNameLabel, 5, 1);
	baseLayout->addWidget(OKButton, 6, 0);
	baseLayout->addWidget(CancelButton, 6, 1);
}

void CTDirPLYDialog::slotOpenCTDir()
{
	QString QSCTDirPath = QFileDialog::getExistingDirectory(this);
	CTDirLineEdit->setText(QSCTDirPath);
}

void CTDirPLYDialog::slotOpenPLY()
{
	QString QSPLYDirPath = QFileDialog::getExistingDirectory(this);
	PLYDirLineEdit->setText(QSPLYDirPath);
}

void CTDirPLYDialog::slotTransform()
{
	if (CTDirLineEdit->text().length() == 0 ||
		PLYDirLineEdit->text().length() == 0 ||
		PLYNameEdit->text().length() == 0)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
			QString::fromLocal8Bit("请输入相关文件的信息"));
		return;
	}

	grayValue = grayValueSpinBox->value();

	QString QSCTDirpath = CTDirLineEdit->text();
	filespath.append(QSCTDirpath);
	QString QSPLYpath = PLYDirLineEdit->text() + "/" + PLYNameEdit->text();
	filespath.append(QSPLYpath);
	//获取扩展名
	int exetension = PLYNameEdit->text().lastIndexOf(".");
	QString fileExt = PLYNameEdit->text().right(PLYNameEdit->text().length() - exetension - 1);
	QByteArray Extba = fileExt.toLocal8Bit();
	const char* fileExten = Extba.data();
	if (_stricmp(fileExten, "ply"))
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
			QString::fromLocal8Bit("文件名应该带有.ply后缀"));
		return;
	}
	QDialog::accept();
}

QStringList CTDirPLYDialog::getfilespath()
{
	return filespath;
}

int CTDirPLYDialog::getGrayValue()
{
	return grayValue;
}

AToBDialog::AToBDialog(QWidget *parent, const char* AfileType, const char* BfileType)
	: QDialog(parent), AName(AfileType), BName(BfileType)
{
	//AName = AfileType;
	//BName = BfileType;
	std::string AToB = std::string(AName) + "转成" + std::string(BName);
	setWindowTitle(QString::fromLocal8Bit(AToB.c_str()));
	createbase();
}

void AToBDialog::createbase()
{
	std::string AWords = "输入" +  std::string(AName) + "文件的地址";
	ALabel = new QLabel(QString::fromLocal8Bit(AWords.c_str()));
	ALineEdit = new QLineEdit();
	AButton = new QPushButton();
	AButton->setIcon(QPixmap("Icons/open.png"));
	AButton->setFixedSize(QSize(28, 28));
	connect(AButton, SIGNAL(clicked()), this, SLOT(slotOpenA()));
	std::string BWords = "输入" + std::string(BName) + "文件的地址";
	BLabel = new QLabel(QString::fromLocal8Bit(BWords.c_str()));
	BLineEdit = new QLineEdit();
	BButton = new QPushButton();
	BButton->setIcon(QPixmap("Icons/open.png"));
	BButton->setFixedSize(QSize(28, 28));
	//pcdButton->setIconSize(QPixmap("Icons/open.png").size());
	connect(BButton, SIGNAL(clicked()), this, SLOT(slotOpenB()));
	OKButton = new QPushButton(QString::fromLocal8Bit("确定"));
	connect(OKButton, SIGNAL(clicked()), this, SLOT(slotTransform()));
	CancelButton = new QPushButton(QString::fromLocal8Bit("取消"));
	connect(CancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	BNameLabel = new QLabel(QString::fromLocal8Bit("新文件名"));
	BNameEdit = new QLineEdit();
	baseLayout = new QGridLayout(this);
	baseLayout->addWidget(ALabel, 0, 0, 1, 2);
	baseLayout->addWidget(ALineEdit, 1, 0);
	baseLayout->addWidget(AButton, 1, 1);
	baseLayout->addWidget(BLabel, 2, 0, 1, 2);
	baseLayout->addWidget(BLineEdit, 3, 0);
	baseLayout->addWidget(BButton, 3, 1);
	baseLayout->addWidget(BNameEdit, 4, 0);
	baseLayout->addWidget(BNameLabel, 4, 1);
	baseLayout->addWidget(OKButton, 5, 0);
	baseLayout->addWidget(CancelButton, 5, 1);
}

void AToBDialog::slotOpenA()
{
	std::string FirstWords = "选择需要转化的" + std::string(AName) + "文件";
	std::string SecondWords = std::string(AName) + " files" + "(*." + std::string(AName) + ")";
	QString QSobjPath = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit(FirstWords.c_str()), "/",
		SecondWords.c_str());
	ALineEdit->setText(QSobjPath);
}

void AToBDialog::slotOpenB()
{
	QString QSBfilePath = QFileDialog::getExistingDirectory(this);
	BLineEdit->setText(QSBfilePath);
}

void AToBDialog::slotTransform()
{
	if (ALineEdit->text().length() == 0 ||
		BLineEdit->text().length() == 0 ||
		BNameEdit->text().length() == 0)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
			QString::fromLocal8Bit("请输入相关文件的信息"));
		return;
	}
	QString QSApath = ALineEdit->text();
	filespath.append(QSApath);
	QString QSBpath = BLineEdit->text() + "/" + BNameEdit->text();
	filespath.append(QSBpath);
	//获取扩展名
	int exetension = BNameEdit->text().lastIndexOf(".");
	QString fileExt = BNameEdit->text().right(BNameEdit->text().length() - exetension - 1);
	QByteArray Extba = fileExt.toLocal8Bit();
	const char* fileExten = Extba.data();
	if (_stricmp(fileExten, BName))
	{
		std::string Info = "文件名应该带有." + std::string(BName) + "后缀";
		QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
			QString::fromLocal8Bit(Info.c_str()));
		return;
	}
	QDialog::accept();
}

QStringList AToBDialog::getfilespath()
{
	return filespath;
}

/*-----------------------网格简化模型的对话框Start-----------------------------*/
PlySimplyDialog::PlySimplyDialog(QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle(QString::fromLocal8Bit("网格简化，PLY文件"));
	createbase();
}

void PlySimplyDialog::createbase()
{
	std::string AWords = "输入简化前的PLY文件的地址";
	ALabel = new QLabel(QString::fromLocal8Bit(AWords.c_str()));
	ALineEdit = new QLineEdit();
	AButton = new QPushButton();
	AButton->setIcon(QPixmap("Icons/open.png"));
	AButton->setFixedSize(QSize(28, 28));
	connect(AButton, SIGNAL(clicked()), this, SLOT(slotOpenA()));
	factorLabel = new QLabel(QString::fromLocal8Bit("简化的系数"));
	factorSpinBox = new QDoubleSpinBox();
	factorSpinBox->setRange(0.01, 1);
	factorSpinBox->setSingleStep(0.01);
	factorSpinBox->setValue(0.1);
	std::string BWords = "输入简化后的PLY文件夹的地址";
	BLabel = new QLabel(QString::fromLocal8Bit(BWords.c_str()));
	BLineEdit = new QLineEdit();
	BButton = new QPushButton();
	BButton->setIcon(QPixmap("Icons/open.png"));
	BButton->setFixedSize(QSize(28, 28));
	//pcdButton->setIconSize(QPixmap("Icons/open.png").size());
	connect(BButton, SIGNAL(clicked()), this, SLOT(slotOpenB()));
	OKButton = new QPushButton(QString::fromLocal8Bit("确定"));
	connect(OKButton, SIGNAL(clicked()), this, SLOT(slotTransform()));
	CancelButton = new QPushButton(QString::fromLocal8Bit("取消"));
	connect(CancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	BNameLabel = new QLabel(QString::fromLocal8Bit("新文件名"));
	BNameEdit = new QLineEdit();
	baseLayout = new QGridLayout(this);
	baseLayout->addWidget(ALabel, 0, 0, 1, 2);
	baseLayout->addWidget(ALineEdit, 1, 0);
	baseLayout->addWidget(AButton, 1, 1);
	baseLayout->addWidget(factorLabel, 2, 0);
	baseLayout->addWidget(factorSpinBox, 2, 1);
	baseLayout->addWidget(BLabel, 3, 0, 1, 2);
	baseLayout->addWidget(BLineEdit, 4, 0);
	baseLayout->addWidget(BButton, 4, 1);
	baseLayout->addWidget(BNameEdit, 5, 0);
	baseLayout->addWidget(BNameLabel, 5, 1);
	baseLayout->addWidget(OKButton, 6, 0);
	baseLayout->addWidget(CancelButton, 6, 1);
}

void PlySimplyDialog::slotOpenA()
{
	QString QSplyPath = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择需要简化的ply文件"), "/",
		"PLY files (*.ply)");
	ALineEdit->setText(QSplyPath);
}

void PlySimplyDialog::slotOpenB()
{
	QString QSBfilePath = QFileDialog::getExistingDirectory(this);
	BLineEdit->setText(QSBfilePath);
}

void PlySimplyDialog::slotTransform()
{
	if (ALineEdit->text().length() == 0 ||
		BLineEdit->text().length() == 0 ||
		BNameEdit->text().length() == 0)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
			QString::fromLocal8Bit("请输入相关文件的信息"));
		return;
	}

	factor = factorSpinBox->value();

	QString QSApath = ALineEdit->text();
	filespath.append(QSApath);
	QString QSBpath = BLineEdit->text() + "/" + BNameEdit->text();
	filespath.append(QSBpath);
	//获取扩展名
	int exetension = BNameEdit->text().lastIndexOf(".");
	QString fileExt = BNameEdit->text().right(BNameEdit->text().length() - exetension - 1);
	QByteArray Extba = fileExt.toLocal8Bit();
	const char* fileExten = Extba.data();
	if (_stricmp(fileExten, "ply"))
	{
		std::string Info = "文件名应该带有.ply后缀";
		QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
			QString::fromLocal8Bit(Info.c_str()));
		return;
	}
	QDialog::accept();
}

QStringList PlySimplyDialog::getfilespath()
{
	return filespath;
}

float PlySimplyDialog::getFactor()
{
	return factor;
}
/*------------------------网格简化模型的对话框End----------------------------*/

/*------------------------PLYInformation的对话框Start----------------------------*/
PLYInformationDialog::PLYInformationDialog(QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle(QString::fromLocal8Bit("统计PLY模型的信息"));
	createbase();
}

void PLYInformationDialog::createbase()
{
	ALabel = new QLabel(QString::fromLocal8Bit("输入PLY模型的地址"));
	ALineEdit = new QLineEdit();
	AButton = new QPushButton();
	AButton->setIcon(QPixmap("Icons/open.png"));
	AButton->setFixedSize(QSize(28, 28));
	connect(AButton, SIGNAL(clicked()), this, SLOT(slotOpenA()));
	OKButton = new QPushButton(QString::fromLocal8Bit("确定"));
	connect(OKButton, SIGNAL(clicked()), this, SLOT(slotTransform()));
	CancelButton = new QPushButton(QString::fromLocal8Bit("取消"));
	connect(CancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	baseLayout = new QGridLayout(this);
	baseLayout->addWidget(ALabel, 0, 0, 1, 2);
	baseLayout->addWidget(ALineEdit, 1, 0);
	baseLayout->addWidget(AButton, 1, 1);
	baseLayout->addWidget(OKButton, 2, 0);
	baseLayout->addWidget(CancelButton, 2, 1);
}

void PLYInformationDialog::slotOpenA()
{
	QString QSplyPath = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择ply文件"), "/",
		"PLY files (*.ply)");
	ALineEdit->setText(QSplyPath);
}


void PLYInformationDialog::slotTransform()
{
	if (ALineEdit->text().length() == 0)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
			QString::fromLocal8Bit("请输入相关文件的信息"));
		return;
	}
	QString QSApath = ALineEdit->text();
	filespath.append(QSApath);
	QDialog::accept();
}

QStringList PLYInformationDialog::getfilespath()
{
	return filespath;
}
/*------------------------PLYInformation的对话框End----------------------------*/