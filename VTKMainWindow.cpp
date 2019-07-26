#include <VTKMainWindow.h>
#include "vtkSmartPointer.h" 
#include <ImgProcessor.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qinputdialog.h>
#include <VTK3D.h>
#include <volumeRender.h>
#include <stdio.h>
#include <stdlib.h>
#include <InputDialog.h>
#include <Data.h>
#include <FileWidget.h>
#include <PCLPoint.h>
#include <VTKPick.h>
#include <TestFunction.h>
#include "PythonImageSegment.h"
#include "AToBTransform.h"
#include <qtextstream.h>
#include <qiodevice.h>
#include "PlySimply.h"
#include "Mesh_Saliency.h"

#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()
VTKMainWindow::VTKMainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	QIcon icon;
	icon.addFile(QStringLiteral("Icons/VTK.png"), QSize(), QIcon::Normal, QIcon::Off);
	setWindowIcon(icon);
	//setIconSize(QSize(22, 22));
	setWindowTitle(QString::fromLocal8Bit("3D模型处理"));         //设置窗体标题
	showwidget = new ShowWidget(this);
	setCentralWidget(showwidget);
	firstActor = NULL;
	//初始化保存的类型
	save = none;
	/* 创建动作、菜单、工具栏的函数 */
	createActions();
	createMenus();
	createToolBars();
	connect(showwidget->practice->CloseBtn, SIGNAL(clicked()), this, SLOT(close()));
	connect(showwidget->practice->ToolBtn, SIGNAL(clicked()), this, SLOT(slotRemind()));
}

void VTKMainWindow::createActions()
{
	//"打开"动作
	openFileAction = new QAction(QIcon("Icons/open.png"), QString::fromLocal8Bit("打开"), this);
	openFileAction->setShortcut(tr("Ctrl+O"));
	openFileAction->setStatusTip(QString::fromLocal8Bit("打开一个文件"));
	connect(openFileAction, SIGNAL(triggered()), this, SLOT(slotShow()));
	//保存文件
	filesaveAction = new QAction(QIcon("Icons/filesave.png"), QString::fromLocal8Bit("保存"), this);
	filesaveAction->setShortcut(tr("Ctrl + S"));
	filesaveAction->setStatusTip(QString::fromLocal8Bit("保存文件"));
	connect(filesaveAction, SIGNAL(triggered()), this, SLOT(slotSaveFile()));
	//"新建"动作
	NewFileAction = new QAction(QIcon("Icons/new.png"), QString::fromLocal8Bit("新建"), this);
	NewFileAction->setShortcut(tr("Ctrl+N"));
	NewFileAction->setStatusTip(QString::fromLocal8Bit("新建一个文件"));
	connect(NewFileAction, SIGNAL(triggered()), this, SLOT(slotRemind()));      //待完善
	//"退出"动作
	exitAction = new QAction(QString::fromLocal8Bit("退出"), this);
	exitAction->setShortcut(tr("Ctrl+Q"));
	exitAction->setStatusTip(QString::fromLocal8Bit("退出程序"));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
	//"复制"动作
	copyAction = new QAction(QIcon("Icons/copy.png"), QString::fromLocal8Bit("复制"), this);
	copyAction->setShortcut(tr("Ctrl+C"));
	copyAction->setStatusTip(QString::fromLocal8Bit("复制文件"));
	connect(copyAction, SIGNAL(triggered()), this, SLOT(slotRemind()));       //待完善
	//"剪切"动作
	cutAction = new QAction(QIcon("Icons/cut.png"), QString::fromLocal8Bit("剪切"), this);
	cutAction->setShortcut(tr("Ctrl+X"));
	cutAction->setStatusTip(QString::fromLocal8Bit("剪切文件"));
	connect(cutAction, SIGNAL(triggered()), this, SLOT(slotRemind()));      //待完善
	//"粘贴"动作
	pasteAction = new QAction(QIcon("Icons/paste.png"), QString::fromLocal8Bit("粘贴"), this);
	pasteAction->setShortcut(tr("Ctrl+V"));
	pasteAction->setStatusTip(QString::fromLocal8Bit("粘贴文件"));
	connect(pasteAction, SIGNAL(triggered()), this, SLOT(slotRemind()));      //待完善
	//"关于"动作
	aboutAction = new QAction(QString::fromLocal8Bit("关于"), this);
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(slotAbout()));
	//"打印3D模型"动作
	Print3DModelAction = new QAction(QIcon("Icons/printText.png"), QString::fromLocal8Bit("打印3D模型"), this);
	Print3DModelAction->setStatusTip(QString::fromLocal8Bit("打印一个3D模型"));
	connect(Print3DModelAction, SIGNAL(triggered()), this, SLOT(slotRemind()));      //待完善
	//"打印图像"动作
	PrintImageAction = new QAction(QIcon("Icons/printImage.png"), QString::fromLocal8Bit("打印图像"), this);
	PrintImageAction->setStatusTip(QString::fromLocal8Bit("打印一副图像"));
	connect(PrintImageAction, SIGNAL(triggered()), this, SLOT(slotRemind()));      //待完善
	//"放大"动作
	zoomInAction = new QAction(QIcon("Icons/zoomin.png"), QString::fromLocal8Bit("放大"), this);
	zoomInAction->setStatusTip(QString::fromLocal8Bit("放大一张图片"));
	connect(zoomInAction, SIGNAL(triggered()), this, SLOT(slotRemind()));      //待完善
	//"缩小"动作
	zoomOutAction = new QAction(QIcon("Icons/zoomout.png"), QString::fromLocal8Bit("缩小"), this);
	zoomOutAction->setStatusTip(QString::fromLocal8Bit("缩小一张图片"));
	connect(zoomOutAction, SIGNAL(triggered()), this, SLOT(slotRemind()));      //待完善
	//实现图像旋转的动作(Action)
	//旋转90度
	rotate90Action = new QAction(QIcon("Icons/rotate90.png"), QString::fromLocal8Bit("旋转90度"), this);
	rotate90Action->setStatusTip(QString::fromLocal8Bit("将一副图旋转90度"));
	connect(rotate90Action, SIGNAL(triggered()), this, SLOT(slotRemind()));      //待完善
	//旋转180度
	rotate180Action = new QAction(QIcon("Icons/rotate180.png"), QString::fromLocal8Bit("旋转180度"), this);
	rotate180Action->setStatusTip(QString::fromLocal8Bit("将一幅图旋转180度"));
	connect(rotate180Action, SIGNAL(triggered()), this, SLOT(slotRemind()));      //待完善
	//旋转270度
	rotate270Action = new QAction(QIcon("Icons/rotate270.png"), QString::fromLocal8Bit("旋转270度"), this);
	rotate270Action->setStatusTip(QString::fromLocal8Bit("将一幅图旋转270度"));
	connect(rotate270Action, SIGNAL(triggered()), this, SLOT(slotRemind()));      //待完善
	//实现图像镜像的动作(Action)
	//纵向镜像
	mirrorVerticalAction = new QAction(QIcon("Icons/mirrorVertical.png"), QString::fromLocal8Bit("纵向镜像"), this);
	mirrorVerticalAction->setStatusTip(QString::fromLocal8Bit("对一幅图做纵向镜像"));
	connect(mirrorVerticalAction, SIGNAL(triggered()), this, SLOT(slotRemind()));      //待完善
	//横向镜像
	mirrorHorizontalAction = new QAction(QIcon("Icons/mirrorHorizontal.png"), QString::fromLocal8Bit("横向镜像"), this);
	mirrorHorizontalAction->setStatusTip(QString::fromLocal8Bit("对一幅图做横向镜像"));
	connect(mirrorHorizontalAction, SIGNAL(triggered()), this, SLOT(slotRemind()));      //待完善
	//实现撤销和恢复的动作(Action)
	//撤销和恢复
	undoAction = new QAction(QIcon("Icons/undo.png"), "撤销", this);
	undoAction->setStatusTip(QString::fromLocal8Bit("撤销"));
	connect(undoAction, SIGNAL(triggered()), this, SLOT(slotRemind()));      //待完善
	redoAction = new QAction(QIcon("Icons/redo.png"), "重做", this);
	redoAction->setStatusTip(QString::fromLocal8Bit("重做"));
	connect(redoAction, SIGNAL(triggered()), this, SLOT(slotRemind()));      //待完善

	//VTK的功能 -- 体绘/面绘
	CPUVolumeAction = new QAction(QString::fromLocal8Bit("CPU体绘"));
	connect(CPUVolumeAction, SIGNAL(triggered()), this, SLOT(slotCPUVolumeRender()));
	GPUVolumeAction = new QAction(QString::fromLocal8Bit("GPU体绘"));
	connect(GPUVolumeAction, SIGNAL(triggered()), this, SLOT(slotGPUVolumeRender()));
	GPUVolumeAction->setStatusTip(QString::fromLocal8Bit("使用独立显卡绘制"));
	ctkVTKGPUVolumeAction = new QAction(QString::fromLocal8Bit("使用ctk控制体绘制参数"));
	connect(ctkVTKGPUVolumeAction, SIGNAL(triggered()), this, SLOT(slotctkVTKGPUVolumeRender()));
	MarchingCubesAction = new QAction(QString::fromLocal8Bit("面绘"));
	connect(MarchingCubesAction, SIGNAL(triggered()), this, SLOT(slotShowCubes()));
	MarchingDirCTAction = new QAction(QString::fromLocal8Bit("按目录面绘（DICOM图像）"));
	connect(MarchingDirCTAction, SIGNAL(triggered()), this, SLOT(slotDirCTCubes()));

	OBJOutputAction = new QAction(QString::fromLocal8Bit("输出OBJ模型"));

	//VTK的功能 -- 点/面的拾取
	SinglePointPickAction = new QAction(QString::fromLocal8Bit("单个点的标记"));
	connect(SinglePointPickAction, SIGNAL(triggered()), this, SLOT(slotSinglePointPick()));
	MutiplePointPickAction = new QAction(QString::fromLocal8Bit("多个点的标记"));
	CellPickAction = new QAction(QString::fromLocal8Bit("单元的标记"));
	AreaPickAction = new QAction(QString::fromLocal8Bit("面积的标记"));
	connect(AreaPickAction, SIGNAL(triggered()), this, SLOT(slotAreaPick()));
	PropPickAction = new QAction(QString::fromLocal8Bit("整个物体的拾取"));
	connect(PropPickAction, SIGNAL(triggered()), this, SLOT(slotActorPick()));

	//PCL的功能
	PCDInputAction = new QAction(QString::fromLocal8Bit("PCD模型的输入"));
	connect(PCDInputAction, SIGNAL(triggered()), this, SLOT(slotPCD()));
	obj2pcdAction = new QAction(QString::fromLocal8Bit("OBJ模型转为PCD模型"));
	connect(obj2pcdAction, SIGNAL(triggered()), this, SLOT(slotobj2pcd())); 

	//Python的功能
	PyImageSegmentAction = new QAction(QString::fromLocal8Bit("Python图像分割"));
	connect(PyImageSegmentAction, SIGNAL(triggered()), this, SLOT(slotPyImageSegment()));

	//特征点检测的功能
	STL2PLYAction = new QAction(QString::fromLocal8Bit("STL模型转为PLY模型"));
	connect(STL2PLYAction, SIGNAL(triggered()), this, SLOT(slotSTL2PLY()));
	OBJ2PLYAction = new QAction(QString::fromLocal8Bit("OBJ模型转为PLY模型"));
	connect(OBJ2PLYAction, SIGNAL(triggered()), this, SLOT(slotOBJ2PLY()));
	PLY2BPAction = new QAction(QString::fromLocal8Bit("PLY模型转为点云"));
	connect(PLY2BPAction, SIGNAL(triggered()), this, SLOT(slotPLY2BP()));
	PLYSimplyAction = new QAction(QString::fromLocal8Bit("PLY模型简化"));
	connect(PLYSimplyAction, SIGNAL(triggered()), this, SLOT(slotPLYSimply()));
	PLYInformationAction = new QAction(QString::fromLocal8Bit("PLY模型的信息"));
	connect(PLYInformationAction, SIGNAL(triggered()), this, SLOT(slotPLYInformation()));
	PLYFeatureAction = new QAction(QString::fromLocal8Bit("PLY模型的特征点"));
	connect(PLYFeatureAction, SIGNAL(triggered()), this, SLOT(slotPLYFeature()));

	//-----------其他的小功能--------------------------------
	WordsEditAction = new QAction(QString::fromLocal8Bit("文字编辑器"), this);
	connect(WordsEditAction, SIGNAL(triggered()), this, SLOT(ShowWordsEdit()));
	FuctionlogAction = new QAction(QString::fromLocal8Bit("功能日志测试"), this);
	connect(FuctionlogAction, SIGNAL(triggered()), this, SLOT(slotWriteLog()));

	//文件浏览的相应
	connect(showwidget->fileview->fileListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slotItemClicked()));
	connect(showwidget->fileview->fileListWidget, SIGNAL(itemSelectionChanged()),
		this, SLOT(slotSelectionChanged()));
	connect(showwidget->fileview->fileListWidget, SIGNAL(itemChanged(QListWidgetItem*)), this, 
		SLOT(slotItemChanged(QListWidgetItem*)));
	connect(showwidget->fileview->RenderButton, SIGNAL(clicked(bool)), this, SLOT(slotRender()));
	connect(showwidget->fileview->fileLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(slotClear()));
}

void VTKMainWindow::createMenus()
{
	//文件菜单
	fileMenu = menuBar()->addMenu(QString::fromLocal8Bit("文件"));
	fileMenu->addAction(openFileAction);
	fileMenu->addAction(filesaveAction);
	fileMenu->addAction(NewFileAction);
	fileMenu->addAction(Print3DModelAction);
	fileMenu->addAction(PrintImageAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);
	//缩放菜单
	zoomMenu = menuBar()->addMenu(QString::fromLocal8Bit("编辑"));
	zoomMenu->addAction(copyAction);
	zoomMenu->addAction(cutAction);
	zoomMenu->addAction(pasteAction);
	zoomMenu->addSeparator();
	zoomMenu->addAction(zoomInAction);
	zoomMenu->addAction(zoomOutAction);
	//图像处理菜单
	pictureMenu = menuBar()->addMenu(QString::fromLocal8Bit("图像处理"));
	//图像处理子菜单--旋转菜单
	rotateMenu = pictureMenu->addMenu(QString::fromLocal8Bit("旋转"));
	rotateMenu->addAction(rotate90Action);
	rotateMenu->addAction(rotate180Action);
	rotateMenu->addAction(rotate270Action);
	//图像处理子菜单--镜像菜单
	mirrorMenu = pictureMenu->addMenu(QString::fromLocal8Bit("镜像"));
	mirrorMenu->addAction(mirrorVerticalAction);
	mirrorMenu->addAction(mirrorHorizontalAction);
	//Python图像分割
	pictureMenu->addAction(PyImageSegmentAction);
	//VTK功能
	vtkMenu = menuBar()->addMenu(QString::fromLocal8Bit("3D模型重建"));
	vtkMenu->addAction(CPUVolumeAction);
	vtkMenu->addAction(GPUVolumeAction);
	vtkMenu->addAction(MarchingCubesAction);
	vtkMenu->addAction(MarchingDirCTAction);
	vtkMenu->addAction(OBJOutputAction);
	vtkMenu->addAction(ctkVTKGPUVolumeAction);
	PickMenu = vtkMenu->addMenu(QString::fromLocal8Bit("拾取操作"));
	PickMenu->addAction(SinglePointPickAction);
	//PickMenu->addAction(MutiplePointPickAction);
	//PickMenu->addAction(CellPickAction);
	PickMenu->addAction(AreaPickAction);
	PickMenu->addAction(PropPickAction);
	//PCL功能
	pclMenu = menuBar()->addMenu(QString::fromLocal8Bit("点云模型"));
	pclMenu->addAction(PCDInputAction);
	pclMenu->addAction(obj2pcdAction);
	//特征点检测功能
	featureMenu = menuBar()->addMenu(QString::fromLocal8Bit("特征点检测"));
	featureMenu->addAction(STL2PLYAction);
	featureMenu->addAction(OBJ2PLYAction);
	featureMenu->addAction(PLY2BPAction);
	featureMenu->addAction(PLYSimplyAction);
	featureMenu->addAction(PLYInformationAction);
	featureMenu->addAction(PLYFeatureAction);
	//其他功能的菜单
	OtherMenu = menuBar()->addMenu(QString::fromLocal8Bit("其他功能"));
	OtherMenu->addAction(WordsEditAction);
	//OtherMenu->addAction(FuctionlogAction);
	OtherMenu->addAction(aboutAction);
}

void VTKMainWindow::createToolBars()
{
	//文件工具条
	fileTool = addToolBar("File");
	fileTool->addAction(openFileAction);
	fileTool->addAction(filesaveAction);
	fileTool->addAction(NewFileAction);
	fileTool->addAction(Print3DModelAction);
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
}

void VTKMainWindow::ShowWordsEdit()
{
	ImgProcessor *newImgProcessor = new ImgProcessor;
	newImgProcessor->show();
}

//日志检测
void VTKMainWindow::slotWriteLog()
{
	//freopen("C:\\Users\\chenjiaxing\\Desktop\\CenterWidget\\Deps\\log\\log.txt", "w", stdout); 
	//freopen("CON", "w", stdout); //recover stdout(Windows)
	//freopen("C:\\Users\\chenjiaxing\\Desktop\\CenterWidget\\Deps\\log\\log.txt", "w", stdout);
	//ColorPickDialog *test = new ColorPickDialog(this);
	//if (!test->exec())
	//{
	//	printf("Cancel");
	//	return;
	//}
	//double radius = test->getRadius();
	//printf("radius:%f\n", radius);
	//QRgb rgb = test->getRGB();
	//
	//double RGB[3] = { qRed(rgb), qBlue(rgb), qGreen(rgb) };
	//printf("Red:%f\n", RGB[0]);
	//printf("Blue:%f\n", RGB[1]);
	//printf("Green:%f\n", RGB[2]);
	//freopen("CON", "w", stdout); //recover stdout(Windows)
	//TestFunction(showwidget->practice->onewidget->qvtkwidget1);
	ColorDialog *test = new ColorDialog(0);
	if (!test->exec())
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
			QString::fromLocal8Bit("已点击取消"));
		return;
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
			QString::fromLocal8Bit("已点击确定"));
	}
}

void VTKMainWindow::slotShow()
{
	QString QSfilePath = QFileDialog::getOpenFileName(this);
	Show(QSfilePath);
}
void VTKMainWindow::Show(QString &QSfilePath)
{
	if (QSfilePath == NULL)
	{
		return;
	}
	QByteArray QBfilePath = QSfilePath.toLocal8Bit();
	const char* filepath = QBfilePath.data();
	//获取扩展名
	/*int exetension = QSfilePath.lastIndexOf(".");
	QString fileExt = QSfilePath.right(QSfilePath.length() - exetension - 1);
	QByteArray Extba = fileExt.toLocal8Bit();
	const char* fileExten = Extba.data();*/
	std::string extension = vtksys::SystemTools::GetFilenameLastExtension(std::string(filepath));
	
	int currentrow = showwidget->list->currentRow();
	if (currentrow == 0)      //单个面板
	{
		//if (!_stricmp(fileExten, "jpg") || !_stricmp(fileExten, "bmp") || !_stricmp(fileExten, "png"))
		if (extension == ".jpg" || extension == ".bmp" || extension == ".png")
		{
			if (showwidget->practice->onewidget->initialCount == 0)
			{
				showwidget->practice->onewidget->initialImage();
			}
			showwidget->practice->onewidget->ShowImage(filepath);
		}
		else if (extension == ".dcm")
		{
			if (showwidget->practice->onewidget->initialCount == 0)
			{
				showwidget->practice->onewidget->initialImage();
			}
			showwidget->practice->onewidget->ShowDICOM(filepath);
		}
		else if (extension == ".stl" || extension == ".obj" || extension == ".ply" ||
			     extension == ".vtp" || extension == ".g" || extension == ".vtk" ||
			     extension == ".fp"  || extension == ".bp")
		{
			VTK3D *vtk3d = new VTK3D(showwidget->practice->onewidget->qvtkwidget1);
			vtk3d->ShowVTK3D(filepath);
		}
		else if (extension == ".pcd")
		{
			PCLPoint *pcl = new PCLPoint(showwidget->practice->onewidget->qvtkwidget1);
			pcl->ShowPointCloud(filepath);
		}
		else if(extension.empty())
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
				QString::fromLocal8Bit("请输入带有后缀名的文件"));
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
				QString::fromLocal8Bit("暂不支持这种格式的文件"));
		}
	}
	else if (currentrow == 1)  //双面板
	{
		bool ok;
		int i = QInputDialog::getInt(this, QString::fromLocal8Bit("选择显示的面板"),
			QString::fromLocal8Bit("请输入面板的标号"), 1, 1, 2, 1, &ok);
		if (!ok)
		{
			return;
		}
		switch (i)
		{
		case 1:
			usedwidget = showwidget->practice->twowidget->qvtkwidge1;
			break;
		case 2:
			usedwidget = showwidget->practice->twowidget->qvtkwidge2;
			break;
		}
		if (extension == ".jpg" || extension == ".bmp" || extension == ".png")
		{
			if (showwidget->practice->twowidget->initialCount[i - 1] == 0)
			{
				showwidget->practice->twowidget->initialImage(i);
			}
			showwidget->practice->twowidget->ShowImage(filepath, i);
		}
		else if (extension == ".dcm")
		{
			if (showwidget->practice->twowidget->initialCount[i - 1] == 0)
			{
				showwidget->practice->twowidget->initialImage(i);
			}
			showwidget->practice->twowidget->ShowDICOM(filepath, i);
		}
		else if (extension == ".stl" || extension == ".obj" || extension == ".ply" ||
			     extension == ".vtp" || extension == ".g" || extension == ".vtk" ||
			     extension == ".fp" || extension == ".bp")
		{
			VTK3D *vtk3d = new VTK3D(usedwidget);
			vtk3d->ShowVTK3D(filepath);
		}
		else if (extension == ".pcd")
		{
			PCLPoint *pcl = new PCLPoint(usedwidget);
			pcl->ShowPointCloud(filepath);
		}
		else if (extension.empty())
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
				QString::fromLocal8Bit("请输入带有后缀名的文件"));
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
				QString::fromLocal8Bit("暂不支持这种格式的文件"));
		}
	}
	else if (currentrow == 2)  //四个面板
	{
		//showwidget->practice->fourwidget->testImage();
		bool ok;
		int i = QInputDialog::getInt(this, QString::fromLocal8Bit("选择显示的面板"),
			QString::fromLocal8Bit("请输入面板的标号"), 1, 1, 4, 1, &ok);
		if (!ok)
		{
			return;
		}
		switch (i)
		{
		case 1:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge1;
			break;
		case 2:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge2;
			break;
		case 3:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge3;
			break;
		case 4:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge4;
			break;
		}
		if (extension == ".jpg" || extension == ".bmp" || extension == ".png")
		{
			if (showwidget->practice->fourwidget->initialCount[i-1] == 0)
			{
				showwidget->practice->fourwidget->initialImage(i);
			}
			showwidget->practice->fourwidget->ShowImage(filepath, i);
		}
		else if (extension == ".dcm")
		{
			if (showwidget->practice->fourwidget->initialCount[i - 1] == 0)
			{
				showwidget->practice->fourwidget->initialImage(i);
			}
			showwidget->practice->fourwidget->ShowDICOM(filepath, i);
		}
		else if (extension == ".stl" || extension == ".obj" || extension == ".ply" ||
			     extension == ".vtp" || extension == ".g" || extension == ".vtk" ||
			     extension == ".fp" || extension == ".bp")
		{
			VTK3D *vtk3d = new VTK3D(usedwidget);
			vtk3d->ShowVTK3D(filepath);
		}
		else if (extension == ".pcd")
		{
			PCLPoint *pcl = new PCLPoint(usedwidget);
			pcl->ShowPointCloud(filepath);
		}
		else if (extension.empty())
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
				QString::fromLocal8Bit("请输入带有后缀名的文件"));
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
				QString::fromLocal8Bit("暂不支持这种格式的文件"));
		}
	}
}

void VTKMainWindow::slotSaveFile()
{
	const char* filepath = NULL;
	if (save == OBJ)
	{
		QString Savename = QFileDialog::getSaveFileName(this,
			QString::fromLocal8Bit("保存文件"),
			"");//,
				//tr("obj Files (*.obj);; pcd Files(*.pcd)"));
		if (Savename.length() == 0)
		{
			return;
		}
		// 支持带中文路径的文件名读取
		QByteArray ba = Savename.toLocal8Bit();
		filepath = ba.data();
	}
	switch (save)
	{
	case none:
		showwidget->textEdit->setText(QString::fromLocal8Bit("目前仅支持OBJ、PCD数据的保存"));
		QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
			QString::fromLocal8Bit("暂不能保存这种类型的数据"));
		break;
	case OBJ:
		marching->WriteOBJ(filepath);
		break;	
	case PCD:
		break;
	case PointTxt:
		WritePointTxt();
		break;
	default:
		break;
	}
}

void VTKMainWindow::WritePointTxt()
{
	QString pointPath = QFileDialog::getOpenFileName(this, tr("Open File"), " ", tr("txt file(*.txt)"));
	QFile file(pointPath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::critical(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("无法打开文件"));
		return;
	}
	QTextStream intxt(&file);
	intxt << showwidget->textEdit->toPlainText().remove(QString::fromLocal8Bit("可以保存为txt文件"));
	intxt.flush();
	file.close();
}

void VTKMainWindow::slotShowCubes()
{
	QString QSpicturePath = QFileDialog::getOpenFileName(this);
	if (QSpicturePath == NULL)
	{
		return;
	}
	QByteArray QBpicturePath = QSpicturePath.toLocal8Bit();
	const char* picturepath = QBpicturePath.data();
	//获取扩展名
	int exetension = QSpicturePath.lastIndexOf(".");
	QString fileExt = QSpicturePath.right(QSpicturePath.length() - exetension - 1);
	QByteArray Extba = fileExt.toLocal8Bit();
	const char* fileExten = Extba.data();
	//获取目录名
	int dirindex = QSpicturePath.lastIndexOf("/");
	QString dirpath = QSpicturePath.left(dirindex);
	QByteArray dirba = dirpath.toLocal8Bit();
	const char* dirpath2 = dirba.data();
	QDir dir(dirpath2);
	QStringList filter;
	//filter << "*.jpg";
	//dir.setNameFilters(filter);
	//QFileInfoList list = dir.entryInfoList();//获取文件信息列表  //测试

	int currentrow = showwidget->list->currentRow();
	if (currentrow == 0)      //单个面板
	{
		if (!_stricmp(fileExten, "jpg"))
		{
			filter << "*.jpg";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			MarchingDialogSingle *test = new MarchingDialogSingle(this, 1, list.length()-1);  //从0开始
			int rec = test->exec();
			if (rec ==	QDialog::Rejected)
			{
				return;
			}
			int skinvalue = test->getskinvalue();
			int startvalue = test->getstartvalue();
			int endvalue = test->getendvalue();
			marching = new MarchingCubes(showwidget->practice->onewidget->qvtkwidget1);
			marching->JPEGMarchingCubes(dirpath2, skinvalue, startvalue, endvalue);
			save = OBJ;
			showwidget->textEdit->setText(QString::fromLocal8Bit("可以将当前文件保存为OBJ文件"));
		}
		else if (!_stricmp(fileExten, "dcm"))
		{
			filter << "*.dcm";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			MarchingDialogSingle *test = new MarchingDialogSingle(this, 0, list.length() - 1);  //从0开始
			int rec = test->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			int skinvalue = test->getskinvalue();
			int bonevalue = test->getbonevalue();
			int startvalue = test->getstartvalue();
			int endvalue = test->getendvalue();
			marching = new MarchingCubes(showwidget->practice->onewidget->qvtkwidget1);
			marching->DICOMMarchingCubes(dirpath2, skinvalue, bonevalue, startvalue, endvalue);
			save = OBJ;
			showwidget->textEdit->setText(QString::fromLocal8Bit("可以将当前文件保存为OBJ文件"));
		}
		else if (exetension == -1)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
				QString::fromLocal8Bit("请输入带有后缀名的文件"));
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
				QString::fromLocal8Bit("暂不支持这种格式的文件"));
		}
	}
	else if (currentrow == 1)  //双面板
	{
		bool ok;
		int i = QInputDialog::getInt(this, QString::fromLocal8Bit("选择显示的面板"),
			QString::fromLocal8Bit("请输入面板的标号"), 1, 1, 2, 1, &ok);
		if (!ok)
		{
			return;
		}
		switch (i)
		{
		case 1:
			usedwidget = showwidget->practice->twowidget->qvtkwidge1;
			break;
		case 2:
			usedwidget = showwidget->practice->twowidget->qvtkwidge2;
			break;
		}
		if (!_stricmp(fileExten, "jpg"))
		{
			filter << "*.jpg";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			MarchingDialogSingle *test = new MarchingDialogSingle(this, 1, list.length()-1);
			int rec = test->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			int skinvalue = test->getskinvalue();
			int startvalue = test->getstartvalue();
			int endvalue = test->getendvalue();
			marching = new MarchingCubes(usedwidget);
			marching->JPEGMarchingCubes(dirpath2, skinvalue, startvalue, endvalue);
		}
		else if (!_stricmp(fileExten, "dcm"))
		{
			filter << "*.dcm";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			MarchingDialogSingle *test = new MarchingDialogSingle(this, 0, list.length() - 1);  //从0开始
			int rec = test->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			int skinvalue = test->getskinvalue();
			int bonevalue = test->getbonevalue();
			int startvalue = test->getstartvalue();
			int endvalue = test->getendvalue();
			marching = new MarchingCubes(usedwidget);
			marching->DICOMMarchingCubes(dirpath2, skinvalue, bonevalue, startvalue, endvalue);
		}
		else if (exetension == -1)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
				QString::fromLocal8Bit("请输入带有后缀名的文件"));
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
				QString::fromLocal8Bit("暂不支持这种格式的文件"));
		}
	}
	else if (currentrow == 2)  //四个面板
	{
		bool ok;
		int i = QInputDialog::getInt(this, QString::fromLocal8Bit("选择显示的面板"),
			QString::fromLocal8Bit("请输入面板的标号"), 1, 1, 4, 1, &ok);
		if (!ok)
		{
			return;
		}
		switch (i)
		{
		case 1:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge1;
			break;
		case 2:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge2;
			break;
		case 3:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge3;
			break;
		case 4:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge4;
			break;
		}
		if (!_stricmp(fileExten, "jpg"))
		{
			filter << "*.jpg";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			MarchingDialogSingle *test = new MarchingDialogSingle(this, 1, list.length()-1);
			int rec = test->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			int skinvalue = test->getskinvalue();
			int startvalue = test->getstartvalue();
			int endvalue = test->getendvalue();
			marching = new MarchingCubes(usedwidget);
			marching->JPEGMarchingCubes(dirpath2, skinvalue, startvalue, endvalue);
		}
		else if (!_stricmp(fileExten, "dcm"))
		{
			filter << "*.dcm";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			MarchingDialogSingle *test = new MarchingDialogSingle(this, 0, list.length() - 1);  //从0开始
			int rec = test->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			int skinvalue = test->getskinvalue();
			int bonevalue = test->getbonevalue();
			int startvalue = test->getstartvalue();
			int endvalue = test->getendvalue();
			marching = new MarchingCubes(usedwidget);
			marching->DICOMMarchingCubes(dirpath2, skinvalue, bonevalue, startvalue, endvalue);
		}
		else if (exetension == -1)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
				QString::fromLocal8Bit("请输入带有后缀名的文件"));
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
				QString::fromLocal8Bit("暂不支持这种格式的文件"));
		}
	}
}

void VTKMainWindow::slotGPUVolumeRender()
{
	QString QSpicturePath = QFileDialog::getOpenFileName(this);
	if (QSpicturePath == NULL)
	{
		return;
	}
	QByteArray QBpicturePath = QSpicturePath.toLocal8Bit();
	const char* picturepath = QBpicturePath.data();
	//获取扩展名
	int exetension = QSpicturePath.lastIndexOf(".");
	QString fileExt = QSpicturePath.right(QSpicturePath.length() - exetension - 1);
	QByteArray Extba = fileExt.toLocal8Bit();
	const char* fileExten = Extba.data();
	//获取目录名
	int dirindex = QSpicturePath.lastIndexOf("/");
	QString dirpath = QSpicturePath.left(dirindex);
	QByteArray dirba = dirpath.toLocal8Bit();
	const char* dirpath2 = dirba.data();
	QDir dir(dirpath2);
	QStringList filter;

	int currentrow = showwidget->list->currentRow();
	if (currentrow == 0)      //单个面板
	{
		if (!_stricmp(fileExten, "jpg"))
		{
			filter << "*.jpg";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			VolumeInputDialog *inputvolume = new VolumeInputDialog(this, list.count() - 1);
			int rec = inputvolume->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			VolumeData *volumedata = inputvolume->getdata();
			//VolumeRender *volume = new VolumeRender(showwidget->practice->onewidget->qvtkwidget1, volumedata);
			VolumeRender *volume = new VolumeRender(showwidget->practice->onewidget->qvtkwidget1, 
				volumedata, showwidget->volumePropertywidget);
			volume->GPU(dirpath2, fileExten);
		}
		else if (!_stricmp(fileExten, "dcm"))
		{
			filter << "*.dcm";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			VolumeInputDialog *inputvolume = new VolumeInputDialog(this, list.count() - 1);
			int rec = inputvolume->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			VolumeData *volumedata = inputvolume->getdata();
			//VolumeRender *volume = new VolumeRender(showwidget->practice->onewidget->qvtkwidget1, volumedata);
			VolumeRender *volume = new VolumeRender(showwidget->practice->onewidget->qvtkwidget1, volumedata
				,showwidget->volumePropertywidget);
			volume->GPU(dirpath2, fileExten);
		}
		else if (exetension == -1)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
				QString::fromLocal8Bit("请输入带有后缀名的文件"));
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
				QString::fromLocal8Bit("暂不支持这种格式的文件"));
		}
	}
	else if (currentrow == 1)  //双面板
	{
		bool ok;
		int i = QInputDialog::getInt(this, QString::fromLocal8Bit("选择显示的面板"),
			QString::fromLocal8Bit("请输入面板的标号"), 1, 1, 2, 1, &ok);
		if (!ok)
		{
			return;
		}
		switch (i)
		{
		case 1:
			usedwidget = showwidget->practice->twowidget->qvtkwidge1;
			break;
		case 2:
			usedwidget = showwidget->practice->twowidget->qvtkwidge2;
			break;
		}
		if (!_stricmp(fileExten, "jpg"))
		{
			filter << "*.jpg";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			VolumeInputDialog *inputvolume = new VolumeInputDialog(this, list.count() - 1);
			int rec = inputvolume->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			VolumeData *volumedata = inputvolume->getdata();
			VolumeRender *volume = new VolumeRender(usedwidget, volumedata, showwidget->volumePropertywidget);
			volume->GPU(dirpath2, fileExten);
		}
		else if (!_stricmp(fileExten, "dcm"))
		{
			filter << "*.dcm";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			VolumeInputDialog *inputvolume = new VolumeInputDialog(this, list.count() - 1);
			int rec = inputvolume->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			VolumeData *volumedata = inputvolume->getdata();
			VolumeRender *volume = new VolumeRender(usedwidget, volumedata, showwidget->volumePropertywidget);
			volume->GPU(dirpath2, fileExten);
		}
		else if (exetension == -1)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
				QString::fromLocal8Bit("请输入带有后缀名的文件"));
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
				QString::fromLocal8Bit("暂不支持这种格式的文件"));
		}
	}
	else if (currentrow == 2)  //四个面板
	{
		bool ok;
		int i = QInputDialog::getInt(this, QString::fromLocal8Bit("选择显示的面板"),
			QString::fromLocal8Bit("请输入面板的标号"), 1, 1, 4, 1, &ok);
		if (!ok)
		{
			return;
		}
		switch (i)
		{
		case 1:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge1;
			break;
		case 2:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge2;
			break;
		case 3:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge3;
			break;
		case 4:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge4;
			break;
		}
		if (!_stricmp(fileExten, "jpg"))
		{
			filter << "*.jpg";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			VolumeInputDialog *inputvolume = new VolumeInputDialog(this, list.count() - 1);
			int rec = inputvolume->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			VolumeData *volumedata = inputvolume->getdata();
			VolumeRender *volume = new VolumeRender(usedwidget, volumedata, showwidget->volumePropertywidget);
			volume->GPU(dirpath2, fileExten);
		}
		else if (!_stricmp(fileExten, "dcm"))
		{
			filter << "*.dcm";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			VolumeInputDialog *inputvolume = new VolumeInputDialog(this, list.count() - 1);
			int rec = inputvolume->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			VolumeData *volumedata = inputvolume->getdata();
			VolumeRender *volume = new VolumeRender(usedwidget, volumedata, showwidget->volumePropertywidget);
			volume->GPU(dirpath2, fileExten);
		}
		else if (exetension == -1)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
				QString::fromLocal8Bit("请输入带有后缀名的文件"));
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
				QString::fromLocal8Bit("暂不支持这种格式的文件"));
		}
	}
} 
void VTKMainWindow::slotCPUVolumeRender()
{
	QString QSpicturePath = QFileDialog::getOpenFileName(this);
	if (QSpicturePath == NULL)
	{
		return;
	}
	QByteArray QBpicturePath = QSpicturePath.toLocal8Bit();
	const char* picturepath = QBpicturePath.data();
	//获取扩展名
	int exetension = QSpicturePath.lastIndexOf(".");
	QString fileExt = QSpicturePath.right(QSpicturePath.length() - exetension - 1);
	QByteArray Extba = fileExt.toLocal8Bit();
	const char* fileExten = Extba.data();
	//获取目录名
	int dirindex = QSpicturePath.lastIndexOf("/");
	QString dirpath = QSpicturePath.left(dirindex);
	QByteArray dirba = dirpath.toLocal8Bit();
	const char* dirpath2 = dirba.data();
	QDir dir(dirpath2);
	QStringList filter;

	int currentrow = showwidget->list->currentRow();
	if (currentrow == 0)      //单个面板
	{
		if (!_stricmp(fileExten, "jpg"))
		{
			filter << "*.jpg";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			VolumeInputDialog *inputvolume = new VolumeInputDialog(this, list.count() - 1);
			int rec = inputvolume->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			VolumeData *volumedata = inputvolume->getdata();
			VolumeRender *volume = new VolumeRender(showwidget->practice->onewidget->qvtkwidget1, volumedata);
			volume->CPU(dirpath2, fileExten);
		}
		else if (!_stricmp(fileExten, "dcm"))
		{
			filter << "*.dcm";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			VolumeInputDialog *inputvolume = new VolumeInputDialog(this, list.count() - 1);
			int rec = inputvolume->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			VolumeData *volumedata = inputvolume->getdata();
			VolumeRender *volume = new VolumeRender(showwidget->practice->onewidget->qvtkwidget1, volumedata);
			volume->CPU(dirpath2, fileExten);
		}
		else if (exetension == -1)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
				QString::fromLocal8Bit("请输入带有后缀名的文件"));
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
				QString::fromLocal8Bit("暂不支持这种格式的文件"));
		}
	}
	else if (currentrow == 1)  //双面板
	{
		bool ok;
		int i = QInputDialog::getInt(this, QString::fromLocal8Bit("选择显示的面板"),
			QString::fromLocal8Bit("请输入面板的标号"), 1, 1, 2, 1, &ok);
		if (!ok)
		{
			return;
		}
		switch (i)
		{
		case 1:
			usedwidget = showwidget->practice->twowidget->qvtkwidge1;
			break;
		case 2:
			usedwidget = showwidget->practice->twowidget->qvtkwidge2;
			break;
		}
		if (!_stricmp(fileExten, "jpg"))
		{
			filter << "*.jpg";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			VolumeInputDialog *inputvolume = new VolumeInputDialog(this, list.count() - 1);
			int rec = inputvolume->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			VolumeData *volumedata = inputvolume->getdata();
			VolumeRender *volume = new VolumeRender(usedwidget, volumedata);
			volume->CPU(dirpath2, fileExten);
		}
		else if (!_stricmp(fileExten, "dcm"))
		{
			filter << "*.dcm";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			VolumeInputDialog *inputvolume = new VolumeInputDialog(this, list.count() - 1);
			int rec = inputvolume->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			VolumeData *volumedata = inputvolume->getdata();
			VolumeRender *volume = new VolumeRender(usedwidget, volumedata);
			volume->CPU(dirpath2, fileExten);
		}
		else if (exetension == -1)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
				QString::fromLocal8Bit("请输入带有后缀名的文件"));
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
				QString::fromLocal8Bit("暂不支持这种格式的文件"));
		}
	}
	else if (currentrow == 2)  //四个面板
	{
		bool ok;
		int i = QInputDialog::getInt(this, QString::fromLocal8Bit("选择显示的面板"),
			QString::fromLocal8Bit("请输入面板的标号"), 1, 1, 4, 1, &ok);
		if (!ok)
		{
			return;
		}
		switch (i)
		{
		case 1:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge1;
			break;
		case 2:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge2;
			break;
		case 3:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge3;
			break;
		case 4:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge4;
			break;
		}
		if (!_stricmp(fileExten, "jpg"))
		{
			filter << "*.jpg";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			VolumeInputDialog *inputvolume = new VolumeInputDialog(this, list.count() - 1);
			int rec = inputvolume->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			VolumeData *volumedata = inputvolume->getdata();
			VolumeRender *volume = new VolumeRender(usedwidget, volumedata);
			volume->CPU(dirpath2, fileExten);
		}
		else if (!_stricmp(fileExten, "dcm"))
		{
			filter << "*.dcm";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			VolumeInputDialog *inputvolume = new VolumeInputDialog(this, list.count() - 1);
			int rec = inputvolume->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			VolumeData *volumedata = inputvolume->getdata();
			VolumeRender *volume = new VolumeRender(usedwidget, volumedata);
			volume->CPU(dirpath2, fileExten);
		}
		else if (exetension == -1)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
				QString::fromLocal8Bit("请输入带有后缀名的文件"));
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
				QString::fromLocal8Bit("暂不支持这种格式的文件"));
		}
	}
}

void VTKMainWindow::slotItemClicked()
{
	QListWidgetItem *clickedItem = showwidget->fileview->fileListWidget->currentItem();
	if (clickedItem == NULL)
	{
		return;
	}
	QFileInfo fileinfo(clickedItem->text());
	QString filename(clickedItem->text());
	if (fileinfo.suffix() != NULL)
	{
		QString dirs(showwidget->fileview->fileLineEdit->text());
		dirs += "/";
		QString absolutepath = dirs + filename;
		if (dirspath.count() == 0 && filespath.count() > 1)
		{
			slotRender();
		}
		else
		{
			Show(absolutepath);
		}
	}
}

void VTKMainWindow::slotSelectionChanged()
{
	QListWidgetItem *clickedItem = showwidget->fileview->fileListWidget->currentItem();
	if (clickedItem == NULL)
	{
		return;
	}
	QFileInfo fileinfo(clickedItem->text());
	QString filename(clickedItem->text());
	if (fileinfo.suffix() != NULL)
	{
		QString dirs(showwidget->fileview->fileLineEdit->text());
		dirs += "/";
		QString absolutepath = dirs + filename;
		
		if (dirspath.count() == 0 && filespath.count() > 1)
		{
			slotRender();
		}
		else
		{
			Show(absolutepath);
		}
	}
}

void VTKMainWindow::slotItemChanged(QListWidgetItem * item)
{
	QFileInfo fileinfo(item->text());
	if (Qt::Checked == item->checkState())
	{
		if (fileinfo.suffix() == NULL)
		{
			QString filepath(showwidget->fileview->fileLineEdit->text());
			filepath += "/";
			filepath += item->text();
			dirspath.push_back(filepath);
		}
		else
		{
			QString filepath(showwidget->fileview->fileLineEdit->text());
			filepath += "/";
			filepath += item->text();
			filespath.push_back(filepath);
		}
	}
	else if (Qt::Unchecked == item->checkState())
	{
		if (fileinfo.suffix() == NULL)
		{
			QString filepath(showwidget->fileview->fileLineEdit->text());
			filepath += "/";
			filepath += item->text();
			int index = dirspath.indexOf(filepath);
			dirspath.removeAt(index);
		}
		else
		{
			QString filepath(showwidget->fileview->fileLineEdit->text());
			filepath += "/";
			filepath += item->text();
			int index = filespath.indexOf(filepath);
			filespath.removeAt(index);
		}
	}
}

void VTKMainWindow::slotRender()
{
	if (dirspath.count() == 0 && filespath.count() == 0)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提醒"), QString::fromLocal8Bit("请选择要绘制的文件"));
		return;
	}
	if (dirspath.count() > 0)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提醒"), QString::fromLocal8Bit("不支持文件的绘制"));
		return;
	}
	int currentrow = showwidget->list->currentRow();
	if (currentrow == 0)      //单个面板
	{	
	   VTK3D *vtk3d = new VTK3D(showwidget->practice->onewidget->qvtkwidget1);
	   vtk3d->ShowMulti(filespath);
	}
	else if (currentrow == 1)  //双面板
	{
		bool ok;
		int i = QInputDialog::getInt(this, QString::fromLocal8Bit("选择显示的面板"),
			QString::fromLocal8Bit("请输入面板的标号"), 1, 1, 2, 1, &ok);
		if (!ok)
		{
			return;
		}
		switch (i)
		{
		case 1:
			usedwidget = showwidget->practice->twowidget->qvtkwidge1;
			break;
		case 2:
			usedwidget = showwidget->practice->twowidget->qvtkwidge2;
			break;
		}
		VTK3D *vtk3d = new VTK3D(usedwidget);
		vtk3d->ShowMulti(filespath);
	}
	else if (currentrow == 2)  //四个面板
	{
		bool ok;
		int i = QInputDialog::getInt(this, QString::fromLocal8Bit("选择显示的面板"),
			QString::fromLocal8Bit("请输入面板的标号"), 1, 1, 4, 1, &ok);
		if (!ok)
		{
			return;
		}
		switch (i)
		{
		case 1:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge1;
			break;
		case 2:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge2;
			break;
		case 3:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge3;
			break;
		case 4:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge4;
			break;
		}
		VTK3D *vtk3d = new VTK3D(usedwidget);
		vtk3d->ShowMulti(filespath);
	}
}

void VTKMainWindow::slotClear()
{
	dirspath.clear();
	filespath.clear();
}

void VTKMainWindow::slotPCD()
{
	QString QSfilePath = QFileDialog::getOpenFileName(this);
	if (QSfilePath == NULL)
	{
		return;
	}
	QByteArray QBfilePath = QSfilePath.toLocal8Bit();
	const char* filepath = QBfilePath.data();
	int currentrow = showwidget->list->currentRow();
	if (currentrow == 0)      //单个面板
	{	
		PCLPoint *pcl = new PCLPoint(showwidget->practice->onewidget->qvtkwidget1);
		pcl->ShowPointCloud(filepath);
	}
	else if (currentrow == 1)  //双面板
	{
		bool ok;
		int i = QInputDialog::getInt(this, QString::fromLocal8Bit("选择显示的面板"),
			QString::fromLocal8Bit("请输入面板的标号"), 1, 1, 2, 1, &ok);
		if (!ok)
		{
			return;
		}
		switch (i)
		{
		case 1:
			usedwidget = showwidget->practice->twowidget->qvtkwidge1;
			break;
		case 2:
			usedwidget = showwidget->practice->twowidget->qvtkwidge2;
			break;
		}
		PCLPoint *pcl = new PCLPoint(usedwidget);
		pcl->ShowPointCloud(filepath);
	}
	else if (currentrow == 2)  //四个面板
	{
		bool ok;
		int i = QInputDialog::getInt(this, QString::fromLocal8Bit("选择显示的面板"),
			QString::fromLocal8Bit("请输入面板的标号"), 1, 1, 4, 1, &ok);
		if (!ok)
		{
			return;
		}
		switch (i)
		{
		case 1:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge1;
			break;
		case 2:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge2;
			break;
		case 3:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge3;
			break;
		case 4:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge4;
			break;
		}	
		PCLPoint *pcl = new PCLPoint(usedwidget);
		pcl->ShowPointCloud(filepath);
	}
}

void VTKMainWindow::slotobj2pcd()
{
	obj2pcdDialog test = new obj2pcdDialog();
	if (!test.exec())
	{
		return;
	}
	QStringList filepaths = test.getfilespath();
	QString QSobjpath = filepaths.at(0);
	QByteArray QBobjpath = QSobjpath.toLocal8Bit();
	const char* objpath = QBobjpath.data();
	QString QSpcdpath = filepaths.at(1);
	QByteArray QBpcdpath = QSpcdpath.toLocal8Bit();
	const char* pcdpath = QBpcdpath.data();
	PCLPoint *pcl = new PCLPoint();
	pcl->obj2pcd(objpath, pcdpath);
}

void VTKMainWindow::slotSinglePointPick()
{
	int currentrow = showwidget->list->currentRow();
	if (currentrow == 0)      //单个面板
	{
		ColorPointPickDialog *test = new ColorPointPickDialog(0);
		if (!test->exec())
		{
			//printf("Cancel");
			return;
		}
		double radius = test->getRadius();
		//printf("radius:%f\n", radius);
		QRgb rgb = test->getRGB();

		double RGB[3] = { qRed(rgb), qGreen(rgb), qBlue(rgb)};
		// Set the custom stype to use for interaction.
		vtkSmartPointer<SinglePointPickingStyle> style =
			vtkSmartPointer<SinglePointPickingStyle>::New();
		
	    style->SetDefaultRenderer(showwidget->practice->onewidget->qvtkwidget1->
			GetRenderWindow()->GetRenderers()->GetFirstRenderer());
		//style->SetDefaultRenderer(showwidget->practice->onewidget->getRender());
		style->setRadius(radius);
		style->setRGB(RGB);
		style->setTextEdit(showwidget->textEdit);
		showwidget->practice->onewidget->qvtkwidget1->GetInteractor()->SetInteractorStyle(style);
		//showwidget->practice->onewidget->getInteractor()->SetInteractorStyle(style);
		//showwidget->practice->onewidget->getInteractor()->Initialize();
		//showwidget->practice->onewidget->getInteractor()->Start();
		showwidget->textEdit->setText(QString::fromLocal8Bit("可以保存为txt文件"));
		save = PointTxt;
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
			QString::fromLocal8Bit("拾取操作仅支持在单个面板下使用"));
	}
}

void VTKMainWindow::slotAreaPick()
{
	int currentrow = showwidget->list->currentRow();
	if (currentrow == 0)      //单个面板
	{
		ColorPointPickDialog *test = new ColorPointPickDialog(0);
		if (!test->exec())
		{
			return;
		}
		double radius = test->getRadius();
		//printf("radius:%f\n", radius);
		QRgb rgb = test->getRGB();
		double RGB[3] = { qRed(rgb), qGreen(rgb), qBlue(rgb) };
		// Set the custom stype to use for interaction.
		vtkSmartPointer<AreaPickingStyle> style =
			vtkSmartPointer<AreaPickingStyle>::New();
		style->SetDefaultRenderer(showwidget->practice->onewidget->qvtkwidget1->
			GetRenderWindow()->GetRenderers()->GetFirstRenderer());
		style->setRadius(radius);
		style->setRGB(RGB);
		style->setTextEdit(showwidget->textEdit);
		/*vtkSmartPointer<vtkActor> actor =
			showwidget->practice->onewidget->qvtkwidget1->
			GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor();*/
		if (firstActor == NULL)
		{
			firstActor = showwidget->practice->onewidget->qvtkwidget1->
				GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor();
		}
		vtkSmartPointer<vtkGeometryFilter> gf = vtkSmartPointer<vtkGeometryFilter>::New();
		vtkSmartPointer<vtkAreaPicker> areaPicker = vtkSmartPointer<vtkAreaPicker>::New();
		showwidget->practice->onewidget->qvtkwidget1->GetInteractor()->SetPicker(areaPicker);
		gf->SetInputConnection(firstActor->GetMapper()->GetInputConnection(0, 0));
		gf->Update();
		vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
		polydata->DeepCopy(gf->GetOutput());
		style->SetPolyData(polydata);
		showwidget->practice->onewidget->qvtkwidget1->GetInteractor()->SetInteractorStyle(style);
		showwidget->textEdit->setText(QString::fromLocal8Bit("英文输入法下键盘R键进入拾取模式"));
		//save = PointTxt;
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
			QString::fromLocal8Bit("拾取操作仅支持在单个面板下使用"));
	}
}

void VTKMainWindow::slotActorPick()
{
	int currentrow = showwidget->list->currentRow();
	if (currentrow == 0)      //单个面板
	{
		ColorDialog *test = new ColorDialog(0);
		if (!test->exec())
		{
			return;
		}
		QRgb rgb = test->getRGB();
		double RGB[3] = { qRed(rgb), qGreen(rgb), qBlue(rgb) };
		// Set the custom stype to use for interaction.
		vtkSmartPointer<ActorPickingStyle> style =
			vtkSmartPointer<ActorPickingStyle>::New();
		style->SetDefaultRenderer(showwidget->practice->onewidget->qvtkwidget1->
			GetRenderWindow()->GetRenderers()->GetFirstRenderer());
		style->setRGB(RGB);
		showwidget->practice->onewidget->qvtkwidget1->GetInteractor()->SetInteractorStyle(style);
		//showwidget->textEdit->setText(QString::fromLocal8Bit("英文输入法下键盘R键进入拾取模式"));
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
			QString::fromLocal8Bit("拾取操作仅支持在单个面板下使用"));
	}
}

void VTKMainWindow::slotPyImageSegment()
{
	PyImageSegmentDialog test = new PyImageSegmentDialog();
	if (!test.exec())
	{
		return;
	}
	QStringList filepaths = test.getfilespath();
	QString QSSrcpath = filepaths.at(0);
	QSSrcpath.replace("/", "\\");
	QSSrcpath.append("\\");
	QByteArray QBSrcpath = QSSrcpath.toLocal8Bit();
	const char* Srcpath = QBSrcpath.data();
	QString QSGrowingpath = filepaths.at(1);
	QSGrowingpath.replace("/", "\\");
	QSGrowingpath.append("\\");
	QByteArray QBGrowingpath = QSGrowingpath.toLocal8Bit();
	const char* Growingpath = QBGrowingpath.data();
	QString QSEdgepath = filepaths.at(2);
	QSEdgepath.replace("/", "\\");
	QSEdgepath.append("\\");
	QByteArray QBEdgepath = QSEdgepath.toLocal8Bit();
	const char* Edgepath = QBEdgepath.data();
	PythonImageSegment(Srcpath, Growingpath, Edgepath);
	QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
		QString::fromLocal8Bit("分割完成"));
}

void VTKMainWindow::slotRemind()
{
	QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
		QString::fromLocal8Bit("功能未开放"));
}

void VTKMainWindow::slotAbout()
{
	QMessageBox::information(this, QString::fromLocal8Bit("开发信息"),
		QString::fromLocal8Bit("当前版本为1.3.1\n"));
}

void VTKMainWindow::slotDirCTCubes()
{
	int currentrow = showwidget->list->currentRow();
	usedwidget = showwidget->practice->onewidget->qvtkwidget1;
	if (currentrow == 0)      //单个面板
	{
		CTDirPLYDialog test = new CTDirPLYDialog();
		if (!test.exec())
		{
			return;
		}
		QStringList filepaths = test.getfilespath();
		QString QSCTDirpath = filepaths.at(0);
		QByteArray QBCTDirpath = QSCTDirpath.toLocal8Bit();
		const char* CTDirpath = QBCTDirpath.data();
		QString QSplypath = filepaths.at(1);
		QByteArray QBplypath = QSplypath.toLocal8Bit();
		const char* plypath = QBplypath.data();
		int grayValue = test.getGrayValue();
		marching = new MarchingCubes(usedwidget);
		marching->CTDirMarchingCubes(CTDirpath, grayValue, plypath);
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
			QString::fromLocal8Bit("拾取操作仅支持在单个面板下使用"));
	}
}

void VTKMainWindow::slotSTL2PLY()
{
	const char* stlName = "stl";
	const char* plyName = "ply";
	AToBDialog *test = new AToBDialog(0, stlName, plyName);
	if (!test->exec())
	{
		return;
	}
	QStringList filepaths = test->getfilespath();
	QString QSstlpath = filepaths.at(0);
	QByteArray QBstlpath = QSstlpath.toLocal8Bit();
	const char* stlpath = QBstlpath.data();
	QString QSplypath = filepaths.at(1);
	QByteArray QBplypath = QSplypath.toLocal8Bit();
	const char* plypath = QBplypath.data();
	STL2PLY(stlpath, plypath);
	QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
		QString::fromLocal8Bit("转换完成"));
}

void VTKMainWindow::slotOBJ2PLY()
{
	const char* objName = "obj";
	const char* plyName = "ply";
	AToBDialog *test = new AToBDialog(0, objName, plyName);
	if (!test->exec())
	{
		return;
	}
	QStringList filepaths = test->getfilespath();
	QString QSstlpath = filepaths.at(0);
	QByteArray QBstlpath = QSstlpath.toLocal8Bit();
	const char* stlpath = QBstlpath.data();
	QString QSplypath = filepaths.at(1);
	QByteArray QBplypath = QSplypath.toLocal8Bit();
	const char* plypath = QBplypath.data();
	OBJ2PLY(stlpath, plypath);
	QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
		QString::fromLocal8Bit("转换完成"));
}

void VTKMainWindow::slotPLYSimply()
{
	PlySimplyDialog test = new PlySimplyDialog(0);
	if (!test.exec())
	{
		return;
	}
	QStringList filepaths = test.getfilespath();
	QString QSstlpath = filepaths.at(0);
	QByteArray QBstlpath = QSstlpath.toLocal8Bit();
	const char* sourcePath = QBstlpath.data();
	QString QSplypath = filepaths.at(1);
	QByteArray QBplypath = QSplypath.toLocal8Bit();
	const char* destPath = QBplypath.data();
	float factor = test.getFactor();
	PlySimply *simply = new PlySimply(sourcePath, destPath, factor);
	simply->Compute();
	delete simply;
	QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
		QString::fromLocal8Bit("简化完成"));
}

void VTKMainWindow::slotPLYInformation()
{
	PLYInformationDialog test = new PLYInformationDialog(0);
	if (!test.exec())
	{
		return;
	}
	QStringList filepaths = test.getfilespath();
	QString QSplypath = filepaths.at(0);
	QByteArray QBplypath = QSplypath.toLocal8Bit();
	const char* sourcePath = QBplypath.data();

	vtkSmartPointer<vtkPLYReader> reader =
		vtkSmartPointer<vtkPLYReader>::New();
	reader->SetFileName(sourcePath);
	reader->Update();
	vtkSmartPointer<vtkPolyData> polydata = reader->GetOutput();

	std::string information = "顶点数目  " + std::to_string(polydata->GetNumberOfPoints())
		+ " \n" + "面的数目  " + std::to_string(polydata->GetNumberOfCells());
	
	QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
		QString::fromLocal8Bit(information.c_str()));
}

void VTKMainWindow::slotPLYFeature()
{
	const char* plyName = "ply";
	const char* fpName = "fp";
	AToBDialog *test = new AToBDialog(0, plyName, fpName);
	if (!test->exec())
	{
		return;
	}
	QStringList filepaths = test->getfilespath();
	QString QSplypath = filepaths.at(0);
	QByteArray QBplypath = QSplypath.toLocal8Bit();
	const char* plypath = QBplypath.data();
	QString QSfppath = filepaths.at(1);
	QByteArray QBfppath = QSfppath.toLocal8Bit();
	const char* fppath = QBfppath.data();
	
	MeshSaliency mesh(plypath);
	//mesh.setFaceReduceOne();  //Armadilo
	mesh.ComputeSaliency_OneLevel();
	mesh.WriteOneLevel(fppath);
	//mesh.WriteOneLevelAndLocalMax(fppath);

	QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
		QString::fromLocal8Bit("特征点检测完成"));
}

void VTKMainWindow::slotPLY2BP()
{
	const char* plyName = "ply";
	const char* bpName = "bp";
	AToBDialog *test = new AToBDialog(0, plyName, bpName);
	if (!test->exec())
	{
		return;
	}
	QStringList filepaths = test->getfilespath();
	QString QSplypath = filepaths.at(0);
	QByteArray QBplypath = QSplypath.toLocal8Bit();
	const char* plypath = QBplypath.data();
	QString QSBPpath = filepaths.at(1);
	QByteArray QBBPpath = QSBPpath.toLocal8Bit();
	const char* BPpath = QBBPpath.data();
	PLY2BP bp(plypath, BPpath);
	bp.Write();
	QMessageBox::information(this, QString::fromLocal8Bit("提醒信息"),
		QString::fromLocal8Bit("转换完成"));
}

//使用ctk框架控制体绘制参数
void VTKMainWindow::slotctkVTKGPUVolumeRender()
{
	// show file dialog. change filename only when the new filename is not empty.
	QString filter("Meta image file (*.mhd *.mha)");
	QString filename = "";
	filename = QFileDialog::getOpenFileName(this, QString(tr("Open a volume data set")), filename, filter);
	if (filename.trimmed().isEmpty())
	{
		qDebug("文件名为空!");
		return;
	}
	

	// show filename on window title
	this->setWindowTitle(QString::fromUtf8("Volume Renderer - ") + filename);

	// get local 8-bit representation of the string in locale encoding (in case the filename contains non-ASCII characters) 
	QByteArray ba = filename.toLocal8Bit();
	const char *filename_str = ba.data();


	// read Meta Image (.mhd or .mha) files
	auto reader = vtkSmartPointer<vtkMetaImageReader>::New();
	reader->SetFileName(filename_str);

	//// read a series of raw files in the specified folder
	//auto reader = vtkSmartPointer<vtkVolume16Reader>::New();
	//reader->SetDataDimensions(512, 512);
	//reader->SetImageRange(1, 361);
	//reader->SetDataByteOrderToBigEndian();
	//reader->SetFilePrefix(filename_str);
	//reader->SetFilePattern("%s%d");
	//reader->SetDataSpacing(1, 1, 1);

	//// read NRRD files
	//vtkNew<vtkNrrdReader> reader;
	//if (!reader->CanReadFile(filename_str))
	//{
	//	std::cerr << "Reader reports " << filename_str << " cannot be read.";
	//	exit(EXIT_FAILURE);
	//}
	//reader->SetFileName(filename_str);
	//reader->Update();


	// scale the volume data to unsigned char (0-255) before passing it to volume mapper
	auto shiftScale = vtkSmartPointer<vtkImageShiftScale>::New();
	shiftScale->SetInputConnection(reader->GetOutputPort());
	shiftScale->SetOutputScalarTypeToUnsignedChar();

	// Create transfer mapping scalar value to opacity.
	auto opacityTransferFunction = vtkSmartPointer<vtkPiecewiseFunction>::New();
	opacityTransferFunction->AddPoint(0.0, 0.0);
	opacityTransferFunction->AddPoint(36.0, 0.125);
	opacityTransferFunction->AddPoint(72.0, 0.25);
	opacityTransferFunction->AddPoint(108.0, 0.375);
	opacityTransferFunction->AddPoint(144.0, 0.5);
	opacityTransferFunction->AddPoint(180.0, 0.625);
	opacityTransferFunction->AddPoint(216.0, 0.75);
	opacityTransferFunction->AddPoint(255.0, 0.875);

	// Create transfer mapping scalar value to color.
	auto colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
	colorTransferFunction->AddRGBPoint(0.0, 0.0, 0.0, 0.0);
	colorTransferFunction->AddRGBPoint(36.0, 1.0, 0.0, 0.0);
	colorTransferFunction->AddRGBPoint(72.0, 1.0, 1.0, 0.0);
	colorTransferFunction->AddRGBPoint(108.0, 0.0, 1.0, 0.0);
	colorTransferFunction->AddRGBPoint(144.0, 0.0, 1.0, 1.0);
	colorTransferFunction->AddRGBPoint(180.0, 0.0, 0.0, 1.0);
	colorTransferFunction->AddRGBPoint(216.0, 1.0, 0.0, 1.0);
	colorTransferFunction->AddRGBPoint(255.0, 1.0, 1.0, 1.0);

	// set up volume property
	auto volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	volumeProperty->SetColor(colorTransferFunction);
	volumeProperty->SetScalarOpacity(opacityTransferFunction);
	volumeProperty->ShadeOff();
	volumeProperty->SetInterpolationTypeToLinear();

	// assign volume property to the volume property widget
	showwidget->volumePropertywidget->setVolumeProperty(volumeProperty);
	if (showwidget->volumePropertywidget == NULL)
	{
		return;
	}

	auto volumeMapper = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
	
	volumeMapper->SetInputConnection(shiftScale->GetOutputPort());

	// The volume holds the mapper and the property and can be used to position/orient the volume.
	auto volume = vtkSmartPointer<vtkVolume>::New();
	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);

	// add the volume into the renderer
	auto renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddVolume(volume);
	renderer->SetBackground(1, 1, 1);

	
	usedwidget = showwidget->practice->onewidget->qvtkwidget1;
	auto window = vtkSmartPointer<vtkRenderWindow>::New();
	usedwidget->SetRenderWindow(window);
	window->AddRenderer(renderer);
	window->Render();
	usedwidget->GetInteractor()->Initialize();
	
}