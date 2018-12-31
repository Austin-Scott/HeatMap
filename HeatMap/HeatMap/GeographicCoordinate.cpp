#include "GeographicCoordinate.h"

#include <math.h>
#include <iostream>

using namespace std;

GeographicCoordinate::GeographicCoordinate()
{
	latitude = 0.0;
	longitude = 0.0;
}

GeographicCoordinate::GeographicCoordinate(double latitude, double longitude)
{
	this->latitude = latitude;
	this->longitude = longitude;
}

double GeographicCoordinate::getLat()
{
	return latitude;
}

double GeographicCoordinate::getLon()
{
	return longitude;
}

string GeographicCoordinate::toString()
{
	string result = "Latitude: " + to_string(latitude) + " Longitude: " + to_string(longitude);
	return result;
}

GeographicCoordinate geoCoord(double latitude, double longitude)
{
	return GeographicCoordinate(latitude, longitude);
}
