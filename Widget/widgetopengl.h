#ifndef WIDGETOPENGL_H
#define WIDGETOPENGL_H

#include <QWidget>
#include <vtkSmartPointer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkOpenGLRenderer.h>

namespace Ui {
class WidgetOpenGL;
}

class vtkPolyData;
class WidgetOpenGL : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetOpenGL(QWidget *parent = nullptr);
    ~WidgetOpenGL();

    void init();

signals:
    void sgl_reload_map();
    void sgl_export_data(const QString& data);

public slots:
    void slot_update_data(vtkSmartPointer<vtkPolyData> data);

private slots:
    void on_btnExport_clicked();

    void on_btnReload_clicked();

private:
    Ui::WidgetOpenGL *ui;

    // 舞台只要一个
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> mRenderWindow;
    vtkSmartPointer<vtkOpenGLRenderer> mRender;
};

#endif // WIDGETOPENGL_H
