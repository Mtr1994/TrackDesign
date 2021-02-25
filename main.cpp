#include "mainwindow.h"

#include <QApplication>
#include <vtkPolyData.h>
#include <vtkOutputWindow.h>

int main(int argc, char *argv[])
{
    // 设置环境变量
    qputenv("GDAL_DATA", "D:/GDAL/GDAL232/data");
    QApplication a(argc, argv);

    qRegisterMetaType<vtkSmartPointer<vtkPolyData>>("vtkSmartPointer<vtkPolyData>");


    // 禁止弹出警告窗口
    vtkOutputWindow::SetGlobalWarningDisplay(0);

    MainWindow w;
    w.show();
    return a.exec();
}
