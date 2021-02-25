#ifndef COORDINATETRANSFORM_H
#define COORDINATETRANSFORM_H

class OGRCoordinateTransformation;
class CoordinateTransform
{
public:
    enum CoordinateType {Coordinate_Mercator = 0, Coordinate_Wgs84};
    CoordinateTransform(CoordinateType type, double longitude = 0, double latitude = 0);
    ~CoordinateTransform();

    bool transform(double &longitude, double &latitude);

private:
    OGRCoordinateTransformation *mLonLat2XY = nullptr;
};

#endif // COORDINATETRANSFORM_H
