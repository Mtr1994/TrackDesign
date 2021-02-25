#include "dataparser.h"
#include "Reader/rasterfilereader.h"
#include "Utils/topologyutil.h"

#include <QDebug>

DataParser::DataParser(QObject *parent) : QObject(parent)
{

}

DataParser* DataParser::getInstance()
{
    static DataParser parser;
    return &parser;
}

void DataParser::parse(const QString &path, const QString &filter)
{
    Q_UNUSED(filter);

    RasterFileReader reader;
    RasterFileDetails *details  = reader.readFile(path);

    TopologyUtil delaunary;
    vtkSmartPointer<vtkPolyData> data = vtkSmartPointer<vtkPolyData>::New();
    delaunary.DelaunayRasterPoints(data, details->points);

    emit sgl_read_poly_data_finish(data);
}
