#pragma once
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/io/io.h>
#include <pcl/io/vtk_lib_io.h>
#include <pcl/io/obj_io.h>
#include <pcl/PolygonMesh.h>
#include <pcl/point_cloud.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <QVTKWidget.h>
class PCLPoint {
public:
	PCLPoint(QVTKWidget *widget = 0 );
	void ShowPointCloud(const char *fileName);
	void obj2pcd(const char* obj, const char* pcd);
private:
	QVTKWidget *qvtkwidget;
	//µãÔÆÊý¾Ý´æ´¢
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud;
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer;
	void initialVtkWidget();
};