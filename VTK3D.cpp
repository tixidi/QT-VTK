#include <VTK3D.h>
#include <array>
#include <vtkProperty.h>
#include <vtkPolyDataReader.h>
#include <vtksys/SystemTools.hxx>
#include<stdlib.h>
#include <vector>
VTK3D::VTK3D(QVTKWidget *widget)
{
	qvtkwidget = widget;
	bpFlag = false;
	colorsSet[0] = "Red";
	colorsSet[1] = "Orange";
	colorsSet[2] = "Yellow";
	colorsSet[3] = "Magenta";
	colorsSet[4] = "Purple";
	colorsSet[5] = "Lime";
	colorsSet[6] = "DarkGreen";
	colorsSet[7] = "Cyan";
	colorsSet[8] = "Snow";
	colorsSet[9] = "Gray";
}
//vtkSmartPointer<vtkPolyData> ReadPolyData(const char *fileName);
//vtkSmartPointer<vtkPoints> ReadPoints(const char* filepath);
void VTK3D::ShowMulti(QList <QString> &filepaths)
{
	int filenumbers = filepaths.count();
	// Setup render window
	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	std::vector<vtkSmartPointer<vtkActor>> actors;
	for (int i = 0; i < filenumbers; ++i)
	{
		// Visualize
		vtkSmartPointer<vtkNamedColors> colors =
			vtkSmartPointer<vtkNamedColors>::New();
		QByteArray QBfilePath = filepaths[i].toLocal8Bit();
		const char* filepath = QBfilePath.data();
		//获取扩展名
		vtkSmartPointer<vtkPolyData> polyData =
			ReadPolyData(filepath);
		vtkSmartPointer<vtkPolyDataMapper> mapper =
			vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputData(polyData);

		vtkSmartPointer<vtkActor> actor =
			vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);
		if (bpFlag)
		{
			actor->GetProperty()->SetDiffuseColor(colors->GetColor3d("DarkGray").GetData());
			bpFlag = false;   //重置
		}
		else
		{
			actor->GetProperty()->SetDiffuseColor(colors->GetColor3d(colorsSet[i % 9]).GetData());
		}	
		actor->GetProperty()->SetSpecular(.6);
		actor->GetProperty()->SetSpecularPower(30);		
		//renderer->SetBackground(colors->GetColor3d("mint").GetData());
		actors.push_back(actor);
	}
	// Setup renderer
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	for (int i = 0; i < actors.size(); i++)
	{
		renderer->AddActor(actors[i]);
	}
	renderWindow->AddRenderer(renderer);
	qvtkwidget->SetRenderWindow(renderWindow);
	qvtkwidget->GetRenderWindow()->Render();
	qvtkwidget->GetInteractor()->Initialize();
}

vtkSmartPointer<vtkPolyData> VTK3D::ReadPolyData(const char *filepath)
{
	vtkSmartPointer<vtkPolyData> polyData;
	std::string extension = vtksys::SystemTools::GetFilenameLastExtension(std::string(filepath));
	if (extension == ".ply")
	{
		vtkSmartPointer<vtkPLYReader> reader =
			vtkSmartPointer<vtkPLYReader>::New();
		reader->SetFileName(filepath);
		reader->Update();
		polyData = reader->GetOutput();
	}
	else if (extension == ".vtp")
	{
		vtkSmartPointer<vtkXMLPolyDataReader> reader =
			vtkSmartPointer<vtkXMLPolyDataReader>::New();
		reader->SetFileName(filepath);
		reader->Update();
		polyData = reader->GetOutput();
	}
	else if (extension == ".obj")
	{
		vtkSmartPointer<vtkOBJReader> reader =
			vtkSmartPointer<vtkOBJReader>::New();
		reader->SetFileName(filepath);
		reader->Update();
		polyData = reader->GetOutput();
	}
	else if (extension == ".stl")
	{
		vtkSmartPointer<vtkSTLReader> reader =
			vtkSmartPointer<vtkSTLReader>::New();
		reader->SetFileName(filepath);
		reader->Update();
		polyData = reader->GetOutput();
	}
	else if (extension == ".vtk")
	{
		vtkSmartPointer<vtkPolyDataReader> reader =
			vtkSmartPointer<vtkPolyDataReader>::New();
		reader->SetFileName(filepath);
		reader->Update();
		polyData = reader->GetOutput();
	}
	else if (extension == ".fp")
	{
		vtkSmartPointer<vtkPoints> FeaturePoints = ReadPoints(filepath);
		vtkSmartPointer<vtkPolyData> FeaturePolyData =
			vtkSmartPointer<vtkPolyData>::New();
		FeaturePolyData->SetPoints(FeaturePoints);
		vtkSmartPointer<vtkVertexGlyphFilter> FeatureGlyphFilter =
			vtkSmartPointer<vtkVertexGlyphFilter>::New();
		FeatureGlyphFilter->SetInputData(FeaturePolyData);
		FeatureGlyphFilter->Update();
		polyData = FeatureGlyphFilter->GetOutput();
	}
	else if (extension == ".bp")
	{
		vtkSmartPointer<vtkPoints> FeaturePoints = ReadPoints(filepath);
		vtkSmartPointer<vtkPolyData> FeaturePolyData =
			vtkSmartPointer<vtkPolyData>::New();
		FeaturePolyData->SetPoints(FeaturePoints);
		vtkSmartPointer<vtkVertexGlyphFilter> FeatureGlyphFilter =
			vtkSmartPointer<vtkVertexGlyphFilter>::New();
		FeatureGlyphFilter->SetInputData(FeaturePolyData);
		FeatureGlyphFilter->Update();
		polyData = FeatureGlyphFilter->GetOutput();
		bpFlag = true;
	}
	else if (extension == ".g")
	{
		vtkSmartPointer<vtkBYUReader> reader =
			vtkSmartPointer<vtkBYUReader>::New();
		reader->SetGeometryFileName(filepath);
		reader->Update();
		polyData = reader->GetOutput();
	}
	else
	{
		vtkSmartPointer<vtkSphereSource> source =
			vtkSmartPointer<vtkSphereSource>::New();
		source->Update();
		polyData = source->GetOutput();
	}
	return polyData;
}
void VTK3D::ShowVTK3D(const char* filepath, const char* fileExten)
{
	if (!_stricmp(fileExten, "stl"))
	{
		displaySTL(filepath);
	}
	else if (!_stricmp(fileExten, "obj"))
	{
		displayOBJ(filepath);
	}
	else if (!_stricmp(fileExten, "ply"))
	{
		displayPLY(filepath);
	}
	else if (!_stricmp(fileExten, "fp"))
	{
		displayFP(filepath);
	}
	else if (!_stricmp(fileExten, "bp"))
	{
		displayBP(filepath);
	}
}

void VTK3D::displaySTL(const char *filepath)
{
	vtkSmartPointer<vtkNamedColors> colors =
		vtkSmartPointer<vtkNamedColors>::New();
	/*std::array<unsigned char, 4> bkg{ { 51, 77, 102, 255 } };
	colors->SetColor("BkgColor", bkg.data());*/
	vtkSmartPointer<vtkSTLReader> reader =
		vtkSmartPointer<vtkSTLReader>::New();
	reader->SetFileName(filepath);
	reader->Update();
	// Visualize
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(reader->GetOutputPort());

	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(colors->GetColor3d(colorsSet[6]).GetData());
	
	//aRenderer->SetBackground(colors->GetColor3d("BkgColor").GetData());
	// Initialize the event loop and then start it.
	/*vtksmartpointer<vtkrenderwindowinteractor> interactor = vtksmartpointer<vtkrenderwindowinteractor>::new();
	vtksmartpointer<vtkaxesactor>  axes = vtksmartpointer<vtkaxesactor>::new();
	vtksmartpointer<vtkorientationmarkerwidget>  widget =
		vtksmartpointer<vtkorientationmarkerwidget>::new();
	widget->setinteractor(interactor);
	widget->setorientationmarker(axes);
	widget->setoutlinecolor(1, 1, 1);
	widget->setviewport(0, 0, 0.2, 0.2);
	widget->setenabled(1);*/
	//widget->InteractiveOn();
	//interactor->SetRenderWindow(qvtkwidget->GetRenderWindow());

	vtkSmartPointer<vtkRenderer> aRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	qvtkwidget->SetRenderWindow(renWin);
	qvtkwidget->GetRenderWindow()->AddRenderer(aRenderer);
	aRenderer->AddActor(actor);
	qvtkwidget->GetRenderWindow()->Render();
	qvtkwidget->GetInteractor()->Initialize();
}

void VTK3D::displayOBJ(const char *filepath)
{
	/*vtkSmartPointer<vtkNamedColors> colors =
		vtkSmartPointer<vtkNamedColors>::New();
	std::array<unsigned char, 4> bkg{ { 51, 77, 102, 255 } };
	colors->SetColor("BkgColor", bkg.data());*/
	vtkSmartPointer<vtkOBJReader> reader =
		vtkSmartPointer<vtkOBJReader>::New();
	reader->SetFileName(filepath);
	reader->Update();
	// Visualize
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(reader->GetOutputPort());

	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	vtkSmartPointer<vtkRenderer> aRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	qvtkwidget->SetRenderWindow(renWin);
	qvtkwidget->GetRenderWindow()->AddRenderer(aRenderer);
	aRenderer->AddActor(actor);
	//aRenderer->SetBackground(colors->GetColor3d("BkgColor").GetData());
	// Initialize the event loop and then start it.
	qvtkwidget->GetRenderWindow()->Render();
	qvtkwidget->GetInteractor()->Initialize();
}

void VTK3D::displayPLY(const char *filepath)
{
	vtkSmartPointer<vtkNamedColors> colors =
		vtkSmartPointer<vtkNamedColors>::New();
	vtkSmartPointer<vtkPLYReader> reader =
		vtkSmartPointer<vtkPLYReader>::New();
	reader->SetFileName(filepath);
	reader->Update();
	// Visualize
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(reader->GetOutputPort());

	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(colors->GetColor3d(colorsSet[6]).GetData());

	vtkSmartPointer<vtkRenderer> aRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	qvtkwidget->SetRenderWindow(renWin);
	qvtkwidget->GetRenderWindow()->AddRenderer(aRenderer);
	aRenderer->AddActor(actor);
	qvtkwidget->GetRenderWindow()->Render();
	qvtkwidget->GetInteractor()->Initialize();
}

void VTK3D::displayFP(const char *filepath)
{
	vtkSmartPointer<vtkNamedColors> colors =
		vtkSmartPointer<vtkNamedColors>::New();
	vtkSmartPointer<vtkPoints> FeaturePoints = ReadPoints(filepath);
	vtkSmartPointer<vtkPolyData> FeaturePolyData =
		vtkSmartPointer<vtkPolyData>::New();
	FeaturePolyData->SetPoints(FeaturePoints);
	vtkSmartPointer<vtkVertexGlyphFilter> FeatureGlyphFilter =
		vtkSmartPointer<vtkVertexGlyphFilter>::New();
	FeatureGlyphFilter->SetInputData(FeaturePolyData);
	FeatureGlyphFilter->Update();
	// Visualize
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(FeatureGlyphFilter->GetOutputPort());
	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(colors->GetColor3d(colorsSet[0]).GetData());

	vtkSmartPointer<vtkRenderer> aRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	qvtkwidget->SetRenderWindow(renWin);
	qvtkwidget->GetRenderWindow()->AddRenderer(aRenderer);
	aRenderer->AddActor(actor);
	qvtkwidget->GetRenderWindow()->Render();
	qvtkwidget->GetInteractor()->Initialize();
}

void VTK3D::displayBP(const char *filepath)
{
	vtkSmartPointer<vtkNamedColors> colors =
		vtkSmartPointer<vtkNamedColors>::New();
	vtkSmartPointer<vtkPoints> FeaturePoints = ReadPoints(filepath);
	vtkSmartPointer<vtkPolyData> FeaturePolyData =
		vtkSmartPointer<vtkPolyData>::New();
	FeaturePolyData->SetPoints(FeaturePoints);
	vtkSmartPointer<vtkVertexGlyphFilter> FeatureGlyphFilter =
		vtkSmartPointer<vtkVertexGlyphFilter>::New();
	FeatureGlyphFilter->SetInputData(FeaturePolyData);
	FeatureGlyphFilter->Update();
	// Visualize
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(FeatureGlyphFilter->GetOutputPort());
	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(colors->GetColor3d("DarkGray").GetData());

	vtkSmartPointer<vtkRenderer> aRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	qvtkwidget->SetRenderWindow(renWin);
	qvtkwidget->GetRenderWindow()->AddRenderer(aRenderer);
	aRenderer->AddActor(actor);
	qvtkwidget->GetRenderWindow()->Render();
	qvtkwidget->GetInteractor()->Initialize();
}

vtkSmartPointer<vtkPoints> VTK3D::ReadPoints(const char *filepath)
{
	std::ifstream filestream(filepath);

	std::string line;
	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();

	while (std::getline(filestream, line))
	{
		double x, y, z;
		std::stringstream linestream;
		linestream << line;
		linestream >> x >> y >> z;

		points->InsertNextPoint(x, y, z);
	}

	filestream.close();

	return points;
}