#pragma once
class GeographicCoordinate {
private:
	double latitude;
	double longitude;
public:
	GeographicCoordinate();
	GeographicCoordinate(double latitude, double longitude);
	double getLat();
	double getLon();
};
GeographicCoordinate geoCoord(double latitude, double longitude);