#include "topologyutil.h"

#include <vtkFloatArray.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkPolygon.h>
#include <vtkDelaunay2D.h>
#include <vtkLoopSubdivisionFilter.h>
#include <QMap>
#include <vtkTriangleStrip.h>

#include <QDebug>

TopologyUtil::TopologyUtil(QObject *parent) : QObject(parent)
{

}

TopologyUtil::~TopologyUtil()
{

}

void TopologyUtil::DelaunayRasterPoints(vtkPolyData *data, const BlockPoints blockpoints)
{
    vtkSmartPointer<vtkFloatArray> scalars = vtkSmartPointer<vtkFloatArray>::New();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    size_t len = blockpoints.points.size();
    if (len == 0) return;
    QList<int> indexErrorPoints;
    double max = 0;
    for (unsigned int i = 0; i < len; i++)
    {
        Point *point = blockpoints.points.at(i);
        int index = points->InsertNextPoint(point->x, point->y, point->z);
        if (point->empty)
        {
            indexErrorPoints.push_back(index);
        }
        else
        {
            scalars->InsertTuple1(i, point->scalar);
            max = max < point->scalar ? point->scalar : max;
        }
    }

    vtkSmartPointer<vtkCellArray> polygons = vtkSmartPointer<vtkCellArray>::New();
    vtkIdType triangleVertexId[3];
    int xSize = blockpoints.xSize;
    int ySize = blockpoints.ySize;
    for (int m = 0; m < xSize - 1; m++)
    {
        for (int n = 0; n < ySize - 1; n++)
        {
            // First triangle
            triangleVertexId[0] = ySize * m + n;
            triangleVertexId[1] = ySize * m + (n + 1);
            triangleVertexId[2] = ySize * (m + 1) + (n + 1);
            polygons->InsertNextCell(3, triangleVertexId);

            // Second triangle
            triangleVertexId[0] = ySize * m + n;
            triangleVertexId[1] = ySize * (m + 1) + (n + 1);
            triangleVertexId[2] = ySize * (m + 1) + n;
            polygons->InsertNextCell(3, triangleVertexId);
        }
    }

    data->SetPoints(points);
    data->SetPolys(polygons);

    int errorLen = indexErrorPoints.length();
    data->BuildLinks();
    for (int l = 0; l < errorLen; l++) // 删除异常数据的 cell
    {
        vtkSmartPointer<vtkIdList> cellList = vtkSmartPointer<vtkIdList>::New();
        data->GetPointCells(indexErrorPoints.at(l), cellList);
        int count =  cellList->GetNumberOfIds();
        for (int k = 0; k < count; k++)
        {
            data->DeleteCell(cellList->GetId(k));
        }
        scalars->InsertTuple1(indexErrorPoints.at(l), max);
    }
    data->RemoveDeletedCells();
    data->GetPointData()->SetScalars(scalars);
}

void TopologyUtil::DelaunayStructPoints(vtkPolyData *data, const vector<vector<Point *> > listpoints, int frombeam, int endbeam, int fromrange, int endrange)
{
    vtkSmartPointer<vtkFloatArray> scalars = vtkSmartPointer<vtkFloatArray>::New();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    int len = int(listpoints.size());
    for (int i = frombeam; (i < len) && (i < endbeam); i++)
    {
        int size = int(listpoints.at(i).size());
        for (int j = fromrange; (j < size) && (j < endrange); j++)
        {
            Point *point = listpoints.at(i).at(j);
            points->InsertNextPoint(point->x, point->y, point->z);
            scalars->InsertNextTuple1(point->scalar);
        }
    }

    vtkIdType triangleVertexId[3];
    vtkSmartPointer<vtkCellArray> polygons = vtkSmartPointer<vtkCellArray>::New();
    int currentSampleIndex = 0;
    for (int m = frombeam; (m < len - 1) && (m < endbeam - 1); m++)
    {
        int end = int(listpoints.at(m).size()) - endrange;
        int samplenumber1 = int(listpoints.at(m).size()) - fromrange - (end > 0 ? end : 0);
        if (samplenumber1 < 0) continue;
        currentSampleIndex += samplenumber1;
        end = int(listpoints.at(m + 1).size()) - endrange;
        int samplenumber2 = int(listpoints.at(m + 1).size()) - fromrange - (end > 0 ? end : 0);
        if (samplenumber2 < 0) continue;
        for (int n = 0; (n < samplenumber1 - 1) && (n < endrange - 1); n++)
        {
            // First triangle
            triangleVertexId[0] = (currentSampleIndex - samplenumber1) + n;
            triangleVertexId[1] = (currentSampleIndex - samplenumber1) + (n + 1);
            int secondBeam = (n + 1) >= (samplenumber2 - 1) ? (samplenumber2 - 1) : (n + 1);
            triangleVertexId[2] = currentSampleIndex + secondBeam;
            polygons->InsertNextCell(3, triangleVertexId);

            // Second triangle
            triangleVertexId[0] = (currentSampleIndex - samplenumber1) + n;
            if((n + 1) > (samplenumber2 - 1)) continue;
            triangleVertexId[1] = currentSampleIndex + (n + 1);
            triangleVertexId[2] = currentSampleIndex + n;
            polygons->InsertNextCell(3, triangleVertexId);

            // end
            if (n != (samplenumber1 - 2)) continue;
            if (samplenumber2 <= samplenumber1) continue;
            n++;
            int leftCount = samplenumber2 - samplenumber1;
            for (int j = 0; j < leftCount; j++)
            {
                triangleVertexId[0] = (currentSampleIndex - samplenumber1) + n;
                triangleVertexId[1] = currentSampleIndex + (n + 1) + j;
                triangleVertexId[2] = currentSampleIndex + n + j;
                polygons->InsertNextCell(3, triangleVertexId);
            }
        }
    }

    data->SetPoints(points);
    data->SetPolys(polygons);
    data->GetPointData()->SetScalars(scalars);
}

void TopologyUtil::DelaunayStructPoints(vtkUnstructuredGrid *data, int length, vector<vector<vector<Point *> > > &listpoints, QMap<int, int> &mapsamples, QMap<int, int> &mapbeams)
{
    QString str;
    Position position;
    memset(&position, 0, sizeof (Position));

    vtkSmartPointer<vtkFloatArray> scalars = vtkSmartPointer<vtkFloatArray>::New();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    for (uint m = 0; m < listpoints.size(); m++) // Fan 数量
    {
        int size = int(listpoints.at(m).size());  // Beam 数据量 288 ,默认一致
        for (int i = 0; i < size; i++)
        {
            int count = int(listpoints.at(m).at(i).size()); // 单波束采样点数量
            for (int j = 0; j < count; j++)
            {
                Point *point = listpoints.at(m).at(i).at(j);
                points->InsertNextPoint(point->x, point->y, point->z);
                scalars->InsertNextTuple1(point->scalar);
            }
        }
    }

    // 两个扇面之间 3 个点可以构成三个四面体，结构为
    // A1 D1
    // B1 C1
    data->Allocate(360 * 287 * 6 * (length - 1));
    int fanPointIndex = 0;
    for (int m = 0; m < length - 1; m++) // Fan 数量 288 * 125
    {
        if (!mapsamples.contains(m)) continue;
        int samples = mapsamples.value(m);
        if (mapsamples.contains(m - 1)) fanPointIndex += mapsamples.value(m - 1);
        if (!mapbeams.contains(m)) continue;
        int beams = mapbeams.value(m);
        int pointIndex1 = 0, pointIndex2 = 0;

        vector<vector<Point *> > listA = listpoints.at(0);
        listpoints.erase(listpoints.begin());
        vector<vector<Point *> > listB = listpoints.at(0);
        for (int i = 0; i < beams - 1; i++)
        {
            // 放弃空波束
            if (i > 0) pointIndex1 += int(listA.at(i - 1).size());
            if (i > 0) pointIndex2 += int(listB.at(i - 1).size());
            if (listA.at(i).at(10)->z == 0) continue;
            if (listA.at(i + 1).at(10)->z == 0) continue;
            if (listB.at(i).at(10)->z == 0) continue;
            if (listB.at(i + 1).at(10)->z == 0) continue;
            int lenPointsLeft1 = int(listA.at(i).size());
            int lenPointsRight1 = int(listA.at(i + 1).size());
            int lenPointsLeft2 = int(listB.at(i).size());
            int lenPointsRight2 = int(listB.at(i + 1).size());
            int lenPoints = qMin(qMin(lenPointsLeft1, lenPointsRight1), qMin(lenPointsLeft2, lenPointsRight2));
            // 判断同一扇面右侧是否存在 C1 点, 判断后一个扇面是否存在 A2 B2 C2
            for (int j = 0; j < lenPoints - 1; j++)
            {
                int A1 = fanPointIndex + pointIndex1 + j;
                int B1 = fanPointIndex + pointIndex1 + j + 1;
                int C1 = fanPointIndex + pointIndex1 + lenPointsLeft1 + j + 1;
                int D1 = fanPointIndex + pointIndex1 + lenPointsLeft1 + j;
                int A2 = fanPointIndex + pointIndex2 + samples + j;
                int B2 = fanPointIndex + pointIndex2 + samples + j + 1;
                int C2 = fanPointIndex + pointIndex2 + samples + lenPointsLeft2 + j + 1;
                int D2 = fanPointIndex + pointIndex2 + samples + lenPointsLeft2 + j;

                vtkIdType pts1[3][4]={{A1, B1, C1, A2}, {B1, C1, A2, B2}, {C1, A2, B2, C2}};
                // 下三角 3 个 四面体
                data->InsertNextCell(VTK_TETRA, 4, pts1[0]);
                data->InsertNextCell(VTK_TETRA, 4, pts1[1]);
                data->InsertNextCell(VTK_TETRA, 4, pts1[2]);

                vtkIdType pts2[3][4]={{A1, C1, D1, A2}, {C1, D1, A2, C2}, {D1, A2, C2, D2}};
                // 上三角 3 个 四面体
                data->InsertNextCell(VTK_TETRA, 4, pts2[0]);
                data->InsertNextCell(VTK_TETRA, 4, pts2[1]);
                data->InsertNextCell(VTK_TETRA, 4, pts2[2]);
            }
        }
    }

    data->SetPoints(points);
    data->GetPointData()->SetScalars(scalars);
}
