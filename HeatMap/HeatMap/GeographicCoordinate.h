#pragma once
#include <string>

using namespace std;

class GeographicCoordinate {
private:
	double latitude;
	double longitude;
public:
	GeographicCoordinate();
	GeographicCoordinate(double latitude, double longitude);
	double getLat();
	double getLon();
	string toString();
};
GeographicCoordinate geoCoord(double latitude, double longitude);