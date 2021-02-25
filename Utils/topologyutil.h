#ifndef DELAUNAY2D_H
#define DELAUNAY2D_H

#include <QObject>
#include <vtkPolyData.h>
#include <vtkUnstructuredGrid.h>
#include <vtkSmartPointer.h>
#include <QMap>

#include "Base/defines.h"

using namespace std;

class TopologyUtil : public QObject
{
    Q_OBJECT
public:
    explicit TopologyUtil(QObject *parent = nullptr);
    ~TopologyUtil();

    void DelaunayRasterPoints(vtkPolyData* data, const BlockPoints blockpoints);
    void DelaunayStructPoints(vtkPolyData* data, const vector<vector<Point*>> listpoints, int frombeam = 0, int endbeam = 65534, int fromrange = 0, int endrange = 65534);
    void DelaunayStructPoints(vtkUnstructuredGrid* data, int length, vector<vector<vector<Point*>>>& listpoints, QMap<int, int>& mapsamples, QMap<int, int>& mapbeams);

signals:
    void sgl_delaunay_finish(const QString& id);
};

#endif // DELAUNAY2D_H
