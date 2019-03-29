#pragma once
class VolumeData
{
public:
	VolumeData(int start, int end ,int Pn, int Sn, int Gn, int Cn)
	{
		this->start = start;
		this->end = end;
		this->Pn = Pn;
		this->Sn = Sn;
		this->Gn = Gn;
		this->Cn = Cn;
		Property = new double[Pn][4];
		Scalar = new  double[Sn][4];
		Gradient = new double[Gn][4];
		Color = new double[Cn][6];
	}
public:
	int start;
	int end;
	int Pn;
	int Sn;
	int Gn;
	int Cn;
	double (*Property)[4];
	double (*Scalar)[4];
	double (*Gradient)[4];
	double (*Color)[6];
};