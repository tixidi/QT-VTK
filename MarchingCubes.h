#pragma once
#include <vtkinclude.h>
#include <QVTKWidget.h>
#include <vtkPLYWriter.h>
class MarchingCubes
{
public:
	MarchingCubes(QVTKWidget *widget);
	void DICOMMarchingCubes(const char* filepath, int skinvalue, int bonevalue, int startvalue, int endvalue);
	void CTDirMarchingCubes(const char* directorPath, int grayValue, const char* plyPath);
	void JPEGMarchingCubes(const char* filepath, int value, int startvalue, int endvalue);
	void WriteOBJ(const char* filepath);
	void test();
private:
	QVTKWidget *qvtkwidget;
	//vtkSmartPointer<vtkRenderWindow> renWin;
};
