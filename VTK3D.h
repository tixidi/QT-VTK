#pragma once
#include <vtkPolyData.h>
#include <vtkBYUReader.h>
#include <vtkOBJReader.h>
#include <vtkPLYReader.h>
#include <vtkPolyDataReader.h>
#include <vtkSTLReader.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkSphereSource.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkNamedColors.h>
#include <QVTKWidget.h>
#include <qlist.h>
#include <qstring.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <sstream>
#include <vtkVertexGlyphFilter.h>
class VTK3D {
public:
	VTK3D(QVTKWidget *widget);
	void ShowVTK3D(const char* filepath);
	void ShowMulti(QList <QString> &filepaths);
private:
	QVTKWidget * qvtkwidget;
	const char *colorsSet[10];
	bool bpFlag;    //背景顶点特有的暗灰色
	vtkSmartPointer<vtkPoints> ReadPoints(const char *filepath);
	vtkSmartPointer<vtkPolyData> VTK3D::ReadPolyData(const char *filepath);
	void display3D(const char* filepath);  //显示单个3D格式的文件
	void displayPoint(const char* filepath);  //显示3D特征点,点的三维坐标
};
