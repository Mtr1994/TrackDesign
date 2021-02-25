#include "designtrackstyle.h"
#include "Utils/coordinatetransform.h"

#include <vtkObjectFactory.h>
#include <vtkPointPicker.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkOpenGLPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkLineSource.h>

#include <QDebug>

vtkStandardNewMacro(DesignTrackStyle);

DesignTrackStyle::DesignTrackStyle() : vtkInteractorStyleImage()
{
     mMapperTrack = vtkSmartPointer<vtkOpenGLPolyDataMapper>::New();
     mActorTrack =vtkSmartPointer<vtkActor>::New();
     mActorTrack->SetMapper(mMapperTrack);

     mMapperPoints = vtkSmartPointer<vtkOpenGLPolyDataMapper>::New();
     mActorPoints =vtkSmartPointer<vtkActor>::New();
     mActorPoints->SetMapper(mMapperPoints);
}

void DesignTrackStyle::clearPoints()
{
    mMapperPoints->SetInputData(nullptr);
    mMapperTrack->SetInputData(nullptr);
}

std::string DesignTrackStyle::getTrackData()
{
    CoordinateTransform ctf(CoordinateTransform::Coordinate_Wgs84, 0, 0);
    std::string track;

    vtkSmartPointer<vtkPolyData> data = mMapperPoints->GetInput();
    if (nullptr == data) return "";
    int number = data->GetNumberOfPoints();
    for (int i = 0; i < number; i++)
    {
        double* pos = data->GetPoint(i);
        if (!ctf.transform(pos[0], pos[1])) continue;
        track.append(std::to_string(pos[0]));
        track.append("    ");
        track.append(std::to_string(pos[1]));
        track.append("\n");
    }

    return track;
}

void DesignTrackStyle::OnLeftButtonDown()
{
    vtkInteractorStyleImage::OnLeftButtonDown();
}

void DesignTrackStyle::OnLeftButtonUp()
{
    vtkInteractorStyleImage::OnLeftButtonUp();

    if (!mPressSpace) return;

    // add a point to windows
    int* pos = this->GetInteractor()->GetEventPosition();
    addPoint(pos[0], pos[1]);
}

void DesignTrackStyle::OnKeyPress()
{
    int code = (int)Interactor->GetKeyCode();
    mPressSpace = (code == 32) ? true : mPressSpace;
    vtkInteractorStyleImage::OnKeyPress();
}

void DesignTrackStyle::OnKeyRelease()
{
    int code = (int)Interactor->GetKeyCode();
    mPressSpace = (code == 32) ? false : mPressSpace;
    if (code == 8) removePreviousPoint(); // key delete

    vtkInteractorStyleImage::OnKeyRelease();
}

void DesignTrackStyle::addPoint(int x, int y)
{
    vtkSmartPointer<vtkPointPicker> picker = dynamic_cast<vtkPointPicker*>(this->GetInteractor()->GetPicker());
    if (nullptr == picker) return;
    picker->Pick(x, y, 0, CurrentRenderer);
    double* worldPosition = picker->GetPickPosition();

    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    if (nullptr != mMapperPoints->GetInput()) points->DeepCopy(mMapperPoints->GetInput()->GetPoints());

    points->InsertNextPoint(worldPosition[0], worldPosition[1], 0.00001); // point is higher then raster

    if (points->GetNumberOfPoints() > 1)
    {
        vtkSmartPointer<vtkLineSource> line = vtkSmartPointer<vtkLineSource>::New();
        vtkSmartPointer<vtkPoints> pointsline = vtkSmartPointer<vtkPoints>::New();
        pointsline->DeepCopy(points);
        line->SetPoints(pointsline);
        line->Update();

        mMapperTrack->SetInputData(line->GetOutput());
        mMapperTrack->Update();
    }

    if (mActorTrack->GetReferenceCount() == 1)
    {
        mActorTrack->GetProperty()->SetLineWidth(2.4);
        mActorTrack->GetProperty()->SetColor(245 / 255.0, 254 / 255.0, 0 / 255.0);
        CurrentRenderer->AddActor(mActorTrack);
    }

    vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->SetPoints(points);

    vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    vertexFilter->SetInputData(polydata);
    vertexFilter->Update();

    mMapperPoints->SetInputData(vertexFilter->GetOutput());
    mMapperPoints->Update();

    if (mActorPoints->GetReferenceCount() == 1)
    {
        mActorPoints->GetProperty()->SetColor(174 / 255.0, 200 / 255.0, 34 / 255.0);
        mActorPoints->GetProperty()->SetPointSize(8);
        mActorPoints->GetProperty()->SetRepresentationToPoints();
        CurrentRenderer->AddActor(mActorPoints);
    }

    GetInteractor()->GetRenderWindow()->Render();
    HighlightProp(NULL);
}

void DesignTrackStyle::removePreviousPoint()
{
    vtkSmartPointer<vtkPolyData> data = mMapperPoints->GetInput();
    if (nullptr == data) return;

    // 更新线
    vtkSmartPointer<vtkPoints> points = mMapperPoints->GetInput()->GetPoints();
    if ((nullptr == points) || (points->GetNumberOfPoints() == 0)) return;

    points->SetNumberOfPoints(points->GetNumberOfPoints() - 1);

    vtkSmartPointer<vtkLineSource> line = vtkSmartPointer<vtkLineSource>::New();
    vtkSmartPointer<vtkPoints> pointsline = vtkSmartPointer<vtkPoints>::New();
    pointsline->DeepCopy(points);
    line->SetPoints(pointsline);
    line->Update();

    mMapperTrack->SetInputData(line->GetOutput());
    mMapperTrack->Update();

    // 更新点
    vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->SetPoints(points);

    vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    vertexFilter->SetInputData(polydata);
    vertexFilter->Update();

    mMapperPoints->SetInputData(vertexFilter->GetOutput());
    mMapperPoints->Update();

    GetInteractor()->GetRenderWindow()->Render();
}
