#include "PlySimply.h"

PlySimply::PlySimply(const char*sourcePath, const char* destPath, float factor)
{
	this->sourcePath = sourcePath;
	this->destPath = destPath;
	this->factor = factor;
	facesNumber = 0;
	targetFaces = 0;
	tmpBefore = "tmpBefore.smf";
	tmpAfter = "tmpAfter.smf";
}

PlySimply::~PlySimply()
{
	//清除产生的临时文件tmpBefore.smf, tmpAfter.smf
	remove(tmpBefore);
	remove(tmpAfter);
}

/*
 * 网格简化计算的总共过程
 */
void PlySimply::Compute()
{
	Ply2Smf();
	Simply();
	WritePly();
}

/*
 * 将ply文件的数据导入到临时的smf文件中
 * smf文件从1开始
 */
int PlySimply::Ply2Smf()
{
	vtkSmartPointer<vtkPLYReader> reader =
		vtkSmartPointer<vtkPLYReader>::New();
	reader->SetFileName(sourcePath);
	reader->Update();
	vtkSmartPointer<vtkPolyData> polydata = reader->GetOutput();
	FILE*fp = NULL;//需要注意
	fp = fopen(tmpBefore, "w");  //创建文件
	if (NULL == fp)
		return -1;//要返回错误代码
	facesNumber = polydata->GetNumberOfCells();
	fprintf(fp, "#$SMF 1.0\n");
	fprintf(fp, "#$vertices %d\n#$faces %d\n", polydata->GetNumberOfPoints(), polydata->GetNumberOfCells());
	for (vtkIdType i = 0; i < polydata->GetNumberOfPoints(); i++)
	{
		double p[3];
		polydata->GetPoint(i, p);
		fprintf(fp, "v %.6f %.6f %.6f\n", p[0], p[1], p[2]);
	}
	for (vtkIdType i = 0; i < polydata->GetNumberOfCells(); i++)
	{
		vtkSmartPointer<vtkCell> cell = polydata->GetCell(i);
		int Id1 = cell->GetPointId(0) + 1;
		int Id2 = cell->GetPointId(1) + 1;
		int Id3 = cell->GetPointId(2) + 1;
		fprintf(fp, "f %d %d %d\n", Id1, Id2, Id3);
	}
	std::cout << "Ply2smfdone!" << std::endl;
	fclose(fp);
	fp = NULL;//需要指向空，否则会指向原打开文件地址
	return 0;
}

/*
 * 网格简化, 执行qslim.exe程序
 */
void PlySimply::Simply()
{
	targetFaces = ceil(facesNumber * factor);
	std::string cmd = "qslim.exe -o tmpAfter.smf -t  " + std::to_string(targetFaces) + " tmpBefore.smf";
	system(cmd.c_str());
	std::cout << "Simplydone!" << std::endl;
}

/*
 * 网格简化前的smf  #$SMF 1.0 开头
 */
vtkPolyData *PlySimply::Smf_Reader1()
{
	std::ifstream infile(tmpBefore);
	char SmfVersion[20];
	float version;
	infile >> SmfVersion >> version;
	char verticesName[20]; int verticesNumber;
	infile >> verticesName >> verticesNumber;
	char facesName[20]; int facesNumber;
	infile >> facesName >> facesNumber;

	std::cout << SmfVersion << "  " << version << std::endl;
	std::cout << verticesName << "  " << verticesNumber << std::endl;
	std::cout << facesName << "  " << facesNumber << std::endl;

	vtkIdType number_of_points, number_of_triangles;
	number_of_points = verticesNumber;
	number_of_triangles = facesNumber;
	verticesNumber = number_of_points;   //保存顶点的数量
	facesNumber = number_of_triangles;   //保存三角形的数量

	vtkSmartPointer<vtkPoints> points
		= vtkSmartPointer<vtkPoints>::New();
	points->SetNumberOfPoints(number_of_points);
	for (vtkIdType i = 0; i < number_of_points; i++)
	{
		char v[4];
		infile >> v; //pass first v
		double x, y, z;
		infile >> x >> y >> z;
		points->SetPoint(i, x, y, z);
	}

	vtkSmartPointer<vtkCellArray> polys
		= vtkSmartPointer<vtkCellArray>::New();
	for (vtkIdType i = 0; i < number_of_triangles; i++)
	{
		char f[4];
		infile >> f;  //pass f
		vtkIdType a, b, c;
		infile >> a >> b >> c;
		polys->InsertNextCell(3);
		/*bumpy*/
		/*polys->InsertCellPoint(a);
		polys->InsertCellPoint(b);
		polys->InsertCellPoint(c);*/

		/*Armadillo*/
		polys->InsertCellPoint(a - 1);
		polys->InsertCellPoint(b - 1);
		polys->InsertCellPoint(c - 1);
	}
	vtkPolyData * polydata = vtkPolyData::New();
	polydata->SetPoints(points);
	polydata->SetPolys(polys);

	infile.close();
	return polydata;
}

/*
 * 网格简化后的smf  begin开头 end结尾
 */
vtkPolyData* PlySimply::Smf_Reader2()
{
	std::ifstream infile(tmpAfter);
	char begin[10];
	infile >> begin;    //跳过开头

	vtkIdType IDi = 0;
	vtkIdType IDj = 0;

	vtkSmartPointer<vtkPoints> points
		= vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> polys
		= vtkSmartPointer<vtkCellArray>::New();
	bool flag = true;
	while (flag)
	{
		char first[5];   //记录每行的开头，判断是不是end
		infile >> first;
		if (first[0] == 'v') //顶点数据
		{
			double x, y, z;
			infile >> x >> y >> z;
			points->InsertPoint(IDi, x, y, z);
			IDi++;
		}
		else if (first[0] == 'f')  //三角面的数据
		{
			IDj++;
			vtkIdType a, b, c;
			infile >> a >> b >> c;
			polys->InsertNextCell(3);
			polys->InsertCellPoint(a - 1);      //smf的顶点编号从1开始
			polys->InsertCellPoint(b - 1);
			polys->InsertCellPoint(c - 1);
		}
		else  //end结尾
		{
			flag = false;   //跳出循环
		}
	}

	vtkPolyData * polydata = vtkPolyData::New();
	polydata->SetPoints(points);
	polydata->SetPolys(polys);

	infile.close();
	verticesNumber = IDi;    //保存顶点的数量
	facesNumber = IDj;       //保存三角形的数量
	return polydata;
}

/*
 * 输出PLY模型
 */
void PlySimply::WritePly()
{
	vtkSmartPointer<vtkPolyData> polydata
		= vtkSmartPointer<vtkPolyData>::Take(Smf_Reader2());
	// Write ply file
	vtkSmartPointer<vtkPLYWriter> plyWriter =
		vtkSmartPointer<vtkPLYWriter>::New();
	plyWriter->SetFileName(destPath);
	plyWriter->SetInputData(polydata);
	plyWriter->Write();
}

PLY2BP::PLY2BP(const char* plyPath, const char* bpPath)
{
	this->plyPath = plyPath;
	this->bpPath = bpPath;
}

void PLY2BP::Write()
{
	vtkSmartPointer<vtkPLYReader> reader =
		vtkSmartPointer<vtkPLYReader>::New();
	reader->SetFileName(plyPath);
	reader->Update();
	vtkSmartPointer<vtkPolyData> polydata = reader->GetOutput();
	FILE*fp = NULL;//需要注意
	fp = fopen(bpPath, "w");  //创建文件
	if (NULL == fp)
		return ;//要返回错误代码
	for (vtkIdType i = 0; i < polydata->GetNumberOfPoints(); i++)
	{
		double p[3];
		polydata->GetPoint(i, p);
		fprintf(fp, "%.6f %.6f %.6f\n", p[0], p[1], p[2]);
	}
	fclose(fp);
	fp = NULL;//需要指向空，否则会指向原打开文件地址
	return;
}