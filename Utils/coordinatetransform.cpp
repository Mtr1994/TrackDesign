#include "coordinatetransform.h"
#include "gdal_priv.h"

#include <string>

/*
 * 经纬度（0，0）没法正常转换
 *
 *
*/

CoordinateTransform::CoordinateTransform(CoordinateTransform::CoordinateType type, double longitude, double latitude)
{
    if (type == Coordinate_Mercator)
    {
        // 投影坐标系
        OGRSpatialReference spatialReference;
        spatialReference.SetWellKnownGeogCS("WGS84");
        spatialReference.SetMercator(latitude, longitude, 1, 0, 0);
        //spatialReference.SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER); // gdal 3.0 之后需要使用

        // 地理坐标系
        OGRSpatialReference *pLonLat = spatialReference.CloneGeogCS();
        mLonLat2XY = OGRCreateCoordinateTransformation(pLonLat, &spatialReference);
    }
    else if (type == Coordinate_Wgs84)
    {
        // 投影坐标系
        OGRSpatialReference spatialReference;
        spatialReference.SetWellKnownGeogCS("WGS84");
        spatialReference.SetMercator(latitude, longitude, 1, 0, 0);
        //spatialReference.SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER); // gdal 3.0 之后需要使用

        // 地理坐标系
        OGRSpatialReference *pLonLat = spatialReference.CloneGeogCS();
        mLonLat2XY = OGRCreateCoordinateTransformation(&spatialReference, pLonLat);
    }
}

CoordinateTransform::~CoordinateTransform()
{
    OGRCoordinateTransformation::DestroyCT(mLonLat2XY);
    mLonLat2XY = nullptr;
}

bool CoordinateTransform::transform(double &longitude, double &latitude)
{
    if (!mLonLat2XY) return false;
    return mLonLat2XY->Transform(1, &longitude, &latitude);
}

