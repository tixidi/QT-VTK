#include <PCLPoint.h>
#include <vtkRenderWindow.h>
PCLPoint::PCLPoint(QVTKWidget *widget)
{
	qvtkwidget = widget;
}

void PCLPoint::initialVtkWidget()
{
	cloud.reset(new pcl::PointCloud<pcl::PointXYZ>);
	viewer.reset(new pcl::visualization::PCLVisualizer("viewer", false));
	viewer->addPointCloud(cloud, "cloud");

	qvtkwidget->SetRenderWindow(viewer->getRenderWindow());
	viewer->setupInteractor(qvtkwidget->GetInteractor(), qvtkwidget->GetRenderWindow());
	qvtkwidget->update();
}

//读取文本型和二进制型点云数据
void PCLPoint::ShowPointCloud(const char *fileName)
{
	initialVtkWidget();
	//只能打开PCD文件
	
	if (fileName != NULL)
	{
		//sensor_msgs::PointCloud2 cloud2;
		pcl::PCLPointCloud2 cloud2;
		//pcl::PointCloud<Eigen::MatrixXf> cloud2;
		Eigen::Vector4f origin;
		Eigen::Quaternionf orientation;
		int pcd_version;
		int data_type;
		unsigned int data_idx;
		int offset = 0;
		pcl::PCDReader rd;
		rd.readHeader(fileName, cloud2, origin, orientation, pcd_version, data_type, data_idx);

		if (data_type == 0)
		{
			pcl::io::loadPCDFile(fileName, *cloud);
		}
		else if (data_type == 2)
		{
			pcl::PCDReader reader;
			reader.read<pcl::PointXYZ>(fileName, *cloud);
		}

		viewer->updatePointCloud(cloud, "cloud");
		viewer->resetCamera();
		qvtkwidget->update();
	}
}

void PCLPoint::obj2pcd(const char* obj, const char* pcd)
{
	pcl::PolygonMesh mesh;
	pcl::io::loadPolygonFileOBJ(obj, mesh);
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::fromPCLPointCloud2(mesh.cloud, *cloud);
	pcl::io::savePCDFileASCII(pcd, *cloud);
}