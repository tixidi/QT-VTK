#pragma once
#include <vtkinclude.h> 
#include <QVTKWidget.h>
#include <Data.h>
#include <vtkAbstractVolumeMapper.h>
class VolumeRender
{
public:
	VolumeRender(QVTKWidget* widget, VolumeData *input);
	void GPU(const char* file_path, const char* fileExt);
	void CPU(const char* file_path, const char* fileExt);
	void Rendering(vtkAbstractVolumeMapper *mapper);
private:
	QVTKWidget *qvtkwidget;
	VolumeData *data;
};
