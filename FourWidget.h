#pragma once
#include <qgridlayout.h>
#include <QVTKWidget.h>
#include <vtkinclude.h>
#include <vector>

class FourWidget : public QWidget
{
	Q_OBJECT
public:
	explicit FourWidget(QWidget *parent = 0);
	int initialCount[4];
	void initialImage(int i);
	void ShowImage(const char* filepath, int i);
	void ShowDICOM(const char* filepath, int i);
	QVTKWidget *qvtkwidge1, *qvtkwidge2, *qvtkwidge3, *qvtkwidge4, *currentwidget;
signals:

	public slots :
private:
	std::vector<QVTKWidget*> widgetArray;
	QGridLayout *hlayout;

	//VTK∂‘œÛ
	vtkSmartPointer<vtkVectorText> text[4];
	vtkSmartPointer<vtkElevationFilter> elevation[4];
	vtkSmartPointer<vtkPolyDataMapper> mapper[4];
	vtkSmartPointer<vtkActor> actor[4];
	vtkSmartPointer<vtkRenderer> ren[4];
	vtkSmartPointer<vtkRenderWindow> renwindow[4];
	//-------------Õº∆¨œ‘ æ
	vtkSmartPointer<vtkImageReader2Factory> readerFactory4[4];
	vtkSmartPointer<vtkDICOMImageReader> DICOMreader4[4];
	vtkSmartPointer<vtkImageReader2> imageReader4[4];
	vtkSmartPointer<vtkImageActor> actorImage4[4];
	vtkSmartPointer<vtkNamedColors> colors4[4];
	vtkSmartPointer<vtkInteractorStyleImage> style4[4];
};
