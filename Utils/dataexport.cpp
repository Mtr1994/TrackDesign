#include "dataexport.h"

#include <QFile>
#include <QTextStream>

DataExport::DataExport(QObject *parent) : QObject(parent)
{

}

DataExport* DataExport::getInstance()
{
    static DataExport tool;
    return &tool;
}

void DataExport::exportData(const QString &path, const QString &data)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        return;

    QTextStream in(&file);
    in << data;

    file.close();
}
