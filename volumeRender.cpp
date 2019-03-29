#include <volumeRender.h>
#include <qfiledialog.h>
#include <qdir.h>
#include <qfileinfo.h>
VolumeRender::VolumeRender(QVTKWidget* widget, VolumeData *input)
{
	data = input;
	qvtkwidget = widget;
}

void VolumeRender::GPU(const char* file_path, const char* fileExt)
{
	vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper =
		vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
	if (!_stricmp(fileExt, "jpg"))
	{
		//读取JPG序列图像
		vtkSmartPointer<vtkJPEGReader>reader =
			vtkSmartPointer<vtkJPEGReader>::New();
		reader->SetFilePrefix(file_path);
		reader->SetFilePattern("%s/%d.jpg");
		reader->SetDataExtent(0, 511, 0, 511, data->start, data->end);//图像大小是512*512  
		reader->SetFileNameSliceOffset(0);
		reader->SetFileNameSliceSpacing(1);
		reader->SetDataSpacing(1, 1, 1);
		reader->Update();
		vtkSmartPointer<vtkImageCast> readerImageCast = vtkSmartPointer<vtkImageCast>::New();
		readerImageCast->SetInputConnection(reader->GetOutputPort());
		readerImageCast->SetOutputScalarTypeToUnsignedShort();
		readerImageCast->Update();
		volumeMapper->SetInputData(readerImageCast->GetOutput());
	}
	if (!_stricmp(fileExt, "dcm"))
	{
		//vtkSmartPointer<vtkImageReader2> reader =
		//	vtkSmartPointer<vtkImageReader2>::New();
		//reader->SetFilePrefix(file_path);
		//reader->SetFilePattern("%s/%d.dcm");
		///*reader->SetDataDimensions(512, 512);
		//reader->SetImageRange(data->start, data->end);*/
		//reader->SetDataExtent(0, 511, 0, 511, data->start, data->end);
		////reader->SetDataByteOrderToLittleEndian();
		//reader->SetDataSpacing(1, 1, 1.0);
		//reader->Update();
		//volumeMapper->SetInputData(reader->GetOutput());
		vtkSmartPointer<vtkDICOMImageReader> dicomReader = vtkSmartPointer<vtkDICOMImageReader>::New();
		dicomReader->SetDirectoryName(file_path);
		dicomReader->Update();
		volumeMapper->SetInputData(dicomReader->GetOutput());

	}
	volumeMapper->SetSampleDistance(volumeMapper->GetSampleDistance() / 2); //设置光线采样距离  
	//volumeMapper->SetAutoAdjustSampleDistances(0);//设置图像采样步长  
	//volumeMapper->SetImageSampleDistance(4);  
    /*************************************************************************/
	Rendering(volumeMapper);
}

void VolumeRender::CPU(const char* file_path, const char* fileExt)
{
	vtkSmartPointer<vtkFixedPointVolumeRayCastMapper> volumeMapper =
		vtkSmartPointer<vtkFixedPointVolumeRayCastMapper>::New();
	if (!_stricmp(fileExt, "jpg"))
	{
		//读取JPG序列图像
		vtkSmartPointer<vtkJPEGReader>reader =
			vtkSmartPointer<vtkJPEGReader>::New();
		reader->SetFilePrefix(file_path);
		reader->SetFilePattern("%s/%d.jpg");
		reader->SetDataExtent(0, 511, 0, 511, data->start, data->end);//图像大小是512*512  
		reader->SetFileNameSliceOffset(0);
		reader->SetFileNameSliceSpacing(1);
		reader->SetDataSpacing(1, 1, 1);
		reader->Update();
		vtkSmartPointer<vtkImageCast> readerImageCast = vtkSmartPointer<vtkImageCast>::New();
		readerImageCast->SetInputConnection(reader->GetOutputPort());
		readerImageCast->SetOutputScalarTypeToUnsignedShort();
		readerImageCast->Update();
		volumeMapper->SetInputData(readerImageCast->GetOutput());
	}
	if (!_stricmp(fileExt, "dcm"))
	{
		//vtkSmartPointer<vtkImageReader2> reader =
		//	vtkSmartPointer<vtkImageReader2>::New();
		//reader->SetFilePrefix(file_path);
		//reader->SetFilePattern("%s/%d.dcm");
		///*reader->SetDataDimensions(512, 512);
		//reader->SetImageRange(data->start, data->end);*/
		//reader->SetDataExtent(0, 511, 0, 511, data->start, data->end);
		////reader->SetDataByteOrderToLittleEndian();
		//reader->SetDataSpacing(1, 1, 1.0);
		//reader->Update();
		//volumeMapper->SetInputData(reader->GetOutput());
		vtkSmartPointer<vtkDICOMImageReader> dicomReader = vtkSmartPointer<vtkDICOMImageReader>::New();
		dicomReader->SetDirectoryName(file_path);
		dicomReader->Update();
		volumeMapper->SetInputData(dicomReader->GetOutput());

	}
	volumeMapper->SetSampleDistance(volumeMapper->GetSampleDistance() / 2); //设置光线采样距离  
	//volumeMapper->SetAutoAdjustSampleDistances(0);//设置图像采样步长  
    //volumeMapper->SetImageSampleDistance(4);  
	/*************************************************************************/
	Rendering(volumeMapper);
}


void VolumeRender::Rendering(vtkAbstractVolumeMapper *mapper)
{
	vtkSmartPointer<vtkNamedColors> colors =
		vtkSmartPointer<vtkNamedColors>::New();

	vtkSmartPointer<vtkVolumeProperty> volumeProperty =
		vtkSmartPointer<vtkVolumeProperty>::New();
	volumeProperty->SetInterpolationTypeToLinear();
	volumeProperty->ShadeOn();  //打开或者关闭阴影测试  
	volumeProperty->SetAmbient(data->Property[0][0]);//漫反射  
	volumeProperty->SetDiffuse(data->Property[0][1]);  //镜面反射
	volumeProperty->SetSpecular(data->Property[0][2]);  //设置不透明度   data->Property[0][0]

	if (data->Sn > 0)
	{
		vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity =
			vtkSmartPointer<vtkPiecewiseFunction>::New();
		for (int i = 0; i < data->Sn; i++)
		{
			compositeOpacity->AddPoint(data->Scalar[i][0], data->Scalar[i][1], data->Scalar[i][2], data->Scalar[i][3]);
		}
		volumeProperty->SetScalarOpacity(compositeOpacity); //设置不透明度传输函数  
	}
	
	if (data->Gn > 0)
	{
		vtkSmartPointer<vtkPiecewiseFunction> volumeGradientOpacity =   //设置梯度不透明属性  
			vtkSmartPointer<vtkPiecewiseFunction>::New();
		for (int i = 0; i < data->Gn; i++)
		{
			volumeGradientOpacity->AddPoint(data->Gradient[i][0], data->Gradient[i][1],
				data->Gradient[i][2], data->Gradient[i][3]);
		}
		volumeProperty->SetGradientOpacity(volumeGradientOpacity);//设置梯度不透明度效果对比  
	}
	
	if (data->Cn > 0)
	{
		vtkSmartPointer<vtkColorTransferFunction> color =         //设置颜色属性  
			vtkSmartPointer<vtkColorTransferFunction>::New();
		for (int i = 0; i < data->Cn; i++)
		{
			color->AddRGBPoint(data->Color[i][0], data->Color[i][1], data->Color[i][2], data->Color[i][3],
				data->Color[i][4], data->Color[i][5]);
		}
		volumeProperty->SetColor(color);
	}
	
	vtkSmartPointer<vtkVolume> volume =
		vtkSmartPointer<vtkVolume>::New();
	volume->SetMapper(mapper);
	volume->SetProperty(volumeProperty);
	vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
	//ren->SetBackground(0, 1, 0);
	ren->AddVolume(volume);

	qvtkwidget->GetRenderWindow()->AddRenderer(ren);
	qvtkwidget->GetInteractor()->Initialize();
}