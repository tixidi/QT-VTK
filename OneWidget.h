#pragma once
#include <qgridlayout.h>
#include <QVTKWidget.h>
#include <vtkinclude.h>
#include <qframe.h>

class vtkQtTableView;

class OneWidget : public QWidget
{
	Q_OBJECT
public:
	explicit OneWidget(QWidget *parent = 0);
	void initialImage();
	void ShowImage(const char* filapath);
	void ShowDICOM(const char* filepath);
	vtkRenderer *getRender();
	vtkRenderWindowInteractor *getInteractor();
	QVTKInteractor *getQVTKInteractor();
public:
	//统计数字
	int initialCount;
	QVTKWidget *qvtkwidget1;
	QVBoxLayout *vlayout1;
	//QFrame *tableFrame;
	//VTK对象

signals:

	public slots:
private:
	vtkSmartPointer<vtkVectorText> text1;
	vtkSmartPointer<vtkElevationFilter> elevation1;
	vtkSmartPointer<vtkPolyDataMapper> mapper1;
	vtkSmartPointer<vtkActor> actor1;
	vtkSmartPointer<vtkRenderer> ren1;
	vtkSmartPointer<vtkRenderWindow> renwindow1;
	vtkSmartPointer<vtkQtTableView> TableView1;
	vtkSmartPointer<vtkDataObjectToTable> toTable1;
	//-------------图片显示
	vtkSmartPointer<vtkImageReader2Factory> readerFactory1;
	vtkSmartPointer<vtkDICOMImageReader> DICOMreader1;
	vtkSmartPointer<vtkImageReader2> imageReader1;
	vtkSmartPointer<vtkImageActor> actorImage1;
	vtkSmartPointer<vtkNamedColors> colors1;
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;
	vtkSmartPointer<QVTKInteractor> qvtkInteractor;
	vtkSmartPointer<vtkInteractorStyleImage> style1;
};