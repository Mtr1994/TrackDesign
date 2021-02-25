#include "rasterfilereader.h"
#include "gdal_priv.h"

#include "Utils/topologyutil.h"
#include "Utils/coordinatetransform.h"

#include <QDebug>

RasterFileReader::RasterFileReader(QObject *parent) : QObject(parent)
{

}

RasterFileReader::~RasterFileReader()
{

}

RasterFileDetails *RasterFileReader::readFile(const QString &file)
{
    RasterFileDetails *details = new RasterFileDetails;
    read(file, details);
    return details;
}

/**
 * @brief RasterFileReader::read
 * @param path
 * @param details
 * 栅格文件统一读取接口
 * 使用世界墨卡托投影（将本初子午线作为中心经度， 赤道线作为中心纬度），将经纬度坐标转换为世界墨卡托坐标，
 * 使用 米 为单位，跟高程数据单位保持一致，成像的时候才能是正常的比例
 */

void RasterFileReader::read(const QString &path, RasterFileDetails *details)
{
    if (path.isEmpty()) return;

    GDALDataset *poDataset;
    GDALAllRegister();
    poDataset = (GDALDataset *) GDALOpen(path.toStdString().data(), GA_ReadOnly);

    if(poDataset == nullptr) return;
    int xLength = 0, yLength = 0;
    xLength = poDataset->GetRasterXSize();
    yLength = poDataset->GetRasterYSize();

    double adfGeoTransform[6];
    if(poDataset->GetGeoTransform(adfGeoTransform) != CE_None ) return;

    GDALRasterBand *poBand;
    int nBlockXSize, nBlockYSize;
    int nGotMin, nGotMax;
    double adfMinMax[2];
    poBand = poDataset->GetRasterBand(1);
    poBand->GetBlockSize( &nBlockXSize, &nBlockYSize);
    GDALDataType cellType = poBand->GetRasterDataType();
    int cellSize = GDALGetDataTypeSizeBytes(cellType);
    adfMinMax[0] = poBand->GetMinimum( &nGotMin );
    adfMinMax[1] = poBand->GetMaximum( &nGotMax );
    double noDataValue = poBand->GetNoDataValue();
    if( !( nGotMin && nGotMax )) {
       GDALComputeRasterMinMax( (GDALRasterBandH)poBand, TRUE, adfMinMax );
    }
    BlockPoints blockPoints;
    char *pafScanline = new char[cellSize * yLength];
    CoordinateTransform ctf(CoordinateTransform::Coordinate_Mercator, 0, 0);
    for (int m = 0 ; m < xLength; m++)
    {
        CPLErr err = poBand->RasterIO(GF_Read, m, 0, 1, yLength, pafScanline, 1, yLength , cellType, 0, 0);
        if (err != CE_None) continue;
        for (int n = 0; n < yLength; n++)
        {
            double Xgeo = adfGeoTransform[0] + m * adfGeoTransform[1] + n * adfGeoTransform[2];
            double Ygeo = adfGeoTransform[3] + m * adfGeoTransform[4] + n * adfGeoTransform[5];
            if (!ctf.transform(Xgeo, Ygeo)) continue;
            Point *point = new Point;
            point->x = Xgeo;
            point->y = Ygeo;
            double value = getValue(pafScanline, n, cellSize, cellType);
            point->empty = (noDataValue == value) | (std::isnan(value));
            point->z = 0; // 二维图像，不需要实际 Z 值
            point->scalar = value;
            blockPoints.points.push_back(point);
        }
    }

    delete [] pafScanline;

    blockPoints.xSize = xLength;
    blockPoints.ySize = yLength;

    GDALClose((GDALDatasetH)poDataset);

    details->min = adfMinMax[0];
    details->max = adfMinMax[1];
    details->points = blockPoints;
}

/**
 * @brief RasterFileReader::getValue
 * @param array
 * @param index
 * @param cellsize
 * @param celltype
 * @return
 * 根据不同的类型，将内存中的数据解析为规定的数据类型，使用 double 作为返回值
 */

double RasterFileReader::getValue(char *array, int index, int cellsize, int celltype)
{
    switch (celltype) {
    case 1:
        char value1;
        memcpy(&value1, array + index * cellsize, cellsize);
        return value1;
    case 2:
        unsigned short value2;
        memcpy(&value2, array + index * cellsize, cellsize);
        return value2;
    case 3:
        short value3;
        memcpy(&value3, array + index * cellsize, cellsize);
        return value3;
    case 4:
        unsigned int value4;
        memcpy(&value4, array + index * cellsize, cellsize);
        return value4;
    case 5:
        int value5;
        memcpy(&value5, array + index * cellsize, cellsize);
        return value5;
    case 6:
        float value6;
        memcpy(&value6, array + index * cellsize, cellsize);
        return value6;
    case 7:
        double value7;
        memcpy(&value7, array + index * cellsize, cellsize);
        return value7;
    default:
        return 0;
    }
}
