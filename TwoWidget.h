#pragma once
#include <qgridlayout.h>
#include <QVTKWidget.h>
#include <vtkinclude.h>
#include <vector>

class TwoWidget : public QWidget
{
	Q_OBJECT
public:
	explicit TwoWidget(QWidget *parent = 0);
	int initialCount[2];
	void initialImage(int i);
	void ShowImage(const char* filepath, int i);
	void ShowDICOM(const char* filepath, int i);
	QVTKWidget *qvtkwidge1, *qvtkwidge2, *currentwidget;
signals:

	public slots :
private:
	std::vector<QVTKWidget*> widgetArray;
	QHBoxLayout *hlayout;

	//VTK∂‘œÛ
	vtkSmartPointer<vtkVectorText> text[2];
	vtkSmartPointer<vtkElevationFilter> elevation[2];
	vtkSmartPointer<vtkPolyDataMapper> mapper[2];
	vtkSmartPointer<vtkActor> actor[2];
	vtkSmartPointer<vtkRenderer> ren[2];
	vtkSmartPointer<vtkRenderWindow> renwindow[2];
	//-------------Õº∆¨œ‘ æ
	vtkSmartPointer<vtkImageReader2Factory> readerFactory2[2];
	vtkSmartPointer<vtkDICOMImageReader> DICOMreader2[2];
	vtkSmartPointer<vtkImageReader2> imageReader2[2];
	vtkSmartPointer<vtkImageActor> actorImage2[2];
	vtkSmartPointer<vtkNamedColors> colors2[2];
	vtkSmartPointer<vtkInteractorStyleImage> style2[2];
};