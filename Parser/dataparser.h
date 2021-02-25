#ifndef DATAPARSER_H
#define DATAPARSER_H

#include <QObject>
#include <QString>
#include <vtkSmartPointer.h>

class vtkPolyData;
class DataParser : public QObject
{
    Q_OBJECT
public:
    explicit DataParser(QObject *parent = nullptr);

    static DataParser* getInstance();

    void parse(const QString& path, const QString& filter);

signals:
    void sgl_read_poly_data_finish(vtkSmartPointer<vtkPolyData> data);

};

#endif // DATAPARSER_H
