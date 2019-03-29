#pragma once
#include <vtkSmartPointer.h>
#include <vtkPLYWriter.h>
#include <vtkPolyData.h>
#include <vtkPLYReader.h>
#include <cstdio>
#include <iostream>

class PlySimply
{
public:
	PlySimply(const char*sourcePath, const char* destPath, float factor);
	void Compute();
	~PlySimply();
private:
	const char* sourcePath;      //简化前的ply文件地址
	const char* destPath;        //简化后的ply文件地址
	const char* tmpBefore;       //临时的简化前的smf地址
	const char* tmpAfter;        //临时的简化后的smf地址
	float factor;                //简化的系数
	int verticesNumber;          //简化前的顶点的数目
	int facesNumber;             //简化前的网格面的数目
	int targetFaces;             //简化后的网格面的数目
	void Simply();               //smf文件的简化
	int Ply2Smf();              //PLY文件转化为smf文件
	vtkPolyData *Smf_Reader1();  //网格简化前的smf的读取  #$SMF 1.0 开头
	vtkPolyData *Smf_Reader2();  //网格简化后的smf的读取  begin开头 end结尾
	void WritePly();
};

class PLY2BP
{
public:
	PLY2BP(const char* plyPath, const char* bpPath);
	void Write();
private:
	const char* plyPath;
	const char* bpPath;
};