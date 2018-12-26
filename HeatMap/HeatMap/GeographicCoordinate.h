#pragma once
class GeographicCoordinate {
private:
	double latitude;
	double longitude;
public:
	GeographicCoordinate(double latitude, double longitude);
};
GeographicCoordinate geoCoord(double latitude, double longitude);