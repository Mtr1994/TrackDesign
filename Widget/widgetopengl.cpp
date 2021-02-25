#include "widgetopengl.h"
#include "ui_widgetopengl.h"
#include "Public/designtrackstyle.h"

#include <vtkNamedColors.h>
#include <vtkGenericRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkOpenGLPolyDataMapper.h>
#include <vtkLODActor.h>
#include <vtkDiscretizableColorTransferFunction.h>
#include <vtkPolyData.h>
#include <vtkPointPicker.h>
#include <vtkCamera.h>

WidgetOpenGL::WidgetOpenGL(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetOpenGL)
{
    ui->setupUi(this);

    // 初始化窗口
    init();
}

WidgetOpenGL::~WidgetOpenGL()
{
    delete ui;
}

void WidgetOpenGL::init()
{
    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();

    mRender = vtkSmartPointer<vtkOpenGLRenderer>::New();
    mRender->SetBackground(colors->GetColor3d("SlateGray").GetData());

    mRenderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    mRenderWindow->AddRenderer(mRender);

    vtkSmartPointer<vtkGenericRenderWindowInteractor> interactor = vtkSmartPointer<vtkGenericRenderWindowInteractor>::New();
    vtkSmartPointer<vtkPointPicker> picker = vtkSmartPointer<vtkPointPicker>::New();
    picker->SetTolerance(0.0000001);
    interactor->SetPicker(picker);
    vtkSmartPointer<DesignTrackStyle> style = vtkSmartPointer<DesignTrackStyle>::New();

    interactor->SetInteractorStyle(style);
    interactor->SetRenderWindow(mRenderWindow);
    interactor->Start();

    mRender->GetActiveCamera()->ParallelProjectionOn();

    ui->widget->setRenderWindow(mRenderWindow);
}

void WidgetOpenGL::slot_update_data(vtkSmartPointer<vtkPolyData> data)
{
    vtkSmartPointer<vtkOpenGLPolyDataMapper> mapper = vtkSmartPointer<vtkOpenGLPolyDataMapper>::New();
    mapper->SetInputData(data);

    vtkSmartPointer<vtkLODActor> actor = vtkSmartPointer<vtkLODActor>::New();
    actor->SetMapper(mapper);

    vtkSmartPointer<vtkDiscretizableColorTransferFunction> color = vtkSmartPointer<vtkDiscretizableColorTransferFunction>::New();
    color->SetColorSpaceToDiverging();
    double range[2] = {0};
    data->GetScalarRange(range);

    color->AddRGBPoint(range[0],        0.231373, 0.298039, 0.752941);
    color->AddRGBPoint(range[1] * 0.5,  0.866667, 0.866667, 0.866667);
    color->AddRGBPoint(range[1],        0.705882, 0.015686, 0.14902);

    mapper->SetLookupTable(color);
    mRender->AddActor(actor);

    mRender->ResetCamera();
    ui->widget->renderWindow()->Render();
}

void WidgetOpenGL::on_btnExport_clicked()
{
    vtkSmartPointer<DesignTrackStyle> style = dynamic_cast<DesignTrackStyle*>(mRenderWindow->GetInteractor()->GetInteractorStyle());
    if (nullptr == style) return;

    std::string track = style->getTrackData();
    emit sgl_export_data(QString::fromStdString(track));
}

void WidgetOpenGL::on_btnReload_clicked()
{
    vtkSmartPointer<DesignTrackStyle> style = dynamic_cast<DesignTrackStyle*>(mRenderWindow->GetInteractor()->GetInteractorStyle());
    if (nullptr != style) style->clearPoints();

    mRender->RemoveAllViewProps();
    ui->widget->renderWindow()->Render();

    emit sgl_reload_map();
}
