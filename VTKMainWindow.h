#pragma once
#include <qmainwindow.h>
#include <qimage.h>
#include <qlabel.h>
#include <qmenu.h>
#include <qmenubar.h>
#include <MarchingCubes.h>
#include <qaction.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qtoolbar.h>
#include <qfontcombobox.h>
#include <qtoolbutton.h>
#include <QTextCharFormat>
#include <ImageAndText.h>
#include <showwidget.h>
#include <vtkJPEGReader.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkNamedColors.h>
#include <vtkinclude.h>


class VTKMainWindow : public QMainWindow
{
	Q_OBJECT
public:
	VTKMainWindow(QWidget *parent = 0);
	void createActions();            //创建动作
	void createMenus();              //创建菜单
	void createToolBars();           //创建工具栏
	void Show(QString &QSfilePath);                     //显示文件
	QList <QString> filespath;
	QList <QString> dirspath;
protected:
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;
	vtkSmartPointer<vtkJPEGReader> jpegReader;
	vtkSmartPointer<vtkActor> firstActor;
	//QVTKWidget *mainwidget;
	QVTKWidget *usedwidget;
private:
	void WritePointTxt();
	enum saveType {none,OBJ, PCD, PointTxt};
	saveType save;
	MarchingCubes *marching;
	QMenu *fileMenu;
	QMenu *pictureMenu;
	QMenu *vtkMenu;
	QMenu *featureMenu;
	//QMenu *picturetypeMenu;
	QMenu *pclMenu;
	QMenu *zoomMenu;
	QMenu *rotateMenu;
	QMenu *mirrorMenu;
	QMenu *OtherMenu;
	QImage img;
	QString fileName;
	ImageAndText *imagetext;                    //图像文本处理
	ShowWidget *showwidget;
	QAction *openFileAction;
	QAction *filesaveAction;
	QAction *NewFileAction;
	QAction *Print3DModelAction;
	QAction *PrintImageAction;
	QMenu *PickMenu;                           //拾取菜单
	QAction *SinglePointPickAction; 
	QAction *MutiplePointPickAction;
	QAction *CellPickAction;
	QAction *AreaPickAction;
	QAction *PropPickAction;
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
	//----------------其他的小功能
	QAction *WordsEditAction;
	QAction *FuctionlogAction;                //功能日志检测
	//vtk的功能函数
	QAction *CPUVolumeAction;                 //CPU体绘
	QAction *GPUVolumeAction;                 //GPU体绘
	QAction *ctkVTKGPUVolumeAction;           //使用控制体绘制的参数
	QAction *MarchingCubesAction;             //按图片序列面绘
	QAction *MarchingDirCTAction;             //整个目录的面绘
	QAction *OBJOutputAction;                 //OBJ模型输出
	//pcl的功能
	QAction *PCDInputAction;                  //PCD模型的输入
	QAction *obj2pcdAction;                   //obj到pcd模型的转化
	//Python的功能
	QAction *PyImageSegmentAction;            //Python医学图像分割
	//特征点检测的功能
	QAction *STL2PLYAction;                   //STL转成PLY
	QAction *OBJ2PLYAction;                   //OBJ转成PLY
	QAction *PLY2BPAction;                    //PLY模型的顶点
	QAction *PLYSimplyAction;                 //PLY模型简化
	QAction *PLYInformationAction;            //统计PLY模型的顶点数目与三角面的数目
	QAction *PLYFeatureAction;                //检测PLY模型的特征点

	//菜单功能函数
public slots:
	void ShowWordsEdit();           //显示文本编辑器
	void slotWriteLog();
	void slotShow();
	void slotItemClicked();
	void slotSelectionChanged();
	void slotItemChanged(QListWidgetItem * item);
	void slotRender();
	void slotClear();
	void slotPCD();
	void slotobj2pcd();
	void slotSTL2PLY();
	void slotOBJ2PLY();
	void slotPLY2BP();
	void slotPLYSimply();
	void slotPLYFeature();
	void slotSaveFile();
	void slotShowCubes();
	void slotDirCTCubes();
	void slotGPUVolumeRender();
	void slotCPUVolumeRender();
	void slotctkVTKGPUVolumeRender();
	void slotSinglePointPick();            //拾取功能
	void slotAreaPick();
	void slotActorPick();
	void slotRemind();
	void slotAbout();
	void slotPyImageSegment();
	void slotPLYInformation();
};
 