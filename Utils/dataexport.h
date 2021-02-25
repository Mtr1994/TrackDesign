#ifndef DATAEXPORT_H
#define DATAEXPORT_H

#include <QObject>

class DataExport : public QObject
{
    Q_OBJECT
private:
    explicit DataExport(QObject *parent = nullptr);

public:
    static DataExport* getInstance();

    void exportData(const QString& path, const QString& data);

signals:

};

#endif // DATAEXPORT_H
