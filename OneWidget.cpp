#include <OneWidget.h>
#include <vtkNew.h>
#include <vtkQtTableView.h>
#include <vtkSphereSource.h>


OneWidget::OneWidget(QWidget *parent) : QWidget(parent)
{
	initialCount = 0;
	vlayout1 = new QVBoxLayout(this);
	qvtkwidget1 = new QVTKWidget(this);
	qvtkwidget1->setMinimumSize(QSize(512, 512));
	vlayout1->addWidget(qvtkwidget1);
	//--------------------‰÷»æœ‘ æƒ⁄»›---------------------------
	text1 = vtkSmartPointer<vtkVectorText>::New();
	//vtkNew<vtkVectorText> text;
	//text->SetText("VTK and QT!");
	text1->SetText("WelCome!");
	elevation1 = vtkSmartPointer<vtkElevationFilter>::New();
	elevation1->SetInputConnection(text1->GetOutputPort());
	elevation1->SetLowPoint(0, 0, 0);
	elevation1->SetHighPoint(10, 0, 0);
	//Read the image
	mapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper1->SetInputConnection(elevation1->GetOutputPort());
	actor1 = vtkSmartPointer<vtkActor>::New();
	actor1->SetMapper(mapper1);
	ren1 = vtkSmartPointer<vtkRenderer>::New();
	ren1->AddActor(actor1);
	renwindow1 = vtkSmartPointer<vtkRenderWindow>::New();
	//renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();                  //1
	//renwindow1->SetInteractor(renderWindowInteractor);
	//renderWindowInteractor->SetRenderWindow(renwindow1);                                       //2
	//renderWindowInteractor->Initialize();
	//qvtkInteractor = vtkSmartPointer<QVTKInteractor>::New();
	//renwindow1->SetInteractor(qvtkInteractor);
	qvtkwidget1->SetRenderWindow(renwindow1);
	//qvtkwidget1->GetRenderWindow()->SetInteractor(renderWindowInteractor);
	qvtkwidget1->GetRenderWindow()->AddRenderer(ren1);
	qvtkwidget1->GetRenderWindow()->Render();
	qvtkwidget1->GetRenderWindow()->GetInteractor()->Initialize();
	//renwindow1->AddRenderer(ren1);
	//renwindow1->Render();
	//renderWindowInteractor->Initialize();
	//renderWindowInteractor->Start();

	//≤‚ ‘ƒ⁄»›---------------
}

void OneWidget::initialImage()
{
    readerFactory1 = vtkSmartPointer<vtkImageReader2Factory>::New();
	imageReader1 = vtkSmartPointer<vtkImageReader2>::New();
	// Create an actor
	actorImage1 = vtkSmartPointer<vtkImageActor>::New();
	// Setup renderer
	colors1 = vtkSmartPointer<vtkNamedColors>::New();
	style1 = vtkSmartPointer<vtkInteractorStyleImage>::New();
	DICOMreader1 = vtkSmartPointer<vtkDICOMImageReader>::New();
	initialCount++;
}

void OneWidget::ShowImage(const char* filepath)
{
	imageReader1 = readerFactory1->CreateImageReader2(filepath);
	imageReader1->SetFileName(filepath);
	imageReader1->Update();

	actorImage1->GetMapper()->SetInputConnection(imageReader1->GetOutputPort());

	ren1->AddActor(actorImage1);
	ren1->ResetCamera();
	ren1->SetBackground(colors1->GetColor3d("Slate_grey").GetData());
	qvtkwidget1->GetRenderWindow()->AddRenderer(ren1);

	qvtkwidget1->GetInteractor()->SetInteractorStyle(style1);
	qvtkwidget1->GetRenderWindow()->Render();
	qvtkwidget1->GetInteractor()->Initialize();
}

void OneWidget::ShowDICOM(const char* filepath)
{
	DICOMreader1->SetFileName(filepath);
	DICOMreader1->Update();
	actorImage1->GetMapper()->SetInputConnection(DICOMreader1->GetOutputPort());

	ren1->AddActor(actorImage1);
	ren1->ResetCamera();
	//ren1->SetBackground(colors1->GetColor3d("Slate_grey").GetData());
	qvtkwidget1->GetRenderWindow()->AddRenderer(ren1);

	qvtkwidget1->GetInteractor()->SetInteractorStyle(style1);
	qvtkwidget1->GetRenderWindow()->Render();
	qvtkwidget1->GetInteractor()->Initialize();
}

vtkRenderWindowInteractor *OneWidget::getInteractor()
{
	return this->renderWindowInteractor;
}

vtkRenderer *OneWidget::getRender()
{
	return this->ren1;
}

QVTKInteractor *OneWidget::getQVTKInteractor()
{
	return this->qvtkInteractor;
}