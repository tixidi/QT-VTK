#pragma once
#include <MarchingCubes.h>
#include <qdir.h>
#include <qfileinfo.h>
#include <array>
#include <qmessagebox.h>
//#include <vtkDICOMReader.h> 

MarchingCubes::MarchingCubes(QVTKWidget *widget)
{
	qvtkwidget = widget;
}

void MarchingCubes::DICOMMarchingCubes(const char* filepath, int skinvalue, int bonevalue, int startvalue, int endvalue)
{
	vtkSmartPointer<vtkNamedColors> colors =
		vtkSmartPointer<vtkNamedColors>::New();

	// Set the colors.
	std::array<unsigned char, 4> skinColor{ { 255, 125, 64 } };
	colors->SetColor("SkinColor", skinColor.data());
	/*std::array<unsigned char, 4> bkg{ { 51, 77, 102, 255 } };
	colors->SetColor("BkgColor", bkg.data());*/

	// Create the renderer, the render window, and the interactor. The renderer
	// draws into the render window, the interactor enables mouse- and
	// keyboard-based interaction with the data within the render window.
	//
	vtkSmartPointer<vtkRenderer> aRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	qvtkwidget->SetRenderWindow(renWin);
	qvtkwidget->GetRenderWindow()->AddRenderer(aRenderer);

	vtkSmartPointer<vtkImageReader2> reader =
		vtkSmartPointer<vtkImageReader2>::New();
	reader->SetFilePrefix(filepath);
	reader->SetFilePattern("%s/%d.dcm");
	reader->SetDataExtent(0, 511, 0, 511, startvalue, endvalue);
	reader->SetDataSpacing(1, 1, 1.0);
	reader->SetDataScalarTypeToUnsignedShort();
	reader->UpdateWholeExtent();
	// An isosurface, or contour value of 500 is known to correspond to the
	// skin of the patient.
	// The triangle stripper is used to create triangle strips from the
	// isosurface; these render much faster on many systems.
	vtkSmartPointer<vtkMarchingCubes> skinExtractor =
		vtkSmartPointer<vtkMarchingCubes>::New();
	skinExtractor->SetInputConnection(reader->GetOutputPort());
	skinExtractor->SetValue(0, skinvalue);                             //default skinvalue = 300

	vtkSmartPointer<vtkStripper> skinStripper =
		vtkSmartPointer<vtkStripper>::New();
	skinStripper->SetInputConnection(skinExtractor->GetOutputPort());

	vtkSmartPointer<vtkPolyDataMapper> skinMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	skinMapper->SetInputConnection(skinStripper->GetOutputPort());
	skinMapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> skin =
		vtkSmartPointer<vtkActor>::New();
	skin->SetMapper(skinMapper);
	skin->GetProperty()->SetDiffuseColor(colors->GetColor3d("SkinColor").GetData());
	skin->GetProperty()->SetSpecular(.3);
	skin->GetProperty()->SetSpecularPower(20);
	skin->GetProperty()->SetOpacity(.5);

	// An isosurface, or contour value of 1150 is known to correspond to the
	// bone of the patient.
	// The triangle stripper is used to create triangle strips from the
	// isosurface; these render much faster on may systems.
	vtkSmartPointer<vtkMarchingCubes> boneExtractor =
		vtkSmartPointer<vtkMarchingCubes>::New();
	boneExtractor->SetInputConnection(reader->GetOutputPort());
	boneExtractor->SetValue(0, bonevalue);                    //defalut bonevalue = 1150

	vtkSmartPointer<vtkStripper> boneStripper =
		vtkSmartPointer<vtkStripper>::New();
	boneStripper->SetInputConnection(boneExtractor->GetOutputPort());

	vtkSmartPointer<vtkPolyDataMapper> boneMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	boneMapper->SetInputConnection(boneStripper->GetOutputPort());
	boneMapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> bone =
		vtkSmartPointer<vtkActor>::New();
	bone->SetMapper(boneMapper);
	bone->GetProperty()->SetDiffuseColor(colors->GetColor3d("Ivory").GetData());

	// An outline provides context around the data.
	//
	vtkSmartPointer<vtkOutlineFilter> outlineData =
		vtkSmartPointer<vtkOutlineFilter>::New();
	outlineData->SetInputConnection(reader->GetOutputPort());

	vtkSmartPointer<vtkPolyDataMapper> mapOutline =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapOutline->SetInputConnection(outlineData->GetOutputPort());

	vtkSmartPointer<vtkActor> outline =
		vtkSmartPointer<vtkActor>::New();
	outline->SetMapper(mapOutline);
	outline->GetProperty()->SetColor(colors->GetColor3d("Black").GetData());

	// It is convenient to create an initial view of the data. The FocalPoint
	// and Position form a vector direction. Later on (ResetCamera() method)
	// this vector is used to position the camera to look at the data in
	// this direction.
	vtkSmartPointer<vtkCamera> aCamera = vtkSmartPointer<vtkCamera>::New();
	aCamera->SetViewUp(0, 0, -1);
	aCamera->SetPosition(0, -1, 0);
	aCamera->SetFocalPoint(0, 0, 0);
	aCamera->ComputeViewPlaneNormal();
	aCamera->Azimuth(30.0);
	aCamera->Elevation(30.0);

	// Actors are added to the renderer. An initial camera view is created.
	// The Dolly() method moves the camera towards the FocalPoint,
	// thereby enlarging the image.
	aRenderer->AddActor(outline);
	aRenderer->AddActor(skin);
	aRenderer->AddActor(bone);
	aRenderer->SetActiveCamera(aCamera);
	aRenderer->ResetCamera();
	aCamera->Dolly(1.5);

	// Set a background color for the renderer and set the size of the
	// render window (expressed in pixels).
	//aRenderer->SetBackground(colors->GetColor3d("BkgColor").GetData());

	// Note that when camera movement occurs (as it does in the Dolly()
	// method), the clipping planes often need adjusting. Clipping planes
	// consist of two planes: near and far along the view direction. The
	// near plane clips out objects in front of the plane; the far plane
	// clips out objects behind the plane. This way only what is drawn
	// between the planes is actually rendered.
	aRenderer->ResetCameraClippingRange();

	// Initialize the event loop and then start it.
	qvtkwidget->GetRenderWindow()->Render();
	qvtkwidget->GetInteractor()->Initialize();

	return;
}

void MarchingCubes::JPEGMarchingCubes(const char* filepath, int value, int startvalue, int endvalue)
{
	int marchingvalue = value;
	//读取JPG序列图像
	vtkSmartPointer<vtkJPEGReader>reader =
		vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFilePrefix(filepath);
	reader->SetFilePattern("%s/%d.jpg");
	reader->SetDataExtent(0, 511, 0, 511, startvalue, endvalue);//图像大小是512*512  
	reader->SetFileNameSliceOffset(0);
	reader->SetFileNameSliceSpacing(1);
	reader->SetDataSpacing(1, 1, 1);
	reader->Update();

	vtkSmartPointer<vtkImageCast>readerImageCast = vtkSmartPointer<vtkImageCast>::New();
	readerImageCast->SetInputConnection(reader->GetOutputPort());
	readerImageCast->SetOutputScalarTypeToUnsignedShort();
	readerImageCast->Update();

	vtkSmartPointer<vtkNamedColors> colors =
		vtkSmartPointer<vtkNamedColors>::New();

	// Set the colors.
	std::array<unsigned char, 4> skinColor{ { 255, 125, 64 } };
	colors->SetColor("SkinColor", skinColor.data());
	/*std::array<unsigned char, 4> bkg{ { 51, 77, 102, 255 } };
	colors->SetColor("BkgColor", bkg.data());*/

	vtkSmartPointer<vtkMarchingCubes> skinExtractor =
		vtkSmartPointer<vtkMarchingCubes>::New();
	skinExtractor->SetInputConnection(reader->GetOutputPort());
	skinExtractor->SetValue(0, marchingvalue);

	vtkSmartPointer<vtkStripper> skinStripper =
		vtkSmartPointer<vtkStripper>::New();
	skinStripper->SetInputConnection(skinExtractor->GetOutputPort());

	vtkSmartPointer<vtkPolyDataMapper> skinMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	skinMapper->SetInputConnection(skinStripper->GetOutputPort());
	skinMapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> skin =
		vtkSmartPointer<vtkActor>::New();
	skin->SetMapper(skinMapper);
	skin->GetProperty()->SetDiffuseColor(colors->GetColor3d("SkinColor").GetData());
	skin->GetProperty()->SetSpecular(.3);
	skin->GetProperty()->SetSpecularPower(20);
	skin->GetProperty()->SetOpacity(.5);

	vtkSmartPointer<vtkRenderer> aRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	qvtkwidget->SetRenderWindow(renWin);
	qvtkwidget->GetRenderWindow()->AddRenderer(aRenderer);
	// It is convenient to create an initial view of the data. The FocalPoint
	// and Position form a vector direction. Later on (ResetCamera() method)
	// this vector is used to position the camera to look at the data in
	// this direction.
	vtkSmartPointer<vtkCamera> aCamera = vtkSmartPointer<vtkCamera>::New();
	aCamera->SetViewUp(0, 0, -1);
	aCamera->SetPosition(0, -1, 0);
	aCamera->SetFocalPoint(0, 0, 0);
	aCamera->ComputeViewPlaneNormal();
	aCamera->Azimuth(30.0);
	aCamera->Elevation(30.0);

	// Actors are added to the renderer. An initial camera view is created.
	// The Dolly() method moves the camera towards the FocalPoint,
	// thereby enlarging the image.	
	aRenderer->AddActor(skin);
	aRenderer->SetActiveCamera(aCamera);
	aRenderer->ResetCamera();
	aCamera->Dolly(1.5);

	// Set a background color for the renderer and set the size of the
	// render window (expressed in pixels).
	//aRenderer->SetBackground(colors->GetColor3d("BkgColor").GetData());

	// Note that when camera movement occurs (as it does in the Dolly()
	// method), the clipping planes often need adjusting. Clipping planes
	// consist of two planes: near and far along the view direction. The
	// near plane clips out objects in front of the plane; the far plane
	// clips out objects behind the plane. This way only what is drawn
	// between the planes is actually rendered.
	aRenderer->ResetCameraClippingRange();

	// Initialize the event loop and then start it.
	qvtkwidget->GetRenderWindow()->Render();
	qvtkwidget->GetInteractor()->Initialize();

	//renWin->AddRenderer(aRenderer);                        --bug出现在这里
	return;
}

/*
 * 对整个文件夹中的DICOM图像进行面绘，并输出PLY模型
 */
void MarchingCubes::CTDirMarchingCubes(const char* directorPath, int grayValue, const char* plyPath)
{
	vtkSmartPointer<vtkNamedColors> colors =
		vtkSmartPointer<vtkNamedColors>::New();

	// Set the colors.
	std::array<unsigned char, 4> skinColor{ { 255, 125, 64 } };
	colors->SetColor("SkinColor", skinColor.data());
	vtkSmartPointer<vtkRenderer> aRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	qvtkwidget->SetRenderWindow(renWin);
	qvtkwidget->GetRenderWindow()->AddRenderer(aRenderer);

	vtkSmartPointer<vtkDICOMImageReader> reader =
		vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetDataByteOrderToLittleEndian();
	reader->SetDirectoryName(directorPath);
	reader->SetDataSpacing(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkMarchingCubes> skinExtractor =
		vtkSmartPointer<vtkMarchingCubes>::New();
	skinExtractor->SetInputConnection(reader->GetOutputPort());
	skinExtractor->SetValue(0, grayValue);                             //default skinvalue = 500

	vtkSmartPointer<vtkStripper> skinStripper =
		vtkSmartPointer<vtkStripper>::New();
	skinStripper->SetInputConnection(skinExtractor->GetOutputPort());

	vtkSmartPointer<vtkPolyDataMapper> skinMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	skinMapper->SetInputConnection(skinStripper->GetOutputPort());
	skinMapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> skin =
		vtkSmartPointer<vtkActor>::New();
	skin->SetMapper(skinMapper);
	skin->GetProperty()->SetDiffuseColor(colors->GetColor3d("SkinColor").GetData());

	// An outline provides context around the data
	vtkSmartPointer<vtkOutlineFilter> outlineData =
		vtkSmartPointer<vtkOutlineFilter>::New();
	outlineData->SetInputConnection(reader->GetOutputPort());

	vtkSmartPointer<vtkPolyDataMapper> mapOutline =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapOutline->SetInputConnection(outlineData->GetOutputPort());

	vtkSmartPointer<vtkActor> outline =
		vtkSmartPointer<vtkActor>::New();
	outline->SetMapper(mapOutline);
	outline->GetProperty()->SetColor(colors->GetColor3d("Black").GetData());

	/*PLY模型输出*/
	vtkSmartPointer<vtkPLYWriter> plyWriter =
		vtkSmartPointer<vtkPLYWriter>::New();
	plyWriter->SetFileName(plyPath);
	plyWriter->SetInputConnection(skinExtractor->GetOutputPort());
	plyWriter->Write();

	vtkSmartPointer<vtkCamera> aCamera = vtkSmartPointer<vtkCamera>::New();
	aCamera->SetViewUp(0, 0, -1);
	aCamera->SetPosition(0, -1, 0);
	aCamera->SetFocalPoint(0, 0, 0);
	aCamera->ComputeViewPlaneNormal();
	aCamera->Azimuth(30.0);
	aCamera->Elevation(30.0);

	aRenderer->AddActor(outline);
	aRenderer->AddActor(skin);
	aRenderer->SetActiveCamera(aCamera);
	aRenderer->ResetCamera();
	aCamera->Dolly(1.5);

	aRenderer->ResetCameraClippingRange();

	// Initialize the event loop and then start it.
	qvtkwidget->GetRenderWindow()->Render();
	qvtkwidget->GetInteractor()->Initialize();

	return;
}

void MarchingCubes::WriteOBJ(const char* filepath)
{
	vtkSmartPointer<vtkOBJExporter> porter = vtkSmartPointer<vtkOBJExporter>::New();
	porter->SetFilePrefix(filepath);
	porter->SetInput(qvtkwidget->GetRenderWindow());
	//porter->SetInput(renWin);
	porter->Write();
}

void MarchingCubes::test()
{
	const char* filepath = "D:\\test";
	vtkSmartPointer<vtkOBJExporter> porter = vtkSmartPointer<vtkOBJExporter>::New();
	porter->SetFilePrefix(filepath);
	porter->SetInput(qvtkwidget->GetRenderWindow());
	//porter->SetInput(renWin);
	porter->Write();
}