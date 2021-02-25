#ifndef RASTERFILEREADER_H
#define RASTERFILEREADER_H

#include <QObject>
#include <QList>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>

#include "Base/defines.h"

class RasterFileReader : public QObject
{
    Q_OBJECT
public:
    explicit RasterFileReader(QObject *parent = nullptr);
    ~RasterFileReader();

    RasterFileDetails *readFile(const QString& file);

private:
    void read(const QString& path, RasterFileDetails* details);

    double getValue(char *array, int index, int cellsize, int celltype);
};

#endif // RASTERFILEREADER_H
