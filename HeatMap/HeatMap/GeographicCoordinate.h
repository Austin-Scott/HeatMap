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
	void printCoordinate();
};
GeographicCoordinate geoCoord(double latitude, double longitude);
GeographicCoordinate* computeBoundingBox(GeographicCoordinate bottomCenter, double maxLatitude, int width, int height);