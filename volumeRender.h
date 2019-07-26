#pragma once
#include <vtkinclude.h> 
#include <QVTKWidget.h>
#include <Data.h>
#include <vtkAbstractVolumeMapper.h>
#include <ctkVTKVolumePropertyWidget.h>
class VolumeRender
{
public:
	VolumeRender(QVTKWidget* widget, VolumeData *input, ctkVTKVolumePropertyWidget* volumeWidget = nullptr);
	void GPU(const char* file_path, const char* fileExt);
	void CPU(const char* file_path, const char* fileExt);
	void Rendering(vtkAbstractVolumeMapper *mapper);
private:
	QVTKWidget *qvtkwidget;
	bool GPUEnable;
	VolumeData *data;
	ctkVTKVolumePropertyWidget *volumeWidget;
};
