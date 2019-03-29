#pragma once
#include <vtkPolyData.h>
#include <vtkOBJReader.h>
#include <vtkSTLReader.h>
#include <vtkSmartPointer.h>
#include <vtkPLYWriter.h>
void STL2PLY(const char* stlPath, const char* plyPath);
void OBJ2PLY(const char* objPath, const char* plyPath);