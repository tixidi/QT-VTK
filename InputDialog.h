#pragma once
#include <qlabel.h>
#include <qspinbox.h>
#include <qboxlayout.h>
#include <qdialog.h>
#include <qpushbutton.h>
#include <qtableview.h>
#include <qframe.h>
#include <qstandarditemmodel.h>
#include <Data.h>
#include <qcolor.h>
#include <qcolordialog.h>
#include <qcombobox.h>
#include <qstringlist.h>
class MarchingDialogSingle : public QDialog
{
	Q_OBJECT
public:
	MarchingDialogSingle(QWidget *parent = 0, int picturetype = 0, int maxvalue = 0);   //0为DICOM, 1为JPG																  //方法
	void createJPGDialog(int maxvalue);
	void createDICOMDialog(int maxvalue);
	int getskinvalue();
	int getbonevalue();
	int getstartvalue();
	int getendvalue();
	private slots:
	void setCurrentvalue();
protected :
	//成员变量
	QSpinBox *startindex;
	QSpinBox *endindex;
	QSpinBox *boneSpinBox;
	QSpinBox *skinSpinBox;
	QHBoxLayout *hLayout;
	QVBoxLayout *verLayout;
	QGridLayout *gridLayout;
	int skinvalue;
	int bonevalue;
	int startvalue;
	int endvalue;
	QLabel *skinLabel;
	QLabel *boneLabel;
	QLabel *startLabel;
	QLabel *endLabel;
	QPushButton *OKBtn;             //确定
	QPushButton *CancelBtn;         //取消	
};

class VolumeInputDialog : public QDialog
{
	Q_OBJECT
public:
	VolumeInputDialog(QWidget* parent , int maxvalue );
	void createGridLayout();
	void createPropertyModel();
	void createScalarOpacityModel();
	void createGradientOpacityModel();
	void createColorModel();
	void createHBoxLayout();
	void createVolumeTextCombox();
	void setupView();
	void openFile(QString path);
	VolumeData *getdata();
	

public slots:
	void slotOpen();
	void setVolumevalue();
	void slotComboBoxChanged(const QString &text);

private:
	int value;
	VolumeData *inputdata;
	QLabel *startLabel;
	QLineEdit *startInput;
	QLabel *endLabel;
	QLineEdit *endInput;
	QPushButton *OpenFileButton;
	QPushButton *OKBtn;             //确定
	QPushButton *CancelBtn;         //取消	
	QStandardItemModel *PropertyModel;
	QTableView *PropertyTable;
	QStandardItemModel *ScalarOpacityModel;
	QTableView *ScalarOpacityTable;
	QStandardItemModel *GradientOpacityModel;
	QTableView *GradientOpacityTable;
	QStandardItemModel *ColorModel;
	QTableView *ColorTable;
	QLabel *volumeTextLabel;
	QComboBox *volumeTextComboBox;
	QGridLayout *baseLayout;
	QHBoxLayout *hLayout;
	QVBoxLayout *modelLayout;
	QVBoxLayout *mainLayout;
};

class obj2pcdDialog : public QDialog
{
	Q_OBJECT
public:
	obj2pcdDialog(QWidget *parent = 0);
	QStringList getfilespath();
public slots:
	void slotOpenOBJ();
	void slotOpenPCD();
	void slotTransform();
private:
	void createbase();
	QStringList filespath;
	QLabel *objLabel;
	QLabel *pcdLabel;
	QLabel *pcdNameLabel;
	QPushButton *objButton;
	QPushButton *pcdButton;
	QLineEdit *objLineEdit;
	QLineEdit *pcdLineEdit;
	QLineEdit *pcdNameEdit;
	QPushButton *OKButton;
	QPushButton *CancelButton;
	QGridLayout *baseLayout;
};


class ColorDialog : public QDialog
{
	Q_OBJECT
public:
	ColorDialog(QWidget *parent);
	ColorDialog();
	void createbase();
	QRgb getRGB();
public slots:
	void showColor();
	void slotAccept();
	
protected:
	void setRGB(QRgb rbg);
	QPushButton *ColorButton;
	QFrame *colorFrame;
	QPushButton *OKButton;
	QPushButton *CancelButton;
	QHBoxLayout *hLayout;
	QVBoxLayout *verLayout;
	QGridLayout *gridLayout;
	QRgb rgbdial;
	QColor color;
};

class ColorPointPickDialog : public ColorDialog
{
	Q_OBJECT
public:
	ColorPointPickDialog(QWidget *parent);
	void createbase();
	double getRadius();
public slots:
	void slotAccept();
private:
	void setRadius(double radius);
	QLabel *radiusLabel;
	QLineEdit *radiusvalue;
	double radiusdial;
};

class PyImageSegmentDialog : public QDialog
{
	Q_OBJECT
public:
	PyImageSegmentDialog(QWidget *parent = 0);
	QStringList getfilespath();
	public slots:
	void slotOpenSrc();
	void slotOpenGrowing();
	void slotOpenEdge();
	void slotTransform();
private:
	void createbase();
	QStringList filespath;
	QLabel *SrcLabel;
	QLabel *GrowingLabel;
	QLabel *EdgeLabel;
	QPushButton *SrcButton;
	QPushButton *GrowingButton;
	QPushButton *EdgeButton;
	QLineEdit *SrcLineEdit;
	QLineEdit *GrowingLineEdit;
	QLineEdit *EdgeLineEdit;
	QPushButton *OKButton;
	QPushButton *CancelButton;
	QGridLayout *baseLayout;
};

class CTDirPLYDialog : public QDialog
{
	Q_OBJECT
public:
	CTDirPLYDialog(QWidget *parent = 0);
	int getGrayValue();
	QStringList getfilespath();
public slots:
	void slotOpenCTDir();
	void slotOpenPLY();
	void slotTransform();
private:
	void createbase();
	QStringList filespath;
	QSpinBox *grayValueSpinBox;
	int grayValue;
	QLabel *CTDirLabel;
	QLabel *PLYDirLabel;
	QLabel *PLYNameLabel;
	QLabel *grayValueLabel;
	QPushButton *CTDirButton;
	QPushButton *PLYDirButton;
	QLineEdit *CTDirLineEdit;
	QLineEdit *PLYDirLineEdit;
	QLineEdit *PLYNameEdit;
	QPushButton *OKButton;
	QPushButton *CancelButton;
	QGridLayout *baseLayout;
};

class AToBDialog : public QDialog
{
	Q_OBJECT
public:
	AToBDialog(QWidget *parent , const char* AfileType , const char* BfileType);
	QStringList getfilespath();
public slots:
	void slotOpenA();
	void slotOpenB();
	void slotTransform();
private:
	const char* AName;      //A文件类型名
	const char* BName;      //B文件类型名
	void createbase();
	QStringList filespath;
	QLabel *ALabel;
	QLabel *BLabel;
	QLabel *BNameLabel;
	QPushButton *AButton;
	QPushButton *BButton;
	QLineEdit *ALineEdit;
	QLineEdit *BLineEdit;
	QLineEdit *BNameEdit;
	QPushButton *OKButton;
	QPushButton *CancelButton;
	QGridLayout *baseLayout;
};

class PlySimplyDialog : public QDialog
{
	Q_OBJECT
public:
	PlySimplyDialog(QWidget *parent);
	QStringList getfilespath();
	float getFactor();
public slots:
	void slotOpenA();
	void slotOpenB();
	void slotTransform();
private:
	float factor;      //简化的系数
	void createbase();
	QStringList filespath;
	QDoubleSpinBox *factorSpinBox;
	QLabel *ALabel;
	QLabel *factorLabel;
	QLabel *BLabel;
	QLabel *BNameLabel;
	QPushButton *AButton;
	QPushButton *BButton;
	QLineEdit *ALineEdit;
	QLineEdit *BLineEdit;
	QLineEdit *BNameEdit;
	QPushButton *OKButton;
	QPushButton *CancelButton;
	QGridLayout *baseLayout;
};

class PLYInformationDialog : public QDialog
{
	Q_OBJECT
public:
	PLYInformationDialog(QWidget *parent);
	QStringList getfilespath();
public slots:
	void slotOpenA();
	void slotTransform();
private:
	void createbase();
	QStringList filespath;
	QLabel *ALabel;
	QPushButton *AButton;
	QLineEdit *ALineEdit;
	QPushButton *OKButton;
	QPushButton *CancelButton;
	QGridLayout *baseLayout;
};