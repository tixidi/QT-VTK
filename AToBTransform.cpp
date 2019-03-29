#include "AToBTransform.h"

void STL2PLY(const char* stlPath, const char* plyPath)
{
	// Read STL file
	vtkSmartPointer<vtkSTLReader> reader =
		vtkSmartPointer<vtkSTLReader>::New();
	reader->SetFileName(stlPath);
	reader->Update();

	// Write ply file
	vtkSmartPointer<vtkPLYWriter> plyWriter =
		vtkSmartPointer<vtkPLYWriter>::New();
	plyWriter->SetFileName(plyPath);
	plyWriter->SetInputConnection(reader->GetOutputPort());
	plyWriter->Write();
}

void OBJ2PLY(const char* objPath, const char* plyPath)
{
	// Read STL file
	vtkSmartPointer<vtkOBJReader> reader =
		vtkSmartPointer<vtkOBJReader>::New();
	reader->SetFileName(objPath);
	reader->Update();

	// Write ply file
	vtkSmartPointer<vtkPLYWriter> plyWriter =
		vtkSmartPointer<vtkPLYWriter>::New();
	plyWriter->SetFileName(plyPath);
	plyWriter->SetInputConnection(reader->GetOutputPort());
	plyWriter->Write();
}