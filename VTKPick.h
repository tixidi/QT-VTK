#pragma once

#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkExtractSelection.h>
#include <vtkSphereSource.h>
#include <vtkRendererCollection.h>
#include <vtkCellArray.h>
#include <vtkCellPicker.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkObjectFactory.h>
#include <vtkPlanes.h>
#include <vtkPointData.h>
#include <vtkPlaneSource.h>
#include <vtkIdTypeArray.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <vtkPropPicker.h>
#include <vtkInteractorStyleRubberBandPick.h>
#include <vtkAreaPicker.h>
#include <vtkExtractGeometry.h>
#include <vtkUnstructuredGrid.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkExtractPolyDataGeometry.h>
#include <vtkIdFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkPropPicker.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkExtractSelection.h>
#include <vtkObjectFactory.h>
#include <vtkRenderWindow.h>
#include <vtkDataSetMapper.h>
#include <vtkTriangleFilter.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <qtextedit.h>

#define VTKISRBP_ORIENT 0
#define VTKISRBP_SELECT 1

//点的拾取，并打印
// Handle mouse events
class SinglePointPickingStyle : public vtkInteractorStyleTrackballCamera
{
public:
	static SinglePointPickingStyle* New();
	vtkTypeMacro(SinglePointPickingStyle, vtkInteractorStyleTrackballCamera);

	virtual void OnLeftButtonDown()
	{
		int* clickPos = this->GetInteractor()->GetEventPosition();

		// Pick from this location.
		vtkSmartPointer<vtkPropPicker>  picker =
			vtkSmartPointer<vtkPropPicker>::New();
		picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());

		double* pos = picker->GetPickPosition();
		QString text;
		//text += "Pick position (world coordinates) is: ";
		text += QString::number(pos[0]);
		text += "    ";
	    text += QString::number(pos[1]);
		text += "    ";
		text += QString::number(pos[2]);
		text += "\n";
		
		textEdit->append(text);
		//std::cout << "Picked actor: " << picker->GetActor() << std::endl;
		//Create a sphere
		vtkSmartPointer<vtkSphereSource> sphereSource =
			vtkSmartPointer<vtkSphereSource>::New();
		sphereSource->SetCenter(pos[0], pos[1], pos[2]);
		sphereSource->SetRadius(radius);

		//Create a mapper and actor
		vtkSmartPointer<vtkPolyDataMapper> mapper =
			vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputConnection(sphereSource->GetOutputPort());

		vtkSmartPointer<vtkActor> actor =
			vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);
		//actor->GetProperty()->SetColor(255.0, 0.0, 0.0);
		actor->GetProperty()->SetColor(RGB[0], RGB[1], RGB[2]);

		//this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetDefaultRenderer()->AddActor(actor);
		this->GetDefaultRenderer()->AddActor(actor);
		// Forward events
		vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
	}

	void setRadius(double radius)
	{
		this->radius = radius;
	};

	void setRGB(double *rgb)
	{
		this->RGB[0] = rgb[0] / 255;
		this->RGB[1] = rgb[1] / 255;
		this->RGB[2] = rgb[2] / 255;
	};

	void setTextEdit(QTextEdit *textEdit)
	{
		this->textEdit = textEdit;
	};

private:
	double radius;
	double RGB[3];
	QTextEdit *textEdit;
};

vtkStandardNewMacro(SinglePointPickingStyle);

//多个点的拾取
// Define interaction style
class MutiplePointPickingStyle : public vtkInteractorStyleRubberBandPick
{
public:
	static MutiplePointPickingStyle* New();
	vtkTypeMacro(MutiplePointPickingStyle, vtkInteractorStyleRubberBandPick);

	MutiplePointPickingStyle()
	{
		this->SelectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
		this->SelectedActor = vtkSmartPointer<vtkActor>::New();
		this->SelectedActor->SetMapper(SelectedMapper);
	}

	virtual void OnLeftButtonUp()
	{
		// Forward events
		vtkInteractorStyleRubberBandPick::OnLeftButtonUp();

		vtkPlanes* frustum = static_cast<vtkAreaPicker*>(this->GetInteractor()->GetPicker())->GetFrustum();

		vtkSmartPointer<vtkExtractGeometry> extractGeometry =
			vtkSmartPointer<vtkExtractGeometry>::New();
		extractGeometry->SetImplicitFunction(frustum);
#if VTK_MAJOR_VERSION <= 5
		extractGeometry->SetInput(this->Points);
#else
		extractGeometry->SetInputData(this->Points);
#endif
		extractGeometry->Update();

		vtkSmartPointer<vtkVertexGlyphFilter> glyphFilter =
			vtkSmartPointer<vtkVertexGlyphFilter>::New();
		glyphFilter->SetInputConnection(extractGeometry->GetOutputPort());
		glyphFilter->Update();

		vtkPolyData* selected = glyphFilter->GetOutput();
		std::cout << "Selected " << selected->GetNumberOfPoints() << " points." << std::endl;
		std::cout << "Selected " << selected->GetNumberOfCells() << " cells." << std::endl;
#if VTK_MAJOR_VERSION <= 5
		this->SelectedMapper->SetInput(selected);
#else
		this->SelectedMapper->SetInputData(selected);
#endif
		this->SelectedMapper->ScalarVisibilityOff();

		vtkIdTypeArray* ids = vtkIdTypeArray::SafeDownCast(selected->GetPointData()->GetArray("OriginalIds"));
		for (vtkIdType i = 0; i < ids->GetNumberOfTuples(); i++)
		{
			std::cout << "Id " << i << " : " << ids->GetValue(i) << std::endl;
		}

		this->SelectedActor->GetProperty()->SetColor(255.0, 0.0, 0.0); //(R,G,B)
		this->SelectedActor->GetProperty()->SetPointSize(3);

		this->CurrentRenderer->AddActor(SelectedActor);
		this->GetInteractor()->GetRenderWindow()->Render();
		this->HighlightProp(NULL);
	}

	void SetPoints(vtkSmartPointer<vtkPolyData> points) { this->Points = points; }
private:
	vtkSmartPointer<vtkPolyData> Points;
	vtkSmartPointer<vtkActor> SelectedActor;
	vtkSmartPointer<vtkDataSetMapper> SelectedMapper;

};

vtkStandardNewMacro(MutiplePointPickingStyle);

//区域拾取
// Define interaction style
class AreaPickingStyle : public vtkInteractorStyleRubberBandPick
{
public:
	static AreaPickingStyle* New();
	vtkTypeMacro(AreaPickingStyle, vtkInteractorStyleRubberBandPick);

	AreaPickingStyle() : vtkInteractorStyleRubberBandPick()
	{
		this->SelectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
		this->SelectedActor = vtkSmartPointer<vtkActor>::New();
		this->SelectedActor->SetMapper(SelectedMapper);
	}

	virtual void OnLeftButtonUp()
	{
		// Forward events
		vtkInteractorStyleRubberBandPick::OnLeftButtonUp();

		if (this->CurrentMode == VTKISRBP_SELECT)
		{
			vtkPlanes* frustum = static_cast<vtkAreaPicker*>(this->GetInteractor()->GetPicker())->GetFrustum();

			vtkSmartPointer<vtkExtractPolyDataGeometry> extractPolyDataGeometry =
				vtkSmartPointer<vtkExtractPolyDataGeometry>::New();
#if VTK_MAJOR_VERSION <= 5
			extractPolyDataGeometry->SetInput(this->PolyData);
#else
			extractPolyDataGeometry->SetInputData(this->PolyData);
			//extractPolyDataGeometry->SetInputData(actor);
#endif
			extractPolyDataGeometry->SetImplicitFunction(frustum);
			extractPolyDataGeometry->Update();

			QString text;
			text += "Extracted ";
			text += "    ";
			text += QString::number(extractPolyDataGeometry->GetOutput()->GetNumberOfCells());
			text += " cells.";
			text += "\n";

			textEdit->append(text); 
			

#if VTK_MAJOR_VERSION <= 5
			this->SelectedMapper->SetInputConnection(
				extractPolyDataGeometry->GetOutputPort());
#else
			this->SelectedMapper->SetInputData(extractPolyDataGeometry->GetOutput());
#endif
			this->SelectedMapper->ScalarVisibilityOff();

			//        vtkIdTypeArray* ids = vtkIdTypeArray::SafeDownCast(selected->GetPointData()->GetArray("OriginalIds"));

			this->SelectedActor->GetProperty()->SetColor(RGB[0], RGB[1], RGB[2]); //(R,G,B)
			this->SelectedActor->GetProperty()->SetPointSize(radius);

			this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(SelectedActor);
			this->GetInteractor()->GetRenderWindow()->Render();
			this->HighlightProp(NULL);
		}
	}

	void SetPolyData(vtkSmartPointer<vtkPolyData> polyData) 
	{ 
		this->PolyData = polyData; 
	};

	void setRGB(double *rgb)
	{
		this->RGB[0] = rgb[0] / 255;
		this->RGB[1] = rgb[1] / 255;
		this->RGB[2] = rgb[2] / 255;
	};

	void setTextEdit(QTextEdit *textEdit)
	{
		this->textEdit = textEdit;
	};

	void setRadius(double radius)
	{
		this->radius = radius;
	};

private:
	vtkSmartPointer<vtkPolyData> PolyData;
	vtkSmartPointer<vtkActor> SelectedActor;
	vtkSmartPointer<vtkDataSetMapper> SelectedMapper;
	QTextEdit *textEdit;
	double radius;
	double RGB[3];
};

vtkStandardNewMacro(AreaPickingStyle);

//角色选取
// Handle mouse events
class ActorPickingStyle : public vtkInteractorStyleTrackballCamera
{
public:
	static ActorPickingStyle* New();
	vtkTypeMacro(ActorPickingStyle, vtkInteractorStyleTrackballCamera);

	ActorPickingStyle()
	{
		LastPickedActor = NULL;
		LastPickedProperty = vtkProperty::New();
	}
	virtual ~ActorPickingStyle()
	{
		LastPickedProperty->Delete();
	}
	virtual void OnLeftButtonDown()
	{
		int* clickPos = this->GetInteractor()->GetEventPosition();

		// Pick from this location.
		vtkSmartPointer<vtkPropPicker>  picker =
			vtkSmartPointer<vtkPropPicker>::New();
		picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());

		// If we picked something before, reset its property
		if (this->LastPickedActor)
		{
			this->LastPickedActor->GetProperty()->DeepCopy(this->LastPickedProperty);
		}
		this->LastPickedActor = picker->GetActor();
		if (this->LastPickedActor)
		{
			// Save the property of the picked actor so that we can
			// restore it next time
			this->LastPickedProperty->DeepCopy(this->LastPickedActor->GetProperty());
			// Highlight the picked actor by changing its properties
			this->LastPickedActor->GetProperty()->SetColor(RGB[0], RGB[1], RGB[2]);
			this->LastPickedActor->GetProperty()->SetDiffuse(1.0);
			this->LastPickedActor->GetProperty()->SetSpecular(0.0);
		}

		// Forward events
		vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
	}

	void setRGB(double *rgb)
	{
		this->RGB[0] = rgb[0] / 255;
		this->RGB[1] = rgb[1] / 255;
		this->RGB[2] = rgb[2] / 255;
	};

private:
	vtkActor * LastPickedActor;
	vtkProperty *LastPickedProperty;
	double RGB[3];
};

vtkStandardNewMacro(ActorPickingStyle);

//单元拾取
//Catch mouse events
class CellPickingStyle : public vtkInteractorStyleTrackballCamera
{
public:
	static CellPickingStyle* New();

	CellPickingStyle()
	{
		selectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
		selectedActor = vtkSmartPointer<vtkActor>::New();
	}

	virtual void OnLeftButtonDown()
	{
		// Get the location of the click (in window coordinates)
		int* pos = this->GetInteractor()->GetEventPosition();

		vtkSmartPointer<vtkCellPicker> picker =
			vtkSmartPointer<vtkCellPicker>::New();
		picker->SetTolerance(0.0005);

		// Pick from this location.
		picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());

		double* worldPosition = picker->GetPickPosition();
		std::cout << "Cell id is: " << picker->GetCellId() << std::endl;

		if (picker->GetCellId() != -1)
		{

			std::cout << "Pick position is: " << worldPosition[0] << " " << worldPosition[1]
				<< " " << worldPosition[2] << endl;

			vtkSmartPointer<vtkIdTypeArray> ids =
				vtkSmartPointer<vtkIdTypeArray>::New();
			ids->SetNumberOfComponents(1);
			ids->InsertNextValue(picker->GetCellId());

			vtkSmartPointer<vtkSelectionNode> selectionNode =
				vtkSmartPointer<vtkSelectionNode>::New();
			selectionNode->SetFieldType(vtkSelectionNode::CELL);
			selectionNode->SetContentType(vtkSelectionNode::INDICES);
			selectionNode->SetSelectionList(ids);

			vtkSmartPointer<vtkSelection> selection =
				vtkSmartPointer<vtkSelection>::New();
			selection->AddNode(selectionNode);

			vtkSmartPointer<vtkExtractSelection> extractSelection =
				vtkSmartPointer<vtkExtractSelection>::New();
#if VTK_MAJOR_VERSION <= 5
			extractSelection->SetInput(0, this->Data);
			extractSelection->SetInput(1, selection);
#else
			extractSelection->SetInputData(0, this->Data);
			extractSelection->SetInputData(1, selection);
#endif
			extractSelection->Update();

			// In selection
			vtkSmartPointer<vtkUnstructuredGrid> selected =
				vtkSmartPointer<vtkUnstructuredGrid>::New();
			selected->ShallowCopy(extractSelection->GetOutput());

			std::cout << "There are " << selected->GetNumberOfPoints()
				<< " points in the selection." << std::endl;
			std::cout << "There are " << selected->GetNumberOfCells()
				<< " cells in the selection." << std::endl;


#if VTK_MAJOR_VERSION <= 5
			selectedMapper->SetInputConnection(
				selected->GetProducerPort());
#else
			selectedMapper->SetInputData(selected);
#endif

			selectedActor->SetMapper(selectedMapper);
			selectedActor->GetProperty()->EdgeVisibilityOn();
			selectedActor->GetProperty()->SetEdgeColor(1, 0, 0);
			selectedActor->GetProperty()->SetLineWidth(3);

			this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(selectedActor);

		}
		// Forward events
		vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
	}

	vtkSmartPointer<vtkPolyData> Data;
	vtkSmartPointer<vtkDataSetMapper> selectedMapper;
	vtkSmartPointer<vtkActor> selectedActor;

};

vtkStandardNewMacro(CellPickingStyle);
