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
	auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	std::vector<vtkSmartPointer<vtkActor>> actors;
	for (int i = 0; i < filenumbers; ++i)
	{
		// Visualize
		auto colors = vtkSmartPointer<vtkNamedColors>::New();
		QByteArray QBfilePath = filepaths[i].toLocal8Bit();
		const char* filepath = QBfilePath.data();
		//获取扩展名
		auto polyData = ReadPolyData(filepath);
		auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputData(polyData);

		auto actor = vtkSmartPointer<vtkActor>::New();
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
	auto renderer = vtkSmartPointer<vtkRenderer>::New();
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
		auto reader = vtkSmartPointer<vtkPLYReader>::New();
		reader->SetFileName(filepath);
		reader->Update();
		polyData = reader->GetOutput();
	}
	else if (extension == ".vtp")
	{
		 auto reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
		reader->SetFileName(filepath);
		reader->Update();
		polyData = reader->GetOutput();
	}
	else if (extension == ".obj")
	{
		auto reader = vtkSmartPointer<vtkOBJReader>::New();
		reader->SetFileName(filepath);
		reader->Update();
		polyData = reader->GetOutput();
	}
	else if (extension == ".stl")
	{
		auto reader = vtkSmartPointer<vtkSTLReader>::New();
		reader->SetFileName(filepath);
		reader->Update();
		polyData = reader->GetOutput();
	}
	else if (extension == ".vtk")
	{
		auto reader = vtkSmartPointer<vtkPolyDataReader>::New();
		reader->SetFileName(filepath);
		reader->Update();
		polyData = reader->GetOutput();
	}
	else if (extension == ".fp")
	{
		auto FeaturePoints = ReadPoints(filepath);
		auto FeaturePolyData = vtkSmartPointer<vtkPolyData>::New();
		FeaturePolyData->SetPoints(FeaturePoints);
		auto FeatureGlyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
		FeatureGlyphFilter->SetInputData(FeaturePolyData);
		FeatureGlyphFilter->Update();
		polyData = FeatureGlyphFilter->GetOutput();
	}
	else if (extension == ".bp")
	{
		auto FeaturePoints = ReadPoints(filepath);
		auto FeaturePolyData = vtkSmartPointer<vtkPolyData>::New();
		FeaturePolyData->SetPoints(FeaturePoints);
		auto FeatureGlyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
		FeatureGlyphFilter->SetInputData(FeaturePolyData);
		FeatureGlyphFilter->Update();
		polyData = FeatureGlyphFilter->GetOutput();
		bpFlag = true;
	}
	else if (extension == ".g")
	{
		auto reader = vtkSmartPointer<vtkBYUReader>::New();
		reader->SetGeometryFileName(filepath);
		reader->Update();
		polyData = reader->GetOutput();
	}
	else
	{
		auto source = vtkSmartPointer<vtkSphereSource>::New();
		source->Update();
		polyData = source->GetOutput();
	}
	return polyData;
}
void VTK3D::ShowVTK3D(const char* filepath)
{
	std::string extension = vtksys::SystemTools::GetFilenameLastExtension(std::string(filepath));
	if (extension == ".fp" || extension == ".bp")
		displayPoint(filepath);
	else
		display3D(filepath);
}

vtkSmartPointer<vtkPoints> VTK3D::ReadPoints(const char *filepath)
{
	std::ifstream filestream(filepath);

	std::string line;
	auto points = vtkSmartPointer<vtkPoints>::New();

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

void VTK3D::display3D(const char* filepath)
{
	auto colors = vtkSmartPointer<vtkNamedColors>::New();
	auto polyData = ReadPolyData(filepath);
	// Visualize
	auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(polyData);

	auto actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(colors->GetColor3d(colorsSet[6]).GetData());

	auto aRenderer = vtkSmartPointer<vtkRenderer>::New();
	auto renWin = vtkSmartPointer<vtkRenderWindow>::New();
	qvtkwidget->SetRenderWindow(renWin);
	qvtkwidget->GetRenderWindow()->AddRenderer(aRenderer);
	aRenderer->AddActor(actor);
	qvtkwidget->GetRenderWindow()->Render();
	qvtkwidget->GetInteractor()->Initialize();
}


void VTK3D::displayPoint(const char* filepath)
{
	std::string extension = vtksys::SystemTools::GetFilenameLastExtension(std::string(filepath));
	auto colors = vtkSmartPointer<vtkNamedColors>::New();
	auto FeaturePoints = ReadPoints(filepath);
	auto FeaturePolyData = vtkSmartPointer<vtkPolyData>::New();
	FeaturePolyData->SetPoints(FeaturePoints);
	auto FeatureGlyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
	FeatureGlyphFilter->SetInputData(FeaturePolyData);
	FeatureGlyphFilter->Update();
	// Visualize
	auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(FeatureGlyphFilter->GetOutputPort());
	auto actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	if (extension == ".bp")
	{
		actor->GetProperty()->SetColor(colors->GetColor3d("DarkGray").GetData());
	}
	else if (extension == ".fp")
	{
		actor->GetProperty()->SetColor(colors->GetColor3d(colorsSet[0]).GetData());
	}
	else
	{
		std::cout << "error! file extension is wrong!" << std::endl;
		return;
	}
	auto aRenderer = vtkSmartPointer<vtkRenderer>::New();
	auto renWin = vtkSmartPointer<vtkRenderWindow>::New();
	qvtkwidget->SetRenderWindow(renWin);
	qvtkwidget->GetRenderWindow()->AddRenderer(aRenderer);
	aRenderer->AddActor(actor);
	qvtkwidget->GetRenderWindow()->Render();
	qvtkwidget->GetInteractor()->Initialize();
}