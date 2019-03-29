#include <TwoWidget.h>
TwoWidget::TwoWidget(QWidget *parent) : QWidget(parent)
{
	for (int i = 0; i < 2; i++)
	{
		initialCount[i] = 0;
	}
	hlayout = new QHBoxLayout(this);
	qvtkwidge1 = new QVTKWidget();;
	hlayout->addWidget(qvtkwidge1);
	qvtkwidge2 = new QVTKWidget();;
	hlayout->addWidget(qvtkwidge2);
	widgetArray.push_back(qvtkwidge1);
	widgetArray.push_back(qvtkwidge2);
	//--------------------渲染显示内容---------------------------
	text[0] = vtkSmartPointer<vtkVectorText>::New();
	text[0]->SetText("1");
	text[1] = vtkSmartPointer<vtkVectorText>::New();
	text[1]->SetText("2");
		
	for (int i = 0; i < 2; i++)
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

void TwoWidget::initialImage(int i)
{
	readerFactory2[i - 1] = vtkSmartPointer<vtkImageReader2Factory>::New();
	imageReader2[i - 1] = vtkSmartPointer<vtkImageReader2>::New();
	// Create an actor
	actorImage2[i - 1] = vtkSmartPointer<vtkImageActor>::New();
	// Setup renderer
	colors2[i - 1] = vtkSmartPointer<vtkNamedColors>::New();
	style2[i - 1] = vtkSmartPointer<vtkInteractorStyleImage>::New();
	DICOMreader2[i - 1] = vtkSmartPointer<vtkDICOMImageReader>::New();
	initialCount[i - 1]++;
}

void TwoWidget::ShowImage(const char* filepath, int i)
{
	switch (i)
	{
	case 1:
		currentwidget = qvtkwidge1;
		break;
	case 2:
		currentwidget = qvtkwidge2;
		break;
	default:
		break;
	}
	imageReader2[i - 1] = readerFactory2[i - 1]->CreateImageReader2(filepath);
	imageReader2[i - 1]->SetFileName(filepath);
	imageReader2[i - 1]->Update();

	actorImage2[i - 1]->GetMapper()->SetInputConnection(imageReader2[i - 1]->GetOutputPort());

	ren[i - 1]->AddActor(actorImage2[i - 1]);
	ren[i - 1]->ResetCamera();
	ren[i - 1]->SetBackground(colors2[i - 1]->GetColor3d("Slate_grey").GetData());
	currentwidget->GetRenderWindow()->AddRenderer(ren[i - 1]);

	currentwidget->GetInteractor()->SetInteractorStyle(style2[i - 1]);
	currentwidget->GetRenderWindow()->Render();
	currentwidget->GetInteractor()->Initialize();
}

void TwoWidget::ShowDICOM(const char* filepath, int i)
{
	switch (i)
	{
	case 1:
		currentwidget = qvtkwidge1;
		break;
	case 2:
		currentwidget = qvtkwidge2;
		break;
	default:
		break;
	}
	DICOMreader2[i - 1]->SetFileName(filepath);
	DICOMreader2[i - 1]->Update();
	actorImage2[i - 1]->GetMapper()->SetInputConnection(DICOMreader2[i - 1]->GetOutputPort());

	ren[i - 1]->AddActor(actorImage2[i - 1]);
	ren[i - 1]->ResetCamera();

	currentwidget->GetRenderWindow()->AddRenderer(ren[i - 1]);

	currentwidget->GetInteractor()->SetInteractorStyle(style2[i - 1]);
	currentwidget->GetRenderWindow()->Render();
	currentwidget->GetInteractor()->Initialize();
}