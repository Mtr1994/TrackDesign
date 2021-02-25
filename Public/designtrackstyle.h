#ifndef DESIGNTRACKSTYLE_H
#define DESIGNTRACKSTYLE_H

#include <vtkInteractorStyleImage.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vector>
#include <vtkActor.h>

class vtkOpenGLPolyDataMapper;
class DesignTrackStyle : public vtkInteractorStyleImage
{
public:
    DesignTrackStyle();
    static DesignTrackStyle *New();
    vtkTypeMacro(DesignTrackStyle, vtkInteractorStyleImage);

    void clearPoints();

    std::string getTrackData();

    void OnLeftButtonDown() override;
    void OnLeftButtonUp() override;
    void OnKeyPress() override;
    void OnKeyRelease() override;

private:
    void addPoint(int x, int y);
    void removePreviousPoint();

private:
    bool mPressSpace = false;
    double mPreviousX = 0;
    double mPreviousY = 0;

    // points
    vtkSmartPointer<vtkOpenGLPolyDataMapper> mMapperPoints;
    vtkSmartPointer<vtkActor> mActorPoints;

    // track
    vtkSmartPointer<vtkOpenGLPolyDataMapper> mMapperTrack;
    vtkSmartPointer<vtkActor> mActorTrack;
};

#endif // DESIGNTRACKSTYLE_H
