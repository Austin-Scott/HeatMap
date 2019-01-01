#pragma once
#include <string>
#define _USE_MATH_DEFINES
#include <cmath>

using namespace std;

class GeographicCoordinate {
private:
	double latitude;
	double longitude;

	bool isSet;
public:
	GeographicCoordinate();
	GeographicCoordinate(double latitude, double longitude);
	double getLat();
	double getLon();
	string toString();
	bool getIsSet();
	double getKilometersTo(GeographicCoordinate other);
};
GeographicCoordinate geoCoord(double latitude, double longitude);