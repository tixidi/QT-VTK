#include <QtWidgets/QApplication>
#include <qtextcodec.h>
#include <vtkAutoInit.h>
#include <showwidget.h>
#include <VTKMainWindow.h>
#include <Windows.h>
VTK_MODULE_INIT(vtkRenderingOpenGL)
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL)
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QFont font("AR PL KaitiM GB", 12);  //设置整个程序采用的字体与字号
	a.setFont(font);
	//ShowWidget *widget = new ShowWidget();
	//widget->setMinimumSize(QSize(812, 600));
	//widget->show();
	VTKMainWindow w;
	w.show();
	return a.exec();
}
