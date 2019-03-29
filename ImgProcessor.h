#pragma once
//图像和文本的处理
#include <qmainwindow.h>
#include <qimage.h>
#include <qlabel.h>
#include <qmenu.h>
#include <qmenubar.h>
#include <qaction.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qtoolbar.h>
#include <qfontcombobox.h>
#include <qtoolbutton.h>
#include <QTextCharFormat>
#include <ImageAndText.h>
#include <qtextlist.h>
#include <qtextformat.h>
#include <showwidget.h>

class ImgProcessor : public QMainWindow
{
	Q_OBJECT
public:
	ImgProcessor(QWidget *parent = 0);
	void createActions();            //创建动作
	void createMenus();              //创建菜单
	void createToolBars();           //创建工具栏
	void createFontBars();           //创建字体工具栏
	void createlistBars();           //创建排序工具栏 
	void loadFile(QString filename);
	void mergeFormat(QTextCharFormat);
private:
	QMenu * fileMenu;
	QMenu *zoomMenu;
	QMenu *rotateMenu;
	QMenu *mirrorMenu;
	QImage img;
	QString fileName;
	ImageAndText *imagetext;                    //图像文本处理
	QLabel *fontLabel1;                         //字体设置
	QFontComboBox *fontComboBox;
	QLabel *fontLabel2;
	QComboBox *sizeComboBox;
	QToolButton *boldBtn;
	QToolButton *italicBtn;
	QToolButton *underlineBtn;
	QToolButton *colorBtn;
	QLabel *listLabel;                         //排序设置
	QComboBox *listComboBox;
	QActionGroup *actGrp;
	QAction *leftAction;
	QAction *rightAction;
	QAction *centerAction;
	QAction *justifyAction;
	QToolBar *listToolBar;                    //排序工具栏
	QAction *openFileAction;
	QAction *NewFileAction;
	QAction *PrintTextAction;
	QAction *PrintImageAction;
	QAction *exitAction;
	QAction *copyAction;                       //编辑菜单
	QAction *cutAction;
	QAction *pasteAction;
	QAction *aboutAction;
	QAction *zoomInAction;
	QAction *zoomOutAction;
	QAction *rotate90Action;                  //旋转菜单
	QAction *rotate180Action;
	QAction *rotate270Action;
	QAction *mirrorVerticalAction;            //镜像菜单
	QAction *mirrorHorizontalAction;
	QAction *undoAction;
	QAction *redoAction;
	QToolBar *fileTool;                       //工具栏
	QToolBar *zoomTool;
	QToolBar *rotateTool;
	QToolBar *mirrorTool;
	QToolBar *doToolBar;
	QToolBar *fontToolBar;

protected slots:
void ShowOpenFile();
void ShowPrintText();
void ShowPrintImage();
void ShowZoomIn();
void ShowZoomOut();
void ShowRotate90();
void ShowRotate180();
void ShowRotate270();
void ShowMirrorVertical();
void ShowMirrorHorizontal();
void ShowFontComboBox(QString comboStr);
void ShowSizeSpinBox(QString spinvalue);
void ShowBoldBtn();
void ShowItalicBtn();
void ShowUnderlineBtn();
void ShowColorBtn();
void ShowCurrentFormatChanged(const QTextCharFormat &fmt);
void ShowList(int);
void ShowAlignment(QAction *act);
void ShowCursorPositionChanged(); 
};
