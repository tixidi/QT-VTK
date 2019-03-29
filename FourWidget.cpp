#include <FourWidget.h>
FourWidget::FourWidget(QWidget *parent) : QWidget(parent)
{
	for (int i = 0; i < 4; i++)
	{
		initialCount[i] = 0;
	}
	hlayout = new QGridLayout(this);
	qvtkwidge1 = new QVTKWidget(this);;
	hlayout->addWidget(qvtkwidge1, 0, 0);
	qvtkwidge2 = new QVTKWidget(this);
	hlayout->addWidget(qvtkwidge2, 0, 1);
	qvtkwidge3 = new QVTKWidget(this);;
	hlayout->addWidget(qvtkwidge3, 1, 0);
	qvtkwidge4 = new QVTKWidget(this);
	hlayout->addWidget(qvtkwidge4, 1, 1);
	widgetArray.push_back(qvtkwidge1);
	widgetArray.push_back(qvtkwidge2);
	widgetArray.push_back(qvtkwidge3);
	widgetArray.push_back(qvtkwidge4);
	//--------------------渲染显示内容---------------------------
	text[0] = vtkSmartPointer<vtkVectorText>::New();
	text[0]->SetText("1");
	text[1] = vtkSmartPointer<vtkVectorText>::New();
	text[1]->SetText("2");
	text[2] = vtkSmartPointer<vtkVectorText>::New();
	text[2]->SetText("3");
	text[3] = vtkSmartPointer<vtkVectorText>::New();
	text[3]->SetText("4");
	for (int i = 0; i < 4; i++)
	{
		//初始化------------------
		elevation[i] = vtkSmartPointer<vtkElevationFilter>::New();
		mapper[i] = vtkSmartPointer<vtkPolyDataMapper>::New();
		actor[i] = vtkSmartPointer<vtkActor>::New();
		ren[i] = vtkSmartPointer<vtkRenderer>::New();
		renwindow[i] = vtkSmartPointer<vtkRenderWindow>::New();
		//渲染过程
		elevation[i]->SetInputConnection(text[i]->GetOutputPort());
		elevation[i]->SetLowPoint(0, 0, 0);
		elevation[i]->SetHighPoint(10, 0, 0);
		mapper[i]->SetInputConnection(elevation[i]->GetOutputPort());
		actor[i]->SetMapper(mapper[i]);
		ren[i]->AddActor(actor[i]);
		widgetArray[i]->SetRenderWindow(renwindow[i]);
		widgetArray[i]->GetRenderWindow()->AddRenderer(ren[i]);
	}
}

void FourWidget::initialImage(int i)
{
	readerFactory4[i-1] = vtkSmartPointer<vtkImageReader2Factory>::New();
	imageReader4[i-1] = vtkSmartPointer<vtkImageReader2>::New();
	// Create an actor
	actorImage4[i-1] = vtkSmartPointer<vtkImageActor>::New();
	// Setup renderer
	colors4[i-1] = vtkSmartPointer<vtkNamedColors>::New();
	style4[i-1] = vtkSmartPointer<vtkInteractorStyleImage>::New();
	DICOMreader4[i-1] = vtkSmartPointer<vtkDICOMImageReader>::New();
	initialCount[i-1]++;
}

void FourWidget::ShowImage(const char* filepath, int i)
{
	switch (i)
	{
	case 1:
		currentwidget = qvtkwidge1;
		break;
	case 2:
		currentwidget = qvtkwidge2;
		break;
	case 3:
		currentwidget = qvtkwidge3;
		break;
	case 4:
		currentwidget = qvtkwidge4;
		break;
	default:
		break;
	}
	imageReader4[i - 1] = readerFactory4[i - 1]->CreateImageReader2(filepath);
	imageReader4[i - 1]->SetFileName(filepath);
	imageReader4[i - 1]->Update();

	actorImage4[i - 1]->GetMapper()->SetInputConnection(imageReader4[i - 1]->GetOutputPort());

	ren[i - 1]->AddActor(actorImage4[i - 1]);
	ren[i - 1]->ResetCamera();
	ren[i - 1]->SetBackground(colors4[i - 1]->GetColor3d("Slate_grey").GetData());
	currentwidget->GetRenderWindow()->AddRenderer(ren[i - 1]);
	 
	currentwidget->GetInteractor()->SetInteractorStyle(style4[i - 1]);
	currentwidget->GetRenderWindow()->Render();
	currentwidget->GetInteractor()->Initialize();
}

void FourWidget::ShowDICOM(const char* filepath, int i)
{
	switch (i)
	{
	case 1:
		currentwidget = qvtkwidge1;
		break;
	case 2:
		currentwidget = qvtkwidge2;
		break;
	case 3:
		currentwidget = qvtkwidge3;
		break;
	case 4:
		currentwidget = qvtkwidge4;
		break;
	default:
		break;
	}
	DICOMreader4[i - 1]->SetFileName(filepath);
	DICOMreader4[i - 1]->Update();
	actorImage4[i - 1]->GetMapper()->SetInputConnection(DICOMreader4[i - 1]->GetOutputPort());

	ren[i - 1]->AddActor(actorImage4[i - 1]);
	ren[i - 1]->ResetCamera();

	currentwidget->GetRenderWindow()->AddRenderer(ren[i - 1]);

	currentwidget->GetInteractor()->SetInteractorStyle(style4[i - 1]);
	currentwidget->GetRenderWindow()->Render();
	currentwidget->GetInteractor()->Initialize();
}